#include "GameLayer.hpp"
#include "GameConfig.hpp"
#include "GameResourceManager.hpp"
#include "GameController.hpp"
#include "GameMechanics.hpp"
#include "GameGraphics.hpp"
#include "GameGUI.hpp"
#include <SFML/System.hpp>

sf::RenderWindow window;
sf::View camera(sf::FloatRect(0, 0, 800.f, 600.f));
GameMechanics mAgent;
GameGraphics gAgent;
GameGUI guiAgent;
sf::Clock gameClock;

bool appInit()
{
    srand(unsigned(time(0)));
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(800, 600), "War Dance", sf::Style::Close, settings);
    window.setFramerateLimit(60);
    window.setView(camera);
    if (!config.init()) return false;
    if (!resource.init()) return false;
    if (!controller.init()) return false;
    if (!mAgent.init()) return false;
    if (!gAgent.init()) return false;
    if (!guiAgent.init(&mAgent, &gAgent)) return false;

    Layer::goToMain();

    return true;
}
/*
void updateView(sf::Vector2f pos)
{
    if (pos.x < APP_WIDTH/2) pos.x = APP_WIDTH/2;
    else if (pos.x > mAgent.getGameState().map.x - APP_WIDTH/2) pos.x = mAgent.getGameState().map.x - APP_WIDTH/2;
    if (pos.y < 300) pos.y = 300;
    else if (pos.y > mAgent.getGameState().map.y - APP_HEIGHT/2) pos.y = mAgent.getGameState().map.y - APP_HEIGHT/2;
    camera.setCenter(pos);
}


void paint()
{
    window.setView(camera);
    if (getAppState() == PAUSED || getAppState() == GAME || getAppState() == LEVELENDSEQUENCE) window.draw(gAgent);
sf::View hud(sf::FloatRect(0, 0, float(APP_WIDTH), float(APP_HEIGHT)));
    window.setView(hud);
    window.draw(guiAgent);
    window.setView(camera);
    window.display();
}

void startGame()
{
    appStates.push_back(GAME);
    guiAgent.transitionAppState();
    gameClock.restart();
    mAgent.initLevel();
    mAgent.startLevel();
}

void goToLevelEndSequence()
{
    appStates.push_back(LEVELENDSEQUENCE);
    guiAgent.startLevelEndSequence(mAgent.getLevelStats());
    mAgent.clearEnemyProjectiles();
}

void goToNextLevel()
{
    while (getAppState() != GAME) appStates.pop_back();
    int lvl = config.getInt("level");
    if (lvl < config.getInt("num_levels"))
        config.setInt("level", lvl+1);
    mAgent.clearPlayerProjectiles();
    mAgent.startLevel();
    guiAgent.transitionAppState();
}*/


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
        sf::Time elapsed = gameClock.restart();
        std::vector<sf::Event> events = processEvents();

        //Update from up to bottom
        for (int i = (int)layer.size() - 1; i >= 0; i--) {
            AppLayer::Status s = layer[i]->tick(events, elapsed);
            while (i+1 > (int)layer.size()) i--;
            if (s == AppLayer::HALT) break;
        }
        if (layer.empty()) {
            window.close();
            break;
        }

        //Reverse draw order from bottom to up
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
