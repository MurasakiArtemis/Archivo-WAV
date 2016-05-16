#include "ArchivoWAV.hpp"
#include <cstring>
#include <vector>

double max(valarray<complex<double>> X)
{
  double acumR = -100, acumI = -100;
  for(unsigned int i = 0; i < X.size(); i++)
  {
    if(X[i].real() > acumR)
      acumR = X[i].real();
    if(X[i].imag() > acumI)
      acumI = X[i].imag();
  }
  return acumR > acumI? acumR: acumI;
}

ArchivoWAV::ArchivoWAV(const string& nombreArchivo, unsigned int tamano, unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio):
  numeroCanales(numCanales), bitsPorMuestra(bitsPerSample), bytesPorMuestra(bitsPerSample/8), tamanoAudio(tamAudio),
  numeroMuestras(tamAudio/((bitsPerSample == 0? 8 : bitsPerSample)/8)), frecuenciaMuestreo(frecMuestreo),
  fileName(nombreArchivo), fileSize(tamano), fileMetadata(new unsigned char[fileSize]),
  fileData(numeroCanales == 2? numeroMuestras/2 : numeroMuestras)
{
  memset(fileMetadata, 0, fileSize);
  if(tamano != 0 && numCanales != 0 && frecMuestreo != 0 && bitsPerSample != 0 && tamAudio != 0)
  {
    escribirInt(0x46464952, 0);
    escribirInt(fileSize-8, 4);
    escribirInt(0x45564157, 8);
    escribirInt(0x20746d66, 12);
    escribirInt(16, 16);
    escribirShort(1, 20);
    escribirShort(numeroCanales, 22);
    escribirInt(frecuenciaMuestreo, 24);
    escribirInt(frecuenciaMuestreo * numeroCanales * bytesPorMuestra, 28);
    escribirShort(numeroCanales * bytesPorMuestra, 32);
    escribirShort(bitsPorMuestra, 34);
    escribirInt(0x61746164, 36);
    escribirInt(tamanoAudio, 40);
  }
}

ArchivoWAV::ArchivoWAV(const ArchivoWAV& arch, const ArchivoWAV& arch1, const string& nombreArchivo):
  numeroCanales(arch.numeroCanales<arch1.numeroCanales?arch.numeroCanales:arch1.numeroCanales),
  bitsPorMuestra(arch.bitsPorMuestra<arch1.bitsPorMuestra?arch.bitsPorMuestra:arch1.bitsPorMuestra),
  bytesPorMuestra(arch.bytesPorMuestra<arch1.bytesPorMuestra?arch.bytesPorMuestra:arch1.bytesPorMuestra),
  tamanoAudio(arch.tamanoAudio<arch1.tamanoAudio?arch.tamanoAudio:arch1.tamanoAudio),
  numeroMuestras(arch.numeroMuestras<arch1.numeroMuestras?arch.numeroMuestras:arch1.numeroMuestras),
  frecuenciaMuestreo(arch.frecuenciaMuestreo<arch1.frecuenciaMuestreo?arch.frecuenciaMuestreo:arch1.frecuenciaMuestreo),
  fileName(nombreArchivo), fileSize(arch.fileSize<arch1.fileSize?arch.fileSize:arch1.fileSize),
  fileMetadata(new unsigned char[fileSize]),
  fileData(arch.fileData.size()<arch1.fileData.size()?arch.fileData.size():arch1.fileData.size())
{ 
  for(unsigned int i = 0; i < fileSize; i++)
  {
    switch(i)
    {
    case 22:
    case 23:
      escribirByte(arch.numeroCanales < arch1.numeroCanales?arch.extraerUByte(i):arch1.extraerUByte(i), i);
      break;
    case 34:
    case 35:
      escribirByte(arch.bitsPorMuestra < arch1.bitsPorMuestra?arch.extraerUByte(i):arch1.extraerUByte(i), i);
    case 40:
    case 41:
    case 42:
    case 43:
      escribirByte(arch.tamanoAudio < arch1.tamanoAudio?arch.extraerUByte(i):arch1.extraerUByte(i), i);
      break;
    default:
      escribirByte(arch.extraerUByte(i), i);
      break;
    }
  }
}

ArchivoWAV::ArchivoWAV(const ArchivoWAV& arch, const string& nombreArchivo):
  numeroCanales(arch.numeroCanales), bitsPorMuestra(arch.bitsPorMuestra), bytesPorMuestra(arch.bytesPorMuestra),
  tamanoAudio(arch.tamanoAudio), numeroMuestras(arch.numeroMuestras), frecuenciaMuestreo(arch.frecuenciaMuestreo),
  fileName(nombreArchivo), fileSize(arch.fileSize), fileMetadata(new unsigned char[fileSize]), fileData(arch.fileData)
{
  memcpy(fileMetadata, arch.fileMetadata, fileSize);
}

ArchivoWAV::ArchivoWAV(const ArchivoWAV& arch):
  numeroCanales(arch.numeroCanales), bitsPorMuestra(arch.bitsPorMuestra),
  bytesPorMuestra(arch.bytesPorMuestra), tamanoAudio(arch.tamanoAudio), numeroMuestras(arch.numeroMuestras),
  frecuenciaMuestreo(arch.frecuenciaMuestreo), fileName(arch.fileName), fileSize(arch.fileSize),
  fileMetadata(new unsigned char[fileSize]), fileData(arch.fileData)
{
  memcpy(fileMetadata, arch.fileMetadata, fileSize);
}

ArchivoWAV::~ArchivoWAV()
{
  delete[] fileMetadata;
}

complex<double> ArchivoWAV::extraerMuestra(const unsigned int desplazamientoEstatico) const
{
  return fileData[desplazamientoEstatico];
}

void ArchivoWAV::insertarMuestra(const complex<double>& muestra, unsigned int posicion)
{
  fileData[posicion] = muestra;
}

double ArchivoWAV::map(double x, double inMin, double inMax, double outMin, double outMax) const
{
  return (x - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
}

complex<double> ArchivoWAV::map(const pair<short, short>& muestra, double inMin, double inMax, double outMin, double outMax) const
{
  return complex<double>(map(muestra.first, inMin, inMax, outMin, outMax), map(muestra.second, inMin, inMax, outMin, outMax));
}

pair<short, short> ArchivoWAV::map(const complex<double>& muestra, double inMin, double inMax, double outMin, double outMax) const
{
  return pair<short, short>(map(muestra.real(), inMin, inMax, outMin, outMax), map(muestra.imag(), inMin, inMax, outMin, outMax));
}

valarray<complex<double>> ArchivoWAV::map(const valarray<short>& data, double inMin, double inMax, double outMin, double outMax) const
{
  valarray<complex<double>> resultado(numeroCanales == 2? data.size()/2 : data.size());
  for(unsigned int i = 0, j = 0; i < data.size() && j < resultado.size(); i += numeroCanales, j++)
  {
    double real = 0, imag = 0;
    real = map(data[i], inMin, inMax, outMin, outMax);
    if(numeroCanales == 2)
      imag = map(data[i+1], inMin, inMax, outMin, outMax);
    resultado[j] = complex<double>(real, imag);
  }
  return resultado;
}

valarray<short> ArchivoWAV::map(const valarray<complex<double>>& data) const
{
  valarray<short> resultado(numeroCanales == 2? data.size()*2 : data.size());
  for(unsigned int i = 0, j = 0; i < data.size() && j < resultado.size(); i++, j += numeroCanales)
  {
    pair<short, short> res = map(data[i], -1, 1, -32768, 32767);
    resultado[j] = res.first;
    if(numeroCanales == 2)
      resultado[j+1] = res.second;
  }
  return resultado;
}

short* ArchivoWAV::map(valarray<short> data)
{
  short* resultado = new short[data.size()];
  for(unsigned int i = 0; i < data.size(); i++)
    resultado[i] = data[i];
  return resultado;
}

unsigned char ArchivoWAV::extraerUByte(unsigned int i) const
{
  if(i < 44)
    return fileMetadata[i];
  return 0;
}

char ArchivoWAV::extraerSByte(unsigned int i) const
{
  if(i < 44)
    return (char)fileMetadata[i];
  return 0;
}

unsigned short ArchivoWAV::extraerUShort(unsigned int i) const
{
  if(i < 44)
    return *((unsigned short*)(fileMetadata + i));
  return 0;
}

short ArchivoWAV::extraerSShort(unsigned int i) const
{
  if(i < 44)
    return *((short*)(fileMetadata + i));
  return 0;
}

unsigned int ArchivoWAV::extraerUInt(unsigned int i) const
{
  if(i < 44)
    return *((unsigned int*)(fileMetadata + i));
  return 0;
}

int ArchivoWAV::extraerSInt(unsigned int i) const
{
  if(i < 44)
    return *((int*)(fileMetadata + i));
  return 0;
}

unsigned long ArchivoWAV::extraerULong(unsigned int i) const
{
  if(i < 44)
    return *((unsigned long*)(fileMetadata + i));
  return 0;
}

long ArchivoWAV::extraerSLong(unsigned int i) const
{
  if(i < 44)
    return *((long*)(fileMetadata + i));
  return 0;
}

void ArchivoWAV::escribirByte(unsigned char byte, unsigned int posicion)
{
  if(posicion <= 43)
    fileMetadata[posicion] = byte;
}

void ArchivoWAV::escribirShort(unsigned short entero, unsigned int posicion)
{
  if(posicion <= 42)
    memcpy(fileMetadata+posicion, &entero, sizeof(unsigned short));
}

void ArchivoWAV::escribirInt(unsigned int entero, unsigned int posicion)
{
  if(posicion <= 40)
    memcpy(fileMetadata+posicion, &entero, sizeof(unsigned int));
}

void ArchivoWAV::escribirLong(unsigned long entero, unsigned int posicion)
{
  if(posicion <= 36)
    memcpy(fileMetadata+posicion, &entero, sizeof(unsigned long));
}

std::ostream& operator<<(std::ostream& out, const ArchivoWAV& arch)
{
  out << "Nombre: " << arch.fileName << std::endl;
  out << "Número de canales: " << arch.numeroCanales << std::endl;
  out << "Bits por muestra: " << arch.bitsPorMuestra << std::endl;
  out << "Bytes por muestra: " << arch.bytesPorMuestra << std::endl;
  out << "Tamaño del audio: " << arch.tamanoAudio << std::endl;
  out << "Número de muestras: " << arch.numeroMuestras << std::endl;
  out << "Número de muestras complejas: " << arch.fileData.size() << std::endl;
  out << "Frecuencia de muestreo: " << arch.frecuenciaMuestreo << std::endl;
  vector<double> cos, sin;
  arch.obtenerFrecuencias(cos, sin);
  return out;
}

ArchivoWAV ArchivoWAV::operator/(const double& valor) const
{
  string str(fileName);
  str.erase(str.end() - 4, str.end());
  str += " dividido entre ";
  str += std::to_string((int)valor);
  str += ".wav";
  ArchivoWAV salida(*this, str);
  salida.fileData = fileData/complex<double>(valor, 0);
  return salida;
}

ArchivoWAV ArchivoWAV::operator*(const ArchivoWAV& arch) const
{
  string str(fileName);
  str.erase(str.end() - 4, str.end());
  string str2(arch.fileName);
  str2.erase(str2.end() - 4, str2.end());
  str += " multiplicado con " + str2 + ".wav";
  ArchivoWAV salida(*this, arch, str);
  salida.fileData = fileData*arch.fileData;
  return salida;
}

void ArchivoWAV::convolucion(unsigned int numMuestras, const valarray<complex<double>> h, ArchivoWAV& salida) const
{
  valarray<complex<double>> x(numMuestras);
  for(unsigned int i = 0; i < fileData.size(); i++)
  {
    x = x.shift(-1);
    x[0] = extraerMuestra(i);
    salida.insertarMuestra((x*h).sum(), i);
  }
  double acum = max(salida.fileData);
  salida.fileData /= complex<double>(acum, 0);
}

ArchivoWAV ArchivoWAV::simularCircuitoRC(const string& name, unsigned int numMuestras, unsigned int frecCorte, bool ideal) const
{
  string str(name);
  if(name == "")
  {
    str = string(fileName);
    str.erase(str.end() - 4, str.end());
    str += " filtrado a " + std::to_string(frecCorte) + ".wav";
  }
  ArchivoWAV salida(*this, str);
  if(frecCorte == 3000 && numMuestras == 10 && !ideal)
  {
    const complex<double> array[] = {{1.000000000, 0}, {0.652184637, 0}, {0.425344800, 0}, {0.277403344, 0}, {0.180918199, 0},
				     {0.117992070, 0}, {0.076952615, 0}, {0.050187313, 0}, {0.032731395, 0}, {0.021346913, 0}};
    const valarray<complex<double>> h(array, numMuestras);
    this->convolucion(numMuestras, h, salida);
  }
  else if(frecCorte == 500 && numMuestras == 32 && ideal)
  {
    std::cout << "H" << std::endl;
    const complex<double> array[] = {{0.999970, 0}, {0.676820, 0}, {0.066620, 0}, {-0.21976, 0},
				     {-0.06665, 0}, {0.124690, 0}, {0.066620, 0}, {-0.08124, 0},
				     {-0.06665, 0}, {0.054690, 0}, {0.066620, 0}, {-0.03564, 0},
				     {-0.06665, 0}, {-0.02020, 0}, {-0.06662, 0}, {-0.00656, 0},
				     {-0.06665, 0}, {0.006560, 0}, {0.066620, 0}, {0.020200, 0},
				     {-0.06665, 0}, {-0.03564, 0}, {-0.06662, 0}, {0.054690, 0},
				     {-0.06665, 0}, {-0.08124, 0}, {0.066620, 0}, {0.124690, 0},
				     {0.066650, 0}, {-0.21976, 0}, {0.066620, 0}, {0.676820, 0}};
    const valarray<complex<double>> h(array, numMuestras);
    this->convolucion(numMuestras, h, salida);
  }
  return salida;
}


void ArchivoWAV::transformadaFourier(valarray<complex<double>>& X, bool inversa, const ArchivoWAV& salida) const
{
  const double pi = std::acos(complex<double>(-1.0, 0)).real()*(inversa?1:-1);
  for(unsigned int k = 0; k < salida.fileData.size(); k++)
  {
    for(unsigned int n = 0; n < fileData.size(); n++)
    {
      complex<double> x_n = extraerMuestra(n);
      complex<double> exponente = std::polar(1.0, 2*pi*(k)*n/(fileData.size()));
      X[k] += x_n*exponente;
    }
  }
}

void transformadaRapida(valarray<complex<double>>& X, const double pi)
{
  const unsigned int numMuestras = X.size();
  if(numMuestras <= 1)
    return;
  valarray<complex<double>> pares = X[std::slice(0, numMuestras/2, 2)];
  valarray<complex<double>> impares = X[std::slice(1, numMuestras/2, 2)];
  transformadaRapida(pares, pi);
  transformadaRapida(impares, pi);
  for(unsigned int k = 0; k < numMuestras/2; ++k)
  {
    complex<double> t = std::polar(1.0, 2 * pi * k / numMuestras) * impares[k];
    X[k] = pares[k] + t;
    X[k+numMuestras/2] = pares[k] - t;
  }
}

ArchivoWAV ArchivoWAV::transformadaFourier(const string& name, const bool rapida, const unsigned int opcion) const
{
  string str(name);
  if(name == "")
  {
    str = string(fileName);
    str.erase(str.end() - 4, str.end());
    str += " transformado ";
    str += (rapida?"rapido":"lento");
    str += " con opcion " + std::to_string(opcion) + ".wav";
  }
  ArchivoWAV salida(str, numeroCanales == 1? fileSize + tamanoAudio: fileSize, 2, frecuenciaMuestreo, bitsPorMuestra, numeroCanales == 1? 2*(tamanoAudio):tamanoAudio);
  valarray<complex<double>> X;
  double acum;
  if(rapida)
  {
    X = valarray<complex<double>>(fileData);
    transformadaRapida(X, std::acos(complex<double>(-1.0, 0)).real()*-1);
  }
  else
  {
    X = valarray<complex<double>>(fileData.size());
    transformadaFourier(X, false, salida);
  }
  acum = max(X);
  switch(opcion)
  {
  case 0:
    salida.fileData = X;
    salida.fileData /= complex<double>(acum, 0);
    break;
  case 1:
    for(unsigned int k = 0, n = 0; k < salida.fileData.size(); k++, n++)
    {
      complex<double> muestra = n < fileData.size()? extraerMuestra(n) : complex<double>(0, 0);
      double magnitud = std::abs(X[k]);
      double raizDos = std::sqrt(complex<double>(2, 0)).real();
      complex<double> resultado(muestra.real(), map(magnitud, -raizDos*acum, raizDos*acum, -1, 1));
      salida.insertarMuestra(resultado, k);
    }
    break;
  case 2:
    for(unsigned int k = 0; k < fileData.size(); k++)
    {
      double magnitud = std::abs(X[k]);
      double fase = std::arg(X[k]);
      double raizDos = std::sqrt(complex<double>(2, 0)).real();
      double pi = std::acos(complex<double>(-1.0, 0)).real();
      complex<double> resultado( map(magnitud, -raizDos*acum, raizDos*acum, -1, 1), map(fase, -pi/4, pi/4, -1, 1));
      salida.insertarMuestra(resultado, k);
    }
    break;
  }
  return salida;
}

ArchivoWAV ArchivoWAV::transformadaInversa(const string& name, const bool rapida) const
{
  string str(name);
  if(name == "")
  {
    str = string(fileName);
    str.erase(str.end() - 4, str.end());
    str += " transformada inversa ";
    str += (rapida?"rapida":"lenta");
    str += ".wav";
  }
  ArchivoWAV salida(str, numeroCanales == 1? fileSize + tamanoAudio: fileSize, 2, frecuenciaMuestreo, bitsPorMuestra, numeroCanales == 1? 2*(tamanoAudio):tamanoAudio);
  valarray<complex<double>> x;
  double acum;
  if(rapida)
  {
    x = valarray<complex<double>>(fileData);
    transformadaRapida(x, std::acos(complex<double>(-1.0, 0)).real());
  }
  else
  {
    x = valarray<complex<double>>(fileData.size());
    transformadaFourier(x, true, salida);
  }
  acum = max(x);
  salida.fileData = x;
  salida.fileData /= complex<double>(acum, 0);
  return salida;
}

void ArchivoWAV::obtenerFrecuencias(vector<double>& cos, vector<double>& sin, const int partir) const
{
  double resolucionFrecuencia = double(frecuenciaMuestreo)/double(fileData.size());
  double acum;
  const valarray<complex<double>>& X = fileData;
  int ancho = X.size()/partir;
  for(unsigned int i = 0; i < X.size(); i += ancho)
  {
    std::cout << "Resolución en frecuencia: " << double(frecuenciaMuestreo)/double(ancho) << std::endl;
    valarray<complex<double>> f = X[std::slice(i, ancho, 1)];
    transformadaRapida(f, std::acos(complex<double>(-1.0, 0)).real()*-1);
    acum = max(f);
    f /= complex<double>(acum, 0);
    for(unsigned int i = 0; i < f.size()/2; i++)
    {
      if(std::abs(f[i].real()) > 0.5)
	cos.push_back(i*resolucionFrecuencia);
      if(std::abs(f[i].imag()) > 0.5)
	sin.push_back(i*resolucionFrecuencia);
    }
  }
  for(std::vector<double>::const_iterator i = cos.begin(), j = sin.begin(); i < cos.end() || j < sin.end(); i++, j++)
  {
    if(i < cos.end())
      std::cout << "Cos: " << *i << std::endl;
    if(j < sin.end())
      std::cout << "Sin: " << *j << std::endl;
  }
}
