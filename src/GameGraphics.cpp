#include "GameGraphics.hpp"
#include "GameMechanics.hpp"
#include "Actors.hpp"
#include "Util.hpp"

bool GameGraphics::init()
{
	if (!m_map_tex[0].loadFromFile("Images/map0.png") || !m_spritesheet.loadFromFile("Images/spritesheet.png"))
		return 0;
	m_map_tex[0].setSmooth(true);
	m_map_tex[0].setRepeated(true);
	m_map.setTexture(m_map_tex[0]);
	m_map.setTextureRect(sf::IntRect(0, 0, Util::MAP_WIDTH, Util::MAP_HEIGHT));
	m_spritesheet.setSmooth(true);
	return 1;
}

void GameGraphics::affixPos(sf::Vertex sprite[4], const sf::Vector2f &center, float offsetLY, float offsetRY, float offsetLX, float offsetRX)
{
	sprite[0].position = center + sf::Vector2f(offsetLX, offsetLY);
	sprite[1].position = center + sf::Vector2f(offsetRX, offsetLY);
	sprite[2].position = center + sf::Vector2f(offsetRX, offsetRY);
	sprite[3].position = center + sf::Vector2f(offsetLX, offsetRY);
}

void GameGraphics::affixPos(sf::Vertex sprite[4], const sf::Vector2f &center)
{
	affixPos(sprite, center, -50.f, 50.f, -50.f, 50.f);
}

void GameGraphics::affixTexture(sf::Vertex sprite[4], int type)
{
	float offsetX = (type % 2) * 100.f;
	float offsetY = (type / 2) * 100.f;
	sprite[0].texCoords = sf::Vector2f(offsetX, offsetY);
	sprite[1].texCoords = sf::Vector2f(offsetX+100, offsetY);
	sprite[2].texCoords = sf::Vector2f(offsetX+100, offsetY+100);
	sprite[3].texCoords = sf::Vector2f(offsetX, offsetY+100);
}

void GameGraphics::rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f &center)
{
	sf::Transform transform;
	transform.rotate(dir, center);
	for (unsigned i = 0; i < 4; i++)
		sprite[i].position = transform.transformPoint(sprite[i].position);
}

void GameGraphics::transformSprite(sf::Vertex sprite[4], const Actor &actor)
{
	affixPos(sprite, actor.getPos());
	affixTexture(sprite, actor.getType());
	if (actor.getDir() != 0.f) rotateSprite(sprite, actor.getDir() * 180.f / PI, actor.getPos());
}

void GameGraphics::affixHealthBar(sf::Vertex bar[8], const Fighter &fighter)
{
	float hpOffset = 40.f * fighter.getHP() / fighter.getMaxHP();
	affixPos(bar, fighter.getPos() + sf::Vector2f(0, -30), -50.f, 50.f, -20.f, 20.f);
	affixPos(bar+4, fighter.getPos() + sf::Vector2f(0, -30), -50.f, 50.f, -20.f, -20.f + hpOffset);
	affixTexture(bar, 4);
	affixTexture(bar+4, 5);
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