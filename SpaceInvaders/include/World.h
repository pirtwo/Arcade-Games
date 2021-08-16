#ifndef WORLD_H_INCLUDE
#define WORLD_H_INCLUDE

#include "Entity.h"
#include "Event.h"
#include <memory>

struct World
{
    std::vector<std::shared_ptr<Entity>> items;
    std::vector<std::shared_ptr<Event>> events;
};

#endif