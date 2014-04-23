#ifndef GAMESCRIPT_H
#define GAMESCRIPT_H

#include <SFML/Graphics.hpp>
#include <list>

class BattleMechanics;

struct Event
{
    Event();

    float time;
    std::string name;
    std::string item;
    sf::Vector2f pos;
    bool random;
    int count;
};

/*
  Right now, all that is supported is a simple [time [typename number] (1 or more of these clauses)] syntax,
  with newlines acting as separators between lines.
*/
class GameScript
{
public:
    GameScript(BattleMechanics* origin);
    ~GameScript();
    bool parseFile(const std::string &path);
    bool parse(const std::string &script);
    void tick(float t);
    bool isDone();
private:
    BattleMechanics* m_origin;
    std::list<Event> m_events;
    std::string m_script;
};

#endif
