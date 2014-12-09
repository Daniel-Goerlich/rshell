compiler=g++
opt=-Wall -Werror -ansi -pedantic

all: 
	( ( [ ! -d bin ] && mkdir bin ) || [ -d bin ] )	
	cd bin; $(compiler) $(opt) ../src/main.cpp -o rshell
	cd bin; $(compiler) $(opt) ../src/cp.cpp -o cp
	cd bin; $(compiler) $(opt) ../src/ls.cpp -o ls

rshell:
	( ( [ ! -d bin ] && mkdir bin ) || [ -d bin ] )
	cd bin; $(compiler) $(opt) ../src/main.cpp -o rshell

cp:
	( ( [ ! -d bin ] && mkdir bin ) || [ -d bin ] )
	cd bin; $(compiler) $(opt) ../src/cp.cpp -o cp
ls:
	( ( [ ! -d bin ] && mkdir bin ) || [ -d bin ] )
	cd bin; $(compiler) $(opt) ../src/ls.cpp -o ls

