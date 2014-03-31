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
    m_type = GameLayer::BATTLE;
    camera = sf::View(sf::FloatRect(0, 0, 800.f, 600.f));

    mAgent = new BattleMechanics();
    gAgent = new BattleGraphics();
    hAgent = new BattleHUD();
    if (!mAgent->init()) return false;
    if (!gAgent->init()) return false;
    if (!hAgent->init()) return false;

    mAgent->initLevel();
    mAgent->startLevel();

    gAgent->updateState(mAgent->getBattleState());
    hAgent->updateState(mAgent->getBattleState());

    m_transitioning = false;
    m_scoring = false;

    return true;
}

AppLayer::Status Battle::tick(std::vector<sf::Event> &e, const sf::Time &t, sf::Vector2f m)
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

    //Adjust mouse position relative to map (camera)
    sf::Vector2f hsize = sf::Vector2f(camera.getSize().x/2, camera.getSize().y/2);
    m += camera.getCenter() - hsize;

    //Direct level transition
    if (m_transitioning) {
        m_transitionDuration -= t.asSeconds();
        if (m_transitionDuration <= 0) {
            endLevelTransition();
        }
        else {
            float alpha = 1.f - m_transitionDuration / 1.5f; 
            alpha *= alpha*10*alpha;
            gAgent->setTransition(alpha);
            hAgent->setTransition(alpha);
        }
    }

    //Progress components
    mAgent->updateState(t, m);
    gAgent->updateState(mAgent->getBattleState());
    hAgent->updateState(mAgent->getBattleState());
    
    //Update camera
    sf::Vector2f map = mAgent->getBattleState().map;
    sf::Vector2f pos = mAgent->getBattleState().player->getPos();
    if (pos.x < hsize.x) pos.x = hsize.x;
    else if (pos.x > map.x - hsize.x) pos.x = map.x - hsize.x;
    if (pos.y < hsize.y) pos.y = hsize.y;
    else if (pos.y > map.y - hsize.y) pos.y = map.y - hsize.y;
    camera.setCenter(pos);

    //Check for end conditions
    if (mAgent->isPlayerDead()) {
        Layer::back();
    }
    else if (!m_scoring && mAgent->isLevelDone()) {
        m_scoring = true;
        mAgent->saveLevelStats();
        mAgent->clearEnemyProjectiles();
        Layer::goToScoreScreen();
    }
    
    return AppLayer::HALT;
}

AppLayer::Status Battle::drawStatus() const
{
    return AppLayer::HALT;
}

void Battle::draw(sf::RenderWindow &w) const
{
    sf::View screen(w.getDefaultView());
    w.setView(camera);
    w.draw(*gAgent);
    w.setView(screen);
    w.draw(*hAgent);
}

void Battle::startLevelTransition()
{
    m_transitionDuration = 1.5f;
    m_transitioning = true;
}

void Battle::endLevelTransition()
{
    mAgent->clearPlayerProjectiles();
    m_transitioning = false;
    m_scoring = false;
    gAgent->setTransition(0);
    hAgent->setTransition(0);
    int lvl = config.getInt("level");
    if (lvl < config.getInt("num_levels"))
        config.setInt("level", lvl+1);
    mAgent->startLevel();
}

void Battle::incScore(int value)
{
    mAgent->getBattleState().score += value;
}