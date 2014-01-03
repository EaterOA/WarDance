#include "Util.hpp"
#include "Config.hpp"
#include "GameGraphics.hpp"
#include "GameMechanics.hpp"

bool GameGraphics::init()
{
	std::ifstream fin;
	
	//Initializing texture and map lists
	m_numSheets = unsigned(config["num_sheets"]);
	unsigned numLevels = unsigned(config["num_levels"]);
	m_lvlBackgroundTex = std::vector<sf::Texture>(numLevels);
	m_spritesheet = std::vector<sf::Texture>(m_numSheets);
	m_frameMap = std::map<std::string, int>();

	//Loading level background textures
	for (unsigned i = 0; i < numLevels; i++) {
		std::stringstream ss;
		ss << "images/bg" << i << ".png";
		if (!m_lvlBackgroundTex[i].loadFromFile(ss.str())) return false;
		m_lvlBackgroundTex[i].setRepeated(true);
	}

	//Loading spritesheets
	for (unsigned i = 0; i < m_numSheets; i++) {
		std::stringstream ss;
		ss << "images/spritesheet" << i << ".png";
		if (!m_spritesheet[i].loadFromFile(ss.str())) return false;
		m_spritesheet[i].setSmooth(true);
	}

	//Reading sprite data
	//Data format: frame name, sheet #, 4 texture coordinates, 2 position offsets
    fin.open("config/spritedata.txt");
	if (!fin) return 0;
	for (unsigned i = 0; i < 1000;) {
		std::string frame;
		if (!(fin >> frame)) break;
		else {
			m_frameMap[frame] = (int)i;
			if (!(fin >> m_sheetNum[i])) return false;
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

	return true;
}

void GameGraphics::affixPos(sf::Vertex sprite[4], sf::Vector2f center, const std::string& frame, float offsetLX, float offsetRX, float offsetLY, float offsetRY)
{
	float* coord = m_sprCoords + m_frameMap[frame] * 2;

	center += sf::Vector2f(coord[0], coord[1]);
	sprite[0].position = center + sf::Vector2f(offsetLX, offsetLY);
	sprite[1].position = center + sf::Vector2f(offsetRX, offsetLY);
	sprite[2].position = center + sf::Vector2f(offsetRX, offsetRY);
	sprite[3].position = center + sf::Vector2f(offsetLX, offsetRY);
}

void GameGraphics::affixPos(sf::Vertex sprite[4], sf::Vector2f center, const std::string& frame)
{
	float* coord = m_texCoords + m_frameMap[frame] * 4;

	float hwidth = coord[2] / 2.f;
	float hheight = coord[3] / 2.f;
	affixPos(sprite, center, frame, -hwidth, hwidth, -hheight, hheight);
}

void GameGraphics::affixTexture(sf::Vertex sprite[4], const std::string& frame)
{
	float* coord = m_texCoords + m_frameMap[frame] * 4;

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

void GameGraphics::addSprite(const Actor &actor)
{
	unsigned sheetNum = m_sheetNum[m_frameMap[actor.getFrame()]];

	sf::Vertex sprite[4];
	affixPos(sprite, actor.getPos(), actor.getFrame());
	affixTexture(sprite, actor.getFrame());
	rotateSprite(sprite, util::toDeg(actor.getDir()), actor.getPos());
	m_sprites[sheetNum].insert(m_sprites[sheetNum].end(), sprite, sprite+4);
}

void GameGraphics::addHealthBar(const Fighter &fighter)
{
	unsigned sheetNum = m_sheetNum[m_frameMap[fighter.getFrame()]];

	sf::Vertex bar[8];
	float hpOffset = 40.f * fighter.getHP() / fighter.getMaxHP();
	affixPos(bar, fighter.getPos(), "reg_hpcase", -20.f, 20.f, -4.5f, 4.5f);
	affixPos(bar+4, fighter.getPos(), "reg_hpbar", -20.f, -20.f + hpOffset, -4.5f, 4.5f);
	affixTexture(bar, "reg_hpcase");
	affixTexture(bar+4, "reg_hpbar");
	m_sprites[sheetNum].insert(m_sprites[sheetNum].end(), bar, bar+8);
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
	m_background.setTexture(m_lvlBackgroundTex[(unsigned)config["level"] - 1]);

	//Recalculating sprite appearance
	m_sprites = std::vector<std::vector<sf::Vertex> >(m_numSheets);
	if (m_hitbox_enabled)
		m_hitboxes = std::vector<sf::Vertex>();

	//Adding sprites. Order matters except hitboxes
	if (m_hitbox_enabled) {
		addHitbox(*state.player);
		for (unsigned i = 0; i < state.enemies.size(); i++)
			if (m_hitbox_enabled) addHitbox(*state.enemies[i]);
	}
    for (unsigned i = 0; i < state.items.size(); i++)
        addSprite(*state.items[i]);
	addSprite(*state.player);
    for (unsigned i = 0; i < state.enemies.size(); i++)
		addSprite(*state.enemies[i]);
    for (unsigned i = 0; i < state.projectiles.size(); i++)
		addSprite(*state.projectiles[i]);
    for (unsigned i = 0; i < state.enemies.size(); i++) {
		addHealthBar(*state.enemies[i]);
	}

}

void GameGraphics::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_background);
	for (unsigned i = 0; i < m_numSheets; i++) {
		if (!m_sprites[i].empty()) {
			sf::RenderStates spriteState(&m_spritesheet[i]);
			target.draw(&m_sprites[i][0], m_sprites[i].size(), sf::Quads, spriteState);
		}
	}
    if (m_hitbox_enabled) target.draw(&m_hitboxes[0], m_hitboxes.size(), sf::Lines);
}
