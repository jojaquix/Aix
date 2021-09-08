
cmake_minimum_required (VERSION 3.8)

add_executable (AixServer "Server.h" "Server.cpp")

target_link_libraries(
  AixServer
  AixCommon
  cxx_settings
)