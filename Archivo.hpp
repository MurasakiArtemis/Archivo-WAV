/*!
  \file   Archivo.hpp
  \author Erwin Hdz <erwin@linux-ph79>
  \date   Mon Apr 11 14:36:24 2016
  
  \brief  Declaración de la clase Archivo
*/

#ifndef ARCHIVO_HPP

#define ARCHIVO_HPP

#include <string>

using std::string;
//! Clase Archivo
/*! 
  Declaración de la clase Archivo, almacena el nombre de archivo, tamaño e información del mismo
  No proporciona medios para llenar la información.
  Proporciona métodos para extraer los distintos tipos de dato básicos del archivo
 */

class Archivo
{
  friend class ArchivoW;
  friend class ArchivoR;
public:
  //! Constructor básico para la clase Archivo
  /*! 
    Almacena nombre, tamaño y el arreglo de información
    \param nombreArchivo El nombre del archivo
    \param tamano El tamaño del archivo
    \param datos El arreglo de información del archivo
  */
  Archivo(const string& nombreArchivo, unsigned int tamano=0, unsigned char* datos=nullptr);
  //! Constructor copia para la clase
  /*! 
    Copia el nombre, tamaño e información del archivo en una nueva instancia
    \param arch El archivo a copiar
  */
  Archivo(const Archivo& arch);
  //! Destructor de la clase 
  /*! 
    Libera la memoria dinámica reservada para contener la información
  */
  virtual ~Archivo();
  //! Extrae un byte de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El byte extraído
  */
  unsigned char extraerUByte(unsigned int i) const;
  //! Extrae un byte con signo de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El byte extraído
  */
  char extraerSByte(unsigned int i) const;
  //! Extrae un entero corto sin signo de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El entero extraído
  */
  unsigned short extraerUShort(unsigned int i) const;
  //! Extrae un entero corto de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El entero extraído
  */
  short extraerSShort(unsigned int i) const;
  //! Extrae un entero sin signo de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El entero extraído
  */
  unsigned int extraerUInt(unsigned int i) const;
  //! Extrae un entero de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El entero extraído
  */
  int extraerSInt(unsigned int i) const;
  //! Extrae un entero largo sin signo de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El entero extraído
  */
  unsigned long extraerULong(unsigned int i) const;
  //! Extrae un entero largo de información del archivo
  /*! 
    
    \param i La posición desde donde se extraerá
    
    \return El entero extraído
  */
  long extraerSLong(unsigned int i) const;
protected:
  string fileName;		/*!< Nombre del archivo */
  unsigned int fileSize;	/*!< Tamaño del archivo */
  unsigned char* fileData;	/*!< Bloque de información del archivo */
};

#endif
