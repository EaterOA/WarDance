#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP

#include "Config.hpp"

class GameConfig: public Config
{
public:
    bool init();    
};

extern GameConfig config;

#endif
