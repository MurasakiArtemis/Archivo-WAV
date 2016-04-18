/*!
  \file   ArchivoR.hpp
  \author Erwin Hdz <erwin@linux-ph79>
  \date   Tue Apr 12 18:03:06 2016
  
  \brief  Declaración de la clase Archivo de lectura
  
  
*/

#ifndef ARCHIVOR_HPP

#define ARCHIVOR_HPP

#include "Archivo.hpp"
#include <fstream>

using std::ifstream;
using std::ios;
//! Clase Archivo
/*! 
  Extiende la clase Archivo para extraer información de un archivo de sólo lectura
 */
class ArchivoR : public Archivo
{
public:
  //! Constructor de la clase
  /*! 
    El tamaño de archivo y el arreglo de información se ajustan de acuerdo al contenido del archivo
    \param nombreArchivo El nombre del archivo que se va a manipular

  */
  ArchivoR(const string& nombreArchivo);
  //! Constructor copia
  /*! 
    
    \param arch El archivo a ser copiado

  */
  ArchivoR(const ArchivoR& arch);
  //! Destructor de la clase
  /*! 
    Cierra el ifstream empleado
    
  */
  virtual ~ArchivoR();
protected:
  ifstream fIn;			/*!< ifstream empleado para extraer información de disco */
};

#endif
