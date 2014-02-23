#include "GameResourceManager.hpp"
#include "Util.hpp"

GameResourceManager resource;

bool GameResourceManager::init()
{
    const int numLevelBGs = 5;
    const int numSpriteSheets = 2;

    //Loading necessary textures
    std::map<std::string, std::string> loadList;
    loadList["guisheet"] = "images/guisheet.png";
    loadList["main_bg"] = "images/main.png";
    loadList["settings_bg"] = "images/settings.png";
    loadList["scoring"] = "images/scoring.png";
    for (int i = 0; i < numLevelBGs; i++) {
        loadList[util::appendNumber("bg", i)] = util::appendNumber("images/bg", i) + std::string(".png");
    }
    for (int i = 0; i < numSpriteSheets; i++) {
        loadList[util::appendNumber("spritesheet", i)] = util::appendNumber("images/spritesheet", i) + std::string(".png");
    }
    for (std::map<std::string, std::string>::iterator iter = loadList.begin(); iter != loadList.end(); iter++)
        if (!loadTexture(iter->first, iter->second))
            return false;
    for (std::map<std::string, sf::Texture>::iterator iter = db_tex.begin(); iter != db_tex.end(); iter++) {
        iter->second.setSmooth(true);
        if (util::isPrefix("bg", iter->first)) iter->second.setRepeated(true);
    }

    //Loading necessary fonts
    loadList = std::map<std::string, std::string>();
    loadList["stencil"] = "fonts/stenc_ex.ttf";
    loadList["liberation"] = "fonts/LiberationSerif-Regular.ttf";
    for (std::map<std::string, std::string>::iterator iter = loadList.begin(); iter != loadList.end(); iter++)
        if (!loadFont(iter->first, iter->second))
            return false;

    return true;
}