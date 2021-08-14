#ifndef ANIMATION_H_INCLUDE
#define ANIMATION_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
class Atlas;

class Animation : public sf::Drawable, public sf::Transformable
{
protected:
    int _frameIdx;
    int _frameNum;
    int _lastFrameMS;
    int _frameTimeMS;
    int _progressMS;
    std::vector<std::shared_ptr<sf::Sprite>> _frames;

public:
    Animation(Atlas &atlas, const std::list<std::string> &frames);

    ~Animation();

    bool completed();

    void setSpeed(int speedMS);

    void update(int timestepMS);

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
};

#endif