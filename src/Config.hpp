#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <string>
#include <map>
#include <istream>

class Config
{
public:
    bool load(const std::string& path, const std::map<std::string, std::string>& defaultValues = std::map<std::string, std::string>());
    bool load(const std::vector<std::string>& paths, const std::map<std::string, std::string>& defaultValues = std::map<std::string, std::string>());
    bool load(const std::map<std::string, std::string>& db);
    std::string getStr(const std::string& key);
    std::string getStr(const std::string& key, const std::string &defaultValue);
    void setStr(const std::string& key, const std::string &value);
    int getInt(const std::string& key);
    int getInt(const std::string& key, int defaultValue);
    void setInt(const std::string& key, int value);
private:
    void parse(std::istream& in);

    std::map<std::string, int> db_int;
    std::map<std::string, std::string> db_str;
};

#endif
