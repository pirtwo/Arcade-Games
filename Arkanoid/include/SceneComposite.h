#ifndef SCENECOMPOSITE_H_INCLUDE
#define SCENECOMPOSITE_H_INCLUDE

#include "SceneComponent.h"
#include <list>
#include <memory>

class SceneComposite : public SceneComponent
{
protected:
    bool _pause;
    bool _visible;

public:
    std::list<std::shared_ptr<SceneComponent>> children;

public:
    virtual ~SceneComposite() {}

    virtual void onUpdate(float delta) {}

    virtual void beforeDraw(sf::RenderTarget &target, sf::RenderStates states) const {}

    virtual void afterDraw(sf::RenderTarget &target, sf::RenderStates states) const {}

    void pause() { _pause = true; }

    void unpause() { _pause = false; }

    void show() { _visible = true; }

    void hide() { _visible = false; }

    void update(float delta = 1.f)
    {
        if (_pause)
            return;

        onUpdate(delta);
        for (auto &&i : children)
            i->update(delta);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        if (!_visible)
            return;

        beforeDraw(target, states);
        for (auto &&i : children)
            target.draw(*i, states.transform * getTransform() * i->getTransform());
        afterDraw(target, states);
    }
};

#endif