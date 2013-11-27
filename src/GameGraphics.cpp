#include "GameGraphics.hpp"
#include "GameMechanics.hpp"
#include "Actors.hpp"
#include "Util.hpp"
#include <fstream>

bool GameGraphics::init()
{
	std::ifstream fin;	
    fin.open("config/spritesheet.txt");
	if (!fin) return 0;
	for (int i = 0; i < 100*4 && fin >> m_texCoords[i] >> m_texCoords[i+1] >> m_texCoords[i+2] >> m_texCoords[i+3]; i += 4) fin.ignore(1000, '\n');
    fin.close();
	if (!m_map_tex[0].loadFromFile("images/map0.png")) return 0;
	if (!m_spritesheet.loadFromFile("images/spritesheet.png")) return 0;
	m_map_tex[0].setSmooth(true);
	m_map_tex[0].setRepeated(true);
	m_map.setTexture(m_map_tex[0]);
	m_map.setTextureRect(sf::IntRect(0, 0, 1600, 1200));
	m_spritesheet.setSmooth(true);
	return 1;
}

void GameGraphics::affixPos(sf::Vertex sprite[4], const sf::Vector2f &center, int type, float offsetLX, float offsetRX, float offsetLY, float offsetRY)
{
	sprite[0].position = center + sf::Vector2f(offsetLX, offsetLY);
	sprite[1].position = center + sf::Vector2f(offsetRX, offsetLY);
	sprite[2].position = center + sf::Vector2f(offsetRX, offsetRY);
	sprite[3].position = center + sf::Vector2f(offsetLX, offsetRY);
}

void GameGraphics::affixPos(sf::Vertex sprite[4], const sf::Vector2f &center, int type)
{
	affixPos(sprite, center, type, -m_texCoords[type*4+2]/2, m_texCoords[type*4+2]/2, -m_texCoords[type*4+3]/2, m_texCoords[type*4+3]/2);
}

void GameGraphics::affixTexture(sf::Vertex sprite[4], int type)
{
	float x = m_texCoords[type*4+0];
	float y = m_texCoords[type*4+1];
	float w = m_texCoords[type*4+2];
	float h = m_texCoords[type*4+3];
	sprite[0].texCoords = sf::Vector2f(x, y);
	sprite[1].texCoords = sf::Vector2f(x+w, y);
	sprite[2].texCoords = sf::Vector2f(x+w, y+h);
	sprite[3].texCoords = sf::Vector2f(x, y+h);
}

void GameGraphics::rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f center)
{
	sf::Transform transform;
	transform.rotate(dir, center);
	for (unsigned i = 0; i < 4; i++)
		sprite[i].position = transform.transformPoint(sprite[i].position);
}

void GameGraphics::transformSprite(sf::Vertex sprite[4], const Actor &actor)
{
	affixPos(sprite, actor.getPos(), actor.getType());
	affixTexture(sprite, actor.getType());
	if (actor.getDir() != 0.f) rotateSprite(sprite, actor.getDir() * 180.f / util::PI, actor.getPos());
}

void GameGraphics::affixHealthBar(sf::Vertex bar[8], const Fighter &fighter)
{
	float hpOffset = 40.f * fighter.getHP() / fighter.getMaxHP();
	affixPos(bar, fighter.getPos() + sf::Vector2f(0, -30), 0, -20.f, 20.f, -4.5f, 4.5f);
	affixPos(bar+4, fighter.getPos() + sf::Vector2f(0, -30), 1, -20.f, -20.f + hpOffset, -4.5f, 4.5f);
	affixTexture(bar, 0);
	affixTexture(bar+4, 1);
}

void GameGraphics::updateSprites(const GameState &state)
{
	unsigned i = 4, j;
	unsigned numSprites = 1 + 3*state.enemies.size() + state.projectiles.size();
	m_sprites = sf::VertexArray(sf::Quads, 4*numSprites);

	transformSprite(&m_sprites[0], *state.player);
	for (j = 0; j < state.enemies.size(); i += 3*4, j++) {
		transformSprite(&m_sprites[i], *state.enemies[j]);
		affixHealthBar(&m_sprites[i+4], *state.enemies[j]);
	}
	for (j = 0; j < state.projectiles.size(); i += 4, j++)
		transformSprite(&m_sprites[i], *state.projectiles[j]);

}

void GameGraphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_map);
	target.draw(m_sprites, &m_spritesheet);
}
