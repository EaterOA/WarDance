#include "Util.hpp"
#include "Config.hpp"
#include "GameGraphics.hpp"
#include "GameMechanics.hpp"

bool GameGraphics::init()
{
	std::ifstream fin;
	
	//Initializing texture and map lists
	unsigned numLevels = unsigned(config["num_levels"]);
	m_lvlBackgroundTex = std::vector<sf::Texture>(numLevels);
	m_lvlSpritesheet = std::vector<sf::Texture>(numLevels);
	m_lvlFrameMap = std::vector<std::map<std::string, int> >(numLevels);

	//Loading level background textures
	for (unsigned i = 0; i < numLevels; i++) {
		std::stringstream ss;
		ss << "images/bg" << i << ".png";
		if (!m_lvlBackgroundTex[i].loadFromFile(ss.str())) return false;
		m_lvlBackgroundTex[i].setRepeated(true);
	}

	//Loading level spritesheets
	for (unsigned i = 0; i < numLevels; i++) {
		std::stringstream ss;
		ss << "images/spritesheet" << i << ".png";
		if (!m_lvlSpritesheet[i].loadFromFile(ss.str())) return false;
		m_lvlSpritesheet[i].setSmooth(true);
	}

	//Reading sprite data
	//Data format: frame name, 4 texture coordinates, 2 position offsets
    fin.open("config/spritedata.txt");
	if (!fin) return 0;
	for (unsigned i = 0, j = 0; i < 1000;) {
		std::string frame;
		if (!(fin >> frame)) break;
		if (frame[0] == '-') {
			j++;
		}
		else {
			m_lvlFrameMap[j][frame] = (int)i;
			if (!util::readf(fin, 4, m_texCoords + i*4, false)) return false;
			if (!util::readf(fin, 2, m_sprCoords + i*2, false)) return false;
            i++;
		}
		fin.ignore(1000, '\n');
	}
    fin.close();

	//Initializing some sprites and settings
	m_background.setTexture(m_lvlBackgroundTex[0]);
	m_background.setTextureRect(sf::IntRect(0, 0, 1600, 1200));
	m_hitbox_enabled = config["hitbox_enabled"];
	m_level = (unsigned)config["level"];

	return true;
}

void GameGraphics::affixPos(sf::Vertex sprite[4], sf::Vector2f center, const std::string& frame, float offsetLX, float offsetRX, float offsetLY, float offsetRY)
{
	float* coord = m_sprCoords + m_lvlFrameMap[(unsigned)m_level][frame] * 2;

	center += sf::Vector2f(coord[0], coord[1]);
	sprite[0].position = center + sf::Vector2f(offsetLX, offsetLY);
	sprite[1].position = center + sf::Vector2f(offsetRX, offsetLY);
	sprite[2].position = center + sf::Vector2f(offsetRX, offsetRY);
	sprite[3].position = center + sf::Vector2f(offsetLX, offsetRY);
}

void GameGraphics::affixPos(sf::Vertex sprite[4], sf::Vector2f center, const std::string& frame)
{
	float* coord = m_texCoords + m_lvlFrameMap[(unsigned)m_level][frame] * 4;

	float hwidth = coord[2] / 2.f;
	float hheight = coord[3] / 2.f;
	affixPos(sprite, center, frame, -hwidth, hwidth, -hheight, hheight);
}

void GameGraphics::affixTexture(sf::Vertex sprite[4], const std::string& frame)
{
	float* coord = m_texCoords + m_lvlFrameMap[(unsigned)m_level][frame] * 4;

	float x = coord[0];
	float y = coord[1];
	float w = coord[2];
	float h = coord[3];
	sprite[0].texCoords = sf::Vector2f(x, y);
	sprite[1].texCoords = sf::Vector2f(x+w, y);
	sprite[2].texCoords = sf::Vector2f(x+w, y+h);
	sprite[3].texCoords = sf::Vector2f(x, y+h);
}

void GameGraphics::rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f center)
{
	sf::Transform tr;
	tr.rotate(dir, center);
	for (unsigned i = 0; i < 4; i++)
		sprite[i].position = tr.transformPoint(sprite[i].position);
}

void GameGraphics::transformSprite(sf::Vertex sprite[4], const Actor &actor)
{
	affixPos(sprite, actor.getPos(), actor.getFrame());
	affixTexture(sprite, actor.getFrame());
	rotateSprite(sprite, util::toDeg(actor.getDir()), actor.getPos());
}

void GameGraphics::affixHealthBar(sf::Vertex bar[8], const Fighter &fighter)
{
	float hpOffset = 40.f * fighter.getHP() / fighter.getMaxHP();
	affixPos(bar, fighter.getPos(), "reg_hpcase", -20.f, 20.f, -4.5f, 4.5f);
	affixPos(bar+4, fighter.getPos(), "reg_hpbar", -20.f, -20.f + hpOffset, -4.5f, 4.5f);
	affixTexture(bar, "reg_hpcase");
	affixTexture(bar+4, "reg_hpbar");
}

void GameGraphics::addHitbox(const Fighter &f)
{
    sf::Transform tr;
    util::ShapeVector size = f.getSize();
    if (size.s == util::Rectangle) {
        sf::Vector2f c = f.getPos();
        sf::Vector2f hsize(size.x / 2.f, size.y / 2.f);
        tr.rotate(util::toDeg(f.getDir()), c);
        sf::Vector2f tp[] = {tr.transformPoint(c - hsize),
                             tr.transformPoint(sf::Vector2f(c.x + hsize.x, c.y - hsize.y)),
                             tr.transformPoint(c + hsize),
                             tr.transformPoint(sf::Vector2f(c.x - hsize.x, c.y + hsize.y))};
        for (int i = 0, j = 1; i < 4; i++, j = (j+1)%4) {
            m_hitboxes.push_back(sf::Vertex(tp[i]));
            m_hitboxes.push_back(sf::Vertex(tp[j]));
        }
    }
	else if (size.s == util::Circle) {
		sf::Vector2f c = f.getPos();
		float r = size.x;
		int numPt = (int)(sqrt(r) * 3);
		tr.rotate(360.f / numPt, c);
		sf::Vector2f cur(c.x + r, c.y), next;
		for (int i = 0; i < numPt; i++) {
			next = tr.transformPoint(cur);
			m_hitboxes.push_back(sf::Vertex(cur));
			m_hitboxes.push_back(sf::Vertex(next));
			cur = next;
		}
	}
}

void GameGraphics::updateSprites(const GameState &state)
{
	//Updating settings
	m_hitbox_enabled = config["hitbox_enabled"];
	m_level = (unsigned)config["level"];
	m_background.setTexture(m_lvlBackgroundTex[m_level]);

	//Recalculating sprite appearance
	unsigned i = 4, j;
	unsigned numSprites = 1 + 3*state.enemies.size() + state.projectiles.size() + state.items.size();
	m_sprites = sf::VertexArray(sf::Quads, 4*numSprites);
	if (m_hitbox_enabled)
		m_hitboxes = std::vector<sf::Vertex>();

	transformSprite(&m_sprites[0], *state.player);
    if (m_hitbox_enabled) addHitbox(*state.player);

	for (j = 0; j < state.enemies.size(); i += 3*4, j++) {
		transformSprite(&m_sprites[i], *state.enemies[j]);
		affixHealthBar(&m_sprites[i+4], *state.enemies[j]);
        if (m_hitbox_enabled) addHitbox(*state.enemies[j]);
	}

	for (j = 0; j < state.projectiles.size(); i += 4, j++) {
		transformSprite(&m_sprites[i], *state.projectiles[j]);
	}
    for (j = 0; j < state.items.size(); i += 4, j++) {
        transformSprite(&m_sprites[i], *state.items[j]);
    }

}

void GameGraphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	target.draw(m_sprites, &m_lvlSpritesheet[m_level]);
    if (m_hitbox_enabled) target.draw(&m_hitboxes[0], m_hitboxes.size(), sf::Lines);
}
