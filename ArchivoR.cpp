#include "ArchivoR.hpp"

ArchivoR::ArchivoR(const string& nombreArchivo):
  Archivo(nombreArchivo)
{
  fIn = ifstream(nombreArchivo, ios::binary);
  fIn.seekg(0, ios::end);
  fileSize = fIn.tellg();
  fIn.seekg(0, ios::beg);
  fileData = new unsigned char[fileSize];  
  fIn.read((char*)fileData, fileSize);
}

ArchivoR::ArchivoR(const ArchivoR& arch):
  Archivo(arch), fIn(fileName, ios::binary)
{ }

ArchivoR::~ArchivoR()
{
  fIn.close();
}
