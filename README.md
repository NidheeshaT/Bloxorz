<img width="100" alt=" logo" src="https://github.com/NidheeshaT/Bloxorz/assets/83623339/bac3c975-16f8-4d23-beaa-f314cd598aac"><h1>Bloxorz</h1>

https://github.com/NidheeshaT/Bloxorz/assets/83623339/88713d0c-c031-4757-ba0a-9d5e132af063

The **Bloxorz** game is a 3D puzzle game where players must move a rectangular box across a plane to meet the target tile at a specific point. The game is built using pure OpenGL2.0 GLUT in C++. The project features a space-themed lighting setup with sun lighting and stars in the background.

<a href="https://github.com/NidheeshaT/Bloxorz/releases/tag/v1.0.0" target="_blank"><img align="left" alt="Download game" src="https://dabuttonfactory.com/button.png?t=Download+Bloxorz&f=Open+Sans-Bold&ts=26&tc=f00&hp=45&vp=20&c=11&bgt=unicolored&bgc=eee" height=26px></a>
<br/>

### Local development instructions
1. Clone the repository
```
git clone https://github.com/NidheeshaT/Bloxorz.git
```
2. Run the following commands in the terminal
```
cd Bloxorz
python run.py
```

>**Note**: Download freeglut from [here](https://www.transmissionzero.co.uk/software/freeglut-devel/) and add the path to the environment variables.

### Objects used:
- **Player**: Represents the rectangular box that the player moves across the platform.
- **Light**: Provides the lighting for the game environment. PlatformCube: Represents the cubes that make up the game platform.
- **Camera**: Determines the player's perspective and view of the game.

### Textures used:
- **2k_sum.bmp**: Sun Background 
- **stars.bmp**: Stars Background 
- **bricks.bmp**: Platform cubes 
- **gold.bmp**: Target tile

### User Interaction
Using Alphabetical Keys: 
- **Camera Movement**: WASD Keys 
- **Player Movements**: Arrow Keys 
- **Pause key**: P
- **Mouse key**: for buttons on different screen

### References
1. [OpenGL Tutorials by Swiftless](https://www.swiftless.com/opengltuts.html)
2. [OpenGL Tutorials by lazyfoo](https://lazyfoo.net/tutorials/OpenGL/index.php)