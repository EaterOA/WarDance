#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include "Config.hpp"
#include <list>

class GameMechanics;

struct Event
{
	float time;
	std::string name;
	sf::Vector2f pos;
	bool random;

	Event(float t, std::string n, sf::Vector2f p);
	Event(float t, std::string n);
};

/*
  Right now, all that is supported is a simple [time [typename number] (1 or more of these clauses)] syntax,
  with newlines acting as separators between lines.
*/
class GameScript
{
public:
	GameScript(std::string script, GameMechanics* origin);
	~GameScript();

	void tick(float t);
	
private:
	GameMechanics* m_origin;
	std::list<Event> m_events;
	std::string m_script;
};

#endif