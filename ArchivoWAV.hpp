#ifndef ARCHIVOWAV_HPP

#define ARCHIVOWAV_HPP

#include <string>
#include <complex>
#include <utility>
#include <iostream>

using std::string;
using std::pair;
using std::complex;

class ArchivoWAV
{  
  friend std::ostream& operator<<(std::ostream&, const ArchivoWAV&);
public:
  ArchivoWAV(const string& nombreArchivo, unsigned int tamano=0, unsigned short numCanales=0, unsigned int frecMuestreo=0, unsigned int bitsPerSample=0, unsigned int tamAudio=0);
  ArchivoWAV(const ArchivoWAV& arch, const ArchivoWAV& arch1, const string& nombreArchivo);
  ArchivoWAV(const ArchivoWAV& arch, const string& nombreArchivo);
  ArchivoWAV(const ArchivoWAV& arch);
  virtual ~ArchivoWAV();
  pair<short, short> extraerMuestra(const unsigned int) const;
  void insertarMuestra(pair<short, short>, unsigned int);
  ArchivoWAV operator/(const double&) const;
  ArchivoWAV operator*(const double&) const;
  ArchivoWAV operator*(const ArchivoWAV&) const;
  ArchivoWAV simularCircuitoRC(const string&, unsigned int=10, unsigned int=3000, bool ideal=false) const;
  ArchivoWAV transformadaFourier(const string&, const unsigned int=0) const;
  ArchivoWAV transformadaInversa(const string&) const;
  // ArchivoWAVN transformadaRapida(const string&, const unsigned int=0) const;
  // ArchivoWAVN transformadaRapidaInversa(const string&) const;
  double map(double, double, double, double, double) const;
  complex<double> map(const pair<short, short>&, double, double, double, double) const;
  pair<short, short> map(const complex<double>&, double, double, double, double) const;
  unsigned char extraerUByte(unsigned int i) const;
  char extraerSByte(unsigned int i) const;
  unsigned short extraerUShort(unsigned int i) const;
  short extraerSShort(unsigned int i) const;
  unsigned int extraerUInt(unsigned int i) const;
  int extraerSInt(unsigned int i) const;
  unsigned long extraerULong(unsigned int i) const;
  long extraerSLong(unsigned int i) const;
  void escribirByte(unsigned char byte, unsigned int posicion);
  void escribirShort(unsigned short entero, unsigned int posicion);
  void escribirInt(unsigned int entero, unsigned int posicion);
  void escribirLong(unsigned long entero, unsigned int posicion);
protected:
  unsigned short numeroCanales;
  unsigned short bitsPorMuestra;
  unsigned short bytesPorMuestra;
  unsigned int tamanoAudio;
  unsigned short numeroMuestras;
  unsigned short frecuenciaMuestreo;
  string fileName;		/*!< Nombre del archivo */
  unsigned int fileSize;	/*!< Tamaño del archivo */
  unsigned char* fileData;	/*!< Bloque de información del archivo */
private:
  void convolucion(unsigned int numMuestras, complex<double>* x, const double* h, ArchivoWAV& salida) const;
  double transformadaFourier(complex<double>* X, bool inversa, const ArchivoWAV& salida) const;
  // double transformadaRapida(complex<double>* X, bool inversa, const ArchivoWAV& salida) const;
};

#endif
