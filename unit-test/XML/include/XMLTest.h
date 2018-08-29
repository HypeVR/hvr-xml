// Copyright 2018 Qi Yao

#ifndef UNIT_TEST_XMLPARSER_INCLUDE_XMLTest_H_
#define UNIT_TEST_XMLPARSER_INCLUDE_XMLTest_H_

// HVR_WINDOWS_DISABLE_ALL_WARNING
#include "gtest/gtest.h"
// HVR_WINDOWS_ENABLE_ALL_WARNING

class XMLTest : public ::testing::Test
{
 protected:
  XMLTest();
  ~XMLTest() override;

  void SetUp() override;
  void TearDown() override;
};

#endif  // UNIT_TEST_XMLPARSER_INCLUDE_XMLTest_H_
