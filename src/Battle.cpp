#include "GameLayer.hpp"
#include "GameResourceManager.hpp"
#include "GameConfig.hpp"
#include "GameController.hpp"
#include "Util.hpp"
#include "BattleMechanics.hpp"
#include "BattleGraphics.hpp"
#include "BattleHUD.hpp"

bool Battle::init()
{
    mAgent = new BattleMechanics();
    gAgent = new BattleGraphics();
    hAgent = new BattleHUD();
    if (!mAgent->init()) return false;
    if (!gAgent->init()) return false;
    if (!hAgent->init()) return false;

    mAgent->initLevel();
    mAgent->startLevel();
    return true;
}

AppLayer::Status Battle::tick(std::vector<sf::Event> &e, const sf::Time &t, const sf::Vector2f &m)
{
    //Process events
    for (unsigned i = 0; i < e.size(); i++) {
        //Keyboard events
        if (e[i].type == sf::Event::KeyPressed) {
            bool esc = controller.pressing(GameController::K_ESCAPE, e[i].key.code);

            if (esc) {
                Layer::pauseBattle();
                return AppLayer::HALT;
            }
        }
    }

    mAgent->updateGameState(t, m);
    gAgent->updateSprites(mAgent->getGameState());
    
    return AppLayer::HALT;
}

AppLayer::Status Battle::drawStatus() const
{
    return AppLayer::HALT;
}

void Battle::draw(sf::RenderWindow &w) const
{
    w.draw(*gAgent);
}

void Battle::incScore(int value)
{
    mAgent->getGameState().score += value;
}