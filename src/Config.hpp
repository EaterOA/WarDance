#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <map>

extern std::map<std::string, int> config;

bool init_config(std::string path);

#endif
