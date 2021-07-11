#include "Collision.h"

bool rayVsBox(
    sf::Vector2f rayOri,
    sf::Vector2f rayDir,
    sf::Vector2f boxPos,
    sf::Vector2f boxSize,
    sf::Vector2f &contactPoint,
    sf::Vector2f &contactNormal)
{
    sf::Vector2f cp(0.f, 0.f);

    float boxMinX = boxPos.x;
    float boxMinY = boxPos.y;
    float boxMaxX = boxPos.x + boxSize.x;
    float boxMaxY = boxPos.y + boxSize.y;

    float lineMinX = std::min(rayOri.x, rayOri.x + rayDir.x);
    float lineMaxX = std::max(rayOri.x, rayOri.x + rayDir.x);
    float lineMinY = std::min(rayOri.y, rayOri.y + rayDir.y);
    float lineMaxY = std::max(rayOri.y, rayOri.y + rayDir.y);

    float t0x = (boxMinX - rayOri.x) / rayDir.x;
    float t1x = (boxMaxX - rayOri.x) / rayDir.x;
    float t0y = (boxMinY - rayOri.y) / rayDir.y;
    float t1y = (boxMaxY - rayOri.y) / rayDir.y;

    float tMin = std::max(std::min(t0x, t1x), std::min(t0y, t1y));
    float tMax = std::min(std::max(t0x, t1x), std::max(t0y, t1y));

    // calc contact point
    cp =
        tMin >= 0
            ? rayOri + rayDir * tMin
            : rayOri + rayDir * tMax;

    // check ray hit the box and contact
    // point is on the ray
    if (tMax < tMin ||
        cp.x < lineMinX ||
        cp.x > lineMaxX ||
        cp.y < lineMinY ||
        cp.y > lineMaxY)
        return false;

    // set contact point
    contactPoint = cp;

    // set contact normal
    if (contactPoint.x == boxMinX)
    {
        contactNormal.x = tMin >= 0 ? -1 : 1;
        contactNormal.y = 0;
    }
    else if (contactPoint.x == boxMaxX)
    {
        contactNormal.x = tMin >= 0 ? 1 : -1;
        contactNormal.y = 0;
    }
    else if (contactPoint.y == boxMinY)
    {
        contactNormal.x = 0;
        contactNormal.y = tMin >= 0 ? -1 : 1;
    }
    else if (contactPoint.y == boxMaxY)
    {
        contactNormal.x = 0;
        contactNormal.y = tMin >= 0 ? 1 : -1;
    }

    return true;
}
