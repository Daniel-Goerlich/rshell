compiler=g++
opt=-Wall -Werror -ansi -pedantic

all: 
	( [ ! -d bin ] && mkdir bin ) || [ -d bin ] )	
	$(compiler) $(opt) main.cpp
	
rshell:
	( [ ! -d bin ] && mkdir bin ) || [ -d bin ] )
	$(compiler) $(opt) main.cpp

cp:
	( [ ! -d bin ] && mkdir bin ) || [ -d bin ] )
	$(compiler) $(opt) cp.cpp

