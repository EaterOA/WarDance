#include "GameLayer.hpp"
#include "GameResourceManager.hpp"
#include "GameConfig.hpp"
#include "GameController.hpp"
#include "Util.hpp"

bool ScoreScreen::init()
{
    m_type = SCORE;
    unsigned num_e = 6;
    unsigned num_level_icons = 10;

    sf::Vector2f texCoord, size, pos;
    std::ifstream fin;
    fin.open("config/scorescreen.txt");
    if (!fin) return false;

    m_numbers = std::vector<sf::Text>(6, sf::Text());
    for (unsigned i = 0; i < num_e && util::read3v2f(fin, texCoord, size, pos, true); i++) {
        m_numbers[i].setPosition(pos);
        m_numbers[i].setFont(resource.getFont("stencil"));
        if (i+1 < num_e) {
            m_numbers[i].setColor(sf::Color::White);
        }
        else {
            m_numbers[i].setColor(sf::Color::Green);
            m_numbers[i].setCharacterSize(40);
        }
    }
    util::read3v2f(fin, texCoord, size, pos, true);
    util::affixPos(m_levelDisplay, pos, size, 0);

    fin.close();
    fin.open("config/levelicon.txt");
    if (!fin) return false;
    
    int lvl = config.getInt("level");
    unsigned idx = (unsigned)(lvl-1);
    for (unsigned i = 0; i < num_level_icons && util::read3v2f(fin, texCoord, size, pos, true); i++)
        if (i == idx)
            break;
    util::affixTexture(m_levelDisplay, texCoord, size);

    fin.close();

    m_screen.setTexture(resource.getTexture("scoring"));

    std::string dataList[] = {"accuracy", "time", "", "", "", "bonus"};
    for (unsigned i = 0; i < 6; i++) {
        std::wstringstream wss;
        wss << config.getInt(dataList[i], 0);
        m_numbers[i].setString(wss.str());
    }
    m_scoring_timing_stage = 0;
    m_scoring_timing = 1.f;
    m_sequence_timing_stage = 0;
    m_sequence_timing = 2.5f;

    return true;
}

AppLayer::Status ScoreScreen::tick(std::vector<sf::Event> &e, const sf::Time &t, sf::Vector2f m)
{
    //Process events
    for (unsigned i = 0; i < e.size(); i++) {
        //Keyboard events
        if (e[i].type == sf::Event::KeyPressed) {
            bool enter = controller.pressing(GameController::K_ENTER, e[i].key.code);
            bool esc = controller.pressing(GameController::K_ESCAPE, e[i].key.code);

            if (enter) forwardSequence();
            if (esc) Layer::pauseBattle();
        }
    }

    //Initial wait for scorescreen to appear
    if (m_sequence_timing_stage == 0) {
        m_sequence_timing -= t.asSeconds();
        if (m_sequence_timing <= 0) {
            m_sequence_timing_stage++;
        }
    }
    //Waiting for all the scores to be displayed
    else if (m_sequence_timing_stage == 1) {
        m_scoring_timing -= t.asSeconds();
        if (m_scoring_timing <= 0) {
            m_scoring_timing_stage++;
            if (m_scoring_timing_stage == 6) {
                for (unsigned i = 0; i < layer.size(); i++) {
                    if (layer[i]->getType() == BATTLE) {
                        ((Battle*)layer[i])->incScore(config.getInt("bonus"));
                        break;
                    }
                }
                m_sequence_timing_stage++;
            }
            else {
                m_scoring_timing = 0.5f;
            }
        }
    }
    //Score screen gone, waiting for background fade to begin
    else if (m_sequence_timing_stage == 3) {
        m_sequence_timing -= t.asSeconds();
        if (m_sequence_timing <= 0) {
            for (unsigned i = 0; i < layer.size(); i++) {
                if (layer[i]->getType() == BATTLE) {
                    ((Battle*)layer[i])->startLevelTransition();
                    Layer::back();
                    break;
                }
            }
        }
    }

    return GameLayer::PASS;
}

AppLayer::Status ScoreScreen::drawStatus() const
{
    return AppLayer::PASS;
}

void ScoreScreen::draw(sf::RenderWindow &w) const
{
    sf::RenderStates s(&resource.getTexture("guisheet"));
    if (m_sequence_timing_stage == 1 || m_sequence_timing_stage == 2) {
        w.draw(m_screen);
        w.draw(m_levelDisplay, 4, sf::Quads, s);
        for (int j = 0; j < m_scoring_timing_stage; j++) {
            w.draw(m_numbers[(unsigned)j]);
        }
    }
}

void ScoreScreen::forwardSequence()
{
    if (m_sequence_timing_stage <= 1) {
        m_sequence_timing_stage = 1;
        m_scoring_timing_stage = 5;
        m_scoring_timing = 0.f;
    }
    else if (m_sequence_timing_stage == 2) {
        m_sequence_timing_stage = 3;
        m_sequence_timing = 2.f;
    }
}
