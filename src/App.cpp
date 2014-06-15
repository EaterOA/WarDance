#include "GameLayer.hpp"
#include "GameConfig.hpp"
#include "GameResourceManager.hpp"
#include "GameController.hpp"
#include <iostream>

sf::RenderWindow window;
sf::Clock gameClock;

bool appInit()
{
    srand(unsigned(time(0)));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(800, 600), "War Dance", sf::Style::Close, settings);
    window.setFramerateLimit(60);
    if (!config.init()) return false;
    if (!resource.init()) return false;
    if (!controller.init()) return false;

    Layer::goToMain();

    return true;
}


std::vector<sf::Event> processEvents()
{
    std::vector<sf::Event> inputEvents;
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            break;
        }
        else if (event.type == sf::Event::LostFocus) {
            Layer::unfocus();
        }
        else if (event.type == sf::Event::GainedFocus) {
            Layer::refocus();
        }
        else inputEvents.push_back(event);
    }
    return inputEvents;
}

void appStart()
{
    //Game loop
    while (window.isOpen()) {

        //Collecting information
        sf::Time elapsed = gameClock.restart();
        std::vector<sf::Event> events = processEvents();
        sf::Vector2f cursor(sf::Mouse::getPosition(window));
        cursor.y += window.getView().getCenter().y - window.getView().getSize().y / 2;
        cursor.x += window.getView().getCenter().x - window.getView().getSize().x / 2;

        //Update from top to bottom
        for (unsigned i = layer.size(); i > 0; i--) {
            AppLayer::Status s = layer[i-1]->tick(events, elapsed, cursor);
            while (i > layer.size()) i--;
            if (s == AppLayer::HALT) break;
        }
        if (layer.empty()) {
            window.close();
            break;
        }
        
        //Reverse draw order from bottom to top
        std::vector<unsigned> drawIdx;
        for (unsigned i = layer.size(); i > 0; i--) {
            drawIdx.push_back(i-1);
            if (layer[i-1]->drawStatus() == AppLayer::HALT) break;
        }
        for (unsigned i = drawIdx.size(); i > 0; i--) {
            layer[drawIdx[i-1]]->draw(window);
        }

        window.display();
    }
}

int main()
{
    if (!appInit()) {
        std::cerr << "Cannot initialize application. Aborting.\n";
        if (window.isOpen()) window.close();
    }
    else {
        appStart();
    }
}
