#include "Util.hpp"
#include <cstring>

namespace util
{

    int rand(int min, int max)
    {
        int range = max - min + 1;
        return std::rand() % range + min;
    }

    float toDir(float vx, float vy)
    {
        return atan(vy / vx) + (vx < 0 ? PI : 0);
    }

    float toRad(float deg)
    {
        return deg / 180.f * util::PI;
    }

    float toDeg(float rad)
    {
        return rad * 180.f / util::PI;
    }

    float rotateRad(float rad, float amt)
    {
        rad += amt;
        while (rad < 0) rad += 2*PI;
        while (rad > 2*PI) rad -= 2*PI;
        return rad;
    }

    float rotateDeg(float deg, float amt)
    {
        deg += amt;
        while (deg < 0) deg += 360;
        while (deg > 360) deg -= 360;
        return deg;
    }

    float dot(sf::Vector2f v1, sf::Vector2f v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    float crossZ(sf::Vector2f v1, sf::Vector2f v2)
    {
        return v2.y * v1.x - v2.x * v1.y;
    }

    float getLen(sf::Vector2f v)
    {
        return sqrt(dot(v,v));
    }

    float getMaxRad(sf::Vector2f s)
    {
        return getLen(sf::Vector2f(s.x/2, s.y/2));
    }

    float getMinRad(sf::Vector2f s)
    {
        return MIN(s.x / 2, s.y / 2);
    }

    float getDist(sf::Vector2f p1, sf::Vector2f p2)
    {
        return getLen(sf::Vector2f(p2.x-p1.x, p2.y-p1.y));
    }

    float getDist(sf::Vector2f p, sf::Vector2f p1, sf::Vector2f p2)
    {
        sf::Vector2f v(p2.x-p1.x, p2.y-p1.y);
        sf::Vector2f w(p.x-p1.x, p.y-p1.y);

        float dist, len;
        if ((dist = dot(w,v)) <= 0) return getDist(p, p1);
        if ((len = dot(v,v)) <= dist) return getDist(p, p2);
        float b = dist / len;
        sf::Vector2f pb(p1.x + b * v.x, p1.y + b * v.y);
        return getDist(p, pb);
    }

    sf::Vector2f getUnitVector(sf::Vector2f p1, sf::Vector2f p2)
    {
        sf::Vector2f v = p2 - p1;
        float len = getLen(v);
        return sf::Vector2f(v.x / len, v.y / len);
    }

    bool hasCollided(sf::Vector2f c1, util::ShapeVector s1, float dir1, sf::Vector2f c2, util::ShapeVector s2, float dir2)
    {
        if (s1.s == Circle && s2.s == Circle) return hasCollided(c1, s1.x, c2, s2.x);
        if (s1.s == Rectangle && s2.s == Circle) return hasCollided(c1, sf::Vector2f(s1.x, s1.y), dir1, c2, s2.x);
        if (s1.s == Circle && s2.s == Rectangle) return hasCollided(c2, sf::Vector2f(s2.x, s2.y), dir2, c1, s1.x);
        if (s1.s == Rectangle && s2.s == Rectangle) return hasCollided(c1, sf::Vector2f(s1.x, s1.y), dir1, c2, sf::Vector2f(s2.x, s2.y), dir2);
        if (s1.s == Stroke && s2.s == Circle) return hasCollided(c1, s1.x, s1.y, c2, s2.x);
        if (s1.s == Stroke && s2.s == Rectangle) return hasCollided(c1, s1.x, s1.y, c2, sf::Vector2f(s2.x, s2.y), dir2);
        if (s1.s == Circle && s2.s == Stroke) return hasCollided(c2, s2.x, s2.y, c1, s1.x);
        if (s1.s == Rectangle && s2.s == Stroke) return hasCollided(c2, s2.x, s2.y, c1, sf::Vector2f(s1.x, s1.y), dir1);
        return false;
    }

    //Circle vs circle
    bool hasCollided(sf::Vector2f c1, float r1, sf::Vector2f c2, float r2)
    {
        return (r1 + r2 > getDist(c1, c2));
    }

    //Stroke vs circle
    bool hasCollided(sf::Vector2f c1, float r11, float r12, sf::Vector2f c2, float r2)
    {
        float dist = getDist(c1, c2);
        if (r12 + r2 < dist) return false;
        if (r11 < 0) return true;
        return (dist + r2 > r11);
    }

    //Stroke vs rectangle
    bool hasCollided(sf::Vector2f c1, float r11, float r12, sf::Vector2f c2, sf::Vector2f s2, float dir2)
    {
        if (!hasCollided(c2, s2, dir2, c1, r12)) return false;
        if (r11 < 0) return false;

        //Transforming vectors
        float unitX2 = cos(dir2);
        float unitY2 = sin(dir2);
        sf::Vector2f rotWidth2(unitX2 * s2.x / 2.f, unitY2 * s2.x / 2.f);
        sf::Vector2f rotHeight2(-unitY2 * s2.y / 2.f, unitX2 * s2.y / 2.f);
        sf::Vector2f tr2[] = {c2 - rotWidth2 - rotHeight2,
            c2 + rotWidth2 - rotHeight2,
            c2 + rotWidth2 + rotHeight2,
            c2 - rotWidth2 + rotHeight2};
        
        //Inside edge cross check
        if (getDist(c1, tr2[0]) > r11) return true;
        if (getDist(c1, tr2[1]) > r11) return true;
        if (getDist(c1, tr2[2]) > r11) return true;
        if (getDist(c1, tr2[3]) > r11) return true;

        return false;
    }

    //Rectangle vs circle
    bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, float r2)
    {
        //Basic distance check
        float dist = getDist(c1, c2);
        if (getMaxRad(s1) + r2 < dist) return false;
        if (getMinRad(s1) + r2 > dist) return true;

        //Transforming vectors
        float unitX1 = cos(dir1);
        float unitY1 = sin(dir1);
        sf::Vector2f rotWidth1(unitX1 * s1.x / 2.f, unitY1 * s1.x / 2.f);
        sf::Vector2f rotHeight1(-unitY1 * s1.y / 2.f, unitX1 * s1.y / 2.f);
        sf::Vector2f tr1[] = {c1 - rotWidth1 - rotHeight1,
            c1 + rotWidth1 - rotHeight1,
            c1 + rotWidth1 + rotHeight1,
            c1 - rotWidth1 + rotHeight1};

        //Inner check
        float si1 = crossZ(tr1[1] - tr1[0], c2 - tr1[0]);
        float si2 = crossZ(tr1[2] - tr1[1], c2 - tr1[1]);
        float si3 = crossZ(tr1[3] - tr1[2], c2 - tr1[2]);
        float si4 = crossZ(tr1[0] - tr1[3], c2 - tr1[3]);
        if (si1 < 0 && si2 < 0 && si3 < 0 && si4 < 0) return true;
        if (si1 > 0 && si2 > 0 && si3 > 0 && si4 > 0) return true;

        //Edge cross check
        if (getDist(c2, tr1[0], tr1[1]) < r2) return true;
        if (getDist(c2, tr1[1], tr1[2]) < r2) return true;
        if (getDist(c2, tr1[2], tr1[3]) < r2) return true;
        if (getDist(c2, tr1[3], tr1[0]) < r2) return true;

        return false;
    }

    //Rectangle vs rectangle
    bool hasCollided(sf::Vector2f c1, sf::Vector2f s1, float dir1, sf::Vector2f c2, sf::Vector2f s2, float dir2)
    {
        //Basic distance check
        float dist = getDist(c1, c2);
        if (getMaxRad(s1) + getMaxRad(s2) < dist) return false;
        if (getMinRad(s1) + getMinRad(s2) > dist) return true;

        //Transforming vectors
        float unitX1 = cos(dir1);
        float unitY1 = sin(dir1);
        sf::Vector2f rotWidth1(unitX1 * s1.x / 2.f, unitY1 * s1.x / 2.f);
        sf::Vector2f rotHeight1(-unitY1 * s1.y / 2.f, unitX1 * s1.y / 2.f);
        sf::Vector2f tr1[] = {c1 - rotWidth1 - rotHeight1,
            c1 + rotWidth1 - rotHeight1,
            c1 + rotWidth1 + rotHeight1,
            c1 - rotWidth1 + rotHeight1};
        float unitX2 = cos(dir2);
        float unitY2 = sin(dir2);
        sf::Vector2f rotWidth2(unitX2 * s2.x / 2.f, unitY2 * s2.x / 2.f);
        sf::Vector2f rotHeight2(-unitY2 * s2.y / 2.f, unitX2 * s2.y / 2.f);
        sf::Vector2f tr2[] = {c2 - rotWidth2 - rotHeight2,
            c2 + rotWidth2 - rotHeight2,
            c2 + rotWidth2 + rotHeight2,
            c2 - rotWidth2 + rotHeight2};

        //Axis separation theorem
        for (int i = 0; i < 4; i++) {
            sf::Vector2f separator = tr1[(i+1)%4] - tr1[i];
            float side = crossZ(tr1[(i+2)%4] - tr1[i], separator);
            int sign = (side > 0? -1 : 1);
            if (crossZ(tr2[0]-tr1[i], separator) * sign > 0 &&
                crossZ(tr2[1]-tr1[i], separator) * sign > 0 &&
                crossZ(tr2[2]-tr1[i], separator) * sign > 0 &&
                crossZ(tr2[3]-tr1[i], separator) * sign > 0)
                return false;
        }
        for (int i = 0; i < 4; i++) {
            sf::Vector2f separator = tr2[(i+1)%4] - tr2[i];
            float side = crossZ(tr2[(i+2)%4] - tr2[i], separator);
            int sign = (side > 0? -1 : 1);
            if (crossZ(tr1[0]-tr2[i], separator) * sign > 0 &&
                crossZ(tr1[1]-tr2[i], separator) * sign > 0 &&
                crossZ(tr1[2]-tr2[i], separator) * sign > 0 &&
                crossZ(tr1[3]-tr2[i], separator) * sign > 0)
                return false;
        }

        return true;
    }

    void copySprite(const sf::Vertex src[4], sf::Vertex dest[4])
    {
        dest[0] = src[0];
        dest[1] = src[1];
        dest[2] = src[2];
        dest[3] = src[3];
    }

    void copyTexture(const sf::Vertex src[4], sf::Vertex dest[4])
    {
        dest[0].texCoords = src[0].texCoords;
        dest[1].texCoords = src[1].texCoords;
        dest[2].texCoords = src[2].texCoords;
        dest[3].texCoords = src[3].texCoords;
    }

    void setAlpha(sf::Vertex sprite[4], unsigned char alpha)
    {
        sprite[0].color.a = alpha;
        sprite[1].color.a = alpha;
        sprite[2].color.a = alpha;
        sprite[3].color.a = alpha;
    }

    void affixTexture(sf::Vertex sprite[4], sf::Vector2f texCoord, sf::Vector2f size)
    {
        sprite[0].texCoords = sf::Vector2f(texCoord.x, texCoord.y);
        sprite[1].texCoords = sf::Vector2f(texCoord.x + size.x, texCoord.y);
        sprite[2].texCoords = sf::Vector2f(texCoord.x + size.x, texCoord.y + size.y);
        sprite[3].texCoords = sf::Vector2f(texCoord.x, texCoord.y + size.y);
    }

    sf::Color toColor(unsigned int rgba)
    {
        unsigned char r = (unsigned char)(rgba >> 24);
        unsigned char g = (unsigned char)((rgba & 0xffffff) >> 16);
        unsigned char b = (unsigned char)((rgba & 0xffff) >> 8);
        unsigned char a = (unsigned char)(rgba & 0xff);
        return sf::Color(r, g, b, a);
    }

    sf::Vector2f translatePos(sf::Vector2f pos, float dir, float offsetX, float offsetY)
    {
        float unitX, unitY;
        unitX = cos(dir);
        unitY = sin(dir);
        return translatePos(pos, unitX, unitY, offsetX, offsetY);
    }

    sf::Vector2f translatePos(sf::Vector2f pos, float unitX, float unitY, float offsetX, float offsetY)
    {
        float rotX, rotY, outX, outY;
        rotX = unitY * -1 * offsetY;
        rotY = unitX * offsetY;
        outX = unitX * offsetX;
        outY = unitY * offsetX;
        return sf::Vector2f(pos.x + outX + rotX, pos.y + outY + rotY);
    }

    void affixPos(sf::Vertex sprite[4], sf::Vector2f pos, sf::Vector2f size, int reference)
    {
        sf::Vector2f hSize(size.x / 2.f, size.y / 2.f);
        sf::Vector2f realPos = pos;
        if (reference == 1) {
            realPos.x += hSize.x;
            realPos.y += hSize.y;
        }
        else if (reference == 2) {
            realPos.x -= hSize.x;
            realPos.y += hSize.y;
        }
        else if (reference == 3) {
            realPos.x -= hSize.x;
            realPos.y -= hSize.y;
        }
        else if (reference == 4) {
            realPos.x += hSize.x;
            realPos.y -= hSize.y;
        }
        sprite[0].position = sf::Vector2f(realPos.x - hSize.x, realPos.y - hSize.y);
        sprite[1].position = sf::Vector2f(realPos.x + hSize.x, realPos.y - hSize.y);
        sprite[2].position = sf::Vector2f(realPos.x + hSize.x, realPos.y + hSize.y);
        sprite[3].position = sf::Vector2f(realPos.x - hSize.x, realPos.y + hSize.y);
    }

    void applyColor(sf::Vertex sprite[4], unsigned int rgba)
    {
        sf::Color c = toColor(rgba);
        sprite[0].color = c;
        sprite[1].color = c;
        sprite[2].color = c;
        sprite[3].color = c;
    }

    void rotateSprite(sf::Vertex sprite[4], float dir, sf::Vector2f center)
    {
        sf::Transform tr;
        tr.rotate(dir, center);
        sprite[0].position = tr.transformPoint(sprite[0].position);
        sprite[1].position = tr.transformPoint(sprite[1].position);
        sprite[2].position = tr.transformPoint(sprite[2].position);
        sprite[3].position = tr.transformPoint(sprite[3].position);
    }

    void translateSprite(sf::Vertex sprite[4], sf::Vector2f translation)
    {
        sprite[0].position += translation;
        sprite[1].position += translation;
        sprite[2].position += translation;
        sprite[3].position += translation;
    }

    bool readf(std::istream &in, unsigned amt, float arr[], bool endline)
    {
        for (unsigned i = 0; i < amt && in >> arr[i]; i++);
        if (endline) in.ignore(1000, '\n');
        return !in.fail();
    }

    bool readv2f(std::istream &in, sf::Vector2f &dest, bool endline)
    {
        in >> dest.x >> dest.y;
        if (endline) in.ignore(1000, '\n');
        return !in.fail();
    }
    
    bool read3v2f(std::istream &in, sf::Vector2f &dest1, sf::Vector2f &dest2, sf::Vector2f &dest3, bool endline)
    {
        in >> dest1.x >> dest1.y;
        in >> dest2.x >> dest2.y;
        in >> dest3.x >> dest3.y;
        if (endline) in.ignore(1000, '\n');
        return !in.fail();
    }

    std::string trim(const std::string &s)
    {
        std::size_t idx1 = s.find_first_not_of("\t\n ");
        std::size_t idx2 = s.find_last_not_of("\t\n ");
        if (idx1 == std::string::npos) return "";
        std::size_t len = idx2 - idx1 + 1;
        return s.substr(idx1, len);
    }

    std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) 
    {
        std::stringstream ss(s);
        std::string item;
        while (std::getline(ss, item, delim)) {
            elems.push_back(item);
        }
        return elems;
    }

    std::vector<std::string> split(const std::string &s, char delim) 
    {
        std::vector<std::string> elems;
        split(s, delim, elems);
        return elems;
    }

    //Split on whitespace [ \t\r\n]
    std::vector<std::string> split(const std::string &s)
    {
        const std::string match = " \t\r\n";
        std::vector<std::string> elems;
        std::size_t idx = s.find_first_not_of(match);
        while (idx != std::string::npos) {
            std::size_t endIdx = s.find_first_of(match, idx);
            elems.push_back(s.substr(idx, endIdx-idx));
            idx = s.find_first_not_of(match, endIdx);
        }
        return elems;
    }
    
    bool isPrefix(const std::string &p, const std::string &s)
    {
        if (p.size() > s.size()) return false;
        return !strncmp(p.c_str(), s.c_str(), p.size());
    }
}
