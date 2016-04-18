#ifndef ARCHIVOWAV_HPP

#define ARCHIVOWAV_HPP

#include "ArchivoR.hpp"
#include "ArchivoW.hpp"
#include <string>
#include <complex>
#include <utility>
#include <iostream>

using std::string;
using std::pair;
using std::complex;

class ArchivoWAVR;

class ArchivoWAV
{
  friend std::ostream& operator<<(std::ostream&, const ArchivoWAV&);
public:
  ArchivoWAV();
  ArchivoWAV(unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio);
  ArchivoWAV(const ArchivoWAV&);
  ArchivoWAV(const ArchivoWAV&, const ArchivoWAV&);
  virtual ~ArchivoWAV();
  virtual double map(double, double, double, double, double) const;
  complex<double> map(const pair<short, short>&, double, double, double, double) const;
  pair<short, short> map(const complex<double>&, double, double, double, double) const;
  virtual pair<short, short> extraerMuestra(const unsigned int) const=0;
protected:
  unsigned short numeroCanales;
  unsigned short bitsPorMuestra;
  unsigned short bytesPorMuestra;
  unsigned int tamanoAudio;
  unsigned short numeroMuestras;
  unsigned short frecuenciaMuestreo;
};

#endif
