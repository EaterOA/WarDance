#include "GameLayer.hpp"

bool NoFocus::init()
{
    m_type = NOFOCUS;
    return true;
}

GameLayer::Status NoFocus::tick(std::vector<sf::Event> &e, const sf::Time &t)
{
    return GameLayer::HALT;
}

AppLayer::Status NoFocus::drawStatus() const
{
    return AppLayer::HALT;
}

void NoFocus::draw(sf::RenderWindow &w) const
{
}