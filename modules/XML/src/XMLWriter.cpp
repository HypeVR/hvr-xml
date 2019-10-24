// Copyright 2019 HypeVR

#include "Hvr/XML/XMLWriter.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <tinyxml2.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
HVR_WINDOWS_ENABLE_ALL_WARNING

namespace hvr
{
struct XMLWriter::Pimpl
{
  Pimpl()
  {
    root_ = std::make_shared<XMLNode>();
  }
  bool RecurseAppend(XMLNode &parent_node, tinyxml2::XMLElement *parent_elem);

  tinyxml2::XMLDocument wrtr_;
  std::shared_ptr<XMLNode> root_;
};

XMLWriter::XMLWriter()
{
  pi_ = std::make_unique<Pimpl>();
}
XMLWriter::XMLWriter(const std::shared_ptr<XMLNode> &in_node)
{
  pi_        = std::make_unique<Pimpl>();
  pi_->root_ = in_node;
}
XMLWriter::~XMLWriter() = default;

bool XMLWriter::AssignXMLNode(const std::shared_ptr<XMLNode> &in_node)
{
  if (in_node == nullptr)
  {
    return false;
  }
  pi_->root_ = in_node;
  return true;
}

bool XMLWriter::Write(const std::string &out_path)
{
  TranferInfo();
  tinyxml2::XMLError stat = pi_->wrtr_.SaveFile(out_path.c_str());
  return stat == tinyxml2::XML_SUCCESS;
}

bool XMLWriter::TranferInfo()
{
  std::string root_nam           = pi_->root_->GetTag();
  tinyxml2::XMLElement *xml_root = pi_->wrtr_.NewElement(root_nam.c_str());
  for (int i = 0; i < static_cast<int>(pi_->root_->GetNumOfSubNodes()); i++)
  {
    XMLNode &cur_node                            = (*pi_->root_)[i];
    std::string tmp_node_nam                     = cur_node.GetTag();
    std::string tmp_val                          = cur_node.GetText();
    std::map<std::string, std::string> tmp_attrs = cur_node.GetAttrs();
    tinyxml2::XMLElement *cur_elem =
        pi_->wrtr_.NewElement(tmp_node_nam.c_str());
    if (!tmp_val.empty())
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
      pi_->RecurseAppend(cur_node, cur_elem);
    }
    xml_root->InsertEndChild(cur_elem);
  }
  pi_->wrtr_.InsertFirstChild(xml_root);
  return true;
}

bool XMLWriter::Pimpl::RecurseAppend(XMLNode &parent_node,
                                     tinyxml2::XMLElement *parent_elem)
{
  for (int i = 0; i < static_cast<int>(parent_node.GetNumOfSubNodes()); i++)
  {
    XMLNode cur_node                             = parent_node[i];
    std::string tmp_node_nam                     = cur_node.GetTag();
    std::string tmp_val                          = cur_node.GetText();
    std::map<std::string, std::string> tmp_attrs = cur_node.GetAttrs();
    tinyxml2::XMLElement *cur_elem = wrtr_.NewElement(tmp_node_nam.c_str());
    if (!tmp_val.empty())
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
