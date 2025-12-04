#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace sf;
using namespace std;

// Global Texture (Loaded only once)
Texture enemyTexture;

// --- GLOBAL DEFINITIONS FOR ENEMY MANAGEMENT using C-STYLE ARRAYS ---
// The size must be fixed (const)
const int MAX_ENEMIES = 5;

// Arrays to store all properties for all enemies
// Each index (0 to 4) corresponds to a single, unique enemy.

// C-style arrays for basic types
float enemyPositionsX[MAX_ENEMIES];
float enemySpeeds[MAX_ENEMIES];
int enemyDirections[MAX_ENEMIES]; // 1 (right) or -1 (left)

// Array of SFML Sprite objects
Sprite enemySprites[MAX_ENEMIES];

// --- NEW ARRAYS FOR DELAYED TURNING LOGIC ---
// Counter for frames remaining in the pause
int enemyDelayCounters[MAX_ENEMIES];
// Flag to indicate if the enemy is currently waiting for the delay to end
bool enemyIsWaiting[MAX_ENEMIES];

/**
 * @brief Initializes and loads all necessary resources for the enemies.
 */
void initializeEnemies(int screen_width, int screen_height)
{
    // 0. Seed random for placement/speed
    srand(time(0));

    // Load Enemy Texture once
    // NOTE: Replace this placeholder path with a path to your actual enemy sprite sheet.
    string texturePath = "Assets/Enemy/enemy_sprite_sheet.png";
    if (!enemyTexture.loadFromFile(texturePath))
    {
        cerr << "Error: Could not load enemy texture from " << texturePath << endl;
        // In a real game, you would exit or use a placeholder here.
    }

    // 1. Populate the Parallel Arrays
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        float startX = 100.0f + (i * 150.0f);
        float startY = 600.0f;
        // Speeds are independent
        float speed = 1.0f + (rand() % 4);

        // Store basic properties in arrays
        enemyPositionsX[i] = startX;
        enemySpeeds[i] = speed;
        enemyDirections[i] = 1; // Start moving right

        // Initialize delay state
        enemyDelayCounters[i] = 0;
        enemyIsWaiting[i] = false;

        // Configure the Sprite object directly in the array index
        enemySprites[i].setTexture(enemyTexture);
        enemySprites[i].setTextureRect(IntRect(0, 0, 32, 32)); // Example initial frame
        enemySprites[i].setScale(2, 2);
        enemySprites[i].setPosition(startX, startY);
    }
}

/**
 * @brief Updates the position and direction of all enemies.
 * This is the movement logic that gives the illusion of simultaneous action.
 */
void updateAllEnemies()
{
    // Iterate through ALL enemies from index 0 up to MAX_ENEMIES-1
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {

        // --- PHASE 1: DELAY COUNTDOWN (When the enemy is paused) ---
        if (enemyIsWaiting[i])
        {
            if (enemyDelayCounters[i] > 0)
            {
                // Enemy is paused, just count down frames
                enemyDelayCounters[i]--;
            }
            else
            {
                // Delay complete: flip direction and resume movement
                enemyDirections[i] *= -1;  // Flip direction
                enemyIsWaiting[i] = false; // Resume movement

                // Flip the sprite scale to match the new direction
                if (enemyDirections[i] == -1)
                    enemySprites[i].setScale(-2, 2);
                else
                    enemySprites[i].setScale(2, 2);
            }
        }

        // --- PHASE 2: MOVEMENT & DELAY TRIGGER (Only runs if NOT waiting) ---
        if (!enemyIsWaiting[i])
        {
            // 1. Apply Movement
            enemyPositionsX[i] += enemySpeeds[i] * enemyDirections[i];

            // 2. Check for Trigger Conditions (Boundary Hit or Random Turn)
            bool triggerDelay = false;

            // Condition A: Boundary Check (Hitting the 'corner' of the platform)
            if (enemyPositionsX[i] > 900 || enemyPositionsX[i] < 100)
            {
                triggerDelay = true;
            }

            // Condition B: Occasional Turn Trigger (e.g., 1 in 300 chance per frame, about once every 5 seconds)
            else if (rand() % 300 == 0)
            {
                triggerDelay = true;
            }

            if (triggerDelay)
            {
                // Set the delay state
                enemyIsWaiting[i] = true;
                // Set a random delay between 60 frames (1 sec) and 120 frames (2 sec)
                enemyDelayCounters[i] = (rand() % 61) + 60;
                // We DON'T flip the direction yet; we wait for the counter to hit 0 (in PHASE 1).
            }
        }

        // --- PHASE 3: UPDATE SPRITE POSITION (Happens every frame) ---
        // This ensures the enemy sprite is always drawn at the current position,
        // even if it is paused and waiting for the delay.
        if (enemyDirections[i] == -1)
            // Need to offset position by sprite width when flipped
            enemySprites[i].setPosition(enemyPositionsX[i] + enemySprites[i].getGlobalBounds().width, enemySprites[i].getPosition().y);
        else
            enemySprites[i].setPosition(enemyPositionsX[i], enemySprites[i].getPosition().y);
    }
}

/**
 * @brief Draws all enemy sprites to the window.
 */
void drawAllEnemies(RenderWindow &window)
{
    // Iterate through ALL sprites and draw them
    for (int i = 0; i < MAX_ENEMIES; ++i)
    {
        window.draw(enemySprites[i]);
    }
}

int main()
{
    RenderWindow window(VideoMode(1024, 768), "Enemy Manager Demo (C-Arrays)", Style::Default);
    window.setFramerateLimit(60);

    // Initialize all enemies and their properties
    initializeEnemies(1024, 768);

    // --- Main Game Loop ---
    Event event;
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        // 1. Update all enemy positions and AI logic
        updateAllEnemies();

        // --- Drawing ---
        window.clear(Color::Black);

        // 2. Draw all enemies
        drawAllEnemies(window);

        window.display();
    }

    return 0;
}