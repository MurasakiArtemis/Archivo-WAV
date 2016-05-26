#ifndef ARCHIVOWAVW_HPP

#define ARCHIVOWAVW_HPP

#include "ArchivoWAV.hpp"
#include <fstream>

using std::ios;
using std::string;
using std::ofstream;

class ArchivoWAVW : public ArchivoWAV
{
public:
  ArchivoWAVW(const string& nombreArchivo, unsigned int tamArchivo, unsigned short numCanales, unsigned int frecMuestreo, unsigned int bitsPerSample, unsigned int tamAudio);
  ArchivoWAVW(const ArchivoWAV&, const ArchivoWAV&, const string&);
  ArchivoWAVW(const ArchivoWAV& arch, const string& nombreArchivo);
  ArchivoWAVW(const ArchivoWAV& arch);
  virtual ~ArchivoWAVW();
private:
  ofstream fOut;		/*!< El ofstream empleado para escribir la información */
};

#endif
 
