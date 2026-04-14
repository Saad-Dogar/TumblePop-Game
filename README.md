# Tumble-Pop Replica

A faithful recreation of the classic arcade game **Tumble-Pop** built in C++ using SFML. Capture enemies with a vacuum and fire them back as projectiles to eliminate others.

---

## Gameplay

Select a character, then navigate the level to vacuum up ghosts and skeletons. Captured enemies are stored in your bag and can be fired as rolling projectiles that chain-hit other enemies on contact. Avoid enemies while your vacuum is off — one touch costs a life.

---

## Features

### Character Selection
- Choose between two characters at startup, each with different movement speeds
- A brief selection animation plays before the game begins

### Player Movement
- Left/right movement with sprite flipping
- Jumping with ceiling collision
- Crouching (Down) with a reduced hitbox
- Fall-through on pass-through platforms (Down + Up)
- Walking animation with frame cycling

### Vacuum Mechanic
- Hold `Space` to activate the vacuum — pulls nearby enemies toward the player
- Enemies enter a "being captured" animation as they slide in
- Captured enemies are stacked in the bag (capacity increases on higher levels)
- Powerups change the vacuum's visual colour to indicate the active effect

### Shooting
- `I` — fire one enemy from the bag
- `O` — fire the entire bag at once
- `W` / `S` — aim upward or downward before firing
- Default direction is horizontal (left or right based on player facing)
- Fired enemies roll along floors, bounce off walls, and can chain into other enemies

### Enemy Types

**Ghost**
- Patrols platforms, reverses at walls and floor edges
- Randomly pauses with an idle animation before resuming
- Plays a capture animation when being sucked in

**Skeleton**
- Patrols and randomly jumps; fully gravity-affected
- Ceiling and floor collision handled independently
- Plays its own capture animation

### Collision & Chain Reactions
- Rolling projectiles collide with live enemies and send them rolling too
- Ghost projectiles can trigger skeleton rolls and vice versa
- Projectiles that exit the level boundaries are removed cleanly

### Powerup System
Powerups spawn when a projectile hits a wall or ceiling:

| Powerup | Effect |
|---|---|
| Speed | Doubles movement speed |
| Range | Increases vertical tolerance for vacuum suction |
| Power | Extends vacuum suction range |
| Extra Life | Grants one additional life |

### Lives & Death
- 3 lives per run
- On collision with a live enemy, a 4-frame death animation plays
- Player respawns at the starting position after the animation
- Game closes on 0 lives remaining

### Levels
- **Level 1** — fixed platform layout with a central block structure and scattered platforms
- **Level 2** — randomised platform layout (one of two diagonal patterns chosen at runtime), different block tileset

---

## Controls

| Key | Action |
|---|---|
| `Left / Right` | Move |
| `Up` | Jump |
| `Down` | Crouch |
| `Down + Up` | Fall through platform |
| `Space` | Activate vacuum |
| `I` | Fire one enemy |
| `O` | Fire all enemies |
| `W / S` | Aim up / down when firing |
| `Escape` | Quit |

**Character Select Screen**
| Key | Action |
|---|---|
| `Left Arrow` | Select Character 1 |
| `Right Arrow` | Select Character 2 |

---

## Project Structure

```
main.cpp                    →  Main game loop and all gameplay logic
Assets/
├── Player/player.png       →  Player, bag, and vacuum spritesheet
├── Enemies/ghost.png       →  Ghost spritesheet
├── Enemies/skeleton.png    →  Skeleton spritesheet
└── Data/
    ├── background.png
    ├── blocks.png
    ├── blocks2.png
    ├── character_selection.jpg
    ├── mus.ogg             →  In-game music
    └── select.ogg          →  Character selection music
```

---

## Building

**Requirements:** C++17, [SFML 2.x](https://www.sfml-dev.org/)

```bash
# Linux / macOS
g++ main.cpp -o TumblePop -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

# Windows (MinGW)
g++ main.cpp -o TumblePop.exe -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system
```

Make sure the `Assets/` folder is in the same directory as the executable.

---

## Tech Stack

- **Language:** C++
- **Library:** SFML 2.x (Graphics, Audio, Window)
- **Grid system:** Char-based 2D array (`#` = solid wall, `.` = platform, ` ` = open space)
- **Sprites:** Frame-based animation via `setTextureRect`
