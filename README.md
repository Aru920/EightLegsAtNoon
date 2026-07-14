# Eight Legs at Noon

Eight Legs at Noon is a small Unreal Engine prototype where the player protects a wandering dwarf from waves of spiders. The player controls a fixed top-down shooter setup, aims with the cursor, and clears spiders before they reach the dwarf.

This project was made as a compact gameplay prototype focused on C++ gameplay systems, simple AI behavior, wave spawning, UI hooks, and clean Blueprint integration.

## Gameplay

The player watches the arena from above and shoots incoming spiders before they can attack the dwarf. The dwarf moves around inside a safe zone, reacts when spiders get close, and loses lives when attacked.

The goal is to survive as long as possible while clearing waves.

## Features

- Top-down player camera
- Cursor-based pistol aiming
- Fire montage driven shooting
- Sphere hit detection for shots
- Spider enemies that chase the dwarf
- Spider attack cooldown
- Dwarf lives and damage handling
- Dwarf panic behavior when enemies are nearby
- Safe zone roaming using a Behavior Tree task
- Wave-based spider spawning
- Increasing wave count
- Spider kill tracking
- Game over flow
- Pause menu helpers
- Blueprint events for UI, animation, sound, and effects

## C++ Systems

The project uses C++ for the main gameplay foundation:

- `ELNPlayerCharacter` handles aiming, firing, and shot traces
- `ELNPlayerController` handles input, pause, restart, quit, and menu helpers
- `ELNDwarfCharacter` handles lives, panic state, damage, friendly-fire messages, and movement speed changes
- `ELNSpiderCharacter` handles spider health, death, attack timing, and damage
- `ELNSpiderAIController` handles spider chasing behavior
- `ELNDwarfAIController` runs the dwarf behavior tree
- `ELNBTTask_FindSafeZonePoint` finds random movement points inside the dwarf safe zone
- `ELNSpiderSpawner` spawns spiders around the arena
- `ELNGameMode` manages waves, game over, spider kills, and match flow

## Blueprint Integration

Blueprints are used for the parts that are faster to tune visually:

- Player weapon mesh setup
- Shoot montage
- Spider death montage
- Spider dissolve material setup
- Impact effects
- UI widgets
- Dwarf message widget
- Pause menu
- Game over screen
- Wave start animations
- Sounds and Niagara effects

The C++ code exposes events and settings so the Blueprint side can stay simple and readable.

## AI

The dwarf uses a simple Behavior Tree flow for roaming inside a safe zone. A C++ Behavior Tree task picks a valid point inside the safe zone and sends the dwarf there.

Spiders use a simpler chase setup. They move toward the dwarf, attack when close, then wait for a cooldown before attacking again.

## UI

The UI shows the main gameplay state:

- Current wave
- Spiders killed
- Dwarf lives
- Pause menu
- Game over screen
- Dwarf speech/messages

Most UI updates are driven through Blueprint events exposed from C++.

## Project Goal

This project was built to show practical Unreal Engine gameplay programming in a small scope. The focus was not on making a large game, but on showing clean systems that work together:

- player input
- shooting
- enemy AI
- friendly NPC behavior
- wave spawning
- UI events
- gameplay state flow

## Engine

Built with Unreal Engine 5 using C++ and Blueprints.
