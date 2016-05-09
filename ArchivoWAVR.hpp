#ifndef ARCHIVOWAVR_HPP

#define ARCHIVOWAVR_HPP

#include "ArchivoWAV.hpp"
#include <fstream>

using std::ios;
using std::string;
using std::ofstream;
using std::ifstream;

class ArchivoWAVR : public ArchivoWAV
{
public:
  ArchivoWAVR(const string& nombreArchivo);
  virtual ~ArchivoWAVR();
private:
  ifstream fIn;			/*!< ifstream empleado para extraer información de disco */
};

#endif
 
