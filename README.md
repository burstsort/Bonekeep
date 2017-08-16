# Bonekeep - custom engine showcase

This simple game was made to demonstrate and test the main functionalities of my 2D game engine.

Sprites and music were borrowed from Castlevania.

The main goal of the game is to collect tasty turkey legs while avoiding the spooky skeletons.
For each collected turkey leg a new skeleton is added to the room, making the game more and more challenging.

Player moves the avatar using the arrow keys.
While the mouse cursor is over the game window additional collision debug information is displayed.
Moving entities (player avatar and skeletons) turn red when the game detects a potential collision with the mouse cursor.
These same entities turn green when the actual collision is detected. This demonstrates two phases of the collision detection system.

## Engine info:
The game is running on a custom game engine developed using C++.

The engine is comprised of several pieces:
  - Entity manager
      - Entity manager allows creation and destruction of game objects (entities)
      - Entities are stored in an array
      - Each entity has an index, an unique id, and a collection of components and tags
      - Components are defined by the game (e.g. position, velocity, appearance)
      - Ownership of components and tags is represented using bitmasks for simple and fast queries (component mask and tag mask)
      - Instead of each entity being a structure that holds its components, it keeps the each component type in its own array
      - This approach uses data-oriented-design principles and improves cache coherency and facilitates iteration
      - Each entity is therefore represented as an index into each of those component arrays
      - Entity manager also handles the deleted entities and places newly made entities in their place, saving on memory
  
  - Resource manager
      - Takes care of loading various resources
      - Makes sure the same resource isn't loaded twice
      - Also automatically unloads resources that are no longer used by any entity
  
  - Renderer
      - 2D sprite renderer that uses OpenGL to display images from sprite sheets
      - Uses hardware instancing to minimize draw calls made using the same sprite sheet
      - Also capable of drawing debug information (e.g. bounding boxes)
  
  - Sound manager
      - Allows reproduction of game music and sound effects using OpenAL
      - Sounds are loaded from files using Ogg Vorbis compression format
      - Longer audio files (e.g. music) are streamed from disk
  
  - Collision manager
      - Uses axis-aligned bounding boxes to detect collision between entities
      - Broad phase using spatial hashing
      
Gameplay and game mechanics are constructed using a series of generic functions called *systems*.
Each game tick (render frame atm) each system iterates over the entities and manipulates the state of their components.
Usually each system is mapped to a certain component mask.
For example MovementSystem would iterate over entities with position and velocity components.
Therefore,  entities with no velocity component wouldn't be affected by the MovementSystem.
Adding a new component to an entity would result in it being affected by more systems.
This approach has proven pretty flexible for game development, and since components are laid out contiguously in memory, entities can be iterated through quickly.
