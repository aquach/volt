#pragma once

/* A thing in the game with game logic. */
namespace Volt {

class Entity {
public:
    virtual void Update () = 0;
    virtual void Render () = 0;
};

}
