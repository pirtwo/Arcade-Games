#ifndef SCENEMANAGER_H_INCLUDE
#define SCENEMANAGER_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include <iterator>
#include "SceneComposite.h"

class SceneID
{
    friend class SceneManager;
    typedef std::list<std::shared_ptr<SceneComposite>>::iterator iter;

protected:
    iter _itr;

public:
    SceneID(iter i)
    {
        _itr = i;
    }
};

class SceneManager : public sf::Drawable
{
protected:
    std::list<std::shared_ptr<SceneComposite>> _list;

public:
    SceneID push(std::shared_ptr<SceneComposite> scene)
    {
        _list.push_back(scene);
        return SceneID(_list.end());
    }

    void pop()
    {
        _list.pop_back();
    }

    void clear()
    {
        _list.clear();
    }

    void pauseAll()
    {
        for (auto &&i : _list)
            i->pause();
    }

    void unpauseAll()
    {
        for (auto &&i : _list)
            i->unpause();
    }

    void showAll()
    {
        for (auto &&i : _list)
            i->show();
    }

    void hideAll()
    {
        for (auto &&i : _list)
            i->hide();
    }

    void update(float delta)
    {
        for (auto &&i : _list)
            i->update(delta);
    }

    void draw(sf::RenderTarget &target, sf::RenderStates states) const override
    {
        for (auto &&i : _list)
            target.draw(*i, states);
    }
};

#endif