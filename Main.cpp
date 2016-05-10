#include "ArchivoWAV.hpp"
#include "ArchivoWAVW.hpp"
#include "ArchivoWAVR.hpp"
#include <iostream>
#include <string>

void imprimeError(int opcion, char **argv)
{
  switch(opcion)
  {
  case 0:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <archivo de entrada> [<archivo de salida>]" << std::endl;
    break;
  case 1:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <archivo de entrada> <divisor>" << std::endl;
    break;
  case 2:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <archivo de entrada> [<archivo de salida>] [<número de muestras>=10] [<frecuencia>=3000] [ideal]" << std::endl;
    break;
  case 3:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <factor1> <factor2>" << std::endl;
    break;
  case 4:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <archivo de entrada> [lenta] [<archivo de salida>] [<opción>=0]" << std::endl;
    break;
  case 5:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <archivo de entrada> [lenta] [<archivo de salida>]" << std::endl;
    break;
  case 6:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <archivo de entrada> [<archivo de salida>]" << std::endl;
    break;
  case 7:
    std::cout << "El modo de uso es: " << argv[0] << " " << opcion << " <archivo de entrada> <filtro> [<archivo de salida>]" << std::endl;
    break;
  default:
    std::cout << "Opción inválida" << std::endl;
    break;
  }
}

int main(int argc, char *argv[])
{
  if(argc < 2)
  {
    std::cout << "El modo de uso es: " << argv[0] << " <opción> [<argumentos de opción>]^+" << std::endl;
    std::cout << "Opciones:\n0: Copia\n1: División\n2: Simular circuito\n3: Producto\n4: Transformada\n5: Transformada Inversa\n7: Convolución" << std::endl;
    return -1;
  }
  int opcion = std::stoi(argv[1]);
  if(argc == 2)
  {
    imprimeError(opcion, argv);
    return -1;
  }
  switch(opcion)
  {
  case 0:
    {
      if(argc < 3 || argc > 4)
      {
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      std::string str;
      if(argc == 4)
	str = argv[3];
      else
      {
	str = argv[2];
	str.erase(str.end() - 4, str.end());
	str += " - Copia.wav";
      }
      ArchivoWAVW b(a, str);
    }
    break;
  case 1:
    {
      if(argc != 4)
      {
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      int divisor = std::stoi(argv[3]);
      ArchivoWAVW b(a/divisor);
    }
    break;
  case 2:
    {
      if(argc < 3 || argc > 7)
      {	
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      std::string str;
      int numMuestras, frecCorte;
      if(argc < 4)
	str = "";
      else
	str = argv[3];
      if(argc < 5)
	numMuestras = 10;
      else
	numMuestras = std::stoi(argv[4]);
      if(argc < 6)
        frecCorte = 3000;
      else
        frecCorte = std::stoi(argv[5]);
      ArchivoWAVW b(a.simularCircuitoRC(str, numMuestras, frecCorte, argc == 7));
    }
    break;
  case 3:
    {
      if(argc != 4)
      {
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      ArchivoWAVR b(argv[3]);
      ArchivoWAVW c(a*b);
    }
    break;
  case 4:
    {
      if(argc < 3 || argc > 6)
      {
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      bool rapida = true;
      if(argc > 3)
	rapida = !(string(argv[3]) == "lenta");
      int transfOpcion = 0;
      std::string str;
      if(argc > 4)
	str = argv[4];
      if(argc > 5)
	transfOpcion = std::stoi(argv[5]);
      ArchivoWAVW b(a.transformadaFourier(str, rapida, transfOpcion));
    }
    break;
  case 5:
    {
      if(argc < 3 || argc > 5)
      {
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      bool rapida = true;
      if(argc > 3)
	rapida = !(string(argv[3]) == "lenta");
      std::string str;
      if(argc > 4)
	str = argv[4];
      ArchivoWAVW b(a.transformadaInversa(str, rapida));
    }
    break;
  case 6:
    {
      if(argc < 3 || argc > 4)
      {
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      std::string str;
      if(argc == 4)
	str = argv[3];
      ArchivoWAV b(a.transformadaFourier(str));
      ArchivoWAVW c(b.transformadaInversa(str));
    }
    break;
  case 7:
    {
      if(argc < 4 || argc > 5)
      {
	imprimeError(opcion, argv);
	return -1;
      }
      ArchivoWAVR a(argv[2]);
      ArchivoWAVR b(argv[3]);
      std::string str;
      if(argc == 5)
	str = argv[4];
      ArchivoWAVW c((a.transformadaFourier("")*b.transformadaFourier("")).transformadaInversa(str));
    }
    break;
  default:
    imprimeError(opcion, argv);
    break;
  }
  return 0;
}
