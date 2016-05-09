Main: Main.cpp ArchivoWAVR.o ArchivoWAVW.o ArchivoWAV.o
	g++ Main.cpp ArchivoWAVR.o ArchivoWAVW.o ArchivoWAV.o -Wall -std=c++11 -o Main.out
	cp Main.out ..

ArchivoWAVR.o: ArchivoWAVR.cpp ArchivoWAVR.hpp ArchivoWAV.o
	g++ ArchivoWAVR.cpp -c -Wall -std=c++11
ArchivoWAVW.o: ArchivoWAVW.cpp ArchivoWAVW.hpp ArchivoWAV.o
	g++ ArchivoWAVW.cpp -c -Wall -std=c++11
ArchivoWAV.o: ArchivoWAV.cpp ArchivoWAV.hpp
	g++ ArchivoWAV.cpp -c -Wall -std=c++11

clean:
	rm *.out *.o
