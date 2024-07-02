#pragma once

#include "character.hpp"

namespace Entities
{
    class MainCharacter : public Character
    {
    public:
        void update() override;
        void render(const float dtime) override;
        void setPosition(int x, int y) override;
        void setAnimation(const std::string &animation) override;
        void setDirection(const Entities::Character::Direction direction) override;
    };
}
