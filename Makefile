CC=g++
CFLAGS += -std=c++11
LDFLAGS += 

ifeq ($(OS), Windows_NT)
	BINARY = HaRail_gcc.exe
	RM = del /f
	CFLAGS += -ID:\clib\boost
else
	BINARY = harail
	RM = rm -f
endif

ifeq ($(ANDROID), 1)
	CC=arm-linux-gnueabihf-g++
	LDFLAGS += -static -Wl,--dynamic-linker=/system/bin/linker -Wl,--hash-style=both
endif

all: Release/$(BINARY)

Release/$(BINARY): HaRail/GCC_Release/ArgumentParser.o HaRail/GCC_Release/Graph.o HaRail/GCC_Release/GTFSDataSource.o HaRail/GCC_Release/IDataSource.o HaRail/GCC_Release/main.o HaRail/GCC_Release/StringTokenizer.o HaRail/GCC_Release/TestDataSource.o HaRail/GCC_Release/Utils.o
	$(CC) $+ $(LDFLAGS) -o $@

HaRail/GCC_Release/ArgumentParser.o: HaRail/ArgumentParser.cpp HaRail/ArgumentParser.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

HaRail/GCC_Release/Graph.o: HaRail/Graph.cpp HaRail/Graph.h HaRail/IDataSource.h HaRail/Station.h HaRail/Train.h HaRail/Node.h HaRail/Edge.h HaRail/HashedPair.h HaRail/Utils.h HaRail/HaException.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

HaRail/GCC_Release/GTFSDataSource.o: HaRail/GTFSDataSource.cpp HaRail/GTFSDataSource.h HaRail/IDataSource.h HaRail/Station.h HaRail/Train.h HaRail/Utils.h HaRail/StringTokenizer.h HaRail/HaException.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

HaRail/GCC_Release/IDataSource.o: HaRail/IDataSource.cpp HaRail/IDataSource.h HaRail/Station.h HaRail/Train.h HaRail/HaException.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

HaRail/GCC_Release/main.o: HaRail/main.cpp HaRail/ArgumentParser.h HaRail/IDataSource.h HaRail/GTFSDataSource.h HaRail/TestDataSource.h HaRail/Station.h HaRail/Train.h HaRail/Node.h HaRail/Edge.h HaRail/Graph.h HaRail/HashedPair.h HaRail/StringTokenizer.h HaRail/Utils.h HaRail/HaException.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

HaRail/GCC_Release/StringTokenizer.o: HaRail/StringTokenizer.cpp HaRail/StringTokenizer.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

HaRail/GCC_Release/TestDataSource.o: HaRail/TestDataSource.cpp HaRail/TestDataSource.h HaRail/IDataSource.h HaRail/Station.h HaRail/Train.h HaRail/Utils.h HaRail/HaException.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

HaRail/GCC_Release/Utils.o: HaRail/Utils.cpp HaRail/Utils.h HaRail/HaException.h HaRail/common.h
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean
clean:
	$(RM) Release/$(BINARY) HaRail/GCC_Release/*.o
