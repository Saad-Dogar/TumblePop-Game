#include <iostream>
#include <fstream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace sf;
using namespace std;

int screen_x = 1024;
int screen_y = 896;

void display_level(RenderWindow& window, char**lvl, Texture& bgTex, Sprite& bgSprite, Texture& blockTexture, Sprite& blockSprite, Texture& platformTexture, Sprite& platformSprite, const int height, const int width, const int cell_size);
void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth, bool isJumping, bool fallThrough);
void level_structure(int level, char** lvl, int width, int height);
int characterSelection(RenderWindow &window);

int main() {

	RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Default);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(60);

	//level specifics
	const int cell_size = 64;
	const int height = 14;
	const int width = 16;
	char** lvl;

	//Self created variables
	int frameCount = 0;
	int animationCount = 0;
	int level = 1;
	int i = 0;
	int choice = 0;
	choice = characterSelection(window);
	bool isReversed = false;
	bool fallThrough = false;
	bool vaccum = false;


	//level and background textures and sprites
	Texture bgTex;
	Sprite bgSprite;
	Texture blockTexture;
	Sprite blockSprite;
	Texture platformTexture;
	Sprite platformSprite;

	bgTex.loadFromFile("Assets/Data/background.png");
	bgSprite.setTexture(bgTex);
	bgSprite.setPosition(0,0);

	blockTexture.loadFromFile("Assets/Data/blocks.png");
	blockSprite.setTexture(blockTexture);
	blockSprite.setTextureRect(IntRect(195, 75, 64, 64));

	platformTexture.loadFromFile("Assets/Data/blocks.png");
	platformSprite.setTexture(platformTexture);
	platformSprite.setTextureRect(IntRect(69, 66, 64, 64));

	// //Music initialisation
	// Music lvlMusic;

	// lvlMusic.openFromFile("Data/mus.ogg");
	// lvlMusic.setVolume(20);
	// lvlMusic.play();
	// lvlMusic.setLoop(true);

	//player data
	float player_x = 470;
	float player_y = 48;

	float speed = 5;
	float Sprite_Y_choice; // choice ki basis pa sprite jis sheet sa extract ho rahe ha us ki Y change ho ge (for animation)
    switch (choice)
    {
    case 1:
        speed = 6;
        Sprite_Y_choice = 33;
        break;
    case 2:
        speed = 4;
        Sprite_Y_choice = 222;
        break;
    default:
        speed = 4;
        Sprite_Y_choice = 42; // special case you get skin of yellow; speed of green
        break;
    }

	const float jumpStrength = -20; // Initial jump velocity
	const float gravity = 1;  // Gravity acceleration

	bool isJumping = false;  // Track if jumping

	bool up_collide = false;
	bool left_collide = false;
	bool right_collide = false;

	Texture PlayerTexture;
	Sprite PlayerSprite;
	Texture BagTexture;
	Sprite BagSprite;
	Texture VaccumTexture;
	Sprite VaccumSprite;

	bool onGround = false;

	float offset_x = 0;
	float offset_y = 0;
	float velocityY = 0;

	float terminal_Velocity = 20;

	int PlayerHeight = 144;
	int PlayerWidth = 72;

	bool up_button = false;

	char top_left = '\0';
	char top_right = '\0';
	char top_mid = '\0';

	char left_mid = '\0';
	char right_mid = '\0';

	char bottom_left = '\0';
	char bottom_right = '\0';
	char bottom_mid = '\0';

	char bottom_left_down = '\0';
	char bottom_right_down = '\0';
	char bottom_mid_down = '\0';

	char top_right_up = '\0';
	char top_mid_up = '\0';
	char top_left_up = '\0';

	PlayerTexture.loadFromFile("Assets/Player/player.png");
	PlayerSprite.setTexture(PlayerTexture);
	PlayerSprite.setTextureRect(IntRect(22, Sprite_Y_choice, 24, 39));
	PlayerSprite.setScale(3,3);
	PlayerSprite.setPosition(player_x, player_y);

	BagTexture.loadFromFile("Assets/Player/player.png");
	BagSprite.setTexture(BagTexture);
	BagSprite.setTextureRect(IntRect(68, 157, 18, 27));
	BagSprite.setScale(2,2);
	BagSprite.setPosition(player_x+PlayerWidth-12, player_y+(PlayerHeight/2)-5);

	VaccumTexture.loadFromFile("Assets/Player/player.png");
	VaccumSprite.setTexture(VaccumTexture);
	VaccumSprite.setTextureRect(IntRect(201, 174, 14, 29));
	VaccumSprite.setScale(2.5,2.5);
	VaccumSprite.setPosition(player_x - 48, player_y - 25);

	//creating level array
	lvl = new char* [height];
	for (int i = 0; i < height; i += 1) {
		lvl[i] = new char[width];
	}

	level_structure(level, lvl, width, height);

	Event ev;
	//main loop
	while (window.isOpen()) {

		fallThrough = false;
		PlayerHeight = 144;

		// Tracks the number of frames for mapping to animations
		if (frameCount < 60) frameCount ++;
		else frameCount = 0;
		if (frameCount%7 == 0) animationCount ++;
		if (animationCount == 5) animationCount = 0;

		while (window.pollEvent(ev)) {

			if (ev.type == Event::Closed) {
				window.close();
			}

			if (ev.type == Event::KeyPressed) {
				if (Keyboard::isKeyPressed(Keyboard::Up) && !isJumping) {
					isJumping = true;
				}
			} else
				PlayerSprite.setTextureRect(IntRect(16, Sprite_Y_choice, 32, 48));
		}

		if (Keyboard::isKeyPressed(Keyboard::Right) && player_x < (screen_x-140)) {
			isReversed = true;
			PlayerSprite.setScale(-3, 3);
			BagSprite.setScale(-2, 2);
			VaccumSprite.setScale(-2.5, 2.5);
			// Check to detect walls
			if (lvl[static_cast<int>((player_y+(PlayerHeight/2)) / cell_size)][static_cast<int>((player_x+PlayerWidth) / cell_size)] != '#' && lvl[static_cast<int>((player_y+PlayerHeight) / cell_size)][static_cast<int>((player_x+PlayerWidth) / cell_size)] != '#')
				player_x += speed;
			if (!isJumping) { // Handles the animations, even when jumping
				if (animationCount == 1) PlayerSprite.setTextureRect(IntRect(51, Sprite_Y_choice, 32, 48));
				if (animationCount == 2) PlayerSprite.setTextureRect(IntRect(84, Sprite_Y_choice, 32, 48));
				if (animationCount == 3) PlayerSprite.setTextureRect(IntRect(117, Sprite_Y_choice, 32, 48));
				if (animationCount == 4) PlayerSprite.setTextureRect(IntRect(150, Sprite_Y_choice, 32, 48));
			}
		} else if (Keyboard::isKeyPressed(Keyboard::Left) && player_x > 70) {
			PlayerSprite.setScale(3, 3);
			BagSprite.setScale(2, 2);
			VaccumSprite.setScale(2.5, 2.5);
			isReversed = false;
			// Check to detect walls
			if (lvl[static_cast<int>((player_y+(PlayerHeight/2)) / cell_size)][static_cast<int>(player_x / cell_size)] != '#' && lvl[static_cast<int>((player_y+PlayerHeight) / cell_size)][static_cast<int>(player_x / cell_size)] != '#')
				player_x -= speed;
			if (!isJumping) { // Handles the animations, even when jumping
				if (animationCount == 1) PlayerSprite.setTextureRect(IntRect(51, Sprite_Y_choice, 32, 48));
				if (animationCount == 2) PlayerSprite.setTextureRect(IntRect(84, Sprite_Y_choice, 32, 48));
				if (animationCount == 3) PlayerSprite.setTextureRect(IntRect(117, Sprite_Y_choice, 32, 48));
				if (animationCount == 4) PlayerSprite.setTextureRect(IntRect(150, Sprite_Y_choice, 32, 48));
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) {
			if (animationCount) PlayerSprite.setTextureRect(IntRect(595, Sprite_Y_choice, 32, 48));
			if (Keyboard::isKeyPressed(Keyboard::Up) && (static_cast<int>((player_y+PlayerHeight) / cell_size) != height-1)) {
				PlayerSprite.setTextureRect(IntRect(525, Sprite_Y_choice, 32, 42));
				fallThrough = true;
				isJumping = false;
			} else {
				PlayerHeight = 132;
			}
		}

		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			vaccum = true;
			if (animationCount == 1) VaccumSprite.setTextureRect(IntRect(223, 175, 18, 29));
			if (animationCount == 2) VaccumSprite.setTextureRect(IntRect(251, 174, 35, 29));
			if (animationCount == 3) VaccumSprite.setTextureRect(IntRect(300, 174, 45, 29));
			if (animationCount == 4) VaccumSprite.setTextureRect(IntRect(349, 174, 48, 29));
		} else {
			vaccum = false;
		}
		
		if (isJumping) {
			PlayerSprite.setTextureRect(IntRect(525, Sprite_Y_choice, 32, 48));
			// Check if block above is non-jumpable
			if (lvl[static_cast<int>(player_y / cell_size)][static_cast<int>((player_x+(PlayerWidth/2)) / cell_size)] != '#') {
				player_y -= 10;
				i++;
			} else
				isJumping = false;
			if (i >= 13) { // Jumping across multiple frames
				isJumping = false; 
				i = 0;
				PlayerSprite.setTextureRect(IntRect(16, Sprite_Y_choice, 32, 48));
			}
		}

		//presing escape to close
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		window.clear();

		display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, platformTexture, platformSprite, height, width, cell_size);
		player_gravity(lvl,offset_y,velocityY,onGround,gravity,terminal_Velocity, player_x, player_y, cell_size, PlayerHeight, PlayerWidth, isJumping, fallThrough);

		// Handles the teleportation caused by negetive values in setScale()
		if (isReversed) {
			PlayerSprite.setPosition(player_x + PlayerWidth, player_y);
			BagSprite.setPosition(player_x+10, player_y+(PlayerHeight/2)-5);
			VaccumSprite.setPosition(player_x+192, player_y + 60);
		} else {
			PlayerSprite.setPosition(player_x, player_y);
			BagSprite.setPosition(player_x+PlayerWidth-12, player_y+(PlayerHeight/2)-5);
			VaccumSprite.setPosition(player_x - 120, player_y + 60);
		}

		window.draw(BagSprite);
		window.draw(PlayerSprite);
		if(vaccum) window.draw(VaccumSprite);
		window.display();
	}

	// stopping music and deleting level array
	// lvlMusic.stop();
	for (int i = 0; i < height; i++) {
		delete[] lvl[i];
	}
	delete[] lvl;

	return 0;
}

void display_level(RenderWindow& window, char**lvl, Texture& bgTex,Sprite& bgSprite, Texture& blockTexture, Sprite& blockSprite, Texture& platformTexture, Sprite& platformSprite, const int height, const int width, const int cell_size) {
	window.draw(bgSprite);

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			if (lvl[i][j] == '#') {
				blockSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(blockSprite);
			} else if (lvl[i][j] == '.') { // To draw the grass blocks
				platformSprite.setPosition(j * cell_size, i * cell_size);
				window.draw(platformSprite);
			}
		}
	}

}

void player_gravity(char** lvl, float& offset_y, float& velocityY, bool& onGround, const float& gravity, float& terminal_Velocity, float& player_x, float& player_y, const int cell_size, int& Pheight, int& Pwidth, bool isJumping, bool fallThrough) {

	offset_y = player_y;
	offset_y += velocityY;

	char bottom_mid_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x + Pwidth / 2) / cell_size];

	if ((bottom_mid_down == '#' || bottom_mid_down == '.') && !fallThrough) {
		onGround = true;
	} else {
		player_y = offset_y;
		onGround = false;
	}
	
	if (!onGround && !isJumping) {
		velocityY += gravity;
		if (velocityY >= terminal_Velocity) velocityY = terminal_Velocity;
	} else {
		velocityY = 0;
	}
}

void level_structure (int level, char** lvl, int width, int height) {
	if (level == 1) {
		// Boundary of the level
		for (int i=0; i<width; i++)
			lvl[0][i] = '#';
		for (int i=0; i<height; i++)
			lvl[i][0] = '#';
		for (int i=0; i<width; i++)
			lvl[height-1][i] = '.';
		for (int i=0; i<height; i++)
			lvl[i][width-1] = '#';
		lvl[height-1][width-1] = '.';
			
		// Level 1 Platform Design ('#' is for the central block)
		for (int i=6; i<=9; i++) {
			lvl[5][i] = '#';
			lvl[6][i] = '#';
			lvl[7][i] = '#';
			lvl[8][i] = '#';
			lvl[9][i] = '#';
		}
		for (int i=7; i<=8; i++) {
			lvl[4][i] = '#';
			lvl[10][i] = '#';
		}
		for (int i=3; i<width-3; i++) {
			lvl[3][i] = '.';
			lvl[11][i] = '.';
		}
		for (int i=1; i<width-1; i++)
			if (i<=3 || i>=12) {
				lvl[5][i] = '.';
				lvl[9][i] = '.';
			}
		for (int i=3; i<13; i++)
			if (i<=5 || i>=10)
				lvl[7][i] = '.';
	}
}

int characterSelection(RenderWindow &window)
{
    bool isSelect = false;

    float player1_x = 200;
    float player1_y = 400;

    float player2_x = 720;
    float player2_y = 400;

    int choice = 0;

    Event ev;
    Texture bgTex;
    Sprite bgSprite;
    Texture player1Texture;
    Sprite player1Sprite;
    Texture player2Texture;
    Sprite player2Sprite;

    bgTex.loadFromFile("/home/zaid/Pictures/start.png");
    bgSprite.setTexture(bgTex);
    bgSprite.setPosition(0, 0);

    player1Texture.loadFromFile("Assets/Player/player.png");
    player1Sprite.setTexture(player1Texture);
    player1Sprite.setTextureRect(IntRect(16, 42, 31, 39));
    player1Sprite.setScale(3, 3);
    player1Sprite.setPosition(player1_x, player1_y);

    player2Texture.loadFromFile("Assets/Player/player.png");
    player2Sprite.setTexture(player1Texture);
    player2Sprite.setTextureRect(IntRect(16, 231, 31, 39));
    player2Sprite.setScale(3, 3);
    player2Sprite.setPosition(player2_x, player2_y);

    Music lvlMusic;
    lvlMusic.openFromFile("Assets/Data/select.ogg");
    lvlMusic.setVolume(50);
    lvlMusic.play();
    lvlMusic.setLoop(true);

    while (window.isOpen() && isSelect == false)
    {
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
            {
                window.close();
            }

            if (ev.type == Event::KeyPressed)
            {

                if (Keyboard::isKeyPressed(Keyboard::Left))
                {
                    choice = 1;
                    isSelect = true;
                }

                else if (Keyboard::isKeyPressed(Keyboard::Right))
                {
                    choice = 2;
                    isSelect = true;
                }

                else if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    window.close();
                }
            }

            // window.close();
        }
        window.draw(bgSprite);
        window.draw(player1Sprite);
        window.draw(player2Sprite);
        window.display();
        window.clear();
    }
    int framecount = 0;
    while (window.isOpen() && choice != 0)
    {
        if (framecount < 120)
        {
            framecount++;

            if (choice == 1)
            {
                player1Sprite.setTextureRect(IntRect(784, 86, 40, 44));
                player1Sprite.setScale(4, 4);
            }
            else if (choice == 2)
            {
                player2Sprite.setTextureRect(IntRect(784, 275, 40, 44));
                player2Sprite.setScale(4, 4);
            }
            window.draw(bgSprite);
            window.draw(player1Sprite);
            window.draw(player2Sprite);
            window.display();
            window.clear();
        }
        else
            break;
    }
    return choice;
}