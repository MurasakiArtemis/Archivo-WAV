#include "ArchivoWAVM.hpp"

ArchivoWAVR::ArchivoWAVR(const string& nombreArchivo):
  ArchivoR(nombreArchivo)
{
  numeroCanales = extraerUShort(22);
  frecuenciaMuestreo = extraerUInt(24);
  bitsPorMuestra = extraerUShort(34);
  bytesPorMuestra = bitsPorMuestra/8;
  tamanoAudio = extraerUInt(40);
  numeroMuestras = tamanoAudio/bytesPorMuestra;
}

ArchivoWAVR::ArchivoWAVR(const ArchivoWAVR& arch):
  ArchivoWAV(arch), ArchivoR(arch)
{ }

ArchivoWAVR::~ArchivoWAVR()
{ }

pair<short, short> ArchivoWAVR::extraerMuestra(const unsigned int desplazamientoEstatico) const
{
  pair<short, short> muestra(0,0);
  muestra.first = extraerSShort(desplazamientoEstatico);
    if(numeroCanales == 2)
      muestra.second = extraerSShort(desplazamientoEstatico + bytesPorMuestra);
  return muestra;
}

ArchivoWAVW ArchivoWAVR::operator/(const double& valor) const
{
  string str(fileName);
  str.erase(str.end() - 4, str.end());
  str += " dividido entre ";
  str += std::to_string((int)valor);
  str += ".wav";
  ArchivoWAVW salida(*this, str);
  for(unsigned int i = 0; i < numeroMuestras; i++)
  {
    pair<short, short> muestra = extraerMuestra(44 + i*bytesPorMuestra);
    muestra.first /= valor;
    muestra.second /= valor;
    salida.insertarMuestra(muestra, 44 + i*bytesPorMuestra);
  }
  return salida;
}

ArchivoWAVW ArchivoWAVR::operator*(const ArchivoWAVR& arch) const
{
  string str(fileName);
  str.erase(str.end() - 4, str.end());
  string str2(arch.fileName);
  str2.erase(str2.end() - 4, str2.end());
  str += " multiplicado con " + str2 + ".wav";
  ArchivoWAVW salida(*this, arch, str);
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

ArchivoWAVW ArchivoWAVR::simularCircuitoRC(const string& name, unsigned int numMuestras, unsigned int frecCorte) const
{
  string str(name);
  if(name == "")
  {
    str = string(fileName);
    str.erase(str.end() - 4, str.end());
    str += " filtrado a " + std::to_string(frecCorte) + ".wav";
  }
  ArchivoWAVW salida(*this, str);
  complex<double>* x = new complex<double>[numMuestras];
  if(frecCorte == 3000 && numMuestras == 10)
  {
    const double h[] = { 1.0, 0.652184637, 0.4253448, 0.277403344, 0.180918199, 0.11799207, 0.076952615, 0.050187313, 0.032731395, 0.021346913};
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
  delete[] x;
  return salida;
}

ArchivoWAVW ArchivoWAVR::transformadaFourier(const string& name, const unsigned int opcion)
{
  string str(name);
  if(name == "")
  {
    str = string(fileName);
    str.erase(str.end() - 4, str.end());
    str += " transformado con opcion " + std::to_string(opcion) + ".wav";
  }
  ArchivoWAVW salida(str, fileSize, 2, frecuenciaMuestreo, bitsPorMuestra, 2*(tamanoAudio));
  for(unsigned int k = 0; k < salida.numeroMuestras; k += salida.numeroCanales)
  {
    complex<double> X_k(0.0,0.0);
    for(unsigned int n = 0; n < numeroMuestras; n += numeroCanales)
    {
      pair<short, short> muestra = extraerMuestra(44 + n*bytesPorMuestra);
      complex<double> x_n = map(muestra, -32768, 32768, -1, 1);
      complex<double> exponente = 2*std::acos(complex<double>(-1.0, 0)).real()*(k/2)*n/numeroMuestras;
      complex<double> e = complex<double>(std::cos(exponente).real(), -std::sin(exponente).real());
      X_k += x_n*e;
    }
    std::cout << std::to_string(k/2) << ": " << X_k << std::endl;
    X_k /= 16;
    pair<short, short> resultado = map(X_k, -1, 1, -32768, 32768);
    salida.insertarMuestra(resultado, 44 + k*bytesPorMuestra);
  }
  return salida;
}

ArchivoWAVW::ArchivoWAVW(const string& nombreArchivo, unsigned int tamArchivo, unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio):
  ArchivoWAV(numCanales, frecMuestreo, bitsPerSample, tamAudio),
  ArchivoW(nombreArchivo, tamArchivo, new unsigned char[tamArchivo])
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
};

ArchivoWAVW::ArchivoWAVW(const ArchivoWAVR& arch1, const ArchivoWAVR& arch2, const string& nombreArchivo):
  ArchivoWAV(arch1, arch2),
  ArchivoW(nombreArchivo,
	   arch1.fileSize<arch2.fileSize?arch1.fileSize:arch2.fileSize,
	   new unsigned char[arch1.fileSize<arch2.fileSize?arch1.fileSize:arch2.fileSize])
{
  for(unsigned int i = 0; i < fileSize; i++)
  {
    switch(i)
    {
    case 22:
    case 23:
      escribirByte(arch1.numeroCanales < arch2.numeroCanales?arch1.extraerUByte(i):arch2.extraerUByte(i), i);
      break;
    case 34:
    case 35:
      escribirByte(arch1.bitsPorMuestra < arch2.bitsPorMuestra?arch1.extraerUByte(i):arch2.extraerUByte(i), i);
    case 40:
    case 41:
    case 42:
    case 43:
      escribirByte(arch1.tamanoAudio < arch2.tamanoAudio?arch1.extraerUByte(i):arch2.extraerUByte(i), i);
      break;
    default:
      escribirByte(arch1.extraerUByte(i), i);
      break;
    }
  }
}

ArchivoWAVW::ArchivoWAVW(const ArchivoWAVR& arch, const string& nombreArchivo):
  ArchivoWAV(arch), ArchivoW(arch, nombreArchivo)
{ }

ArchivoWAVW::ArchivoWAVW(const ArchivoWAVW& arch):
  ArchivoWAV(arch), ArchivoW(arch)
{ }

ArchivoWAVW::~ArchivoWAVW()
{ }

pair<short, short> ArchivoWAVW::extraerMuestra(const unsigned int desplazamientoEstatico) const
{
  pair<short, short> muestra(0,0);
  muestra.first = extraerSShort(desplazamientoEstatico);
    if(numeroCanales == 2)
      muestra.second = extraerSShort(desplazamientoEstatico + bytesPorMuestra);
  return muestra;
}

void ArchivoWAVW::insertarMuestra(pair<short, short> muestra, unsigned int posicion)
{
  escribirShort(muestra.first, posicion);
  if(numeroCanales == 2)
    escribirShort(muestra.second, posicion + bytesPorMuestra);
}
