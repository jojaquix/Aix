
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
if(WIN32)
 set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()
FetchContent_MakeAvailable(googletest)


add_executable(
  UTests
  BasicTests.cpp
)
target_link_libraries(
  UTests
  asio
  cxx_settings
  gtest_main
)

enable_testing()




include(GoogleTest)
gtest_discover_tests(UTests)
