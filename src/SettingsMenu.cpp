#include "GameLayer.hpp"
#include "GameResourceManager.hpp"
#include "GameConfig.hpp"
#include "GameController.hpp"
#include "Util.hpp"

bool SettingsMenu::init()
{
    m_type = SETTINGS;
    m_numChoices = 1;

    sf::Vector2f texCoord, size, pos;
    std::ifstream fin;
    fin.open("config/settingsmenu.txt");
    if (!fin) return false;
    
    m_settings = std::vector<sf::Vertex>(m_numChoices*2*4, sf::Vertex());
    m_settingsLit = m_settingsDim = std::vector<sf::Vertex>(m_numChoices*4, sf::Vertex());
    m_settingsOn = m_settingsOff = std::vector<sf::Vertex>(m_numChoices*4, sf::Vertex());
    for (unsigned i = 0; i < m_numChoices && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_settingsLit[i*4], texCoord, size);
        util::affixPos(&m_settingsLit[i*4], pos, size, 0);

        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_settingsDim[i*4], texCoord, size);
        util::affixPos(&m_settingsDim[i*4], pos, size, 0);

        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_settingsOn[i*4], texCoord, size);
        util::affixPos(&m_settingsOn[i*4], pos, size, 0);

        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_settingsOff[i*4], texCoord, size);
        util::affixPos(&m_settingsOff[i*4], pos, size, 0);
    }
    m_menu = &m_settings[0];
    m_switches = &m_settings[m_numChoices*4];

    fin.close();

    m_bg.setTexture(resource.getTexture("settings_bg"));
    for (unsigned i = 0; i < m_numChoices; i++) util::copySprite(&m_settingsDim[i*4], &m_settings[i*4]);

    m_choice = 1;
    selectChoice(1);

    return true;
}

AppLayer::Status SettingsMenu::tick(std::vector<sf::Event> &e, const sf::Time &t, const sf::Vector2f &m)
{
    processSwitches();

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
                break;
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
            else if (esc) {
                Layer::back();
                break;
            }
        }
    }

    return AppLayer::HALT;
}

AppLayer::Status SettingsMenu::drawStatus() const
{
    return AppLayer::HALT;
}

void SettingsMenu::draw(sf::RenderWindow &w) const
{
    sf::RenderStates s(&resource.getTexture("guisheet"));
    w.draw(m_bg);
    w.draw(&m_settings[0], m_settings.size(), sf::Quads, s);
}

void SettingsMenu::selectChoice(unsigned choice)
{
    if (choice == 0) choice += m_numChoices;
    else if (choice > m_numChoices) choice -= m_numChoices;
    unsigned prev = m_choice-1;
    unsigned next = choice-1;
    util::copyTexture(&m_settingsDim[prev*4], &m_menu[prev*4]);
    util::copyTexture(&m_settingsLit[next*4], &m_menu[next*4]);
    m_choice = choice;
}

void SettingsMenu::processChoice()
{
    if (m_choice == 1) {
        config.setInt("hitbox_enabled", !config.getInt("hitbox_enabled"));
    }
}

void SettingsMenu::processSwitches()
{
    util::copySprite((config.getInt("hitbox_enabled") ? &m_settingsOn[0] : &m_settingsOff[0]), &m_switches[0]);
}

unsigned SettingsMenu::translateOption(float x, float y)
{
    sf::Vector2f m(x, y);
    for (unsigned i = 0; i < m_numChoices; i++) {
        unsigned idx = i*4;
        sf::Vector2f size = m_menu[idx+2].position - m_menu[idx].position;
        sf::Vector2f pos = util::referenceToCenter(m_menu[idx].position, size, 1);
        if (util::hasCollided(m, pos, size, 0)) return i+1;
    }
    return 0;
}