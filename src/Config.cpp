#include "Config.hpp"
#include "Util.hpp"
#include <assert.h>
#include <fstream>
#include <sstream>

void Config::parse(std::istream& in)
{
    std::string line;
    while (getline(in, line)) {
        std::size_t idx = line.find("=");
        if (idx != std::string::npos) {
            std::string key = util::trim(line.substr(0, idx));
            std::string value = util::trim(line.substr(idx+1));
            db_str[key] = value;
        }
    }
}

bool Config::load(const std::string& path, const std::map<std::string, std::string>& defaultValues /*= std::map<std::string, std::string>()*/)
{
    std::fstream fin(path.c_str());
    if (fin.bad()) return false;
    parse(fin);
    for (std::map<std::string, std::string>::const_iterator iter = defaultValues.begin(); iter != defaultValues.end(); iter++) {
        if (db_str.find(iter->first) == db_str.end()) {
            db_str[iter->first] = iter->second;
        }
    }
    return true;
}

bool Config::load(const std::vector<std::string>& paths, const std::map<std::string, std::string>& defaultValues /*= std::map<std::string, std::string>()*/)
{
    bool flag = false;
    for (unsigned i = 0; i < paths.size(); i++) {
        std::fstream fin(paths[i].c_str());
        if (fin.bad()) continue;
        flag = true;
        parse(fin);
    }
    for (std::map<std::string, std::string>::const_iterator iter = defaultValues.begin(); iter != defaultValues.end(); iter++) {
        if (db_str.find(iter->first) == db_str.end()) {
            db_str[iter->first] = iter->second;
        }
    }
    return flag;
}

bool Config::load(const std::map<std::string, std::string>& db)
{
    for (std::map<std::string, std::string>::const_iterator iter = db.begin(); iter != db.end(); iter++) {
        db_str[iter->first] = iter->second;
    }
    return true;
}

std::string Config::getStr(const std::string& key)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    assert(iter != db_str.end());
    return iter->second;
}

std::string Config::getStr(const std::string& key, const std::string& defaultValue)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    return iter != db_str.end() ? iter->second : defaultValue;
}

void Config::setStr(const std::string& key, const std::string& value)
{
    db_str[key] = value;
}

int Config::getInt(const std::string& key)
{
    std::map<std::string, std::string>::iterator iter1 = db_str.find(key);
    if (iter1 != db_str.end()) {
        int val;
        assert(std::stringstream(iter1->second) >> val);
        db_int[key] = val;
        db_str.erase(iter1);
    }
    std::map<std::string, int>::const_iterator iter2 = db_int.find(key);
    assert(iter2 != db_int.end());
    return iter2->second;
}


int Config::getInt(const std::string& key, int defaultValue)
{
    std::map<std::string, std::string>::iterator iter1 = db_str.find(key);
    if (iter1 != db_str.end()) {
        int val;
        assert(std::stringstream(iter1->second) >> val);
        db_int[key] = val;
        db_str.erase(iter1);
    }
    std::map<std::string, int>::iterator iter2 = db_int.find(key);
    return (iter2 != db_int.end() ? iter2->second : defaultValue);
}

void Config::setInt(const std::string& key, int value)
{
    std::map<std::string, std::string>::iterator iter = db_str.find(key);
    if (iter != db_str.end()) db_str.erase(iter);
    db_int[key] = value;
}
