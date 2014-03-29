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
    virtual Status draw(sf::RenderWindow &w) = 0;
private:
};

#endif