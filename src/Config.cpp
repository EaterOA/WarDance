#include "Config.hpp"
#include <fstream>

std::map<std::string, int> config;

bool init_config(std::string path)
{
    config["hitbox_enabled"] = 2;
    return true;
}
