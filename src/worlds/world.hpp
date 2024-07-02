#pragma once

namespace Worlds
{
    class World
    {
    public:
        virtual ~World() = default;

        virtual void render() = 0;
        virtual void update(const float dtime) = 0;
    };
}
