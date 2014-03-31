#ifndef BATTLEGRAPHICS_HPP
#define BATTLEGRAPHICS_HPP

#include <SFML/Graphics.hpp>
#include <vector>

class Actor;
class Fighter;
struct BattleState;

struct FrameData
{
    unsigned sheetNum;
    sf::Vector2f size;
    sf::Vector2f texCoord;
    sf::Vector2f posOffset;
};

class BattleGraphics : public sf::Drawable, public sf::Transformable
{
public:
    bool init();
    void updateState(const BattleState& state);
    void setTransition(float alpha);
private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void addHealthBar(const Fighter &fighter);
    void addSprite(const Actor &actor);
    void addHitbox(const Fighter &fighter);
    
    std::map<std::string, FrameData> m_frameMap;
    std::vector<const sf::Texture*> m_lvlBackgroundTex;
    sf::Sprite m_background, m_backgroundNext;
    std::vector<const sf::Texture*> m_spritesheet;
    std::vector<std::vector<sf::Vertex> > m_sprites;
    std::vector<sf::Vertex> m_specialSprites;
    std::vector<sf::Vertex> m_hitboxes;

    int m_hitbox_enabled;
};

#endif
