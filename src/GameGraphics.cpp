#include "Util.hpp"
#include "GameConfig.hpp"
#include "GameResourceManager.hpp"
#include "GameGraphics.hpp"
#include "GameMechanics.hpp"

bool GameGraphics::init()
{
    std::ifstream fin;
    
    //Initializing texture and map lists
    unsigned numSheets = 2;
    unsigned numLevels = unsigned(config.getInt("num_levels"));
    m_lvlBackgroundTex = std::vector<const sf::Texture*>(numLevels);
    m_spritesheet = std::vector<const sf::Texture*>(numSheets);
    m_frameMap = std::map<std::string, FrameData>();
    
    //Loading level background textures
    for (unsigned i = 0; i < numLevels; i++) {
        m_lvlBackgroundTex[i] = &resource.getTexture(util::appendNumber("bg", i));
    }
    for (unsigned i = 0; i < numSheets; i++) {
        m_spritesheet[i] = &resource.getTexture(util::appendNumber("spritesheet", i));
    }

    //Reading sprite frame data
    //Data format: frame name, sheet #, 4 texture coordinates, 2 position offsets (for minute adjustments to texture position)
    fin.open("config/spritedata.txt");
    if (!fin) return 0;
    for (unsigned i = 0; i < 1000; i++) {
        std::string frame;
        if (!(fin >> frame)) break;
        FrameData d;
        if (!(fin >> d.sheetNum)) return false;
        if (!util::readv2f(fin, d.texCoord, false)) return false;
        if (!util::readv2f(fin, d.size, false)) return false;
        if (!util::readv2f(fin, d.posOffset, false)) return false;
        m_frameMap[frame] = d;
        fin.ignore(1000, '\n');
    }
    fin.close();

    //Initializing some sprites and settings
    m_background.setTexture(*m_lvlBackgroundTex[0]);
    m_background.setTextureRect(sf::IntRect(0, 0, 1600, 1200));
    m_backgroundNext.setTexture(*m_lvlBackgroundTex[0]);
    m_backgroundNext.setTextureRect(sf::IntRect(0, 0, 1600, 1200));
    m_hitbox_enabled = config.getInt("hitbox_enabled");

    return true;
}

void GameGraphics::setNextLevelBGOpacity(unsigned char alpha)
{
    if (alpha > 0) {
        int lvl = config.getInt("level");
        unsigned nextIdx = (unsigned)(lvl < config.getInt("num_levels") ? lvl : lvl-1);
        m_backgroundNext.setColor(sf::Color(255, 255, 255, alpha));
        m_backgroundNext.setTexture(*m_lvlBackgroundTex[nextIdx]);
    }
}

void GameGraphics::addSprite(const Actor &actor)
{
    const Actor::Image& img = actor.getImage();
    const FrameData& d = m_frameMap[img.frame];

    //Some sprites have to be specially drawn. This makes for better flexibility.
    if (util::isPrefix("gwiper", img.frame)) {
        sf::Vector2f c = actor.getPos();
        float rOut = actor.getSize().y;
        float rIn = actor.getSize().x;
        if (rIn < 0) rIn = 0;
        int numPt = (int)(sqrt(rOut) * 20);
        sf::Transform tr;
        tr.rotate(360.f / numPt, c);
        sf::Vertex curOut(sf::Vector2f(c.x + rOut, c.y)), nextOut;
        sf::Vertex curIn(sf::Vector2f(c.x + rIn, c.y)), nextIn;
        curOut.color = nextOut.color = img.color;
        curIn.color = nextIn.color = img.color; 
        for (int i = 0; i < numPt; i++) {
            nextOut.position = tr.transformPoint(curOut.position);
            nextIn.position = tr.transformPoint(curIn.position);
            nextIn.color.a = RAND(0, 255);
            if (RAND(0,1)) {
                m_specialSprites.push_back(curOut);
                m_specialSprites.push_back(nextIn);
                m_specialSprites.push_back(nextOut);
                m_specialSprites.push_back(curOut);
            }
            curOut = nextOut;
            curIn = nextIn;
        }
    }
    else if (util::isPrefix("wiper", img.frame)) {
        sf::Vector2f c = actor.getPos();
        float rOut = actor.getSize().y;
        float rIn = actor.getSize().x;
        if (rIn < 0) rIn = 0;
        int numPt = (int)(sqrt(rOut) * 3);
        sf::Transform tr;
        tr.rotate(360.f / numPt, c);
        sf::Vertex curOut(sf::Vector2f(c.x + rOut, c.y)), nextOut;
        sf::Vertex curIn(sf::Vector2f(c.x + rIn, c.y)), nextIn;
        curOut.color = nextOut.color = img.color; 
        curIn.color = nextIn.color = img.color;
        curIn.color.a = nextIn.color.a = 0;
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
    else {
        sf::Vertex sprite[4];
        sf::Vector2f size = (img.resized ? img.size : d.size);
        util::affixPos(sprite, actor.getPos() + d.posOffset, size, 0);
        util::affixTexture(sprite, d.texCoord, d.size);
        if (img.rotated) util::rotateSprite(sprite, util::toDeg(actor.getDir()), actor.getPos());
        util::applyColor(sprite, img.color);
        m_sprites[d.sheetNum].insert(m_sprites[d.sheetNum].end(), sprite, sprite+4);
    }
}

void GameGraphics::addHealthBar(const Fighter &fighter)
{
    sf::Vertex bar[8];
    unsigned sheetNum = m_frameMap[fighter.getImage().frame].sheetNum;
    const FrameData &caseD = m_frameMap["reg_hpcase"];
    const FrameData &barD = m_frameMap["reg_hpbar"];
    float hpPerc = (float)fighter.getHP() / fighter.getMaxHP();
    sf::Vector2f barSize(40.f, 9.f);
    sf::Vector2f halfBarSize(20.f, 4.5f);
    sf::Vector2f reducedBarSize(40.f * hpPerc, 9.f);

    util::affixPos(bar, fighter.getPos() + caseD.posOffset, barSize, 0);
    util::affixPos(bar+4, fighter.getPos() + barD.posOffset - halfBarSize, reducedBarSize, 1);
    util::affixTexture(bar, caseD.texCoord, caseD.size);
    util::affixTexture(bar+4, barD.texCoord, barD.size);

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

void GameGraphics::updateMisc(const GameState &state)
{
    //Updating settings
    m_hitbox_enabled = config.getInt("hitbox_enabled");
    unsigned idx = (unsigned)config.getInt("level") - 1;
    m_background.setTexture(*m_lvlBackgroundTex[idx]);
    m_backgroundNext.setTexture(*m_lvlBackgroundTex[idx]);
}

void GameGraphics::updateSprites(const GameState &state)
{
    //Recalculating sprite appearance
    m_sprites = std::vector<std::vector<sf::Vertex> >(m_spritesheet.size());
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
    target.draw(m_background);
    if (m_backgroundNext.getTexture() != m_background.getTexture()) {
        target.draw(m_backgroundNext);
    }
    for (unsigned i = 0; i < m_spritesheet.size(); i++) {
        if (!m_sprites[i].empty()) {
            sf::RenderStates spriteState(m_spritesheet[i]);
            target.draw(&m_sprites[i][0], m_sprites[i].size(), sf::Quads, spriteState);
        }
    }
    if (!m_specialSprites.empty()) {
        target.draw(&m_specialSprites[0], m_specialSprites.size(), sf::Quads);
    }
    if (m_hitbox_enabled) target.draw(&m_hitboxes[0], m_hitboxes.size(), sf::Lines);
}
