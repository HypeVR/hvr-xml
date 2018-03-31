// Copyright 2018 Qi Yao

#include "XMLParserTest.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <string>

#include "boost/filesystem.hpp"
HVR_WINDOWS_ENABLE_ALL_WARNING

#include "Hvr/XMLParser/XMLParser.h"
#include "Hvr/XMLParser/XMLWriter.h"

extern std::string exe_path;

XMLParserTest::XMLParserTest()
{
}

XMLParserTest::~XMLParserTest()
{
}

void XMLParserTest::SetUp()
{
}

void XMLParserTest::TearDown()
{
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_Pushback_test)
{
  hvr::XMLNode node_p;
  hvr::XMLNode node_c;
  node_p.Pushback(node_c);
  ASSERT_EQ(static_cast<int>(node_p.GetNumOfSubNodes()), 1);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_SetTag_test)
{
  hvr::XMLNode node_p;
  std::string nam = "ABC";
  node_p.SetTag(nam);
  ASSERT_EQ(node_p.GetTag(), "ABC");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_SetValue_test)
{
  hvr::XMLNode node_p;
  node_p.SetValue("123");
  ASSERT_EQ(node_p.GetValue(), "123");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_AddAttr_test)
{
  hvr::XMLNode node_p;
  node_p.AddAttr("name", "Richard");
  ASSERT_EQ(static_cast<int>(node_p.GetNumOfAttr()), 1);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetTag_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[1].GetTag(), "TEST02");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetValue_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[1][0].GetValue(), "3");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetAttrByName_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  ASSERT_EQ((*root)[0].GetAttrByName("abc"), "__NULL__");
  ASSERT_EQ((*root)[0].GetAttrByName("attr"), "yes");
  std::string attr = "";
  ASSERT_EQ((*root)[0].GetAttrByName("abc", attr), false);
  (*root)[0].GetAttrByName("attr", attr);
  ASSERT_EQ(attr, "yes");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetAttrs_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[1].GetAttrs().at("attr"), "no");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetNumOfAttr_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>((*root)[1].GetNumOfAttr()), 1);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetNumOfSubNodes_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>((*root)[0].GetNumOfSubNodes()), 2);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetSubNodeByIndex_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  hvr::XMLNode &node = root->GetSubNodeByIndex(1, err_list);

  ASSERT_EQ(static_cast<int>(node.GetNumOfSubNodes()), 2);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_GetSubNodeByTag_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  hvr::XMLNode &node = root->GetSubNodeByTag("TEST02", err_list);

  ASSERT_EQ(static_cast<int>(node.GetNumOfSubNodes()), 2);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_NodeErrorChecker_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xmlparser_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  hvr::XMLNode &node = root->GetSubNodeByTag("TEST03", err_list);
  bool err_stat      = node.NodeErrorChecker(err_list);
  ASSERT_EQ(err_stat, false);
}

TEST_F(XMLParserTest, hvr_parser_XMLWriter_Write_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      exe_path + "/data/xmlparser_data/xml_writer_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  hvr::XMLWriter cur_wrtr(root);

  std::string path_to_proc =
      exe_path + "/data/xmlparser_data/xml_writer_data/result/";
  // position of "/"
  int pos = static_cast<int>(path_to_proc.find("/", 0));

  // loop through a given string of directory and create the missing directory
  // iteratively
  while (pos != static_cast<int>(std::string::npos))
  {
    std::string path_to_check = path_to_proc.substr(0, pos);
    boost::filesystem::path path(path_to_check);
    boost::system::error_code return_err;

    if (!boost::filesystem::is_directory(path))
    {
      boost::filesystem::create_directory(path, return_err);

      if (return_err)
      {
        std::cerr << return_err << std::endl;
      }
    }

    // push "/" pos back and search for the next "/" pos
    pos = static_cast<int>(path_to_proc.find("/", pos + 1));
  }

  cur_wrtr.Write(exe_path +
                 "/data/xmlparser_data/xml_writer_data/result/test.xml");

  cur_prsr.Parse(exe_path +
                 "/data/xmlparser_data/xml_writer_data/result/test.xml");
  root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>(root->GetNumOfSubNodes()), 2);

  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  std::map<std::string, std::string> attrs =
      root->GetSubNodeByTag("TEST02", err_list).GetAttrs();

  ASSERT_EQ(attrs["attr"], "no");

  ASSERT_EQ(root->NodeErrorChecker(err_list), true);

  std::string val = root->GetSubNodeByTag("TEST02", err_list)
                        .GetSubNodeByIndex(0, err_list)
                        .GetValue();

  ASSERT_EQ(val, "2");

  ASSERT_EQ(root->NodeErrorChecker(err_list), true);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_Brac_Op_Idx_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      exe_path + "/data/xmlparser_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  hvr::XMLNode &tmp_node = (*root)[1][1];
  std::string val        = "";
  ASSERT_EQ(tmp_node.QueryText(val), true);
  ASSERT_EQ(val, "4");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_Brac_Op_Tag_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      exe_path + "/data/xmlparser_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  hvr::XMLNode &tmp_node = (*root)["TEST02"]["VAL2"];
  std::string val        = "";
  ASSERT_EQ(tmp_node.QueryText(val), true);
  ASSERT_EQ(val, "4");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_CheckNodeValidity_test)
{
  hvr::XMLNode tmp_node;

  ASSERT_EQ(tmp_node.CheckNodeValidity(), true);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_SetNodeValidity_test)
{
  hvr::XMLNode tmp_node;
  tmp_node.SetNodeValidity(false);

  ASSERT_EQ(tmp_node.CheckNodeValidity(), false);
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_QueryText_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      exe_path + "/data/xmlparser_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  hvr::XMLNode &tmp_node = (*root)[2][1];
  std::string val        = "";
  ASSERT_EQ(tmp_node.QueryText(val), false);

  hvr::XMLNode &tmp_node2 = (*root)["TEST01"]["VAL1"];
  val                     = "";
  ASSERT_EQ(tmp_node2.QueryText(val), true);
  ASSERT_EQ(val, "1");
}

TEST_F(XMLParserTest, hvr_parser_XMLNode_QueryAttributes_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      exe_path + "/data/xmlparser_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  hvr::XMLNode &tmp_node = (*root)[2][1];
  std::map<std::string, std::string> attrs;
  ASSERT_EQ(tmp_node.QueryAttributes(attrs), false);

  hvr::XMLNode &tmp_node2 = (*root)["TEST02"];
  std::map<std::string, std::string> attrs2;
  ASSERT_EQ(tmp_node2.QueryAttributes(attrs2), true);
  ASSERT_EQ(attrs2["attr"], "no");
}

TEST_F(XMLParserTest, hvr_parser_XMLParser__test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      exe_path + "/data/xmlparser_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>(root->GetNumOfSubNodes()), 2);
}
