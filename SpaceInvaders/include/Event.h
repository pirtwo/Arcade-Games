#ifndef EVENT_H_INCLUDE
#define EVENT_H_INCLUDE

#include <SFML/Graphics.hpp>
#include <string>
#include <variant>
#include "Entity.h"

class Event
{
public:
    enum class Type
    {
        TAKES_DAMAGE,
        DESTROYED,
        COUNT
    };

    struct DealsDamage
    {
        Entity *obj;
        int amount;
    };

    struct TakesDamage
    {
        Entity *obj;
        int amount;
    };

    struct Destroyed
    {
        Entity *obj;
    };

    Type type;
    std::variant<DealsDamage, TakesDamage, Destroyed> value;
};

#endif