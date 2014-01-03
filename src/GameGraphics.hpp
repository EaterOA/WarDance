#ifndef GAME_GRAPHICS_HPP
#define GAME_GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

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
	void affixPos(sf::Vertex sprite[4], sf::Vector2f center, const std::string& frame, float offsetLX, float offsetRX, float offsetLY, float offsetRY);
	void affixPos(sf::Vertex sprite[4], sf::Vector2f center, const std::string& frame);
	void affixTexture(sf::Vertex sprite[4], const std::string& frame);
	void rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f center);
	void addHealthBar(const Fighter &fighter);
	void addSprite(const Actor &actor);
    void addHitbox(const Fighter &fighter);
	
	unsigned m_sheetNum[1*1000];
	float m_texCoords[4*1000];
	float m_sprCoords[2*1000];
	std::map<std::string, int> m_frameMap;
	std::vector<sf::Texture> m_lvlBackgroundTex;
	sf::Sprite m_background;
	std::vector<sf::Texture> m_spritesheet;
	std::vector<std::vector<sf::Vertex> > m_sprites;
    std::vector<sf::Vertex> m_hitboxes;

	int m_hitbox_enabled;
	unsigned m_numSheets;
};

#endif
