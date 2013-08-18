cmake_minimum_required(VERSION 2.8)

PROJECT(XmIni)

include_directories(${CMAKE_SOURCE_DIR})

# build

add_library(XmIni SHARED Common.cc XmIniFile.cc)
set_target_properties(XmIni
    PROPERTIES
    PUBLIC_HEADER "Common.h XmIniFile.h"
    )

add_library(XmIni-static STATIC Common.cc XmIniFile.cc)
set_target_properties(XmIni-static
    PROPERTIES
    OUTPUTNAME "XmIni"
    PREFIX     "lib"
    PUBLIC_HEADER "Common.h XmIniFile.h"
    )
    
#install
install(TARGETS XmIni XmIni-static
    RUNTIME DESTINATION XmIni/bin
    LIBRARY DESTINATION XmIni/lib
    ARCHIVE DESTINATION XmIni/lib
    PUBLIC_HEADER DESTINATION XmIni/include
    )

install(DIRECTORY ../src/ DESTINATION XmIni
    FILES_MATCHING PATTERN "*.txt"
    PATTERN "CMakeLists.txt" EXCLUDE
    )
