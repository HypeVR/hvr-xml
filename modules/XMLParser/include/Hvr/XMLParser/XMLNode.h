// Copyright @ 2016 Caoyang Jiang

#ifndef MODULES_XMLPARSER_INCLUDE_HVR_XMLPARSER_XMLNODE_H_
#define MODULES_XMLPARSER_INCLUDE_HVR_XMLPARSER_XMLNODE_H_

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

  HVR_XMLPARSER_DLL XMLNode();
  HVR_XMLPARSER_DLL ~XMLNode();

  /**
   * @brief      Adds a subnode.
   *
   * @param[in]  subnode  a node.
   */
  HVR_XMLPARSER_DLL
  void Pushback(const XMLNode &subnode);

  /**
   * @brief      Sets the element tag for current node. For example,  <tag ... >
   *             </tag>
   *
   * @param[in]  tag   Element tag.
   */
  HVR_XMLPARSER_DLL
  void SetTag(const std::string &tag);

  /**
   * @brief      Sets the element value for current node. For example, <tag ...
   *             >Value</tag>
   *
   * @param[in]  value  Element value.
   */
  HVR_XMLPARSER_DLL
  void SetValue(const std::string &value);

  /**
   * @brief      Adds an element attribute.
   *
   * @param[in]  attrname      Attribute name.
   * @param[in]  attrproperty  Attribute property.
   */
  HVR_XMLPARSER_DLL
  void AddAttr(const std::string &attrname, const std::string &attrproperty);

  /**
   * @brief      Gets the element tag.
   *
   * @return     element tag.
   */
  HVR_XMLPARSER_DLL
  const std::string &GetTag() const;

  /**
   * @brief      Gets the element value.
   *
   * @return     Element value.
   */
  HVR_XMLPARSER_DLL
  const std::string &GetValue() const;

  /**
   * @brief      For attributes are known in advance, the attribute propoerty
   *             can be retreived with this API.
   *
   * @param[in]  attrname  Attribute name.
   *
   * @return     { description_of_the_return_value }
   */
  HVR_XMLPARSER_DLL
  const std::string &LookUpAttrPropertyByName(
      const std::string &attrname) const;

  /**
   * @brief      Get all attributes associated with this node.
   *
   * @return     All attributes.
   */
  HVR_XMLPARSER_DLL
  const std::map<const std::string, std::string> &GetAttrs() const;

  /**
   * @brief      Get number of attributes in this node.
   *
   * @return     The number of attribute.
   */
  HVR_XMLPARSER_DLL
  size_t GetNumOfAttr() const;

  /**
   * @brief      Gets number of subnodes in this node.
   *
   * @return     The number of subnodes.
   */
  HVR_XMLPARSER_DLL
  size_t GetNumOfSubNodes() const;

  /**
   * @brief      Gets a subnode by subnode index. Caller must ensure index is
   *             valid (using GetNumofSubNodes()).
   *
   * @param[in]  subnodeidx  The subnode index.
   *
   * @return     A subnode.
   */
  HVR_XMLPARSER_DLL
  XMLNode &GetSubNodeByIndex(
      size_t subnodeidx,
      std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list);

  /**
   * @brief      Gets a subnode by tag name. Caller must ensure tag name is
   *             valid.
   *
   * @param[in]  tag_name  The subnode tag name.
   *
   * @return     A subnode.
   */
  HVR_XMLPARSER_DLL
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
  HVR_XMLPARSER_DLL
  bool NodeErrorChecker(
      std::vector<std::pair<std::string, XMLNode::NodeStat>> &err_list) const;

  HVR_XMLPARSER_DLL
  void SetNodeValidity(const bool b);

  HVR_XMLPARSER_DLL
  bool CheckNodeValidity() const;

  HVR_XMLPARSER_DLL
  XMLNode &operator[](int idx);

  HVR_XMLPARSER_DLL
  XMLNode &operator[](const std::string &tag);

  HVR_XMLPARSER_DLL
  bool QueryText(std::string &txt_val) const;

  HVR_XMLPARSER_DLL
  bool QueryAttributes(std::map<const std::string, std::string> &attrs) const;

 private:
  // Subnodes
  std::vector<XMLNode> subnodes_;
  std::shared_ptr<XMLNode> bad_child_;
  // Node information
  std::string tag_   = "";
  std::string value_ = "";
  std::map<const std::string, std::string> attributes_;
  // std::vector<std::pair<std::string, XMLNode::NodeStat>> err_list_;
  bool is_valid_ = true;
};

}  // namespace hvr

#endif  // MODULES_XMLPARSER_INCLUDE_HVR_XMLPARSER_XMLNODE_H_
