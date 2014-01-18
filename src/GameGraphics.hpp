#ifndef GAME_GRAPHICS_HPP
#define GAME_GRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Actor;
class Fighter;
struct GameState;

struct FrameData
{
	unsigned sheetNum;
	float texCoord[4];
	float posOffset[2];
	bool rotatable;
	unsigned rgba;
};

class GameGraphics : public sf::Drawable, public sf::Transformable
{
public:
	bool init();
	void updateSprites(const GameState&, float bgFade);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void affixPos(sf::Vertex sprite[4], sf::Vector2f center, const FrameData &d, float offsetLX, float offsetRX, float offsetLY, float offsetRY);
	void affixPos(sf::Vertex sprite[4], sf::Vector2f center, const FrameData &d);
	void affixTexture(sf::Vertex sprite[4], const FrameData &d);
	void rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f center, const FrameData &d);
	void applyColor(sf::Vertex sprite[4], const FrameData &d);
	void addHealthBar(const Fighter &fighter);
	void addSprite(const Actor &actor);
    void addHitbox(const Fighter &fighter);
	
	std::map<std::string, FrameData> m_frameMap;
	std::vector<sf::Texture> m_lvlBackgroundTex;
	sf::Sprite m_background, m_backgroundNext;
	std::vector<sf::Texture> m_spritesheet;
	std::vector<std::vector<sf::Vertex> > m_sprites;
	std::vector<sf::Vertex> m_specialSprites;
    std::vector<sf::Vertex> m_hitboxes;

	int m_hitbox_enabled;
	unsigned m_numSheets;
};

#endif
