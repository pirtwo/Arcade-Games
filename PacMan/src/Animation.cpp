#include "Animation.h"
#include "Atlas.h"

Animation::Animation(Atlas &atlas, const std::list<std::string> &frames)
{
    _frameIdx = 0;
    _frameNum = (int)frames.size();
    _frameTimeMS = 300;
    _lastFrameMS = 0;
    _progressMS = 0;

    for (auto &&i : frames)
    {
        auto frame = std::make_shared<sf::Sprite>(atlas.texture);
        frame->setTextureRect(atlas.data[i]);
        frame->setOrigin(frame->getLocalBounds().width / 2, frame->getLocalBounds().height / 2);
        _frames.push_back(frame);
    }
}

Animation::~Animation()
{
    _frames.clear();
}

bool Animation::completed()
{
    return _frameIdx == _frameNum - 1;
}

void Animation::setSpeed(int speedMS)
{
    _frameTimeMS = speedMS;
}

void Animation::update(int timestepMS)
{
    _progressMS += timestepMS;
    if (_progressMS - _lastFrameMS > _frameTimeMS)
    {
        _frameIdx++;
        if (_frameIdx >= _frameNum)
            _frameIdx = 0;

        _lastFrameMS = _progressMS;
    }
}

void Animation::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(*_frames[_frameIdx], states.transform * getTransform());
}