complier=g++
opt=-Wall -Werror -ansi -pedantic

all: 
	( [ ! -d bin ] && mkdir bin ) || [ -d bin ]	
	$(complier) $(opt) main.cpp
	
rshell:
	( [ ! -d bin ] && mkdir bin ) || [ -d bin ]
	$(complier) $(opt) main.cpp
