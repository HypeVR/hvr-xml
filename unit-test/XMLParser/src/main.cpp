// Copyright 2015 Jason Juang

#include <string>

HVR_WINDOWS_DISABLE_ALL_WARNING
#include "gtest/gtest.h"
HVR_WINDOWS_ENABLE_ALL_WARNING

std::string exe_path = "";

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);

  exe_path = argv[0];
  exe_path = exe_path.substr(0, exe_path.find_last_of("/\\"));
  return RUN_ALL_TESTS();
}
