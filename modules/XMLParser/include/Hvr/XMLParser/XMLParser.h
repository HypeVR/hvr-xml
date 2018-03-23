// Copyright @ 2016 Caoyang Jiang

#ifndef MODULES_XMLPARSER_INCLUDE_HVR_XMLPARSER_XMLPARSER_H_
#define MODULES_XMLPARSER_INCLUDE_HVR_XMLPARSER_XMLPARSER_H_

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <tinyxml2.h>
HVR_WINDOWS_ENABLE_ALL_WARNING

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Hvr/XMLParser/XMLNode.h"

namespace hvr
{
/**
 * @brief      Parse XML file into tree-structured nodes.
 * @ingroup    XMLParser
 */
class XMLParser
{
 public:
  /**
   * @brief      Default Constructor
   */
  HVR_XMLPARSER_DLL XMLParser();

  /**
   * @brief      Default Destructor
   */
  HVR_XMLPARSER_DLL ~XMLParser();

  /**
   * @brief      Parse a XML file into tree data structure.
   *
   * @param[in]  filename  The XML file name or XML url.
   *
   * @return     True if parsing successful, False otherwise.
   */
  HVR_XMLPARSER_DLL bool Parse(const std::string filename);

  /**
   * @brief      Recursively printing node information.
   *
   * @param[in]  node a XML node.
   */
  HVR_XMLPARSER_DLL static void RecursePrint(XMLNode &node);

  /**
   * @brief      Gets the root XML node.
   *
   * @return     The root XML node.
   */
  HVR_XMLPARSER_DLL std::shared_ptr<XMLNode> &GetRoot();

 private:
  /**
   * @brief      Recursive processing of XML elements.
   *
   * @param[in]  parenode  parent node.
   *
   * @return     0 if end, 1 if more nodes avaliable.
   */
  bool RecurseProc(XMLNode &parent_node);

 private:
  enum NodeType
  {
    ELEMENT    = 1,
    TEXT       = 3,
    COMMENT    = 8,
    WHITESPACE = 14,
    END        = 15,
  };

  tinyxml2::XMLElement *reader_t_;
  std::shared_ptr<XMLNode> root_;
};
}  // namespace hvr

#endif  // MODULES_XMLPARSER_INCLUDE_HVR_XMLPARSER_XMLPARSER_H_
