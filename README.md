HaRail
======

Public transport fastest-route finder for Israel Railways

BUILDING
--------------

OPTION 1: MSVC (Windows only)

You will need Visual Studio 2013 (the runtime is VC 12.0), and boost libraries v1.57 or newer. Open HaRail.sln in VS and edit the project include and lib directories to point to your boost installation.

OPTION 2: MinGW/GCC (Windows\Linux)

0. install boost (libboost-all-dev in debian)

1. Create the directories "Release" and "HaRail/GCC_Release"

2. Run mingw32-make or make, respectively. If you are compiling with mingw on Windows, edit Makefile to point to your boost include directory.

OPTION 3: Cross-GCC (Android)

0. install libboost-all-dev and arm-linux-gnueabihf-g++

1. Create the directories "Release" and "HaRail/GCC_Release"

2. Run make with ANDROID=1 in environment variables OR run the script android_build.sh which will also copy the output binary to your phone's /data/local/tmp directory

INSTALLING
--------------

The program expects to find a GTFS database; at C:\irw_gtfs for debug builds (~/irw_gtfs on linux) and at ./irw_gtfs for release builds.

You can get IRW_GTFS at ftp://gtfs.mot.gov.il/irw_gtfs.zip (see http://he.mot.gov.il/index.php?option=com_content&view=article&id=2244:pub-trn-gtfs&catid=167:pub-trn-dev-info&Itemid=304 for more info).

TL;DR:

1. Download ftp://gtfs.mot.gov.il/irw_gtfs.zip

2. Extract it to irw_gtfs directory in the same directory where you binary is
