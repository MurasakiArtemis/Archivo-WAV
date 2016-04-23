#ifndef ARCHIVOWAVM_HPP

#define ARCHIVOWAVM_HPP

#include "ArchivoR.hpp"
#include "ArchivoW.hpp"
#include "ArchivoWAV.hpp"

class ArchivoWAVN;
class ArchivoWAVW;
class ArchivoWAVR;

class ArchivoWAVR : public ArchivoWAV, public ArchivoR
{
  friend class ArchivoWAVW;
  friend class ArchivoWAVN;
public:
  ArchivoWAVR(const string&);
  ArchivoWAVR(const ArchivoWAVR&);
  virtual ~ArchivoWAVR();
  virtual pair<short, short> extraerMuestra(const unsigned int) const;
  ArchivoWAVN operator/(const double&) const;
  ArchivoWAVN operator*(const ArchivoWAVR&) const;
  ArchivoWAVN simularCircuitoRC(const string&, unsigned int=10, unsigned int=3000) const;
  ArchivoWAVN transformadaFourier(const string&, const unsigned int=0) const;
  ArchivoWAVN transformadaInversa(const string&) const;
};

class ArchivoWAVW : public ArchivoWAV, public ArchivoW
{
  friend class ArchivoWAVR;
  friend class ArchivoWAVN;
public:
  ArchivoWAVW(const string& nombreArchivo, unsigned int tamArchivo, unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio);
  ArchivoWAVW(const ArchivoWAVR&, const ArchivoWAVR&, const string&);
  ArchivoWAVW(const ArchivoWAVR&, const string&);
  ArchivoWAVW(const ArchivoWAVW&);
  ArchivoWAVW(const ArchivoWAVN&);
  virtual ~ArchivoWAVW();
  virtual pair<short, short> extraerMuestra(const unsigned int) const;
  void insertarMuestra(pair<short, short>, unsigned int);
};

class ArchivoWAVN : public ArchivoWAV, public Archivo
{
  friend class ArchivoWAVW;
  friend class ArchivoWAVR;
public:
  ArchivoWAVN(const ArchivoWAVR&);
  ArchivoWAVN(const ArchivoWAVW&);
  ArchivoWAVN(const string& nombreArchivo, unsigned int tamArchivo, unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio);
  ArchivoWAVN(const ArchivoWAVR&, const ArchivoWAVR&, const string&);
  ArchivoWAVN(const ArchivoWAVR&, const string&);
  ArchivoWAVN(const ArchivoWAVN&, const ArchivoWAVN&, const string&);
  ArchivoWAVN(const ArchivoWAVN&, const string&);
  ArchivoWAVN(const ArchivoWAVN&);
  virtual ~ArchivoWAVN();
  virtual pair<short, short> extraerMuestra(const unsigned int) const;
  void insertarMuestra(pair<short, short>, unsigned int);
  ArchivoWAVN operator/(const double&) const;
  ArchivoWAVN operator*(const ArchivoWAVR&) const;
  ArchivoWAVN simularCircuitoRC(const string&, unsigned int=10, unsigned int=3000) const;
  ArchivoWAVN transformadaFourier(const string&, const unsigned int=0) const;
  ArchivoWAVN transformadaInversa(const string&) const;
};

#endif
 
