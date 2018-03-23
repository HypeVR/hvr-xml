// Copyright @ 2016 Caoyang Jiang

#include "Hvr/XMLParser/XMLNode.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
HVR_WINDOWS_ENABLE_ALL_WARNING

namespace hvr
{
XMLNode::XMLNode()
{
  // err_list_ = std::vector<std::pair<std::string, XMLNode::NodeStat>>();
}
XMLNode::~XMLNode()
{
}

void XMLNode::Pushback(const XMLNode &subnode)
{
  subnodes_.push_back(subnode);
}

void XMLNode::SetTag(const std::string &tag)
{
  tag_ = tag;
}

void XMLNode::SetValue(const std::string &value)
{
  value_ = value;
}

void XMLNode::AddAttr(const std::string &attrname,
                      const std::string &attrproperty)
{
  attributes_[attrname] = attrproperty;
}

const std::string &XMLNode::GetTag() const
{
  return tag_;
}

const std::string &XMLNode::GetValue() const
{
  return value_;
}

const std::string &XMLNode::LookUpAttrPropertyByName(
    const std::string &attrname) const
{
  return attributes_.find(attrname)->second;
}

const std::map<const std::string, std::string> &XMLNode::GetAttrs() const
{
  return attributes_;
}

size_t XMLNode::GetNumOfAttr() const
{
  return attributes_.size();
}

size_t XMLNode::GetNumOfSubNodes() const
{
  return subnodes_.size();
}

XMLNode &XMLNode::GetSubNodeByIndex(
    size_t subnodeidx,
    std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list)
{
  std::stringstream num_str;
  num_str << subnodeidx;
  std::pair<std::string, XMLNode::NodeStat> err_info;
  err_info.first  = num_str.str();
  err_info.second = XMLNode::NodeStat::node_good;
  err_list.push_back(err_info);
  return subnodes_[subnodeidx];
}

XMLNode &XMLNode::GetSubNodeByTag(
    const std::string &tag_name,
    std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list)
{
  std::pair<std::string, XMLNode::NodeStat> err_info;

  for (int i = 0; i < static_cast<int>(GetNumOfSubNodes()); i++)
  {
    if (subnodes_[i].GetTag() == tag_name)
    {
      err_info.first  = tag_name;
      err_info.second = XMLNode::NodeStat::node_good;
      return subnodes_[i];
    }
  }
  err_info.first  = tag_name;
  err_info.second = XMLNode::NodeStat::node_good;
  err_list.push_back(err_info);
  return subnodes_[0];
}

bool XMLNode::NodeErrorChecker(
    std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list) const
{
  for (size_t i = 0; i < err_list.size(); i++)
  {
    // sencond position of the std::pair holds value of an enum of whether node
    // is good or bad(defined in XMLNode class)
    if (err_list[i].second == XMLNode::NodeStat::node_bad)
    {
      std::cerr << "Lv 0 node at tier " << i << " called "
                << err_list[i].first.c_str() << " is not the correct node!"
                << std::endl;
      return false;
    }
  }
  err_list.clear();
  return true;
}

void XMLNode::SetNodeValidity(const bool b)
{
  is_valid_ = b;
}

bool XMLNode::CheckNodeValidity() const
{
  return is_valid_;
}

XMLNode &XMLNode::operator[](int idx)
{
  std::vector<std::pair<std::string, XMLNode::NodeStat>> err_list;
  int child_cnt = GetNumOfSubNodes();
  if (idx > child_cnt - 1 || CheckNodeValidity() != true)
  {
    bad_child_ = std::make_shared<XMLNode>();
    bad_child_->SetNodeValidity(false);
    return *bad_child_;
  }
  XMLNode &tmp_node = GetSubNodeByIndex(idx, err_list);

  return tmp_node;
}

XMLNode &XMLNode::operator[](const std::string &tag)
{
  if (CheckNodeValidity())
  {
    for (int i = 0; i < static_cast<int>(GetNumOfSubNodes()); i++)
    {
      if (subnodes_[i].GetTag() == tag)
      {
        return subnodes_[i];
      }
    }
  }
  bad_child_ = std::make_shared<XMLNode>();
  bad_child_->SetNodeValidity(false);
  return *bad_child_;
}

bool XMLNode::QueryText(std::string &txt_val) const
{
  if (CheckNodeValidity())
  {
    txt_val = GetValue();
    return true;
  }

  return false;
}

bool XMLNode::QueryAttributes(
    std::map<const std::string, std::string> &attrs) const
{
  if (CheckNodeValidity())
  {
    attrs = GetAttrs();
    return true;
  }

  return false;
}

}  // namespace hvr
