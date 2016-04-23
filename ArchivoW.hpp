/*!
  \file   ArchivoW.hpp
  \author Erwin Hdz <erwin@linux-ph79>
  \date   Tue Apr 12 18:29:30 2016
  
  \brief  Declaración de la clase ArchivoW
  
  
*/
#ifndef ARCHIVOW_HPP

#define ARCHIVOW_HPP

#include <fstream>
#include "Archivo.hpp"

using std::string;
using std::ifstream;
using std::ofstream;
using std::ios;
//! Clase ArchivoW
/*! 
  Extensión de la clase Archivo para archivos de lectura y escritura, proporciona métodos de escritura de tipos enteros básicos
 */

class ArchivoW : public Archivo
{
public:
  //! Constructor de la clase
  /*! 
    
    \param nombreArchivo El nombre del archivo que se creará
    \param tamano El tamaño del archivo que se creará
    \param datos La información del archivo que se creará
    
  */
  ArchivoW(const string& nombreArchivo, unsigned int tamano, unsigned char* datos);
  //! Constructor para copiar un archivo con un nombre distinto
  /*! 
    
    \param arch El archivo cuyo contenido se copiará
    \param nombreArchivo El nombre del nuevo archivo
    
  */
  ArchivoW(const Archivo& arch, const string& nombreArchivo);
  //! Constructor copia
  /*! Copia un archivo en otro, sin cambiar nombre ni ninguna propiedad
    
    \param arch El archivo a copiar
    
  */
  ArchivoW(const ArchivoW& arch);
  //! Destructor
  /*! 
    Vuelca la información a disco y cierra el ofstream empleado
    
  */
  virtual ~ArchivoW();
protected:
  ofstream fOut;		/*!< El ofstream empleado para escribir la información */
};

#endif
