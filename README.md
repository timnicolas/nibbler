# nibbler [[42](https://www.42.fr/) project]
A Snake implementation with multiples graphical interfaces

![example](/assets/GIFs/example.gif)

## Try 2D or 3D experiences
![multiple-interfaces](/assets/imgs/multiple-interfaces.png)

## Play against real player or AI
![multiplayer](/assets/imgs/multiplayer.png)

## Change the board size and move around (3D)
![size-3Dmoving](/assets/imgs/size-3Dmoving.png)

## Installation

Nibbler is only available on OSX.
To install it, run make install command
```bash
git clone https://github.com/tnicolas42/nibbler
cd nibbler
make install
```

Then compile the project

```make -j8```

```make -j8 DEBUG=1  # to compile in debug mode```

```make help  # get info about make commands```

Run the project:

```./nibbler```

## The project

### How to play
You have a snake that move on a board. You can change his [direction](#player-1).
If you hit a border or a snake, you loose.

#### Goal in singleplayer
In singleplayer, the goal is to have the longest snake. To do that, you can eat some food (red blocs).
The best score is automatically saved.

#### Goal in multiplayer
In multiplayer, the goal is to survive. You can dodge the other snakes and block them.

In addition to the food, you have some bonus (yellow blocs). When you eat a bonus, your queue change to yellow (more bonus you have, more your queue is yellow). When you [use your bonus](#player-1), you put walls behind you (as much as the number of bonuses amassed). The walls disappear after some time (change the time in [settings](#Settings))

#### General commands
- `r` -> restart game
- `space` -> play/pause
- `123` -> change GUI
  - `1` -> 2D: SDL2
  - `2` -> 2D: SFML
  - `3` -> 3D: SDL2 & OpenGL 4

#### Player 1
- `arrow` -> change the snake direction
- `right shift` -> put wall behind you (if you have some bonus)

#### Player 2
available only in multiplayer mode (see [settings](#Settings))
- `wasd` -> change the snake direction
- `left shift` -> put wall behind you (if you have some bonus)

#### 3D commands
available only in singleplayer mode
- `wasd` -> move camera
- `eq` -> move up / down
- `left shift` -> increase moving speed
- `mouse moving` -> change view direction

### Settings
Feel free to change game settings in [assets/settings.json](assets/settings.json).

To see the total list of bonus with description:

```./nibbler --settings```

In the file, you can configure:
- Board size
- Starting snake size & speed
- Nubmer of players
- Number and strength of AI
- Number of foods & bonus on the board
- Enable exit border mode
- Starting gui & sound libs
- Sound & music level
- Sound & music sources
- FPS
- Window size
- ...

You have more than 25 parameters to discover !
