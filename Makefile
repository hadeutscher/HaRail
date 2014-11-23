CFLAGS = -static

all:
	echo run "make harail_gcc" or "make harail_mingw", according to your platform

harail_gcc: HaRail/gtfs.cpp HaRail/gtfs.h HaRail/paths.cpp HaRail/paths.h HaRail/port.h
	echo not supported yet
	
harail_mingw: HaRail/gtfs.cpp HaRail/gtfs.h HaRail/paths.cpp HaRail/paths.h HaRail/port.h
	mingw32-g++ $(CFLAGS) -ID:\clib\boost -LD:\clib\boost\stage_mingw\lib HaRail/paths.cpp HaRail/gtfs.cpp -lboost_filesystem-mgw48-mt-1_57 -lboost_system-mgw48-mt-1_57 -std=c++11 -o HaRail.exe