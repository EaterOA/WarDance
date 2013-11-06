#ifndef ACTORS_H
#define ACTORS_H

#include <iostream> //Debugging purposes
#include <SFML/Graphics.hpp>

struct GameState;

class Actor
{
public:
	Actor(int type, sf::Vector2f pos);
	virtual void act(GameState&);
	sf::Vector2f getPos() const;
	int getType() const;
	float getDir() const;
protected:
	int m_type;
	sf::Vector2f m_pos;
	sf::Vector2f m_vel;
	sf::Vector2f m_acc;
	float m_dir;
};

class Fighter: public Actor
{
public:
	Fighter(int type, sf::Vector2f size, sf::Vector2f pos, int hp, int faction);
	virtual void act(GameState &state) = 0;
	virtual bool isDead();
	virtual void hit(int damage);
	sf::Vector2f getSize();
	int getHP() const;
protected:
	virtual void attack(GameState &state) = 0;
	virtual void cooldown();
	virtual void shootTowards(GameState &state, sf::Vector2f &dest, float offsetX, float offsetY);
	int m_attack_cd;
	int m_hp;
	int m_faction;
	sf::Vector2f m_size;
};

class Projectile: public Fighter
{
public:
	Projectile(int type, sf::Vector2f pos, float radius, int damage, int faction);
	void setVel(sf::Vector2f &v);
	void setDir(float dir);
protected:
	float m_radius;
};

class RegularBullet: public Projectile
{
public:
	RegularBullet(sf::Vector2f pos, int faction);
	virtual void act(GameState&);
	virtual bool isDead();
protected:
	virtual void attack(GameState&);
};

class Player: public Fighter
{
public:
	Player(sf::Vector2f pos, int hp);
	virtual void act(GameState& state);
private:
	virtual void attack(GameState& state);
	float m_max_vy, m_max_vx;
	bool m_firing;
};

class Grunt: public Fighter
{
public:
	Grunt(sf::Vector2f pos);
	virtual void act(GameState& state);
protected:
	virtual void attack(GameState& state);
	sf::Vector2f m_max_v;
	int m_decision_cd;
};

class Sprinkler: public Fighter
{
public:
	Sprinkler(sf::Vector2f pos);
	virtual void act(GameState& state);
protected:
	virtual void attack(GameState& state);
};

#endif