// Copyright @ 2016 Caoyang Jiang & Qi Yao

#include "Hvr/XML/XMLWriter.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
HVR_WINDOWS_ENABLE_ALL_WARNING

namespace hvr
{
XMLWriter::XMLWriter()
{
  root_ = std::make_shared<XMLNode>();
}
XMLWriter::XMLWriter(const std::shared_ptr<XMLNode> in_node) : root_(in_node)
{
}
XMLWriter::~XMLWriter()
{
}

bool XMLWriter::AssignXMLNode(const std::shared_ptr<XMLNode> in_node)
{
  if (in_node == nullptr)
  {
    return false;
  }
  root_ = in_node;
  return true;
}

bool XMLWriter::Write(const std::string &out_path)
{
  TranferInfo();
  tinyxml2::XMLError stat = wrtr_.SaveFile(out_path.c_str());
  if (stat != tinyxml2::XML_SUCCESS)
  {
    return false;
  }
  return true;
}

bool XMLWriter::TranferInfo()
{
  std::string root_nam           = root_->GetTag();
  tinyxml2::XMLElement *xml_root = wrtr_.NewElement(root_nam.c_str());
  for (int i = 0; i < static_cast<int>(root_->GetNumOfSubNodes()); i++)
  {
    XMLNode &cur_node        = (*root_)[i];
    std::string tmp_node_nam = cur_node.GetTag();
    std::string tmp_val      = cur_node.GetText();
    if (tmp_val == "__EMPTY__")
    {
      tmp_val = "";
    }
    std::map<std::string, std::string> tmp_attrs = cur_node.GetAttrs();
    tinyxml2::XMLElement *cur_elem = wrtr_.NewElement(tmp_node_nam.c_str());
    if (tmp_val != "")
    {
      cur_elem->SetText(tmp_val.c_str());
    }

    for (std::map<std::string, std::string>::const_iterator it =
             tmp_attrs.begin();
         it != tmp_attrs.end();
         it++)
    {
      cur_elem->SetAttribute(it->first.c_str(), it->second.c_str());
    }
    if (cur_node.GetNumOfSubNodes() > 0)
    {
      RecurseAppend(cur_node, cur_elem);
    }
    xml_root->InsertEndChild(cur_elem);
  }
  wrtr_.InsertFirstChild(xml_root);
  return true;
}

bool XMLWriter::RecurseAppend(XMLNode &parent_node,
                              tinyxml2::XMLElement *parent_elem)
{
  for (int i = 0; i < static_cast<int>(parent_node.GetNumOfSubNodes()); i++)
  {
    XMLNode cur_node         = parent_node[i];
    std::string tmp_node_nam = cur_node.GetTag();
    std::string tmp_val      = cur_node.GetText();
    if (tmp_val == "__EMPTY__")
    {
      tmp_val = "";
    }
    std::map<std::string, std::string> tmp_attrs = cur_node.GetAttrs();
    tinyxml2::XMLElement *cur_elem = wrtr_.NewElement(tmp_node_nam.c_str());
    if (tmp_val != "")
    {
      cur_elem->SetText(tmp_val.c_str());
    }
    for (std::map<std::string, std::string>::const_iterator it =
             tmp_attrs.begin();
         it != tmp_attrs.end();
         it++)
    {
      cur_elem->SetAttribute(it->first.c_str(), it->second.c_str());
    }
    if (cur_node.GetNumOfSubNodes() > 0)
    {
      RecurseAppend(cur_node, cur_elem);
    }
    parent_elem->InsertEndChild(cur_elem);
  }
  return true;
}

}  // namespace hvr
