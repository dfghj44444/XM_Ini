cmake_minimum_required(VERSION 2.8)

PROJECT(xm_ini)

include_directories(${CMAKE_SOURCE_DIR})

# build

add_library(xm_ini SHARED  XM_IniFile.c)
set_target_properties(xm_ini
    PROPERTIES
    PUBLIC_HEADER "XM_IniFile.h"
    )

add_library(xm_ini-static STATIC XM_IniFile.c)
set_target_properties(xm_ini-static
    PROPERTIES
    OUTPUTNAME "xm_ini"
    PREFIX     "lib"
    PUBLIC_HEADER " XM_IniFile.h"
    )
    
#install
install(TARGETS xm_ini xm_ini-static
    RUNTIME DESTINATION xm_ini/bin
    LIBRARY DESTINATION xm_ini/lib
    ARCHIVE DESTINATION xm_ini/lib
    PUBLIC_HEADER DESTINATION xm_ini/include
    )

install(DIRECTORY ../src/ DESTINATION xm_ini
    FILES_MATCHING PATTERN "*.txt"
    PATTERN "CMakeLists.txt" EXCLUDE
    )
