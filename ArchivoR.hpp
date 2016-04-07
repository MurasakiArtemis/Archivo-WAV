#ifndef ARCHIVOR_HPP

#define ARCHIVOR_HPP

#include "Archivo.hpp"
#include <fstream>

using std::ifstream;
using std::ios;

class ArchivoR : public Archivo
{
public:
  ArchivoR(const string&, unsigned int=0, unsigned char* = nullptr);
  ArchivoR(const ArchivoR&);
  virtual ~ArchivoR();
protected:
  ifstream fIn;
};

#endif
