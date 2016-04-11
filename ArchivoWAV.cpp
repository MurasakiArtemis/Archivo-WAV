#include "ArchivoWAV.hpp"
#include <cstring>
#include <iostream>
ArchivoWAV::ArchivoWAV():
  numeroCanales(0), bitsPorMuestra(0), bytesPorMuestra(0), tamanoAudio(0), numeroMuestras(0), frecuenciaMuestreo(0)
{ }

ArchivoWAV::ArchivoWAV(unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio):
  numeroCanales(numCanales), bitsPorMuestra(bitsPerSample), bytesPorMuestra(bitsPerSample/8), tamanoAudio(tamAudio), numeroMuestras(tamAudio/(bitsPerSample/8)), frecuenciaMuestreo(frecMuestreo)
{ }

ArchivoWAV::ArchivoWAV(const ArchivoWAV& arch):
  numeroCanales(arch.numeroCanales), bitsPorMuestra(arch.bitsPorMuestra),
  bytesPorMuestra(arch.bytesPorMuestra), tamanoAudio(arch.tamanoAudio), numeroMuestras(arch.numeroMuestras)
{ }

ArchivoWAV::ArchivoWAV(const ArchivoWAV& arch, const ArchivoWAV& arch1):
  numeroCanales(arch.numeroCanales<arch1.numeroCanales?arch.numeroCanales:arch1.numeroCanales),
  bitsPorMuestra(arch.bitsPorMuestra<arch1.bitsPorMuestra?arch.bitsPorMuestra:arch1.bitsPorMuestra),
  bytesPorMuestra(arch.bytesPorMuestra<arch1.bytesPorMuestra?arch.bytesPorMuestra:arch1.bytesPorMuestra),
  tamanoAudio(arch.tamanoAudio<arch1.tamanoAudio?arch.tamanoAudio:arch1.tamanoAudio),
  numeroMuestras(arch.numeroMuestras<arch1.numeroMuestras?arch.numeroMuestras:arch1.numeroMuestras)
{ }

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

ArchivoWAV::~ArchivoWAV()
{ }

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
