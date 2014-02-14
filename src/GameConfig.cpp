#include "GameConfig.hpp"

GameConfig config;

bool GameConfig::init()
{
    //Defaults for configuration not in config file
    std::map<std::string, std::string> defaults;
    defaults["hitbox_enabled"] = "0";
    defaults["bind_enter"] = "Key_Return";
    defaults["bind_escape"] = "Key_Escape";
    defaults["bind_up"] = "Key_Up Key_W";
    defaults["bind_down"] = "Key_Down Key_S";
    defaults["bind_left"] = "Key_Left Key_A";
    defaults["bind_right"] = "Key_Right Key_D";

    load("config/config.txt", defaults);

    //Overriding certain internal configurations
    std::map<std::string, std::string> overrides;
    overrides["level"] = "1";
    overrides["num_levels"] = "5";
    overrides["highscore"] = "0";

    load(overrides);

    return true;
}

