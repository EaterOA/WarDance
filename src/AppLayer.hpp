/*
--------------------------------------------------------------------------------
AppLayer
- Defines a generic app layer
- Applications that use this assumes a layer-based structure
--------------------------------------------------------------------------------
*/

#ifndef APPLAYER_HPP
#define APPLAYER_HPP

#include <vector>
#include <SFML/Graphics.hpp>

class AppLayer
{
public:
    enum Status { PASS, HALT };

    virtual bool init() = 0;
    virtual Status tick(std::vector<sf::Event> &e, const sf::Time &t) = 0;
    virtual Status drawStatus() const = 0;
    virtual void draw(sf::RenderWindow &w) const = 0;
private:
};

#endif