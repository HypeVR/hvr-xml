// Copyright @ 2016 Caoyang Jiang & Qi Yao

#include "Hvr/XML/XMLNode.h"

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <iomanip>
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

XMLNode::XMLNode(const std::string &tag) : tag_(tag)
{
}

void XMLNode::PushBack(const XMLNode &subnode)
{
  subnodes_.push_back(subnode);
}

void XMLNode::Clear()
{
  subnodes_.clear();
}

void XMLNode::SetTag(const std::string &tag)
{
  tag_ = tag;
}

void XMLNode::SetText(const std::string &value)
{
  value_ = value;
}

void XMLNode::SetInt(const int &value)
{
  value_ = std::to_string(value);
}

void XMLNode::SetFloat(const float &value)
{
  std::ostringstream out;
  out << std::setprecision(12) << value;
  value_ = out.str();
}

void XMLNode::SetDouble(const double &value)
{
  std::ostringstream out;
  out << std::setprecision(12) << value;
  value_ = out.str();
}

void XMLNode::SetBool(const bool &value)
{
  if (value)
  {
    value_ = "true";
  }
  else
  {
    value_ = "false";
  }
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

const std::string &XMLNode::GetText() const
{
  return value_;
}

int XMLNode::GetInt() const
{
  return std::stoi(value_);
}

float XMLNode::GetFloat() const
{
  return std::stof(value_);
}

double XMLNode::GetDouble() const
{
  return std::stod(value_);
}

bool XMLNode::GetBool() const
{
  if (value_ == "true")
    return true;
  else
    return false;
}

const std::string &XMLNode::GetAttrByName(const std::string &attrname) const
{
  std::map<std::string, std::string>::const_iterator it =
      attributes_.find(attrname);
  if (it == attributes_.end())
  {
    return null_str;
  }
  return it->second;
}

bool XMLNode::GetAttrByName(const std::string &attrname,
                            std::string &attr) const
{
  std::map<std::string, std::string>::const_iterator it =
      attributes_.find(attrname);
  if (it == attributes_.end())
  {
    return false;
  }
  attr = it->second;
  return true;
}

const std::map<std::string, std::string> &XMLNode::GetAttrs() const
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
  err_info.second = XMLNode::NodeStat::node_bad;
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

bool XMLNode::IsValid() const
{
  return is_valid_;
}

XMLNode &XMLNode::operator[](int idx)
{
  int child_cnt = GetNumOfSubNodes();
  if (idx > child_cnt - 1 || IsValid() != true)
  {
    bad_child_ = std::make_shared<XMLNode>();
    bad_child_->SetNodeValidity(false);
    return *bad_child_;
  }
  XMLNode &tmp_node = subnodes_[idx];

  return tmp_node;
}

XMLNode XMLNode::operator[](int idx) const
{
  std::vector<std::pair<std::string, XMLNode::NodeStat>> err_list;
  int child_cnt = GetNumOfSubNodes();
  if (idx > child_cnt - 1 || IsValid() != true)
  {
    std::shared_ptr<XMLNode> bad_child = std::make_shared<XMLNode>();
    bad_child->SetNodeValidity(false);
    return *bad_child;
  }
  XMLNode tmp_node = subnodes_[idx];

  return tmp_node;
}

XMLNode &XMLNode::operator[](const std::string &tag)
{
  if (IsValid())
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

XMLNode XMLNode::operator[](const std::string &tag) const
{
  if (IsValid())
  {
    for (int i = 0; i < static_cast<int>(GetNumOfSubNodes()); i++)
    {
      if (subnodes_[i].GetTag() == tag)
      {
        return subnodes_[i];
      }
    }
  }
  std::shared_ptr<XMLNode> bad_child = std::make_shared<XMLNode>();
  bad_child->SetNodeValidity(false);
  return *bad_child;
}

bool XMLNode::QueryText(std::string &txt_val) const
{
  if (IsValid())
  {
    txt_val = GetText();
    return true;
  }

  return false;
}

bool XMLNode::QueryAttributes(std::map<std::string, std::string> &attrs) const
{
  if (IsValid())
  {
    attrs = GetAttrs();
    return true;
  }

  return false;
}

}  // namespace hvr
