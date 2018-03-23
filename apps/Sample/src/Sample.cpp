// Copyright 2017 Arunabh Sharma, Qi Yao

HVR_WINDOWS_DISABLE_ALL_WARNING
#include <tinyxml2.h>

#include <iostream>
#include <vector>

#include "boost/filesystem.hpp"
#include "red/R3DSDK.h"
HVR_WINDOWS_ENABLE_ALL_WARNING

#include "Hvr/Stream/ParamProperty.h"
#include "Hvr/Stream/RCPParamChart.h"
#include "Hvr/Stream/ServerWrapper.h"
#include "Hvr/Stream/ThreadSafeMap.h"
#include "Hvr/XMLParser/XMLParser.h"

// parse a given error list and check any particular XMLNode retrieved isn"t
// correct
bool NodeErrorChecker(
    std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> &err_list)
{
  for (size_t i = 0; i < err_list.size(); i++)
  {
    // sencond position of the std::pair holds value of an enum of whether node
    // is good or bad(defined in XMLNode class)
    if (err_list[i].second == hvr::XMLNode::NodeStat::node_bad)
    {
      std::cerr << "Lv 0 node at tier " << i << " called " << err_list[i].first
                << " is not the correct node!" << std::endl;
      return false;
    }
  }
  err_list.clear();
  return true;
}

void DirCreator(const std::string &path_to_create)
{
  std::string path_to_proc = path_to_create + "/";
  // position of "/"
  int pos = static_cast<int>(path_to_proc.find("/", 0));

  // loop through a given string of directory and create the missing directory
  // iteratively
  while (pos != static_cast<int>(std::string::npos))
  {
    std::string path_to_check = path_to_proc.substr(0, pos);
    boost::filesystem::path path(path_to_check);
    boost::system::error_code return_err;

    if (!boost::filesystem::is_directory(path))
    {
      boost::filesystem::create_directory(path, return_err);

      if (return_err)
      {
        std::cerr << return_err << std::endl;
      }
    }

    // push "/" pos back and search for the next "/" pos
    pos = static_cast<int>(path_to_proc.find("/", pos + 1));
  }
}

bool SettingXMLParser(const std::string &xml_file,
                      std::map<rcp_param_t, hvr::ParamProperty> &rgb_settings,
                      std::map<rcp_param_t, hvr::ParamProperty> &ir_settings)
{
  hvr::XMLParser cur_parsr;
  if (!cur_parsr.Parse(xml_file))
  {
    std::cerr << "xml parsing failed!" << std::endl;
    return false;
  }
  std::shared_ptr<hvr::XMLNode> root = cur_parsr.GetRoot();

  // generate an empy error list
  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  hvr::XMLNode ir_node = root->GetSubNodeByTag("IR", err_list);
  if (!NodeErrorChecker(err_list)) return false;
  hvr::XMLNode rgb_node = root->GetSubNodeByTag("RGB", err_list);
  if (!NodeErrorChecker(err_list)) return false;

  hvr::ParamProperty tmp_param;
  std::string tmp_val;
  std::string tmp_param_str;
  std::shared_ptr<std::map<std::string, rcp_param_t>> param_chart =
      hvr::StrToRCPParam();

  for (int i = 0; i < static_cast<int>(ir_node.GetNumOfSubNodes()); i++)
  {
    if (ir_node.GetSubNodeByIndex(i, err_list).GetNumOfAttr() != 0)
    {
      if (ir_node.GetSubNodeByIndex(i, err_list)
              .GetAttrs()
              .find("rcp_param_t") !=
          ir_node.GetSubNodeByIndex(i, err_list).GetAttrs().end())
      {
        tmp_param_str = ir_node.GetSubNodeByIndex(i, err_list)
                            .GetAttrs()
                            .find("rcp_param_t")
                            ->second;
        if (!NodeErrorChecker(err_list)) return false;

        std::map<std::string, rcp_param_t>::iterator it =
            param_chart->find(tmp_param_str);
        if (it != param_chart->end())
        {
          tmp_param.param_id = it->second;
          tmp_param.value    = ir_node.GetSubNodeByIndex(i, err_list)
                                .GetSubNodeByTag("VAL", err_list)
                                .GetValue();
          if (!NodeErrorChecker(err_list)) return false;
          std::string b_val = ir_node.GetSubNodeByIndex(i, err_list)
                                  .GetSubNodeByTag("SET", err_list)
                                  .GetValue();
          if (b_val == "true")
          {
            tmp_param.settable = true;
          }
          else
          {
            tmp_param.settable = false;
          }

          if (!NodeErrorChecker(err_list)) return false;

          ir_settings.emplace(tmp_param.param_id, tmp_param);
        }
      }
    }
    if (rgb_node.GetSubNodeByIndex(i, err_list).GetNumOfAttr() != 0)
    {
      if (rgb_node.GetSubNodeByIndex(i, err_list)
              .GetAttrs()
              .find("rcp_param_t") !=
          rgb_node.GetSubNodeByIndex(i, err_list).GetAttrs().end())
      {
        tmp_param_str = rgb_node.GetSubNodeByIndex(i, err_list)
                            .GetAttrs()
                            .find("rcp_param_t")
                            ->second;
        std::map<std::string, rcp_param_t>::iterator it =
            param_chart->find(tmp_param_str);
        if (it != param_chart->end())
        {
          tmp_param.param_id = it->second;
          tmp_param.value    = rgb_node.GetSubNodeByIndex(i, err_list)
                                .GetSubNodeByTag("VAL", err_list)
                                .GetValue();
          if (!NodeErrorChecker(err_list)) return false;

          std::string b_val = rgb_node.GetSubNodeByIndex(i, err_list)
                                  .GetSubNodeByTag("SET", err_list)
                                  .GetValue();
          if (b_val == "true")
          {
            tmp_param.settable = true;
          }
          else
          {
            tmp_param.settable = false;
          }
          if (!NodeErrorChecker(err_list)) return false;

          rgb_settings.emplace(tmp_param.param_id, tmp_param);
        }
      }
    }
  }
  return true;
}

bool ConfigXMLParser(const std::string &xml_file,
                     const std::vector<int> cam_rng,
                     std::vector<std::string> &pc_ip_vec,
                     std::vector<std::string> &camera_ip_vec,
                     std::vector<std::string> &cam_nams,
                     std::vector<std::string> &write_path_vec)
{
  hvr::XMLParser cur_parsr;
  if (!cur_parsr.Parse(xml_file))
  {
    std::cerr << "xml parsing failed!" << std::endl;
    return false;
  }
  std::shared_ptr<hvr::XMLNode> root = cur_parsr.GetRoot();

  if (cam_rng.size() > root->GetNumOfSubNodes())
  {
    std::cerr << "trying to run more camera than xml specified!" << std::endl;
    return false;
  }

  // generate an empy error list
  std::vector<std::pair<std::string, hvr::XMLNode::NodeStat>> err_list;

  for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
  {
    hvr::XMLNode cur_set_node =
        root->GetSubNodeByIndex(std::max<int>(0, cam_rng[i] - 1), err_list);
    if (!NodeErrorChecker(err_list)) return false;
    std::string tmp_cam_ip =
        cur_set_node.GetSubNodeByTag("CAM_IP", err_list).GetValue();
    if (!NodeErrorChecker(err_list)) return false;
    std::string tmp_pc_ip =
        cur_set_node.GetSubNodeByTag("PC_IP", err_list).GetValue();
    if (!NodeErrorChecker(err_list)) return false;
    std::string tmp_cam_nam =
        cur_set_node.GetSubNodeByTag("CAM_NAM", err_list).GetValue();
    if (!NodeErrorChecker(err_list)) return false;
    std::string tmp_wp =
        cur_set_node.GetSubNodeByTag("WP", err_list).GetValue();
    if (!NodeErrorChecker(err_list)) return false;

    camera_ip_vec.emplace_back(tmp_cam_ip);
    pc_ip_vec.emplace_back(tmp_pc_ip);
    write_path_vec.emplace_back(tmp_wp);
    cam_nams.emplace_back(tmp_cam_nam);

    DirCreator(tmp_wp);
  }

  return true;
}

bool InputParser(int argc,
                 char *argv[],
                 std::string &cfg_xml_path,
                 std::string &setting_xml_path,
                 std::string &cam_list)
{
  for (int i = 0; i < argc; i++)
  {
    if (std::string(argv[i]) == "-cx")
    {
      if (i + 1 > argc)
      {
        std::cerr << "missing configuration xml file path!" << std::endl;
        return false;
      }
      cfg_xml_path = std::string(argv[i + 1]);
    }
    else if (std::string(argv[i]) == "-sx")
    {
      if (i + 1 > argc)
      {
        std::cerr << "missing setting xml path!" << std::endl;
        return false;
      }
      setting_xml_path = std::string(argv[i + 1]);
    }
    else if (std::string(argv[i]) == "-cr")
    {
      if (i + 1 > argc)
      {
        std::cerr << "missing camera range path!" << std::endl;
        return false;
      }
      cam_list = std::string(argv[i + 1]);
    }
  }
  return true;
}
bool FindCamRange(const std::string &cam_list, std::vector<int> &cam_rng)
{
  std::string loc_cam_list = cam_list;
  std::vector<std::string> rng_holder;
  while (loc_cam_list.size() != 0)
  {
    if (loc_cam_list.find_first_of(",") <= loc_cam_list.size())
    {
      int split_loc       = static_cast<int>(loc_cam_list.find_first_of(","));
      std::string tmp_elm = loc_cam_list.substr(0, split_loc);
      rng_holder.emplace_back(tmp_elm);
      loc_cam_list = loc_cam_list.substr(split_loc + 1, loc_cam_list.size());
    }
    else
    {
      rng_holder.emplace_back(loc_cam_list);
      loc_cam_list = "";
    }
  }
  for (int i = 0; i < static_cast<int>(rng_holder.size()); i++)
  {
    if (rng_holder[i].find("-") < rng_holder[i].size())
    {
      int tmp_bgn = std::stoi(rng_holder[i].substr(0, rng_holder[i].find("-")));
      int tmp_end = std::stoi(rng_holder[i].substr(rng_holder[i].find("-") + 1,
                                                   rng_holder[i].size()));
      for (int j = tmp_bgn; j <= tmp_end; j++)
      {
        cam_rng.emplace_back(j);
      }
    }
    else
    {
      cam_rng.emplace_back(std::stoi(rng_holder[i]));
    }
  }

  return true;
}

int run_server(int argc, char **argv)
{
  if (argc == 1)
  {
    std::cout << "amount of inputs passing in not matching accepted range"
              << std::endl;

    std::cout << "-cx           configuration xml path " << std::endl;
    std::cout << "-sx           setting xml path " << std::endl;
    std::cout << "-cr           camera count " << std::endl;
    return -1;
  }
  extern bool stopRecord;
  std::string cam_list;
  std::vector<int> cam_rng;
  std::string cfg_xml_path;
  std::string setting_xml_path;
  std::map<rcp_param_t, hvr::ParamProperty> ir_settings;
  std::map<rcp_param_t, hvr::ParamProperty> rgb_settings;
  std::vector<std::string> cam_ip_vec;
  std::vector<std::string> pc_ip_vec;
  std::vector<std::string> write_path_vec;
  std::vector<std::string> cam_names;
  std::vector<
      std::pair<hvr::ServerWrapper, std::map<rcp_param_t, hvr::ParamProperty>>>
      sw_vec;

  std::shared_ptr<std::map<rcp_param_t, std::string>> param_to_str_chart_tmp =
      hvr::RCPParamToStr();

  hvr::ThreadSafeMap<rcp_param_t, std::string> param_to_str_chart;
  param_to_str_chart.AssignMap(param_to_str_chart_tmp.get());

  std::string cam_type = "";

  if (!InputParser(argc, argv, cfg_xml_path, setting_xml_path, cam_list))
  {
    std::cerr << "input parsing failed!" << std::endl;
    return -1;
  }

  FindCamRange(cam_list, cam_rng);

  if (!ConfigXMLParser(cfg_xml_path,
                       cam_rng,
                       pc_ip_vec,
                       cam_ip_vec,
                       cam_names,
                       write_path_vec))
  {
    std::cerr << "config xml parsing failed!" << std::endl;
    return -1;
  }

  if (!SettingXMLParser(setting_xml_path, rgb_settings, ir_settings))
  {
    std::cerr << "setting xml parsing failed!" << std::endl;
    return -1;
  }

  std::cout << cam_rng.size() << std::endl;
  for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
  {
    hvr::ServerWrapper sw_inst(cam_ip_vec[i], pc_ip_vec[i], write_path_vec[i]);

    if (cam_names[i].substr(cam_names[i].size() - 2, 1) == "I")
    {
      std::map<rcp_param_t, hvr::ParamProperty> tmp_settings = ir_settings;
      hvr::ParamProperty temp_param;
      temp_param.param_id = RCP_PARAM_CAMERA_ID;
      temp_param.value    = cam_names[i];
      tmp_settings.emplace(temp_param.param_id, temp_param);
      sw_vec.emplace_back(sw_inst, tmp_settings);
      std::cout << "sending ir setting for " << cam_names[i] << std::endl;
    }
    else
    {
      std::map<rcp_param_t, hvr::ParamProperty> tmp_settings = rgb_settings;
      hvr::ParamProperty temp_param;
      temp_param.param_id = RCP_PARAM_CAMERA_ID;
      temp_param.value    = cam_names[i];
      tmp_settings.emplace(temp_param.param_id, temp_param);
      sw_vec.emplace_back(sw_inst, tmp_settings);
      std::cout << "sending rgb setting for " << cam_names[i] << std::endl;
    }
  }
  for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
  {
    sw_vec[i].first.Connect();
  }

  while (true)
  {
    printf(
        "\nk) CheckSetting \nl) AssignSetting \na) ReloadSettingXML \nc) "
        "StreamingModeOn\nr) "
        "Record\ns) Stop Record\nd) "
        "StreamingModeOff\nq) "
        "Quit\nOption: ");
    std::string option;
    std::getline(std::cin, option);
    switch (option[0])
    {
      case 'k':
      {
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          sw_vec[i].first.CheckSettings(sw_vec[i].second, &param_to_str_chart);
        }

        break;
      }
      case 'l':
      {
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          sw_vec[i].first.SetSettings(sw_vec[i].second);
        }

        break;
      }
      case 'a':
      {
        if (!SettingXMLParser(setting_xml_path, rgb_settings, ir_settings))
        {
          std::cerr << "setting xml parsing failed!" << std::endl;
          return -1;
        }
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          if (cam_names[i].substr(cam_names[i].size() - 2, 1) == "I")
          {
            std::map<rcp_param_t, hvr::ParamProperty> tmp_settings =
                ir_settings;
            hvr::ParamProperty temp_param;
            temp_param.param_id = RCP_PARAM_CAMERA_ID;
            temp_param.value    = cam_names[i];
            tmp_settings.emplace(RCP_PARAM_CAMERA_ID, temp_param);
            sw_vec[i].second = tmp_settings;
            std::cout << "sending ir setting for " << cam_names[i] << std::endl;
          }
          else
          {
            std::map<rcp_param_t, hvr::ParamProperty> tmp_settings =
                rgb_settings;
            hvr::ParamProperty temp_param;
            temp_param.param_id = RCP_PARAM_CAMERA_ID;
            temp_param.value    = cam_names[i];
            tmp_settings.emplace(RCP_PARAM_CAMERA_ID, temp_param);
            sw_vec[i].second = tmp_settings;
            std::cout << "sending rgb setting for " << cam_names[i]
                      << std::endl;
          }
        }
        break;
      }
      case 'c':
      {
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          sw_vec[i].first.StreamingModeON();
        }

        break;
      }
      case 'q':
      {
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          sw_vec[i].first.Quit();
        }
        stopRecord = true;
        return 0;
      }
      case 'r':
      {
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          sw_vec[i].first.Record();
        }
        stopRecord = false;
        break;
      }
      case 'd':
      {
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          sw_vec[i].first.StreamingModeOff();
        }
        stopRecord = true;
        break;
      }

      case 's':
      {
        stopRecord = true;
        for (int i = 0; i < static_cast<int>(cam_rng.size()); i++)
        {
          sw_vec[i].first.Stop();
        }
        break;
      }

      default: { printf("\nUnknown Option\n");
      }
    }
  }
  return 0;
}

int main(int argc, char **argv)
{
  if (R3DSDK::InitializeSdk("/usr/lib/", 0) != R3DSDK::ISInitializeOK)
  {
    auto result = R3DSDK::InitializeSdk("./", 0);
    if (result != R3DSDK::ISInitializeOK)
    {
      printf("Error initializing SDK. Dynamic library not found?\n");
      return -1;
    }
  }

  int retval = run_server(argc, argv);

  R3DSDK::FinalizeSdk();
  return retval;
}
