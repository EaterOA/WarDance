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
	m_frameMap = std::map<std::string, FrameData>();

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
	for (unsigned i = 0; i < 1000; i++) {
		std::string frame;
		if (!(fin >> frame)) break;
		FrameData d;
		if (!(fin >> d.sheetNum)) return false;
		if (!util::readf(fin, 4, d.texCoord, false)) return false;
		if (!util::readf(fin, 2, d.posOffset, false)) return false;
		if (!(fin >> d.rotatable)) return false;
		if (!(fin >> std::hex >> d.rgba)) return false;
		m_frameMap[frame] = d;
		fin.ignore(1000, '\n');
	}
    fin.close();

	//Initializing some sprites and settings
	m_background.setTexture(m_lvlBackgroundTex[0]);
	m_background.setTextureRect(sf::IntRect(0, 0, 1600, 1200));
	m_backgroundNext.setTextureRect(sf::IntRect(0, 0, 1600, 1200));
	m_hitbox_enabled = config["hitbox_enabled"];

	return true;
}

void GameGraphics::affixPos(sf::Vertex sprite[4], sf::Vector2f center, const FrameData &d, float offsetLX, float offsetRX, float offsetLY, float offsetRY)
{
	const float* coord = d.posOffset;

	center += sf::Vector2f(coord[0], coord[1]);
	sprite[0].position = center + sf::Vector2f(offsetLX, offsetLY);
	sprite[1].position = center + sf::Vector2f(offsetRX, offsetLY);
	sprite[2].position = center + sf::Vector2f(offsetRX, offsetRY);
	sprite[3].position = center + sf::Vector2f(offsetLX, offsetRY);
}

void GameGraphics::affixPos(sf::Vertex sprite[4], sf::Vector2f center, const FrameData &d)
{
	const float* coord = d.texCoord;

	float hwidth = coord[2] / 2.f;
	float hheight = coord[3] / 2.f;
	affixPos(sprite, center, d, -hwidth, hwidth, -hheight, hheight);
}

void GameGraphics::affixTexture(sf::Vertex sprite[4], const FrameData &d)
{
	const float* coord = d.texCoord;

	float x = coord[0];
	float y = coord[1];
	float w = coord[2];
	float h = coord[3];
	sprite[0].texCoords = sf::Vector2f(x, y);
	sprite[1].texCoords = sf::Vector2f(x+w, y);
	sprite[2].texCoords = sf::Vector2f(x+w, y+h);
	sprite[3].texCoords = sf::Vector2f(x, y+h);
}

void GameGraphics::rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f center, const FrameData &d)
{
	if (!d.rotatable) return;
	sf::Transform tr;
	tr.rotate(dir, center);
	for (unsigned i = 0; i < 4; i++)
		sprite[i].position = tr.transformPoint(sprite[i].position);
}

void GameGraphics::applyColor(sf::Vertex sprite[4], const FrameData &d)
{
	unsigned char r = (unsigned char)(d.rgba >> 24);
	unsigned char g = (unsigned char)((d.rgba & 0xffffff) >> 16);
	unsigned char b = (unsigned char)((d.rgba & 0xffff) >> 8);
	unsigned char a = (unsigned char)(d.rgba & 0xff);
	sf::Color c(r, g, b, a);
	sprite[0].color = c;
	sprite[1].color = c;
	sprite[2].color = c;
	sprite[3].color = c;
}

void GameGraphics::addSprite(const Actor &actor)
{
	const FrameData& d = m_frameMap[actor.getFrame()];

	//Some sprites have to be specially drawn. It makes for better flexibility.
	if (actor.getFrame() == "wiper") {
		sf::Vector2f c = actor.getPos();
		float rOut = actor.getSize().x;
		float rIn = (rOut > 20 ? rOut - 20 : 0);
		int numPt = (int)(sqrt(rOut) * 3);
		sf::Transform tr;
		tr.rotate(360.f / numPt, c);
		sf::Vertex curOut(sf::Vector2f(c.x + rOut, c.y)), nextOut;
		sf::Vertex curIn(sf::Vector2f(c.x + rIn, c.y)), nextIn;
		curOut.color = nextOut.color = sf::Color(150, 150, 255, 255);
		curIn.color = nextIn.color = sf::Color(150, 150, 255, 0);
		for (int i = 0; i < numPt; i++) {
			nextOut.position = tr.transformPoint(curOut.position);
			nextIn.position = tr.transformPoint(curIn.position);
			m_specialSprites.push_back(curOut);
			m_specialSprites.push_back(nextOut);
			m_specialSprites.push_back(nextIn);
			m_specialSprites.push_back(curIn);
			curOut = nextOut;
			curIn = nextIn;
		}
	}
	if (util::isPrefix("m_laser", actor.getFrame())) {
		const Laser& laser = *(const Laser*)(&actor);
		FrameData custom_d = d;
		unsigned alpha = (unsigned)(laser.getFadePerc() * 255.f);
		custom_d.rgba &= 0xffffff00;
		custom_d.rgba |= alpha;
		sf::Vertex sprite[4];
		affixPos(sprite, actor.getPos(), d);
		sprite[1].position.x = sprite[0].position.x + actor.getSize().x;
		sprite[2].position.x = sprite[3].position.x + actor.getSize().x;
		affixTexture(sprite, d);
		rotateSprite(sprite, util::toDeg(actor.getDir()), actor.getPos(), d);
		applyColor(sprite, custom_d);
		m_sprites[d.sheetNum].insert(m_sprites[d.sheetNum].end(), sprite, sprite+4);
	}
	else {
		sf::Vertex sprite[4];
		affixPos(sprite, actor.getPos(), d);
		affixTexture(sprite, d);
		rotateSprite(sprite, util::toDeg(actor.getDir()), actor.getPos(), d);
		applyColor(sprite, d);
		m_sprites[d.sheetNum].insert(m_sprites[d.sheetNum].end(), sprite, sprite+4);
	}
}

void GameGraphics::addHealthBar(const Fighter &fighter)
{
	unsigned sheetNum = m_frameMap[fighter.getFrame()].sheetNum;

	sf::Vertex bar[8];
	float hpOffset = 40.f * fighter.getHP() / fighter.getMaxHP();
	affixPos(bar, fighter.getPos(), m_frameMap["reg_hpcase"], -20.f, 20.f, -4.5f, 4.5f);
	affixPos(bar+4, fighter.getPos(), m_frameMap["reg_hpbar"], -20.f, -20.f + hpOffset, -4.5f, 4.5f);
	affixTexture(bar, m_frameMap["reg_hpcase"]);
	affixTexture(bar+4, m_frameMap["reg_hpbar"]);
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

void GameGraphics::updateSprites(const GameState &state, float bgFade)
{
	//Updating settings
	m_hitbox_enabled = config["hitbox_enabled"];
	unsigned idx = (unsigned)config["level"] - 1;
	m_background.setTexture(m_lvlBackgroundTex[idx]);
	if (bgFade < 1.f) {
		m_background.setColor(sf::Color(255, 255, 255, (unsigned char)(255.f * bgFade)));
		m_backgroundNext.setTexture(m_lvlBackgroundTex[idx + (config["level"] < config["num_levels"] ? 1 : 0)]);
	}

	//Recalculating sprite appearance
	m_sprites = std::vector<std::vector<sf::Vertex> >(m_numSheets);
	m_specialSprites = std::vector<sf::Vertex>();
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
	if (m_background.getColor().a < 0xff) {
		target.draw(m_backgroundNext);
	}
	target.draw(m_background);
	for (unsigned i = 0; i < m_numSheets; i++) {
		if (!m_sprites[i].empty()) {
			sf::RenderStates spriteState(&m_spritesheet[i]);
			target.draw(&m_sprites[i][0], m_sprites[i].size(), sf::Quads, spriteState);
		}
	}
	if (!m_specialSprites.empty()) {
		target.draw(&m_specialSprites[0], m_specialSprites.size(), sf::Quads);
	}
    if (m_hitbox_enabled) target.draw(&m_hitboxes[0], m_hitboxes.size(), sf::Lines);
}
