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
  Archivo(arch), fOut(arch.fileName, ios::binary)
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
  memcpy(fileData+posicion, &entero, sizeof(unsigned short));
}

void ArchivoW::escribirInt(unsigned int entero, unsigned int posicion)
{
  memcpy(fileData+posicion, &entero, sizeof(unsigned int));
}

void ArchivoW::escribirLong(unsigned long entero, unsigned int posicion)
{
  memcpy(fileData+posicion, &entero, sizeof(unsigned long));
}
