add_library(
    PowerSaver
    power-saver-script.h
    power-saver-script.cpp
)

target_include_directories(PowerSaver PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})