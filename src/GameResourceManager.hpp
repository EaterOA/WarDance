#ifndef GAMERESOURCEMANAGER_HPP
#define GAMERESOURCEMANAGER_HPP

#include "ResourceManager.hpp"

class GameResourceManager: public ResourceManager
{
public:
    bool init();
};

extern GameResourceManager resource;

#endif 