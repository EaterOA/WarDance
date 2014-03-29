#include "GameLayer.hpp"
#include "GameResourceManager.hpp"
#include "GameConfig.hpp"
#include "GameController.hpp"
#include "Util.hpp"

bool PauseMenu::init()
{
    m_type = PAUSE;
    m_numChoices = 3;
    unsigned num_p = 1;

    sf::Vector2f texCoord, size, pos;
    std::ifstream fin;
    fin.open("config/guidata.txt");
    if (!fin) return false;

    m_pause = std::vector<sf::Vertex>((num_p + m_numChoices)*4, sf::Vertex());
    m_pauseLit = m_pauseDim = std::vector<sf::Vertex>(m_numChoices*4, sf::Vertex());
    for (unsigned i = 0; i < num_p && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_pause[i*4], texCoord, size);
        util::affixPos(&m_pause[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < m_numChoices && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_pauseLit[i*4], texCoord, size);
        util::affixPos(&m_pauseLit[i*4], pos, size, 0);
        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_pauseDim[i*4], texCoord, size);
        util::affixPos(&m_pauseDim[i*4], pos, size, 0);
    }
    m_menu = &m_pause[num_p*4];

    fin.close();
    
    m_choice = 1;
    for (unsigned i = 0; i < m_numChoices; i++) util::copySprite(&m_pauseDim[i*4], &m_pause[(num_p+i)*4]);

    return true;
}

AppLayer::Status PauseMenu::tick(std::vector<sf::Event> &e, const sf::Time &t)
{
    for (unsigned i = 0; i < e.size(); i++) {
        //Mouse presses
        if (e[i].type == sf::Event::MouseButtonReleased) {
            if (e[i].mouseButton.button == sf::Mouse::Left) {
                float x = (float)e[i].mouseButton.x;
                float y = (float)e[i].mouseButton.y;
                unsigned opt = translateOption(x, y);
                if (!opt) continue;
                processChoice();
            }
            else if (e[i].mouseButton.button == sf::Mouse::Right) {
                Layer::back();
            }
        }
        //Mouse movement
        else if (e[i].type == sf::Event::MouseMoved) {
            float x = (float)e[i].mouseMove.x;
            float y = (float)e[i].mouseMove.y;
            unsigned opt = translateOption(x, y);
            if (!opt) continue;
            selectChoice(opt);
        }
        //Keyboard events
        else if (e[i].type == sf::Event::KeyPressed) {
            bool up = controller.pressing(GameController::K_UP, e[i].key.code);
            bool down = controller.pressing(GameController::K_DOWN, e[i].key.code);
            bool enter = controller.pressing(GameController::K_ENTER, e[i].key.code);
            bool esc = controller.pressing(GameController::K_ESCAPE, e[i].key.code);

            if (down) selectChoice(m_choice + 1);
            else if (up) selectChoice(m_choice - 1);
            else if (enter) processChoice();
            else if (esc) Layer::back();
            
        }
    }

    return AppLayer::HALT;
}

AppLayer::Status PauseMenu::drawStatus() const
{
    return AppLayer::PASS;
}

void PauseMenu::draw(sf::RenderWindow &w) const
{
    sf::RenderStates s(&resource.getTexture("guisheet"));
    sf::RectangleShape fade;
    fade.setFillColor(sf::Color(0, 0, 0, 100));
    fade.setPosition(0, 0);
    fade.setSize(sf::Vector2f((float)w.getSize().x, (float)w.getSize().y));
    w.draw(fade);
    w.draw(&m_pause[0], m_pause.size(), sf::Quads, s);
}

void PauseMenu::selectChoice(unsigned choice)
{
    if (choice == 0) choice += m_numChoices;
    else if (choice > m_numChoices) choice -= m_numChoices;
    unsigned prev = m_choice-1;
    unsigned next = choice-1;
    util::copyTexture(&m_pauseDim[prev*4], &m_menu[prev*4]);
    util::copyTexture(&m_pauseLit[next*4], &m_menu[next*4]);
    m_choice = choice;
}

void PauseMenu::processChoice()
{
    
    if (m_choice == 1) Layer::back();
    /*else if (m_choice == 2) goToSettings();
    else if (m_choice == 3) goToMain();
    */
}

unsigned PauseMenu::translateOption(float x, float y)
{
    sf::Vector2f m(x, y);
    for (unsigned i = 0; i < m_numChoices; i++) {
        unsigned idx = (i+1)*4;
        sf::Vector2f size = m_pause[idx+2].position - m_pause[idx].position;
        sf::Vector2f pos = util::referenceToCenter(m_pause[idx].position, size, 1);
        if (util::hasCollided(m, pos, size, 0)) return i+1;
    }
    return 0;
}