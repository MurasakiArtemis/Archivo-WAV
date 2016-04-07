#ifndef ARCHIVO_HPP

#define ARCHIVO_HPP

#include <string>

using std::string;

class Archivo
{
  friend class ArchivoW;
  friend class ArchivoR;
public:
  Archivo(const string&, unsigned int=0, unsigned char* = nullptr);
  Archivo(const Archivo&);
  virtual ~Archivo();
  unsigned char extraerUByte(unsigned int) const;
  char extraerSByte(unsigned int) const;
  unsigned short extraerUShort(unsigned int) const;
  short extraerSShort(unsigned int) const;
  unsigned int extraerUInt(unsigned int) const;
  int extraerSInt(unsigned int) const;
  unsigned long extraerULong(unsigned int) const;
  long extraerSLong(unsigned int) const;
protected:
  string fileName;
  unsigned int fileSize;
  unsigned char* fileData;
};

#endif
