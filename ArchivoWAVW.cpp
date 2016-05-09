#include "ArchivoWAVM.hpp"

ArchivoWAVW::ArchivoWAVW(const string& nombreArchivo, unsigned int tamArchivo, unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio):
  ArchivoWAV(nombreArchivo, tamArchivo, numCanales, frecMuestreo, bitsPerSample, tamAudio);
  fOut(fileName, ios::binary)
{ };

ArchivoWAVW::ArchivoWAVW(const ArchivoWAV& arch1, const ArchivoWAVR& arch2, const string& nombreArchivo):
  ArchivoWAV(arch1, arch2, nombreArchivo),
  fOut(nombreArchivo, ios::binary)
{ }

ArchivoWAVW::ArchivoWAVW(const ArchivoWAV& arch, const string& nombreArchivo):
  ArchivoWAV(arch, nombreArchivo), fOut(fileName, ios::binary)
{ }

ArchivoWAVW::ArchivoWAVW(const ArchivoWAV& arch):
  ArchivoWAV(arch), fOut(fileName, ios::binary)
{ }

ArchivoWAVW::~ArchivoWAVW()
{
  fOut.write((char*)fileData, fileSize);
  fOut.close();
}

