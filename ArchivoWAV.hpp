#ifndef ARCHIVOWAV_HPP

#define ARCHIVOWAV_HPP

#include <string>
#include <complex>
#include <utility>
#include <iostream>
#include <valarray>

using std::valarray;
using std::string;
using std::pair;
using std::complex;

double max(valarray<complex<double>> X);

class ArchivoWAV
{  
  friend std::ostream& operator<<(std::ostream&, const ArchivoWAV&);
  friend void transformadaRapida(valarray<complex<double>>& X, const double pi);
public:
  ArchivoWAV(const string& nombreArchivo, unsigned int tamano=0, unsigned short numCanales=0, unsigned int frecMuestreo=0, unsigned int bitsPerSample=0, unsigned int tamAudio=0);
  ArchivoWAV(const ArchivoWAV& arch, const ArchivoWAV& arch1, const string& nombreArchivo);
  ArchivoWAV(const ArchivoWAV& arch, const string& nombreArchivo);
  ArchivoWAV(const ArchivoWAV& arch);
  virtual ~ArchivoWAV();
  complex<double> extraerMuestra(const unsigned int) const;
  void insertarMuestra(const complex<double>&, unsigned int);
  ArchivoWAV operator/(const double&) const;
  ArchivoWAV operator*(const double&) const;
  ArchivoWAV operator*(const ArchivoWAV&) const;
  ArchivoWAV simularCircuitoRC(const string&, unsigned int=10, unsigned int=3000, bool ideal=false) const;
  ArchivoWAV transformadaFourier(const string& name, const bool rapida=true, const unsigned int opcion=0) const;
  ArchivoWAV transformadaInversa(const string&, const bool rapida=true) const;
  double map(double, double, double, double, double) const;
  complex<double> map(const pair<short, short>&, double, double, double, double) const;
  pair<short, short> map(const complex<double>&, double, double, double, double) const;
  valarray<complex<double>> map(const valarray<short>&, double, double, double, double) const;
  valarray<short> map(const valarray<complex<double>>&) const;
  short* map(valarray<short>);
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
  unsigned int numeroMuestras;
  unsigned short frecuenciaMuestreo;
  string fileName;		/*!< Nombre del archivo */
  unsigned int fileSize;	/*!< Tamaño del archivo */
  unsigned char* fileMetadata;	/*!< Bloque de meta información del archivo */
  valarray<complex<double>> fileData;		/*!< Bloque de información del archivo */
private:
  void convolucion(unsigned int numMuestras, const valarray<complex<double>> h, ArchivoWAV& salida) const;
  void transformadaFourier(valarray<complex<double>>& X, bool inversa, const ArchivoWAV& salida) const;
};

#endif
