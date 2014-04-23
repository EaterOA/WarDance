#include "GameScript.hpp"
#include "BattleMechanics.hpp"
#include "Util.hpp"
#include <string>
#include <sstream>
#include <list>

Event::Event()
{
    time = 0;
    count = 1;
    random = true;
}

GameScript::GameScript(BattleMechanics* origin)
{
    m_origin = origin;
}

bool GameScript::parseFile(const std::string &path)
{
    std::ifstream t;
    t.open(path.c_str());
    if (!t) return false;
    std::stringstream buffer;
    buffer << t.rdbuf();
    return parse(buffer.str());
}

bool GameScript::parse(const std::string &script)
{
    m_events.clear();

    m_script = script;
    std::istringstream iss(script);

    std::string line;
    while(std::getline(iss, line))
    {
        std::vector<std::string> events = util::split(line);
        float t = (float)std::atof(events[0].c_str());

        for (unsigned i = 1; i < events.size(); i++)
        {
            Event e;
            e.time = t;
            std::vector<std::string> fields = util::split(events[i], '|');
            e.name = fields[0];
            for (unsigned j = 1; j < fields.size(); j++)
            {
                if (fields[j].empty()) continue;
                char tag = fields[j][0];
                std::string word = fields[j].substr(1);
                switch (tag) {
                    case '#':
                        e.count = atoi(word.c_str()); break;
                    case '!':
                        e.item = word; break;
                    case '@': {
                        e.random = false;
                        std::vector<std::string> pos = util::split(word, '-');
                        e.pos.x = (float)std::atof(pos[0].c_str());
                        e.pos.y = (float)std::atof(pos[1].c_str()); break;
                    }
                    default: break;
                }
            }
            m_events.push_back(e);
        }
    }

    return true;
}

void GameScript::tick(float t)
{
    if (m_events.empty())
    {
        return;
    }

    std::list<Event>::iterator it = m_events.begin();
    while (it != m_events.end())
    {
        if (it->time <= t)
        {
            Event e = *it;
            for (int i = 0; i < e.count; i++) {
                if (e.random)
                    m_origin->spawnEnemy(e.name, e.item);
                else
                    m_origin->spawnEnemy(e.name, e.item, e.pos);
            }
            it = m_events.erase(it);
        }
        else
        {
            break; //assuming in order
        }
    }
}

bool GameScript::isDone()
{
    return m_events.empty(); //we're done if there are no more events to process
}

GameScript::~GameScript()
{
}
