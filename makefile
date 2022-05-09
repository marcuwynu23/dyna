#constants variable
BIN=bin/dyna.exe
SRC=$(wildcard src/*.cpp)

# compiling source file
all:
	c++ -std=c++11 -o $(BIN)  -I include $(SRC) 

# running executable
run:
	$(BIN) -i rd 0


# cleaning executable
clean:
	del $(BIN)
