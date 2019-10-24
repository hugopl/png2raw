all:
	g++ -o png2raw -lpng -O2 main.cpp
install:
	install png2raw /usr/bin
