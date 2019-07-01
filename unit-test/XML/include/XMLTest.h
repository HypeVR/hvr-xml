// Copyright 2019 HypeVR

#ifndef UNIT_TEST_XML_INCLUDE_XMLTEST_H_
#define UNIT_TEST_XML_INCLUDE_XMLTEST_H_

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

#endif  // UNIT_TEST_XML_INCLUDE_XMLTEST_H_
