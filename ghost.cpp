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

void display_level(RenderWindow &window, char **lvl, Texture &bgTex, Sprite &bgSprite, Texture &blockTexture, Sprite &blockSprite, Texture &platformTexture, Sprite &platformSprite, const int height, const int width, const int cell_size);
void player_gravity(char **lvl, float &offset_y, float &velocityY, bool &onGround, const float &gravity, float &terminal_Velocity, float &player_x, float &player_y, const int cell_size, int &Pheight, int &Pwidth, bool isJumping);
void player_gravity(char **lvl, float &offset_y, float &velocityY, bool &onGround, const float &gravity, float &terminal_Velocity, float &player_x, float &player_y, const int cell_size, int &Pheight, int &Pwidth, bool isJumping);
int characterSelection(RenderWindow &window);
void ghostUpdate(RenderWindow &window, Sprite ghostSprite[], float ghost_x[], float ghost_y[], float ghostSpeed[], int ghostTimer[], const int maxGhost, int &frameCount, bool reverseGhost[], int ghostInDelay[])
{
    for (int i = 0; i < maxGhost; i++)
    {

        if (ghostTimer[i] > 0)
        {
            ghostTimer[i]--;
            // Animation: IDLE/Hovering
            ghostSprite[i].setTextureRect(IntRect(1170, 8, 36, 30));

            float SpriteOffset_x = (ghostSprite[i].getScale().x < 0) ? ghost_x[i] + 108 : ghost_x[i];
            ghostSprite[i].setPosition(SpriteOffset_x, ghost_y[i]);
            continue;
        }
        else if (ghostInDelay[i] > 0)
        {
            ghostInDelay[i]--;
            if (frameCount % 60 < 25)
            {
                ghostSprite[i].setTextureRect(IntRect(1170, 8, 36, 30));
            }
            else if (frameCount % 60 < 35)
            {
                ghostSprite[i].setTextureRect(IntRect(965, 8, 34, 30));
            }
            else if (frameCount % 60 < 60)
            {
                ghostSprite[i].setTextureRect(IntRect(916, 8, 29, 30));
            }
            // ghostSprite[i].setTextureRect(IntRect(1170, 8, 36, 30));
            continue;
        }

        if (reverseGhost[i])
        {
            ghostSpeed[i] *= -1;
            reverseGhost[i] = false;
        }

        ghost_x[i] += ghostSpeed[i];

        // Animation (Floating)
        if (frameCount % 20 < 10)
        {
            ghostSprite[i].setTextureRect(IntRect(1170, 8, 36, 30));
        }
        else if (frameCount % 20 < 20)
        {
            ghostSprite[i].setTextureRect(IntRect(1070, 8, 36, 30));
        }
        else if (frameCount % 20 < 30)
        {
            ghostSprite[i].setTextureRect(IntRect(1020, 8, 36, 30));
        }
        else
        {
            ghostSprite[i].setTextureRect(IntRect(1120, 8, 36, 30));
        }

        // Boundary Checks and Direction Change
        const float MIN_X = 197;
        const float MAX_X = 682;
        bool boundary_hit = false;

        if (ghost_x[i] + ghostSpeed[i] >= MAX_X)
        {
            ghostSpeed[i] *= -1;
            ghost_x[i] = MAX_X;
            boundary_hit = true;
        }
        else if (ghost_x[i] + ghostSpeed[i] <= MIN_X)
        {
            ghostSpeed[i] *= -1;
            ghost_x[i] = MIN_X;
            boundary_hit = true;
        }

        if (boundary_hit)
        {
            ghostTimer[i] = 30; // Pause
        }
        else if (rand() % 200 == 0)
        {
            ghostInDelay[i] = 60;
            if (rand() % 10 == 0)
            {
                reverseGhost[i] = true;
                // ghost_speed[i] *= -1;
            }
        }

        // Apply visual flip
        ghostSprite[i].setScale(ghostSpeed[i] > 0 ? 3 : -3, 3);

        // Set position for drawing (Handle flip offset)
        float draw_x = (ghostSprite[i].getScale().x < 0) ? ghost_x[i] + 108 : ghost_x[i];
        ghostSprite[i].setPosition(draw_x, ghost_y[i]);
    }
}
// void skeletonUpdate(RenderWindow &window, Sprite skeletonSprite[], float skeleton_x[], float skeleton_y[], float skeletonSpeed[], int skeletonTimer[], const int maxSkeleton, int &frameCount, bool reverseSkeleton[], int skeletonInDelay[])
// {
//     for (int i = 0; i < maxSkeleton; i++)
//     {

//         if (skeletonTimer[i] > 0)
//         {
//             skeletonTimer[i]--;
//             // Animation: IDLE/Hovering
//             skeletonSprite[i].setTextureRect(IntRect(1318, 34, 32, 38));

//             float SpriteOffset_x = (skeletonSprite[i].getScale().x < 0) ? skeleton_x[i] + 108 : skeleton_x[i];
//             skeletonSprite[i].setPosition(SpriteOffset_x, skeleton_y[i]);
//             continue;
//         }
//         else if (skeletonInDelay[i] > 0)
//         {
//             skeletonInDelay[i]--;

//             if (frameCount % 120 < 30)
//                 skeletonSprite[i].setTextureRect(IntRect(1318, 32, 36, 38));

//             else if (frameCount % 120 < 60)
//                 skeletonSprite[i].setTextureRect(IntRect(1262, 34, 36, 38));

//             else if (frameCount % 120 < 90)
//                 skeletonSprite[i].setTextureRect(IntRect(1213, 34, 36, 38));

//             else if (frameCount % 120 < 120)
//                 skeletonSprite[i].setTextureRect(IntRect(1179, 34, 36, 38));

//             continue;
//         }

//         if (reverseSkeleton[i])
//         {
//             skeletonSpeed[i] *= -1;
//             reverseSkeleton[i] = false;
//         }

//         skeleton_x[i] += skeletonSpeed[i];

//         // Animation (Floating)
//         if (frameCount % 20 < 10)
//         {
//             skeletonSprite[i].setTextureRect(IntRect(1141, 32, 24, 40));
//         }
//         else if (frameCount % 20 < 20)
//         {
//             skeletonSprite[i].setTextureRect(IntRect(1109, 34, 26, 38));
//         }
//         else if (frameCount % 20 < 30)
//         {
//             skeletonSprite[i].setTextureRect(IntRect(102, 8, 36, 30));
//         }
//         else
//         {
//             skeletonSprite[i].setTextureRect(IntRect(1120, 8, 36, 30));
//         }

//         // Boundary Checks and Direction Change
//         const float MIN_X = 197;
//         const float MAX_X = 682;
//         bool boundary_hit = false;

//         if (skeleton_x[i] + skeletonSpeed[i] >= MAX_X)
//         {
//             skeletonSpeed[i] *= -1;
//             skeleton_x[i] = MAX_X;
//             boundary_hit = true;
//         }
//         else if (skeleton_x[i] + skeletonSpeed[i] <= MIN_X)
//         {
//             skeletonSpeed[i] *= -1;
//             skeleton_x[i] = MIN_X;
//             boundary_hit = true;
//         }

//         if (boundary_hit)
//         {
//             skeletonTimer[i] = 30; // Pause
//         }
//         else if (rand() % 200 == 0)
//         {
//             skeletonInDelay[i] = 120;
//             if (rand() % 10 == 0)
//             {
//                 reverseSkeleton[i] = true;
//             }
//         }

//         // Apply visual flip
//         skeletonSprite[i].setScale(skeletonSpeed[i] > 0 ? 3 : -3, 3);

//         // Set position for drawing (Handle flip offset)
//         float draw_x = (skeletonSprite[i].getScale().x < 0) ? skeleton_x[i] + 108 : skeleton_x[i];
//         skeletonSprite[i].setPosition(draw_x, skeleton_y[i]);
//     }
// }
int main()
{
    srand(time(0));
    RenderWindow window(VideoMode(screen_x, screen_y), "Tumble-POP", Style::Default);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);

    int choice = 0; // Kon sa Character select kia ha
    choice = characterSelection(window);

    // level specifics
    const int cell_size = 64;
    const int height = 14;
    const int width = 16;
    char **lvl;

    // Self created variables
    int frameCount = 0;
    int animationCount = 0;
    int i = 0;
    bool isReversed = false;

    // level and background textures and sprites
    Texture bgTex;
    Sprite bgSprite;
    Texture blockTexture;
    Sprite blockSprite;
    Texture platformTexture;
    Sprite platformSprite;

    bgTex.loadFromFile("Assets/Data/background.png");
    bgSprite.setTexture(bgTex);
    bgSprite.setPosition(0, 0);

    blockTexture.loadFromFile("Assets/Data/blocks.png");
    blockSprite.setTexture(blockTexture);
    blockSprite.setTextureRect(IntRect(195, 75, 64, 64));

    platformTexture.loadFromFile("Assets/Data/blocks.png");
    platformSprite.setTexture(platformTexture);
    platformSprite.setTextureRect(IntRect(69, 66, 64, 64));

    // //Music initialisation
    Music lvlMusic;

    lvlMusic.openFromFile("Assets/Data/mus.ogg");
    lvlMusic.setVolume(20);
    lvlMusic.play();
    lvlMusic.setLoop(true);

    // player data
    float player_x = 473;
    float player_y = 50;

    float speed;
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

    Texture PlayerTexture;
    Sprite PlayerSprite;

    // Ghost data
    const int maxGhost = 2;
    float ghost_x[maxGhost] = {400, 600};
    float ghost_y[maxGhost] = {102, 500};
    float ghostSpeed[maxGhost] = {3, 3};
    int ghostTimer[maxGhost] = {};
    int ghostInDelay[maxGhost] = {};
    bool reverseGhost[maxGhost] = {};
    Sprite ghostSprite[maxGhost];
    Texture ghostTexture;
    ghostTexture.loadFromFile("Assets/Enemies/ghost.png");
    for (int i = 0; i < maxGhost; i++)
    {
        ghostSprite[i].setTexture(ghostTexture);
        ghostSprite[i].setTextureRect(IntRect(8, 8, 36, 30));
        ghostSprite[i].setScale(3, 3);
    }

    // SKELETON
    // const int maxSkeleton = 2;
    // float skeleton_x[maxSkeleton] = {400, 200};
    // float skeleton_y[maxSkeleton] = {102, 300};
    // float skeletonSpeed[maxSkeleton] = {3, 3};
    // int skeletonTimer[maxSkeleton] = {};
    // int skeletonInDelay[maxSkeleton] = {};
    // bool reverseSkeleton[maxSkeleton] = {};
    // Sprite skeletonSprite[maxSkeleton];
    // Texture skeletonTexture;
    // skeletonTexture.loadFromFile("Assets/Enemies/skeleton.png");
    // for (int i = 0; i < maxSkeleton; i++)
    // {
    //     skeletonSprite[i].setTexture(skeletonTexture);
    //     skeletonSprite[i].setTextureRect(IntRect(8, 8, 36, 30));
    //     skeletonSprite[i].setScale(3, 3);
    // }

    const float jumpStrength = -20; // Initial jump velocity
    const float gravity = 1;        // Gravity acceleration

    bool isJumping = false; // Track if jumping

    bool up_collide = false;
    bool left_collide = false;
    bool right_collide = false;

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
    PlayerSprite.setTextureRect(IntRect(16, Sprite_Y_choice, 24, 39));
    // switch (choice)
    // {
    // case 1:
    //     PlayerSprite.setTextureRect(IntRect(16, 42, 24, 39));
    //     break;
    // case 2:
    //     PlayerSprite.setTextureRect(IntRect(16, 231, 24, 39));
    //     break;
    // default:
    //     break;
    // }
    PlayerSprite.setScale(3, 3);
    PlayerSprite.setPosition(player_x, player_y);
    window.draw(PlayerSprite);

    // creating level array
    lvl = new char *[height];
    for (int i = 0; i < height; i += 1)
    {
        lvl[i] = new char[width];
    }

    // Boundary of the level
    for (int i = 0; i < width; i++)
        lvl[0][i] = '#';
    for (int i = 0; i < height; i++)
        lvl[i][0] = '#';
    for (int i = 0; i < width; i++)
        lvl[height - 1][i] = '.';
    for (int i = 0; i < height; i++)
        lvl[i][width - 1] = '#';
    lvl[height - 1][width - 1] = '.';

    // Level 1 Platform Design ('#' is for the central block)
    // for (int i = 6; i <= 9; i++)
    // {
    //     lvl[5][i] = '#';
    //     lvl[6][i] = '#';
    //     lvl[7][i] = '#';
    //     lvl[8][i] = '#';
    //     lvl[9][i] = '#';
    // }
    // for (int i = 7; i <= 8; i++)
    // {
    //     lvl[4][i] = '#';
    //     lvl[10][i] = '#';
    // }

    for (int i = 3; i < width - 3; i++)
    {
        lvl[3][i] = '.';
        lvl[11][i] = '.';
    }
    for (int i = 1; i < width - 1; i++)
        if (i <= 3 || i >= 12)
        {
            lvl[5][i] = '.';
            lvl[9][i] = '.';
        }
    for (int i = 3; i < 13; i++)
        if (i <= 5 || i >= 10)
            lvl[7][i] = '.';

    Event ev;
    // main loop
    while (window.isOpen())
    {

        // Tracks the number of frames for mapping to animations
        if (frameCount < 60)
            frameCount++;
        else
            frameCount = 0;
        if (frameCount % 7 == 0)
            animationCount++;
        if (animationCount == 4)
            animationCount = 0;

        while (window.pollEvent(ev))
        {

            if (ev.type == Event::Closed)
            {
                window.close();
            }

            if (ev.type == Event::KeyPressed)
            {
                if (Keyboard::isKeyPressed(Keyboard::Up) && !isJumping)
                {
                    isJumping = true;
                }
            }
            else
                PlayerSprite.setTextureRect(IntRect(16, Sprite_Y_choice, 32, 48));
        }

        if (Keyboard::isKeyPressed(Keyboard::Right) && player_x < (screen_x - 140))
        {
            isReversed = true;
            PlayerSprite.setScale(-3, 3);
            // if (lvl[(int)[player_x+speed / cell_size]][(int)[player_y / cell_size]] != '#') // Not completed yet
            player_x += speed;
            if (!isJumping)
            { // Handles the animations, even when jumping
                if (animationCount == 1)
                    PlayerSprite.setTextureRect(IntRect(51, Sprite_Y_choice, 32, 48));
                if (animationCount == 2)
                    PlayerSprite.setTextureRect(IntRect(84, Sprite_Y_choice, 32, 48));
                if (animationCount == 3)
                    PlayerSprite.setTextureRect(IntRect(117, Sprite_Y_choice, 32, 48));
                if (animationCount == 4)
                    PlayerSprite.setTextureRect(IntRect(150, Sprite_Y_choice, 32, 48));
            }
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left) && player_x > 70)
        {
            PlayerSprite.setScale(3, 3);
            isReversed = false;
            player_x -= speed;
            if (!isJumping)
            { // Handles the animations, even when jumping
                if (animationCount == 1)
                    PlayerSprite.setTextureRect(IntRect(51, Sprite_Y_choice, 32, 48));
                if (animationCount == 2)
                    PlayerSprite.setTextureRect(IntRect(84, Sprite_Y_choice, 32, 48));
                if (animationCount == 3)
                    PlayerSprite.setTextureRect(IntRect(117, Sprite_Y_choice, 32, 48));
                if (animationCount == 4)
                    PlayerSprite.setTextureRect(IntRect(150, Sprite_Y_choice, 32, 48));
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            if (animationCount)
                PlayerSprite.setTextureRect(IntRect(595, Sprite_Y_choice, 32, 48));
        }

        if (isJumping)
        {
            PlayerSprite.setTextureRect(IntRect(490, Sprite_Y_choice, 32, 48));
            player_y -= 10;
            i++;
            if (i >= 13)
            { // Jumping across multiple frames
                isJumping = false;
                i = 0;
                PlayerSprite.setTextureRect(IntRect(16, Sprite_Y_choice, 32, 48));
            }
        }

        // presing escape to close
        if (Keyboard::isKeyPressed(Keyboard::Escape))
        {
            window.close();
        }

        window.clear();

        display_level(window, lvl, bgTex, bgSprite, blockTexture, blockSprite, platformTexture, platformSprite, height, width, cell_size);
        player_gravity(lvl, offset_y, velocityY, onGround, gravity, terminal_Velocity, player_x, player_y, cell_size, PlayerHeight, PlayerWidth, isJumping);

        // Handles the teleportation caused by negetive values in setScale()
        if (isReversed)
            PlayerSprite.setPosition(player_x + 72, player_y);
        else
            PlayerSprite.setPosition(player_x, player_y);

        ghostUpdate(window, ghostSprite, ghost_x, ghost_y, ghostSpeed, ghostTimer, maxGhost, frameCount, reverseGhost, ghostInDelay);
        // skeletonUpdate(window, skeletonSprite, skeleton_x, skeleton_y, skeletonSpeed, skeletonTimer, maxSkeleton, frameCount, reverseSkeleton, skeletonInDelay);
        window.draw(PlayerSprite);
        for (int i = 0; i < maxGhost; i++)
        {
            // ghostSprite[i].setPosition(ghost_x[i], ghost_y[i]);
            window.draw(ghostSprite[i]);
        }
        // for (int i = 0; i < maxSkeleton; i++)
        // {
        //     // ghostSprite[i].setPosition(ghost_x[i], ghost_y[i]);
        //     window.draw(skeletonSprite[i]);
        // }

        window.display();
    }

    // stopping music and deleting level array
    // lvlMusic.stop();
    for (int i = 0; i < height; i++)
    {
        delete[] lvl[i];
    }
    delete[] lvl;

    return 0;
}

void display_level(RenderWindow &window, char **lvl, Texture &bgTex, Sprite &bgSprite, Texture &blockTexture, Sprite &blockSprite, Texture &platformTexture, Sprite &platformSprite, const int height, const int width, const int cell_size)
{
    window.draw(bgSprite);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {

            if (lvl[i][j] == '#')
            {
                blockSprite.setPosition(j * cell_size, i * cell_size);
                window.draw(blockSprite);
            }
            else if (lvl[i][j] == '.')
            { // To draw the grass blocks
                platformSprite.setPosition(j * cell_size, i * cell_size);
                window.draw(platformSprite);
            }
        }
    }
}

void player_gravity(char **lvl, float &offset_y, float &velocityY, bool &onGround, const float &gravity, float &terminal_Velocity, float &player_x, float &player_y, const int cell_size, int &Pheight, int &Pwidth, bool isJumping)
{

    offset_y = player_y;
    offset_y += velocityY;

    // char bottom_left_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x ) / cell_size];
    // char bottom_right_down = lvl[(int)(offset_y  + Pheight) / cell_size][(int)(player_x + Pwidth) / cell_size];
    char bottom_mid_down = lvl[(int)(offset_y + Pheight) / cell_size][(int)(player_x + Pwidth / 2) / cell_size];
    // char upper_mid = lvl[(int)(offset_y / cell_size)][(int)(player_x + Pwidth / 2) / cell_size];

    if (bottom_mid_down == '#' || bottom_mid_down == '.')
    {
        onGround = true;
    }
    else
    {
        player_y = offset_y;
        onGround = false;
    }

    if (!onGround && !isJumping)
    {
        velocityY += gravity;
        if (velocityY >= terminal_Velocity)
            velocityY = terminal_Velocity;
    }
    else
    {
        velocityY = 0;
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

    bgTex.loadFromFile("Assets/Data/start.png");
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
