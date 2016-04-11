#include "Archivo.hpp"
#include <cstring>

Archivo::Archivo(const string& nombreArchivo, unsigned int tamano, unsigned char* datos):
fileName(nombreArchivo), fileSize(tamano), fileData(datos)
{
  memset(fileData, 0, fileSize);
}

Archivo::Archivo(const Archivo& arch):
fileName(arch.fileName), fileSize(arch.fileSize), fileData(new unsigned char[fileSize])
{ 
  memcpy(fileData, arch.fileData, fileSize);
}

unsigned char Archivo::extraerUByte(unsigned int i) const
{
  return fileData[i];
}

char Archivo::extraerSByte(unsigned int i) const
{
  return (char)fileData[i];
}

unsigned short Archivo::extraerUShort(unsigned int i) const
{
  return *((unsigned short*)(fileData + i));//(((unsigned short)fileData[i]) | ((unsigned short)fileData[i+1]) << 8);
}

short Archivo::extraerSShort(unsigned int i) const
{
  return (((short)fileData[i]) | ((short)fileData[i+1]) << 8);
}

unsigned int Archivo::extraerUInt(unsigned int i) const
{
  return (((unsigned int)fileData[i]) | ((unsigned int)fileData[i+1]) << 8 | ((unsigned int)fileData[i+2]) << 16 | ((unsigned int)fileData[i+3]) << 24);
}

int Archivo::extraerSInt(unsigned int i) const
{
  return (((int)fileData[i]) | ((int)fileData[i+1]) << 8 | ((int)fileData[i+2]) << 16 | ((int)fileData[i+3]) << 24);
}

unsigned long Archivo::extraerULong(unsigned int i) const
{
  return (((unsigned long)fileData[i]) | ((unsigned long)fileData[i+1]) << 8 | ((unsigned long)fileData[i+2]) << 16 | ((unsigned long)fileData[i+3]) << 24);
}

long Archivo::extraerSLong(unsigned int i) const
{
  return (((long)fileData[i]) | ((long)fileData[i+1]) << 8 | ((long)fileData[i+2]) << 16 | ((long)fileData[i+3]) << 24);
}

Archivo::~Archivo()
{
  delete[] fileData;
}
