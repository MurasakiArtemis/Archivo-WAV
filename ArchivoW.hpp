#ifndef ARCHIVOW_HPP

#define ARCHIVOW_HPP

#include <fstream>
#include "Archivo.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;

class ArchivoW : public Archivo
{
public:
  ArchivoW(const string&, unsigned int=0, unsigned char* = nullptr);
  ArchivoW(const Archivo&, const string&);
  ArchivoW(const ArchivoW&);
  virtual ~ArchivoW();
  void escribirByte(unsigned char, unsigned int);
  void escribirShort(unsigned short, unsigned int);
  void escribirInt(unsigned int, unsigned int);
  void escribirLong(unsigned long, unsigned int);
protected:
  ofstream fOut;
};

#endif
