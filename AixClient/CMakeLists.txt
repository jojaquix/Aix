﻿
cmake_minimum_required (VERSION 3.8)

add_executable (AixClient "Client.h" "Client.cpp")

target_link_libraries(
  AixClient
  AixCommon
  cxx_settings
)