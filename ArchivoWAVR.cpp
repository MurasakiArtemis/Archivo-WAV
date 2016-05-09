#include "ArchivoWAVR.hpp"

ArchivoWAVR::ArchivoWAVR(const string& nombreArchivo):
  ArchivoWAV(nombreArchivo)
{
  fIn = ifstream(nombreArchivo, ios::binary);
  fIn.seekg(0, ios::end);
  fileSize = fIn.tellg();
  fIn.seekg(0, ios::beg);
  if(fileData != nullptr)
    delete[] fileData;
  fileData = new unsigned char[fileSize];
  fIn.read((char*)fileData, fileSize);
  numeroCanales = extraerUShort(22);
  bitsPorMuestra = extraerUShort(34);
  bytesPorMuestra = bitsPorMuestra/8;
  tamanoAudio = extraerUInt(40);
  numeroMuestras = tamanoAudio/bytesPorMuestra;
  frecuenciaMuestreo = extraerUShort(24);
}

ArchivoWAVR::~ArchivoWAVR()
{
  if(fIn.is_open())
    fIn.close();
}


