#include "GameLayer.hpp"

std::vector<GameLayer*> layer;

GameLayer::Type GameLayer::getType() const
{
    return m_type;
}

GameLayer::~GameLayer()
{
}


//@@@@@@@@@@@@@@@@@@@@ Layer management functions @@@@@@@@@@@@@@@@@@@@

GameLayer::Type Layer::getTopLayer()
{
    if (layer.empty()) return GameLayer::NONE;
    return layer.back()->getType();
}

void Layer::back()
{
    if (layer.empty()) return;

    delete layer.back();
    layer.pop_back();
}

void Layer::unfocus()
{
    if (getTopLayer() == GameLayer::NOFOCUS) return;

    layer.push_back(new NoFocus());
    layer.back()->init();
}

void Layer::refocus()
{
    if (getTopLayer() != GameLayer::NOFOCUS) return;
    
    back();
    if (getTopLayer() == GameLayer::BATTLE) pauseBattle();
}

void Layer::pauseBattle()
{
    if (getTopLayer() == GameLayer::PAUSE) return;

    layer.push_back(new PauseMenu());
    layer.back()->init();
}

void Layer::goToMain()
{
    if (getTopLayer() == GameLayer::MAIN) return;
    
    layer.push_back(new MainMenu());
    layer.back()->init();
}

void Layer::goToSelectLevel()
{
    if (getTopLayer() == GameLayer::SELECTLEVEL) return;
    
    layer.push_back(new SelectLevelDialog());
    layer.back()->init();
}

void Layer::goToSettings()
{
    if (getTopLayer() == GameLayer::SETTINGS) return;
    
    layer.push_back(new SettingsMenu());
    layer.back()->init();
}

void Layer::endGame()
{
    while (!layer.empty()) {
        delete layer.back();
        layer.pop_back();
    }
}

void Layer::startBattle()
{
    if (getTopLayer() == GameLayer::BATTLE) return;
    
    layer.push_back(new Battle());
    layer.back()->init();
}

void Layer::backToMain()
{
    while (!layer.empty() && layer.back()->getType() != GameLayer::MAIN) {
        delete layer.back();
        layer.pop_back();
    }
}

void Layer::goToScoreScreen()
{
    if (getTopLayer() == GameLayer::SCORE) return;
    
    layer.push_back(new ScoreScreen());
    layer.back()->init();
}
