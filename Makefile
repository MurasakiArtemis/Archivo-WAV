Main: Main.cpp ArchivoWAVM.o
	g++ Main.cpp ArchivoWAVM.o ArchivoWAV.o Archivo.o ArchivoW.o ArchivoR.o -Wall -std=c++11 -o Main.out

ArchivoWAVM.o: ArchivoWAVM.cpp ArchivoWAVM.hpp ArchivoWAV.o ArchivoR.o ArchivoW.o
	g++ ArchivoWAVM.cpp -c -Wall -std=c++11

ArchivoWAV.o: ArchivoWAV.cpp ArchivoWAV.hpp
	g++ ArchivoWAV.cpp -c -Wall -std=c++11

ArchivoR.o: ArchivoR.cpp ArchivoR.hpp Archivo.o
	g++ ArchivoR.cpp -c -Wall -std=c++11
ArchivoW.o: ArchivoW.cpp ArchivoW.hpp Archivo.o
	g++ ArchivoW.cpp -c -Wall -std=c++11
Archivo.o: Archivo.cpp Archivo.hpp
	g++ Archivo.cpp -c -Wall -std=c++11


clean:
	rm *~ *.out *.o
