#ifndef ARCHIVOWAVM_HPP

#define ARCHIVOWAVM_HPP

#include "ArchivoR.hpp"
#include "ArchivoW.hpp"
#include "ArchivoWAV.hpp"

class ArchivoWAVW;
class ArchivoWAVR;

class ArchivoWAVR : public ArchivoWAV, public ArchivoR
{
  friend class ArchivoWAVW;
public:
  ArchivoWAVR(const string&);
  ArchivoWAVR(const ArchivoWAVR&);
  virtual ~ArchivoWAVR();
  virtual pair<short, short> extraerMuestra(const unsigned int) const;
  ArchivoWAVW operator/(const double&) const;
  ArchivoWAVW operator*(const ArchivoWAVR&) const;
  ArchivoWAVW simularCircuitoRC(const string&, unsigned int=10, unsigned int=3000) const;
  ArchivoWAVW transformadaFourier(const string&, const unsigned int=0);
};

class ArchivoWAVW : public ArchivoWAV, public ArchivoW
{
  friend class ArchivoWAVR;
public:
  ArchivoWAVW(const ArchivoWAVR&, const ArchivoWAVR&, const string&);
  ArchivoWAVW(const ArchivoWAVR&, const string&);
  ArchivoWAVW(const ArchivoWAVW&);
  virtual ~ArchivoWAVW();
  virtual pair<short, short> extraerMuestra(const unsigned int) const;
  void insertarMuestra(pair<short, short>, unsigned int);
};

#endif
 
