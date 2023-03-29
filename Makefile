out: main.o handleFile.o function.o
	g++ main.o handleFile.o function.o -o out
	rm *.o

main.o: main.cpp
	g++ -c main.cpp

handleFile.o: handleFile.cpp handleFile.hpp
	g++ -c handleFile.cpp

function.o: function.hpp function.cpp
	g++ -c function.cpp

clean:
	rm *.o out
