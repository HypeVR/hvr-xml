// Copyright @ 2016 Caoyang Jiang & Qi Yao

#include "Hvr/XML/XMLParser.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
HVR_WINDOWS_ENABLE_ALL_WARNING

namespace hvr
{
XMLParser::XMLParser()
{
  root_ = std::make_shared<XMLNode>();
}
XMLParser::~XMLParser() = default;

bool XMLParser::Parse(const std::string &filename)
{
  root_ = std::make_shared<XMLNode>();
  tinyxml2::XMLDocument cur_doc;
  tinyxml2::XMLError err_msg = cur_doc.LoadFile(filename.c_str());

  if (err_msg != 0)
  {
    // std::cerr << err_msg << std::endl;
    return false;
  }

  tinyxml2::XMLElement *cur_handle = cur_doc.FirstChildElement();

  if (cur_handle == nullptr)
  {
    std::cerr << "xml retrieving root node failed!" << std::endl;
    return false;
  }

  root_->SetTag(std::string(cur_handle->Name()));

  for (const tinyxml2::XMLAttribute *i = cur_handle->FirstAttribute();
       i != nullptr;
       i = i->Next())
  {
    root_->AddAttr(std::string(i->Name()), std::string(i->Value()));
  }

  reader_t_ = cur_handle;

  // Recursively parsing nodes
  return RecurseProc(*root_);
}

std::shared_ptr<XMLNode> &XMLParser::GetRoot()
{
  return root_;
}

bool XMLParser::RecurseProc(XMLNode &parent_node)
{
  for (tinyxml2::XMLHandle i(reader_t_->FirstChildElement());
       i.ToElement() != nullptr;
       i = i.NextSiblingElement())
  {
    XMLNode node;
    node.SetTag(std::string(i.ToElement()->Name()));
    std::string tag_name = i.ToElement()->Name();

    for (const tinyxml2::XMLAttribute *j = i.ToElement()->FirstAttribute();
         j != nullptr;
         j = j->Next())
    {
      node.AddAttr(std::string(j->Name()), std::string(j->Value()));
    }
    if (i.ToElement()->GetText() != nullptr)
    {
      node.SetText(std::string(i.ToElement()->GetText()));
      // std::string cur_text = std::string(i.ToElement()->GetText());
    }
    else
    {
      node.SetText("__EMPTY__");
    }

    reader_t_ = i.ToElement();

    if (reader_t_ != nullptr)
    {
      RecurseProc(node);
    }
    parent_node.PushBack(node);
  }

  return true;
}

}  // namespace hvr
