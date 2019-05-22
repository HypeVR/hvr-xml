// Copyright 2019 HypeVR

#include "XMLTest.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <string>

#include "boost/filesystem.hpp"
HVR_WINDOWS_ENABLE_ALL_WARNING

#include "Hvr/XML/XMLParser.h"
#include "Hvr/XML/XMLWriter.h"

extern std::string exe_path;

XMLTest::XMLTest() = default;

XMLTest::~XMLTest() = default;

void XMLTest::SetUp()
{
}

void XMLTest::TearDown()
{
}

TEST_F(XMLTest, hvr_parser_XMLNode_Pushback_test)
{
  hvr::XMLNode node_p;
  hvr::XMLNode node_c;
  node_p.PushBack(node_c);
  ASSERT_EQ(static_cast<int>(node_p.GetNumOfSubNodes()), 1);
}

TEST_F(XMLTest, hvr_parser_XMLNode_SetTag_test)
{
  hvr::XMLNode node_p;
  std::string nam = "ABC";
  node_p.SetTag(nam);
  ASSERT_EQ(node_p.GetTag(), "ABC");
}

TEST_F(XMLTest, hvr_parser_XMLNode_SetText_test)
{
  hvr::XMLNode node_p;
  node_p.SetText("WHAT");
  ASSERT_EQ(node_p.GetText(), "WHAT");
}

TEST_F(XMLTest, hvr_parser_XMLNode_SetInt_test)
{
  hvr::XMLNode node_p;
  node_p.SetInt(123);
  ASSERT_EQ(node_p.GetInt(), 123);
}

TEST_F(XMLTest, hvr_parser_XMLNode_SetFloat_test)
{
  hvr::XMLNode node_p;
  node_p.SetFloat(1.234567f);
  ASSERT_EQ(std::abs(node_p.GetFloat() - 1.234567f) < 0.000001, true);
}

TEST_F(XMLTest, hvr_parser_XMLNode_SetDouble_test)
{
  hvr::XMLNode node_p;
  node_p.SetDouble(0.00000000101);
  ASSERT_EQ(std::abs(node_p.GetDouble() - 0.00000000101) < 0.000000000001,
            true);
}

TEST_F(XMLTest, hvr_parser_XMLNode_AddAttr_test)
{
  hvr::XMLNode node_p;
  node_p.AddAttr("name", "Richard");
  ASSERT_EQ(static_cast<int>(node_p.GetNumOfAttr()), 1);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetTag_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[1].GetTag(), "TEST02");
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetText_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[1]["VALTEXT"].GetText(), "When");
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetInt_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[0]["VALINT"].GetInt(), 888);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetFloat_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  std::cout << std::abs((*root)[0]["VALFLOAT"].GetDouble() - 1.234567)
            << std::endl;
  ASSERT_EQ(std::abs((*root)[0]["VALFLOAT"].GetDouble() - 1.234567) < 0.000001,
            true);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetDouble_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  std::cout << std::abs((*root)[0]["VALDOUBLE"].GetDouble() - 9.876543210123)
            << std::endl;

  ASSERT_EQ(std::abs((*root)[0]["VALDOUBLE"].GetDouble() - 9.876543210123) <
                0.000000000001,
            true);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetBool_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[1]["VALBOOL"].GetBool(), false);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetAttrByName_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  ASSERT_EQ((*root)[0].GetAttrByName("abc"), "__NULL__");
  ASSERT_EQ((*root)[0].GetAttrByName("attr"), "yes");
  std::string attr;
  ASSERT_EQ((*root)[0].GetAttrByName("abc", attr), false);
  (*root)[0].GetAttrByName("attr", attr);
  ASSERT_EQ(attr, "yes");
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetAttrs_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ((*root)[1].GetAttrs().at("attr"), "no");
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetNumOfAttr_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>((*root)[1].GetNumOfAttr()), 1);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetNumOfSubNodes_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>((*root)[0].GetNumOfSubNodes()), 7);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetSubNodeByIndex_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  hvr::XMLNode &node                 = (*root)[1];

  ASSERT_EQ(static_cast<int>(node.GetNumOfSubNodes()), 7);
}

TEST_F(XMLTest, hvr_parser_XMLNode_GetSubNodeByTag_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  hvr::XMLNode &node                 = (*root)["TEST02"];

  ASSERT_EQ(static_cast<int>(node.GetNumOfSubNodes()), 7);
}

TEST_F(XMLTest, hvr_parser_XMLNode_NodeErrorChecker_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      (exe_path + "/data/xml_data/xml_node_data/sample.xml");
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  hvr::XMLNode &node = root->GetSubNodeByTag("TEST03", err_list);
  bool err_stat      = node.NodeErrorChecker(err_list);
  ASSERT_EQ(err_stat, false);
}

TEST_F(XMLTest, hvr_parser_XMLWriter_Write_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path =
      exe_path + "/data/xml_data/xml_writer_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  hvr::XMLWriter cur_wrtr(root);

  std::string path_to_proc =
      exe_path + "/data/xml_data/xml_writer_data/result/";
  // position of "/"
  std::size_t pos = path_to_proc.find('/', 0);

  // loop through a given string of directory and create the missing directory
  // iteratively
  while (pos != std::string::npos)
  {
    std::string path_to_check = path_to_proc.substr(0, pos);
    boost::filesystem::path path(path_to_check);
    boost::system::error_code return_err;

    if (!boost::filesystem::is_directory(path))
    {
      boost::filesystem::create_directory(path, return_err);

      if (return_err.value() != boost::system::errc::success)
      {
        std::cerr << return_err << std::endl;
      }
    }

    // push "/" pos back and search for the next "/" pos
    pos = static_cast<int>(path_to_proc.find('/', pos + 1));
  }

  cur_wrtr.Write(exe_path + "/data/xml_data/xml_writer_data/result/test.xml");

  cur_prsr.Parse(exe_path + "/data/xml_data/xml_writer_data/result/test.xml");
  root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>(root->GetNumOfSubNodes()), 2);

  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  std::map<std::string, std::string> attrs =
      root->GetSubNodeByTag("TEST02", err_list).GetAttrs();

  ASSERT_EQ(attrs["attr"], "no");

  ASSERT_EQ(root->NodeErrorChecker(err_list), true);

  ASSERT_EQ(root->GetSubNodeByTag("TEST02", err_list)
                .GetSubNodeByIndex(0, err_list)
                .GetInt(),
            2);

  ASSERT_EQ(root->NodeErrorChecker(err_list), true);
}

TEST_F(XMLTest, hvr_parser_XMLNode_Brac_Op_Idx_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path = exe_path + "/data/xml_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  const hvr::XMLNode &root = *cur_prsr.GetRoot();

  const hvr::XMLNode &tmp_node = root[1][1];

  std::string val;
  ASSERT_EQ(tmp_node.QueryText(val), true);
  ASSERT_EQ(val, "4");
  const hvr::XMLNode tmp_node_const = root[1][1];
  val                               = "";
  ASSERT_EQ(tmp_node_const.QueryText(val), true);
  ASSERT_EQ(val, "4");
}

TEST_F(XMLTest, hvr_parser_XMLNode_Brac_Op_Tag_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path = exe_path + "/data/xml_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  hvr::XMLNode &tmp_node = (*root)["TEST02"]["VAL2"];
  std::string val;
  ASSERT_EQ(tmp_node.QueryText(val), true);
  ASSERT_EQ(val, "4");
  const hvr::XMLNode tmp_node_const = (*root)["TEST02"]["VAL2"];
  val                               = "";
  ASSERT_EQ(tmp_node_const.QueryText(val), true);
  ASSERT_EQ(val, "4");
}

TEST_F(XMLTest, hvr_parser_XMLNode_CheckNodeValidity_test)
{
  hvr::XMLNode tmp_node;

  ASSERT_EQ(tmp_node.IsValid(), true);
}

TEST_F(XMLTest, hvr_parser_XMLNode_SetNodeValidity_test)
{
  hvr::XMLNode tmp_node;
  tmp_node.SetNodeValidity(false);

  ASSERT_EQ(tmp_node.IsValid(), false);
}

TEST_F(XMLTest, hvr_parser_XMLNode_QueryText_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path = exe_path + "/data/xml_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  hvr::XMLNode &tmp_node = (*root)[2][1];
  std::string val;
  ASSERT_EQ(tmp_node.QueryText(val), false);

  hvr::XMLNode &tmp_node2 = (*root)["TEST01"]["VAL1"];
  val                     = "";
  ASSERT_EQ(tmp_node2.QueryText(val), true);
  ASSERT_EQ(val, "1");
}

TEST_F(XMLTest, hvr_parser_XMLNode_QueryAttributes_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path = exe_path + "/data/xml_data/xml_node_data/sample.xml";
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

TEST_F(XMLTest, hvr_parser_XMLParser_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path = exe_path + "/data/xml_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();

  ASSERT_EQ(static_cast<int>(root->GetNumOfSubNodes()), 2);
}

TEST_F(XMLTest, hvr_parser_XMLNode_Clear_test)
{
  hvr::XMLParser cur_prsr;
  std::string file_path = exe_path + "/data/xml_data/xml_node_data/sample.xml";
  cur_prsr.Parse(file_path);
  std::shared_ptr<hvr::XMLNode> root = cur_prsr.GetRoot();
  ASSERT_EQ(static_cast<int>(root->GetNumOfSubNodes()), 2);
  root->Clear();
  ASSERT_EQ(static_cast<int>(root->GetNumOfSubNodes()), 0);
}
