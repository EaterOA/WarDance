#include "GameLayer.hpp"
#include "GameResourceManager.hpp"
#include "GameConfig.hpp"
#include "GameController.hpp"
#include "Util.hpp"

bool MainMenu::init()
{
    m_type = MAIN;
    m_numChoices = 5;
    unsigned num_m = 1;

    sf::Vector2f texCoord, size, pos;
    std::ifstream fin;
    fin.open("config/mainmenu.txt");
    if (!fin) return false;

    m_main = std::vector<sf::Vertex>((num_m + m_numChoices)*4, sf::Vertex());
    m_mainLit = m_mainDim = std::vector<sf::Vertex>(m_numChoices*4, sf::Vertex());
    for (unsigned i = 0; i < num_m && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_main[i*4], texCoord, size);
        util::affixPos(&m_main[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < m_numChoices && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_mainLit[i*4], texCoord, size);
        util::affixPos(&m_mainLit[i*4], pos, size, 0);
        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_mainDim[i*4], texCoord, size);
        util::affixPos(&m_mainDim[i*4], pos, size, 0);
    }
    m_blinker = &m_main[0];
    m_menu = &m_main[num_m*4];

    fin.close();

    m_bg.setTexture(resource.getTexture("main_bg"));
    m_blinkerBaseLoc = m_blinker[0].position;
    m_blinkerAlphaChg = 5;
    m_info.setFont(resource.getFont("liberation"));
    m_info.setPosition(25, 525);
    m_info.setCharacterSize(25);
    m_info.setColor(sf::Color(30, 16, 8));
    for (unsigned i = 0; i < m_numChoices; i++) util::copySprite(&m_mainDim[i*4], &m_main[(num_m+i)*4]);

    m_choice = 1;
    selectChoice(1);

    return true;
}

AppLayer::Status MainMenu::tick(std::vector<sf::Event> &e, const sf::Time &t, const sf::Vector2f &m)
{
    //Blinking highlight
    int a = (int)m_blinker[0].color.a;
    sf::Vector2f main_blinkerSize = m_blinker[2].position - m_blinker[0].position;
    sf::Vector2f main_blinkerLoc = m_blinkerBaseLoc + sf::Vector2f(0, (m_choice-1) * 50.f);
    util::affixPos(&m_blinker[0], main_blinkerLoc, main_blinkerSize, 1);
    if (a + m_blinkerAlphaChg < 0 || a + m_blinkerAlphaChg > 255) m_blinkerAlphaChg *= -1;
    util::setAlpha(&m_main[0], (unsigned char)(a + m_blinkerAlphaChg));

    //Updating info
    std::wstringstream wss;
    wss << "Level " << config.getInt("level") << "\nHighscore " << config.getInt("highscore");
    m_info.setString(wss.str());

    //Process events
    for (unsigned i = 0; i < e.size(); i++) {
        //Mouse presses
        if (e[i].type == sf::Event::MouseButtonReleased) {
            if (e[i].mouseButton.button == sf::Mouse::Left) {
                float x = (float)e[i].mouseButton.x;
                float y = (float)e[i].mouseButton.y;
                unsigned opt = translateOption(x, y);
                if (!opt) continue;
                processChoice();
                if (layer.empty()) break;
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

            if (down) selectChoice(m_choice + 1);
            else if (up) selectChoice(m_choice - 1);
            else if (enter) processChoice();
        }
    }

    return AppLayer::HALT; //Main menu should be base layer
}

AppLayer::Status MainMenu::drawStatus() const
{
    return AppLayer::HALT; //Nothing can be seen under main menu
}

void MainMenu::draw(sf::RenderWindow &w) const
{
    sf::RenderStates s(&resource.getTexture("guisheet"));
    w.draw(m_bg);
    w.draw(&m_main[0], m_main.size(), sf::Quads, s);
    w.draw(m_info);
}

void MainMenu::selectChoice(unsigned choice)
{
    if (choice == 0) choice += m_numChoices;
    else if (choice > m_numChoices) choice -= m_numChoices;
    unsigned prev = m_choice-1;
    unsigned next = choice-1;
    util::copySprite(&m_mainDim[prev*4], &m_menu[prev*4]);
    util::copySprite(&m_mainLit[next*4], &m_menu[next*4]);
    m_choice = choice;
}

void MainMenu::processChoice()
{
    if (m_choice == 1) Layer::startBattle();
    else if (m_choice == 2) Layer::goToSelectLevel();
    else if (m_choice == 3) Layer::goToSettings();
    else if (m_choice == 5) Layer::endGame();
}

unsigned MainMenu::translateOption(float x, float y)
{
    sf::Vector2f m(x, y);
    for (unsigned i = 0; i < m_numChoices; i++) {
        unsigned idx = (i+1)*4;
        sf::Vector2f size = m_main[idx+2].position - m_main[idx].position;
        sf::Vector2f pos = util::referenceToCenter(m_main[idx].position, size, 1);
        if (util::hasCollided(m, pos, size, 0)) return i+1;
    }
    return 0;
}