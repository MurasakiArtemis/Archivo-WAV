#include "ArchivoWAVR.hpp"
#include <cstring>

ArchivoWAVR::ArchivoWAVR(const string& nombreArchivo):
  ArchivoWAV(nombreArchivo)
{
  fIn = ifstream(nombreArchivo, ios::binary);
  fIn.seekg(0, ios::end);
  fileSize = fIn.tellg();
  fIn.seekg(0, ios::beg);
  if(fileMetadata != nullptr)
    delete[] fileMetadata;
  fileMetadata = new unsigned char[fileSize];
  fIn.read((char*)fileMetadata, fileSize);
  numeroCanales = extraerUShort(22);
  bitsPorMuestra = extraerUShort(34);
  bytesPorMuestra = bitsPorMuestra/8;
  tamanoAudio = extraerUInt(40);
  numeroMuestras = tamanoAudio/bytesPorMuestra;
  frecuenciaMuestreo = extraerUShort(24);
  valarray<short> temporaryData = valarray<short>((short*)(fileMetadata+44), numeroMuestras);
  fileData = map(temporaryData, -32768, 32767, -1, 1);
}

ArchivoWAVR::~ArchivoWAVR()
{
  if(fIn.is_open())
    fIn.close();
}


