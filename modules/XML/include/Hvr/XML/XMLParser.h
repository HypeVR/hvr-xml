// Copyright @ 2016 Caoyang Jiang

#ifndef MODULES_XML_INCLUDE_HVR_XML_XMLPARSER_H_
#define MODULES_XML_INCLUDE_HVR_XML_XMLPARSER_H_

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <tinyxml2.h>
HVR_WINDOWS_ENABLE_ALL_WARNING

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Hvr/XML/XMLNode.h"

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
  HVR_XML_DLL XMLParser();

  /**
   * @brief      Default Destructor
   */
  HVR_XML_DLL ~XMLParser();

  /**
   * @brief      Parse a XML file into tree data structure.
   *
   * @param[in]  filename  The XML file name or XML url.
   *
   * @return     True if parsing successful, False otherwise.
   */
  HVR_XML_DLL bool Parse(const std::string filename);

  /**
   * @brief      Gets the root XML node.
   *
   * @return     The root XML node.
   */
  HVR_XML_DLL std::shared_ptr<XMLNode> &GetRoot();

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

#endif  // MODULES_XML_INCLUDE_HVR_XML_XMLPARSER_H_
