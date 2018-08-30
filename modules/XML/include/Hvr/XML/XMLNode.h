// Copyright @ 2016 Caoyang Jiang

#ifndef MODULES_XML_INCLUDE_HVR_XML_XMLNODE_H_
#define MODULES_XML_INCLUDE_HVR_XML_XMLNODE_H_

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace hvr
{
/**
 * @brief      XML parser uses XMLNode to store element information. This
 *             converts XML file to a tree data structure in memory.
 * @ingroup    XMLParser
 */
class XMLNode
{
 public:
  enum NodeStat
  {
    node_good,
    node_bad
  };

  HVR_XML_DLL
  XMLNode();

  HVR_XML_DLL
  ~XMLNode();

  /**
   * @brief      Constructs the object with a input string as Tag of node.
   *
   * @param[in]  tag   The tag
   */
  HVR_XML_DLL
  explicit XMLNode(const std::string &tag);

  /**
   * @brief      Adds a subnode.
   *
   * @param[in]  subnode  a node.
   */
  HVR_XML_DLL
  void PushBack(const XMLNode &subnode);

  /**
   * @brief      Delete all subnodes
   */
  HVR_XML_DLL
  void Clear();

  /**
   * @brief      Sets the element tag for current node. For example,  <tag ... >
   *             </tag>
   *
   * @param[in]  tag   Element tag.
   */
  HVR_XML_DLL
  void SetTag(const std::string &tag);

  /**
   * @brief      Sets the element text value for current node. For example,
   *             <tag>Value</tag>
   *
   * @param[in]  value  Element value.
   */
  HVR_XML_DLL
  void SetText(const std::string &value);

  /**
   * @brief      Sets the element int value for current node.
   *
   * @param[in]  value  The value
   */
  HVR_XML_DLL
  void SetInt(const int &value);

  /**
   * @brief      Sets the element float value for current node.
   *
   * @param[in]  value  The value
   */
  HVR_XML_DLL
  void SetFloat(const float &value);

  /**
   * @brief      Sets the element double value for current node.
   *
   * @param[in]  value  The value
   */
  HVR_XML_DLL
  void SetDouble(const double &value);

  /**
   * @brief      Sets the element boolean value for current node.
   *
   * @param[in]  value  The value
   */
  HVR_XML_DLL
  void SetBool(const bool &value);

  /**
   * @brief      Adds an element attribute.
   *
   * @param[in]  attrname      Attribute name.
   * @param[in]  attrproperty  Attribute property.
   */
  HVR_XML_DLL
  void AddAttr(const std::string &attrname, const std::string &attrproperty);

  /**
   * @brief      Gets the element tag.
   *
   * @return     element tag.
   */
  HVR_XML_DLL
  const std::string &GetTag() const;

  /**
   * @brief      Gets the element text value.
   *
   * @return     Element value.
   */
  HVR_XML_DLL
  const std::string &GetText() const;

  /**
   * @brief      Gets the element int value.
   *
   * @return     Element value.
   */
  HVR_XML_DLL
  int GetInt() const;

  /**
   * @brief      Gets the element float value.
   *
   * @return     Element value.
   */
  HVR_XML_DLL
  float GetFloat() const;

  /**
   * @brief      Gets the element double value.
   *
   * @return     Element value.
   */
  HVR_XML_DLL
  double GetDouble() const;

  /**
   * @brief      Gets the element boolean value.
   *
   * @return     Element value.
   */
  HVR_XML_DLL
  bool GetBool() const;

  /**
   * @brief      For attributes are known in advance, the attribute propoerty
   *             can be retreived with this API. Return a string of value
   *             "__NULL__" if the attribute doesn't exist
   *
   * @param[in]  attrname  Attribute name.
   *
   * @return     The attribute by name.
   */
  HVR_XML_DLL
  const std::string &GetAttrByName(const std::string &attrname) const;

  /**
   * @brief      For attributes are known in advance, the attribute propoerty
   *             can be retreived by reference. If attribute doesn't exist,
   *             function return false.
   *
   * @param[in]  attrname  Attribute name.
   * @param[out] attr      The attribute value
   *
   * @return     whether or not attribute exist.
   */
  HVR_XML_DLL
  bool GetAttrByName(const std::string &attrname, std::string &attr) const;

  /**
   * @brief      Get all attributes associated with this node.
   *
   * @return     All attributes.
   */
  HVR_XML_DLL
  const std::map<std::string, std::string> &GetAttrs() const;

  /**
   * @brief      Get number of attributes in this node.
   *
   * @return     The number of attribute.
   */
  HVR_XML_DLL
  size_t GetNumOfAttr() const;

  /**
   * @brief      Gets number of subnodes in this node.
   *
   * @return     The number of subnodes.
   */
  HVR_XML_DLL
  size_t GetNumOfSubNodes() const;

  /**
   * @brief      Gets a subnode by subnode index. Caller must ensure index is
   *             valid (using GetNumofSubNodes()).(this function is outdated and
   *             stays for backward compatability reason. Please refer to Sample
   *             app on how to retrieve data by [] operator )
   *
   * @param[in]  subnodeidx  The subnode index.
   * @param[out] err_list    vector holding error checking results.
   *
   * @return     A subnode.
   */
  HVR_XML_DLL
  XMLNode &GetSubNodeByIndex(
      size_t subnodeidx,
      std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list);

  /**
   * @brief      Gets a subnode by tag name. Caller must ensure tag name is
   *             valid. (this function is outdated and stays for backward
   *             compatability reason. Please refer to Sample app on how to
   *             retrieve data by [] operator )
   *
   * @param[in]  tag_name  The subnode tag name.
   * @param[out] err_list  vector holding error checking results.
   *
   * @return     A subnode.
   */
  HVR_XML_DLL
  XMLNode &GetSubNodeByTag(
      const std::string &tag_name,
      std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list);

  /**
   * @brief      checks for the error list result, wipes the error list after
   *             the check
   *
   * @param      err_list  error list
   *
   * @return     whether there is an error in the node retrieved
   */
  HVR_XML_DLL
  bool NodeErrorChecker(
      std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list) const;

  /**
   * @brief      Set the boolean value for private member variable is_valid_;
   *
   * @param[in]  b     input value;
   */
  HVR_XML_DLL
  void SetNodeValidity(const bool b);

  /**
   * @brief      Return the private member variable is_valid_ value
   *
   * @return     is_valid_ value
   */
  HVR_XML_DLL
  bool IsValid() const;

  /**
   * @brief      Retrieve the child node based on input index value
   *
   * @param[in]  idx     index value of child node
   */
  HVR_XML_DLL
  XMLNode &operator[](int idx);

  /**
   * @brief      Retrieve the child node based on input index value
   *
   * @param[in]  idx     index value of child node
   */
  HVR_XML_DLL
  XMLNode operator[](int idx) const;

  /**
   * @brief      Retrieve the first child node based on input tag name
   *
   * @param[in]  tag     tag name of child node
   */
  HVR_XML_DLL
  XMLNode &operator[](const std::string &tag);

  /**
   * @brief      Retrieve the first child node based on input tag name
   *
   * @param[in]  tag     tag name of child node
   */
  HVR_XML_DLL
  XMLNode operator[](const std::string &tag) const;

  /**
   * @brief      Queries the text value of the node holdes
   *
   * @param[out] txt_val  The text value
   *
   * @return     whether the operation succeeds
   */
  HVR_XML_DLL
  bool QueryText(std::string &txt_val) const;

  /**
   * @brief      Queries attributes in the form of a map.
   *
   * @param      attrs  The attributes
   *
   * @return     whether the operation succeeds
   */
  HVR_XML_DLL
  bool QueryAttributes(std::map<std::string, std::string> &attrs) const;

 private:
  // Subnodes
  std::vector<XMLNode> subnodes_;
  std::shared_ptr<XMLNode> bad_child_;
  // Node information
  std::string tag_     = "";
  std::string value_   = "";
  std::string null_str = "__NULL__";
  std::map<std::string, std::string> attributes_;
  // std::vector<std::pair<std::string, XMLNode::NodeStat>> err_list_;
  bool is_valid_ = true;
};

}  // namespace hvr

#endif  // MODULES_XML_INCLUDE_HVR_XML_XMLNODE_H_
