// Copyright @ 2016 Caoyang Jiang

#ifndef MODULES_XML_INCLUDE_HVR_XML_XMLWRITER_H_
#define MODULES_XML_INCLUDE_HVR_XML_XMLWRITER_H_

HVR_WINDOWS_DISABLE_ALL_WARNING
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
class XMLWriter
{
 public:
  /**
   * @brief      Default constructor
   */
  HVR_XML_DLL
  XMLWriter();

  /**
   * @brief      Constructor that passes a hvr::XMLNode ojbect directly
   *
   * @param[in]  in_node  In node
   */
  HVR_XML_DLL
  explicit XMLWriter(const std::shared_ptr<XMLNode> &in_node);

  /**
   * @brief      Default destructor
   */
  HVR_XML_DLL
  ~XMLWriter();

  /**
   * @brief      Assign the hvr::XMLNode object that holds on the info
   *
   * @param[in]  in_node  In node
   *
   * @return     whether the operation succeeds
   */
  HVR_XML_DLL
  bool AssignXMLNode(const std::shared_ptr<XMLNode> &in_node);

  /**
   * @brief      Write the info to a .xml file with given path
   *
   * @param[in]  out_path  The out path
   *
   * @return     whether the operation succeeds
   */
  HVR_XML_DLL
  bool Write(const std::string &out_path);

 private:
  bool TranferInfo();

  struct Pimpl;
  std::unique_ptr<Pimpl> pi_;
};
}  // namespace hvr

#endif  // MODULES_XML_INCLUDE_HVR_XML_XMLWRITER_H_
