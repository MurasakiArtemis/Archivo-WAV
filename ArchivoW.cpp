#include "ArchivoW.hpp"
#include <cstring>

ArchivoW::ArchivoW(const string& nombreArchivo, unsigned int tamano, unsigned char* datos):
Archivo(nombreArchivo, tamano, datos), fOut(fileName, ios::binary)
{ }

ArchivoW::ArchivoW(const Archivo& arch, const string& nombreArchivo):
  Archivo(nombreArchivo, arch.fileSize, new unsigned char[arch.fileSize]), fOut(fileName, ios::binary)
{
  memcpy(fileData, arch.fileData, fileSize);
}

ArchivoW::ArchivoW(const ArchivoW& arch):
  Archivo(arch), fOut(fileName, ios::binary)
{ }

ArchivoW::~ArchivoW()
{
  fOut.write((char*)fileData, fileSize);
  fOut.close();
}

void ArchivoW::escribirByte(unsigned char byte, unsigned int posicion)
{
  fileData[posicion] = byte;
}

void ArchivoW::escribirShort(unsigned short entero, unsigned int posicion)
{
  for(unsigned int i = 0; i < sizeof(entero); i++)
    escribirByte((unsigned char)(entero >> 8*i), posicion + i);
}

void ArchivoW::escribirInt(unsigned int entero, unsigned int posicion)
{
  for(unsigned int i = 0; i < sizeof(entero); i++)
    escribirByte((unsigned char)(entero >> 8*i), posicion + i);
}

void ArchivoW::escribirLong(unsigned long entero, unsigned int posicion)
{
  for(unsigned int i = 0; i < sizeof(entero); i++)
    escribirByte((unsigned char)(entero >> 8*i), posicion + i);
}
