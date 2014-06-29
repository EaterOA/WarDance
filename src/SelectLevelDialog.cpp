#include "GameLayer.hpp"
#include "GameResourceManager.hpp"
#include "GameConfig.hpp"
#include "GameController.hpp"
#include "Util.hpp"

bool SelectLevelDialog::init()
{
    m_type = SELECTLEVEL;
    unsigned num_l = 2, num_ll = 2;
    unsigned num_level_icons = 10;

    sf::Vector2f texCoord, size, pos;
    std::ifstream fin;
    fin.open("config/selectleveldialog.txt");
    if (!fin) return false;

    m_select = std::vector<sf::Vertex>((num_l + num_ll)*4, sf::Vertex());
    m_selectLit = m_selectDim = std::vector<sf::Vertex>(num_ll*4, sf::Vertex());
    for (unsigned i = 0; i < num_l && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_select[i*4], texCoord, size);
        util::affixPos(&m_select[i*4], pos, size, 0);
    }
    for (unsigned i = 0; i < num_ll && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_selectLit[i*4], texCoord, size);
        util::affixPos(&m_selectLit[i*4], pos, size, 0);
        util::read3v2f(fin, texCoord, size, pos, true);
        util::affixTexture(&m_selectDim[i*4], texCoord, size);
        util::affixPos(&m_selectDim[i*4], pos, size, 0);
    }

    fin.close();
    fin.open("config/levelicon.txt");
    if (!fin) return false;
    
    m_levelIcons = std::vector<std::vector<sf::Vertex> >(num_level_icons, std::vector<sf::Vertex>(4));
    for (unsigned i = 0; i < num_level_icons && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        util::affixTexture(&m_levelIcons[i][0], texCoord, size);
        util::affixPos(&m_levelIcons[i][0], pos, size, 0);
    }

    fin.close();

    m_upLitTime = m_downLitTime = 0.f;
    util::copyTexture(&m_levelIcons[0][0], &m_select[1*4]);
    for (unsigned i = 0; i < num_ll; i++) util::copySprite(&m_selectDim[i*4], &m_select[(num_l+i)*4]);

    selectChoice((unsigned)config.getInt("level"));

    return true;
}

AppLayer::Status SelectLevelDialog::tick(std::vector<sf::Event> &e, const sf::Time &t, sf::Vector2f m)
{
    //Lighting or dimming select level arrows
    m_upLitTime -= t.asSeconds();
    m_downLitTime -= t.asSeconds();
    if (m_upLitTime > 0) util::copyTexture(&m_selectLit[0*4], &m_select[2*4]);
    else util::copyTexture(&m_selectDim[0*4], &m_select[2*4]);
    if (m_downLitTime > 0) util::copyTexture(&m_selectLit[1*4], &m_select[3*4]);
    else util::copyTexture(&m_selectDim[1*4], &m_select[3*4]);

    for (unsigned i = 0; i < e.size(); i++) {
        //Mouse presses
        if (e[i].type == sf::Event::MouseButtonReleased) {
            if (e[i].mouseButton.button == sf::Mouse::Left) {
                float x = (float)e[i].mouseButton.x;
                float y = (float)e[i].mouseButton.y;
                unsigned opt = translateOption(x, y);
                if (!opt) continue;
                if (opt == 1) {
                    m_upLitTime = 0.1f;
                    selectChoice(m_choice + 1);
                }
                else if (opt == 2) {
                    m_downLitTime = 0.1f;
                    selectChoice(m_choice - 1);
                }
            }
            else if (e[i].mouseButton.button == sf::Mouse::Right) {
                Layer::back();
                break;
            }
        }
        //Keyboard events
        else if (e[i].type == sf::Event::KeyPressed) {
            bool up = controller.pressing(GameController::K_UP, e[i].key.code);
            bool down = controller.pressing(GameController::K_DOWN, e[i].key.code);
            bool enter = controller.pressing(GameController::K_ENTER, e[i].key.code);
            bool esc = controller.pressing(GameController::K_ESCAPE, e[i].key.code);

            if (down) {
                m_downLitTime = 0.1f;
                selectChoice(m_choice - 1);
            }
            else if (up) {
                m_upLitTime = 0.1f;
                selectChoice(m_choice + 1);
            }
            else if (enter) {
                processChoice();
                break;
            }
            else if (esc) {
                Layer::back();
                break;
            }
        }
    }
    
    e = std::vector<sf::Event>(); //Purge all events
    return AppLayer::PASS;
}

AppLayer::Status SelectLevelDialog::drawStatus() const
{
    return AppLayer::PASS;
}

void SelectLevelDialog::draw(sf::RenderWindow &w) const
{
    sf::RenderStates s(&resource.getTexture("guisheet"));
    sf::RectangleShape fade;
    fade.setFillColor(sf::Color(0, 0, 0, 100));
    fade.setPosition(0, 0);
    fade.setSize(sf::Vector2f((float)w.getSize().x, (float)w.getSize().y));
    w.draw(fade);
    w.draw(&m_select[0], m_select.size(), sf::Quads, s);
}

void SelectLevelDialog::selectChoice(unsigned choice)
{
    if (choice == 0 || choice > (unsigned)config.getInt("num_levels")) return;
    m_choice = choice;
    util::copyTexture(&m_levelIcons[choice-1][0], &m_select[1*4]);
}

void SelectLevelDialog::processChoice()
{
    config.setInt("level", (int)m_choice);
    Layer::back();
    Layer::startBattle();
}

unsigned SelectLevelDialog::translateOption(float x, float y)
{
    sf::Vector2f m(x, y);
    for (unsigned i = 0; i < 2; i++) {
        unsigned idx = (i+2)*4;
        sf::Vector2f size = m_select[idx+2].position - m_select[idx].position;
        sf::Vector2f pos = util::referenceToCenter(m_select[idx].position, size, 1);
        if (util::hasCollided(m, pos, size, 0)) return i+1;
    }
    return 0;
}