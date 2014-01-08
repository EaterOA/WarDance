#include "GameScript.hpp"
#include "GameMechanics.hpp"
#include "Util.hpp"
#include <string>
#include <sstream>
#include <list>

Event::Event(float t, std::string n, sf::Vector2f p)
{
	time = t;
	name = n;
	pos = p;
	random = false;
}

Event::Event(float t, std::string n)
{
	time = t;
	name = n;
	random = true;
}

GameScript::GameScript(GameMechanics* origin)
{
	m_origin = origin;
}

bool GameScript::parseFile(const std::string &path, float elapsed)
{
	std::ifstream t;
	t.open(path.c_str());
	if (!t) return false;
	std::stringstream buffer;
	buffer << t.rdbuf();
	return parse(buffer.str(), elapsed);
}

bool GameScript::parse(const std::string &script, float elapsed)
{
	m_script = script;
	std::istringstream iss(script);

	std::string line;
	float t;
	int gruntcount;
	int aliencount;
	int sprinklercount;
	while(std::getline(iss, line)) //format should be 5.0 grunt 3 alien 2 sprinkler 1
	{
		std::vector<std::string> fields = util::split(line, ' ');
		if (fields.size() < 3 || fields.size() % 2 == 0) //must have time, and equal number of types and numbers
		{
			break; //error
		}
		t = (float)std::atof(fields[0].c_str()) + elapsed;

		for (unsigned i = 1; i < fields.size(); i = i + 2)
		{
			std::string word = fields[i];
			if (word == "grunt")
			{
				gruntcount = atoi(fields[i + 1].c_str());
				for (int j = 0; j < gruntcount; j++)
				{
					Event e(t, word);
					m_events.push_back(e);
				}
			}
			else if (word == "alien")
			{
				aliencount = atoi(fields[i + 1].c_str());
				for (int j = 0; j < aliencount; j++)
				{
					Event e(t, word);
					m_events.push_back(e);
				}
			}
			else if (word == "sprinkler")
			{
				sprinklercount = atoi(fields[i + 1].c_str());
				for (int j = 0; j < sprinklercount; j++)
				{
					Event e(t, word);
					m_events.push_back(e);
				}
			}
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
			if (e.random)
			{
				m_origin->spawnEnemy(e.name);
			}
			else
			{
				m_origin->spawnEnemy(e.name, e.pos);
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
