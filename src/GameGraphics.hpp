#ifndef GAME_GRAPHICS_HPP
#define GAME_GRAPHICS_HPP

#include <SFML/Graphics.hpp>

class Actor;
class Fighter;
struct GameState;

class GameGraphics : public sf::Drawable, public sf::Transformable
{
public:
	bool init();
	void updateSprites(const GameState&);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void affixPos(sf::Vertex sprite[4], const sf::Vector2f &center, int type);
	void affixPos(sf::Vertex sprite[4], const sf::Vector2f &center, int type, float offsetLY, float offsetRY, float offsetLX, float offsetRX);
	void affixTexture(sf::Vertex sprite[4], int type);
	void rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f &center);
	void affixHealthBar(sf::Vertex bar[8], const Fighter &fighter);
	void transformSprite(sf::Vertex sprite[4], const Actor &actor);
	float m_texCoords[400];
	sf::VertexArray m_sprites;
	sf::Sprite m_map;
	sf::Texture m_spritesheet;
	sf::Texture m_map_tex[5];
};

#endif