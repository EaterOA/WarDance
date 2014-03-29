#include "GameLayer.hpp"

bool NoFocus::init()
{
    return true;
}

GameLayer::Status NoFocus::tick(std::vector<sf::Event> &e, const sf::Time &t)
{
    return GameLayer::HALT;
}

GameLayer::Status NoFocus::draw(sf::RenderWindow &w)
{
    return GameLayer::HALT;
}