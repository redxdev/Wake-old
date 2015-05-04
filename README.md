# Wake

Wake is a small OpenGL game engine written in C++. It is built around a simple Actor/Component framework.

## Scripting

Wake supports Lua for scripting via LuaJIT. Scripts can be found in the Distribution directory under game, lib, and config.

## Actors

An actor is anything that is in the world. Every physical (and many non-physical) objects are represented as actors. Every actor
has a set of transforms (position, rotation, scale), a list of components, and a subscription to the engine's main loop.

## Components

Components are reusable classes that can be attached to actors. They get a subscription to the engine's main loop through their
parent actor, and they inherit the transforms of their parent actor as well.

# License

See LICENSE
