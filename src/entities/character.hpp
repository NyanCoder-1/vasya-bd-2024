#pragma once

#include <string>

namespace Entities
{
    class Character
    {
    public:
        enum class Direction
        {
            Up,
            Down,
            Left,
            Right
        };

    public:
        virtual ~Character() = default;
        virtual void update() = 0;
        virtual void render(const float dtime) = 0;

        virtual void setPosition(const int x, const int y) = 0;
        virtual void setAnimation(const std::string &animation) = 0;
        virtual void setDirection(const Entities::Character::Direction direction) = 0;
    };
}
