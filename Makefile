CC = g++
CFLAGS = -std=c++17 -g -Wall -pthread
BIN = binaries

all: build
	./$(BIN)/Server.exe

# Make /binaries folder if doesn't exist.
$(BIN):
	if [ ! -d "/$(BIN)" ]; then mkdir $(BIN); fi

files:
	if [ ! -d "/files" ]; then mkdir files; fi

build: $(BIN) files | $(BIN)/responseinfo.o $(BIN)/server.o $(BIN)/responsehelper.o $(BIN)/requestinfo.o $(BIN)/clienthandler.o $(BIN)/requesthandler.o $(BIN)/colors.o
	$(CC) $(CFLAGS) $(BIN)/colors.o $(BIN)/server.o $(BIN)/responseinfo.o $(BIN)/responsehelper.o $(BIN)/requestinfo.o $(BIN)/clienthandler.o $(BIN)/requesthandler.o -o $(BIN)/Server.exe

$(BIN)/server.o: src/server.cpp
	$(CC) $(CFLAGS) -c src/server.cpp -o $(BIN)/server.o

$(BIN)/responsehelper.o: src/responsehelper.hpp src/responsehelper.cpp
	$(CC) $(CFLAGS) -c src/responsehelper.cpp -o $(BIN)/responsehelper.o

$(BIN)/requestinfo.o: src/requestinfo.hpp src/requestinfo.cpp
	$(CC) $(CFLAGS) -c src/requestinfo.cpp -o $(BIN)/requestinfo.o

$(BIN)/clienthandler.o: src/requesthandler.hpp src/clienthandler.hpp  src/clienthandler.cpp
	$(CC) $(CFLAGS) -c src/clienthandler.cpp -o $(BIN)/clienthandler.o

$(BIN)/requesthandler.o: src/requestinfo.hpp src/requesthandler.hpp src/responsehelper.hpp src/requesthandler.cpp
	$(CC) $(CFLAGS) -c src/requesthandler.cpp -o $(BIN)/requesthandler.o

$(BIN)/responseinfo.o: src/responseinfo.hpp src/responseinfo.cpp
	$(CC) $(CFLAGS) -c src/responseinfo.cpp -o $(BIN)/responseinfo.o

$(BIN)/colors.o: src/colors.hpp src/colors.cpp
	$(CC) $(CFLAGS) -c src/colors.cpp -o $(BIN)/colors.o


clean:
	rm $(BIN)/*.o
	rm $(BIN)/*.bin
	rm $(BIN)/Server.exe
	rm -r files
	rmdir $(BIN)

debug: build
	gdb Server.exe