// Copyright 2017 Qi Yao

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <iostream>
#include <map>
#include <vector>

#include "boost/filesystem.hpp"

HVR_WINDOWS_ENABLE_ALL_WARNING
#include "Hvr/XML/XMLParser.h"
#include "Hvr/XML/XMLWriter.h"

void DirCreator(const std::string &path_to_create)
{
  std::string path_to_proc = path_to_create + "/";
  // position of "/"
  std::size_t pos = path_to_proc.find('/', 0);

  // loop through a given string of directory and create the missing directory
  // iteratively
  while (pos != std::string::npos)
  {
    std::string path_to_check = path_to_proc.substr(0, pos);
    boost::filesystem::path path(path_to_check);
    boost::system::error_code return_err;

    if (!boost::filesystem::is_directory(path))
    {
      boost::filesystem::create_directory(path, return_err);

      if (return_err.value() != boost::system::errc::success)
      {
        std::cerr << return_err << std::endl;
      }
    }

    // push "/" pos back and search for the next "/" pos
    pos = static_cast<int>(path_to_proc.find('/', pos + 1));
  }
}

bool InputParser(int argc,
                 char *argv[],
                 std::string &in_path,
                 std::string &out_path)
{
  bool has_flag = false;
  for (int i = 0; i < argc; i++)
  {
    if (std::string(argv[i]) == "-i")
    {
      if (i + 1 > argc)
      {
        std::cerr << "missing input xml path" << std::endl;
        return false;
      }
      in_path  = argv[i + 1];
      has_flag = true;
    }
    else if (std::string(argv[i]) == "-o")
    {
      if (i + 1 > argc)
      {
        std::cerr << "missing output xml path!" << std::endl;
        return false;
      }
      out_path = argv[i + 1];
      has_flag = true;
    }
    else
    {
      has_flag = false;
    }
  }
  if (!has_flag)
  {
    std::cout << "amount of inputs passing in not matching accepted range"
              << std::endl;
    std::cout << "Please input a xml file with the layout same out as "
                 "sample.xml"
              << std::endl;

    std::cout << "-i   input xml file path" << std::endl;
    std::cout << "-o   output xml file path" << std::endl;

    return false;
  }
  return true;
}

int main(int argc, char **argv)
{
  if (argc == 1)
  {
    std::cout << "amount of inputs passing in not matching accepted range"
              << std::endl;
    std::cout << "Please input a xml file with the layout same out as "
                 "sample.xml"
              << std::endl;

    std::cout << "-i   input xml file path" << std::endl;
    std::cout << "-o   output xml file path" << std::endl;
  }

  std::string in_path;
  std::string out_path;

  InputParser(argc, argv, in_path, out_path);

  hvr::XMLParser parser;
  parser.Parse(in_path);

  hvr::XMLNode &root_node = *parser.GetRoot();

  for (int i = 0; i < static_cast<int>(root_node.GetNumOfSubNodes()); i++)
  {
    std::map<std::string, std::string> attrs = root_node[i].GetAttrs();
    for (auto &attr : attrs)
    {
      std::cout << "attribute " << attr.first << " has value of " << attr.second
                << std::endl;
      std::string val;
      if (root_node[i]["VAL1"].QueryText(val))
      {
        std::cout << " Child node VAL1 has value of " << val << std::endl;
      }
      int new_val             = 52;
      hvr::XMLNode &val1_node = root_node[i]["VAL1"];
      if (val1_node.IsValid())
      {
        val1_node.SetInt(new_val);
        std::cout << "The text value of the node VAL1 is now 52" << std::endl;
      }
    }

    // create new node and append to parent example
    hvr::XMLNode additional_node1("EXTRA1");
    additional_node1.SetText("Nothing to see here!");
    hvr::XMLNode additional_node2("EXTRA2");
    additional_node2.SetInt(250);
    hvr::XMLNode additional_node3("EXTRA3");
    additional_node3.SetFloat(250.015036f);
    hvr::XMLNode additional_node4("EXTRA4");
    additional_node4.SetDouble(250.015036076549);
    hvr::XMLNode additional_node5("EXTRA5");
    additional_node5.SetBool(false);
    root_node.PushBack(additional_node1);
    root_node.PushBack(additional_node2);
    root_node.PushBack(additional_node3);
    root_node.PushBack(additional_node4);
    root_node.PushBack(additional_node5);

    // writing to file example
    std::string out_folder = out_path.substr(0, in_path.find_last_of("/\\"));
    DirCreator(out_folder);

    hvr::XMLWriter writer;
    writer.AssignXMLNode(parser.GetRoot());
    writer.Write(out_path);
  }

  return 0;
}
