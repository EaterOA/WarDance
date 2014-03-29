#include "GameLayer.hpp"

std::vector<GameLayer*> layer;

GameLayer::Type GameLayer::getType() const
{
    return m_type;
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
}

void Layer::refocus()
{
    if (getTopLayer() != GameLayer::NOFOCUS) return;

    back();
    if (getTopLayer() == GameLayer::GAME) pauseGame();
}

void Layer::pauseGame()
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