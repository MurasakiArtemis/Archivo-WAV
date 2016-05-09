Main: Main.cpp ArchivoWAVM.o
	g++ Main.cpp ArchivoWAVM.o ArchivoWAV.o -Wall -std=c++11 -o Main.out

ArchivoWAVM.o: ArchivoWAVM.cpp ArchivoWAVM.hpp ArchivoWAV.o
	g++ ArchivoWAVM.cpp -c -Wall -std=c++11

ArchivoWAV.o: ArchivoWAV.cpp ArchivoWAV.hpp
	g++ ArchivoWAV.cpp -c -Wall -std=c++11

clean:
	rm *.out *.o
