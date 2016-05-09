#include "ArchivoWAV.hpp"
#include <cstring>

ArchivoWAV::ArchivoWAV(const string& nombreArchivo, unsigned int tamano, unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio):
  numeroCanales(numCanales), bitsPorMuestra(bitsPerSample), bytesPorMuestra(bitsPerSample/8), tamanoAudio(tamAudio),
  numeroMuestras(tamAudio/(bitsPerSample/8)), frecuenciaMuestreo(frecMuestreo),
  fileName(nombreArchivo), fileSize(tamano),
  fileData(new unsigned char[fileSize])
{
  memset(fileData, 0, fileSize);
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
  fileData(new unsigned char[arch.fileSize<arch1.fileSize?arch.fileSize:arch1.fileSize])
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
  fileName(nombreArchivo), fileSize(arch.fileSize), fileData(new unsigned char[fileSize])
{ 
  memcpy(fileData, arch.fileData, fileSize);
}

ArchivoWAV::ArchivoWAV(const ArchivoWAV& arch):
  numeroCanales(arch.numeroCanales), bitsPorMuestra(arch.bitsPorMuestra),
  bytesPorMuestra(arch.bytesPorMuestra), tamanoAudio(arch.tamanoAudio), numeroMuestras(arch.numeroMuestras),
  frecuenciaMuestreo(arch.frecuenciaMuestreo),
  fileName(arch.fileName), fileSize(arch.fileSize), fileData(new unsigned char[fileSize])
{ 
  memcpy(fileData, arch.fileData, fileSize);
}

ArchivoWAV::~ArchivoWAV()
{
  delete[] fileData;
}

pair<short, short> ArchivoWAV::extraerMuestra(const unsigned int desplazamientoEstatico) const
{
  pair<short, short> muestra(0,0);
  muestra.first = extraerSShort(desplazamientoEstatico);
  if(numeroCanales == 2)
    muestra.second = extraerSShort(desplazamientoEstatico + bytesPorMuestra);
  return muestra;
}

void ArchivoWAV::insertarMuestra(pair<short, short> muestra, unsigned int posicion)
{
  escribirShort(muestra.first, posicion);
  if(numeroCanales == 2)
    escribirShort(muestra.second, posicion + bytesPorMuestra);
}

ArchivoWAV ArchivoWAV::operator/(const double& valor) const
{
  string str(fileName);
  str.erase(str.end() - 4, str.end());
  str += " dividido entre ";
  str += std::to_string((int)valor);
  str += ".wav";
  ArchivoWAV salida(*this, str);
  for(unsigned int i = 0; i < numeroMuestras; i += numeroCanales)
  {
    pair<short, short> muestra = extraerMuestra(44 + i*bytesPorMuestra);
    muestra.first /= valor;
    muestra.second /= valor;
    salida.insertarMuestra(muestra, 44 + i*bytesPorMuestra);
  }
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
  
  for(unsigned int i = 0, j = 0, k = 0; i < salida.numeroMuestras; i += salida.numeroCanales, j += numeroCanales, k += arch.numeroCanales)
  {
    pair<short, short> m1 = extraerMuestra(44 + j*bytesPorMuestra);
    pair<short, short> m2 = arch.extraerMuestra(44 + k*(arch.bytesPorMuestra));
    pair<short, short> resultado;
    complex<double> m1C = map(m1, -32768, 32768, -1, 1);
    complex<double> m2C = map(m2, -32768, 32768, -1, 1);
    complex<double> rC(0,0);
    rC = m1C*m2C;
    resultado = map(rC, -1, 1, -32768, 32768);
    salida.insertarMuestra(resultado, 44 + i*bytesPorMuestra);
  }
  return salida;
}

void ArchivoWAV::convolucion(unsigned int numMuestras, complex<double>* x, const double* h, ArchivoWAV& salida) const
{
  double acum = 0;
  for(unsigned int i = 0; i < numMuestras; i++)
    acum += h[i];
  for(unsigned int i = 0; i < numeroMuestras; i += numeroCanales)
  {
    pair<short, short> muestra = extraerMuestra(44 + i*bytesPorMuestra);
    complex<double> canal;
    for(int i = numMuestras-1; i > 0; i--)
      x[i] = x[i-1];
    x[0] = map(muestra, -32768, 32767, -1, 1);
    for(unsigned int i = 0; i < numMuestras; i++)
      canal += x[i]*h[i];
    pair<short, short> resultado = map(canal, -acum, acum, -32768, 32767);
    salida.insertarMuestra(resultado, 44 + i*bytesPorMuestra);
  }
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
  complex<double>* x = new complex<double>[numMuestras];
  if(frecCorte == 3000 && numMuestras == 10 && !ideal)
  {
    const double h[] = { 1.0, 0.652184637, 0.4253448, 0.277403344, 0.180918199, 0.11799207, 0.076952615, 0.050187313, 0.032731395, 0.021346913};
    this->convolucion(numMuestras, x, h, salida);
  }
  else if(frecCorte == 500 && numMuestras == 32 && ideal)
  {
    std::cout << "H" << std::endl;
    const double h[] = {0.99997, 0.67682, 0.06662, -0.21976, -0.06665, 0.12469, 0.06662, -0.08124, -0.06665, 0.05469, 0.06662, -0.03564, -0.06665, 0.02020, 0.06662, -0.00656, -0.06665, -0.00656, 0.06662, 0.02020, -0.06665, -0.03564, 0.06662, 0.05469, -0.06665, -0.08124, 0.06662, 0.12469, -0.06665, -0.21976, 0.06662, 0.67682};
    this->convolucion(numMuestras, x, h, salida);
  }
  delete[] x;
  return salida;
}

double ArchivoWAV::transformadaFourier(complex<double>* X, bool inversa, const ArchivoWAV& salida) const
{
  double acumR = -100, acumI = -100;
  const double pi = std::acos(complex<double>(-1.0, 0)).real()*(inversa?1:-1);
  for(unsigned int k = 0; k < salida.numeroMuestras; k += salida.numeroCanales)
  {
    for(unsigned int n = 0; n < numeroMuestras; n += numeroCanales)
    {
      pair<short, short> muestra = extraerMuestra(44 + n*(bytesPorMuestra));
      complex<double> x_n = map(muestra, -32768, 32767, -1, 1);
      complex<double> exponente = 2*pi*(k/2)*n/(numeroMuestras);
      complex<double> e = complex<double>(std::cos(exponente).real(), std::sin(exponente).real());
      X[k] += x_n*e;
    }
    if(X[k].real() > acumR)
      acumR = X[k].real();
    if(X[k].imag() > acumI)
      acumI = X[k].imag();
  }
  return acumR > acumI? acumR: acumI;
}

ArchivoWAV ArchivoWAV::transformadaFourier(const string& name, const unsigned int opcion) const
{
  string str(name);
  if(name == "")
  {
    str = string(fileName);
    str.erase(str.end() - 4, str.end());
    str += " transformado con opcion " + std::to_string(opcion) + ".wav";
  }
  ArchivoWAV salida(str, numeroCanales == 1? fileSize + tamanoAudio: fileSize, 2, frecuenciaMuestreo, bitsPorMuestra, numeroCanales == 1? 2*(tamanoAudio):tamanoAudio);
  complex<double>* X = new complex<double>[salida.numeroMuestras];
  double acum = transformadaFourier(X, false, salida);
  switch(opcion)
  {
  case 0:
    for(unsigned int k = 0; k < salida.numeroMuestras; k += salida.numeroCanales)
    {
      pair<short, short> resultado = map(X[k], -acum, acum, -32768, 32767);
      salida.insertarMuestra(resultado, 44 + k*bytesPorMuestra);
    }
    break;
  case 1:
    for(unsigned int k = 0, n = 0; k < salida.numeroMuestras; k += salida.numeroCanales, n += numeroCanales)
    {
      pair<short, short> muestra = extraerMuestra(44 + n*bytesPorMuestra);
      double magnitud = std::abs(X[k]);
      double raizDos = std::sqrt(complex<double>(2, 0)).real();
      pair<short, short> resultado(muestra.first, map(magnitud, -raizDos*acum, raizDos*acum, -32768, 32767));
      salida.insertarMuestra(resultado, 44 + k*bytesPorMuestra);
    }
    break;
  case 2:
    for(unsigned int k = 0; k < salida.numeroMuestras; k += salida.numeroCanales)
    {
      double magnitud = std::abs(X[k]);
      double fase = std::arg(X[k]);
      double raizDos = std::sqrt(complex<double>(2, 0)).real();
      double pi = std::acos(complex<double>(-1.0, 0)).real();
      pair<short, short> resultado(map(magnitud, -raizDos*acum, raizDos*acum, -32768, 32767), map(fase, -pi/4, pi/4, -32768, 32767));
      salida.insertarMuestra(resultado, 44 + k*bytesPorMuestra);
    }
    break;
  }
  delete[] X;
  return salida;
}

ArchivoWAV ArchivoWAV::transformadaInversa(const string& name) const
{
  string str(name);
  if(name == "")
  {
    str = string(fileName);
    str.erase(str.end() - 4, str.end());
    str += " transformada inversa.wav";
  }
  ArchivoWAV salida(str, numeroCanales == 1? fileSize + tamanoAudio: fileSize, 2, frecuenciaMuestreo, bitsPorMuestra, numeroCanales == 1? 2*(tamanoAudio):tamanoAudio);
  complex<double>* x = new complex<double>[salida.numeroMuestras];
  double acum = transformadaFourier(x, true, salida);
  for(unsigned int k = 0; k < salida.numeroMuestras; k += salida.numeroCanales)
  {
    pair<short, short> resultado = map(x[k], -acum, acum, -32768, 32767);
    salida.insertarMuestra(resultado, 44 + k*bytesPorMuestra);
  }
  delete[] x;
  return salida;
}

// double ArchivoWAV::transformadaRapida(complex<double>* X, bool inversa, const ArchivoWAV& salida) const
// {
//   //Si el arreglo tiene sólo dos elementos realizar el cálculo y regresar el número calculado
//   //Partir el arreglo en dos
//   //Procesar la transformadaRapida de los dos arreglos
//   //Sumar los resultados obtenidos del procesamiento de los arreglos
// }

// ArchivoWAV ArchivoWAV::transformadaRapida(const string& name, const unsigned int opcion) const
// {
// }

// ArchivoWAV ArchivoWAV::transformadaRapidaInversa(const string& name) const
// {
// }

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

unsigned char ArchivoWAV::extraerUByte(unsigned int i) const
{
  return fileData[i];
}

char ArchivoWAV::extraerSByte(unsigned int i) const
{
  return (char)fileData[i];
}

unsigned short ArchivoWAV::extraerUShort(unsigned int i) const
{
  return *((unsigned short*)(fileData + i));
}

short ArchivoWAV::extraerSShort(unsigned int i) const
{
  return *((short*)(fileData + i));
}

unsigned int ArchivoWAV::extraerUInt(unsigned int i) const
{
  return *((unsigned int*)(fileData + i));
}

int ArchivoWAV::extraerSInt(unsigned int i) const
{
  return *((int*)(fileData + i));
}

unsigned long ArchivoWAV::extraerULong(unsigned int i) const
{
  return *((unsigned long*)(fileData + i));
}

long ArchivoWAV::extraerSLong(unsigned int i) const
{
  return *((long*)(fileData + i));
}

void ArchivoWAV::escribirByte(unsigned char byte, unsigned int posicion)
{
  fileData[posicion] = byte;
}

void ArchivoWAV::escribirShort(unsigned short entero, unsigned int posicion)
{
  memcpy(fileData+posicion, &entero, sizeof(unsigned short));
}

void ArchivoWAV::escribirInt(unsigned int entero, unsigned int posicion)
{
  memcpy(fileData+posicion, &entero, sizeof(unsigned int));
}

void ArchivoWAV::escribirLong(unsigned long entero, unsigned int posicion)
{
  memcpy(fileData+posicion, &entero, sizeof(unsigned long));
}

std::ostream& operator<<(std::ostream& out, const ArchivoWAV& arch)
{
  out << "Número de canales: " << arch.numeroCanales << std::endl;
  out << "Bits por muestra: " << arch.bitsPorMuestra << std::endl;
  out << "Bytes por muestra: " << arch.bytesPorMuestra << std::endl;
  out << "Tamaño del audio: " << arch.tamanoAudio << std::endl;
  out << "Número de muestras: " << arch.numeroMuestras << std::endl;
  out << "Frecuencia de muestreo: " << arch.frecuenciaMuestreo << std::endl;
  return out;
}
