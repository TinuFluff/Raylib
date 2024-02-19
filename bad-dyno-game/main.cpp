#include "raylib/raylib.h"
#include <math.h>

#define GRAVITY 0.3f
#define JUMP_FORCE 10.0f

float Vector2Distance(Vector2 v1, Vector2 v2) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    return sqrt(dx * dx + dy * dy);
}

bool CollisionDetection(Vector2 PlayerPosition, Vector2 EnemyPosition) {
    // Calculate the distance between the centers of the player and enemy circles
    float distance = Vector2Distance(PlayerPosition, EnemyPosition);

    // Calculate the sum of their radii
    float sumRadii = 50.0f + 25.0f;

    if (distance < sumRadii) {
        // Collision occurred
        return true;
    } else {
        // No collision
        return false;
    }
}

bool OffScreen(Vector2 EnemyPosition) {
    if (EnemyPosition.x <= 0) {
        // Enemy is off the left edge of the screen
        return true;
    } else {
        // Enemy is still on the screen
        return false;
    }
}

// Program main entry point
int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;
    bool game_running = false;
    bool game_over = false;

    InitWindow(screenWidth, screenHeight, "Dyno Clone");

    Vector2 PlayerPosition = { (float)screenWidth/2, (float)screenHeight/2 };
    Vector2 PlayerVelocity = {0};
    bool isJumping = false;

    Vector2 EnemyPosition = { (float)screenWidth + 50, (float)screenHeight / 2 };

    int framesCounter = 0; // Counter to keep track of frames elapsed
    int score = 0; // Player's score (time survived)

    SetTargetFPS(60);

    // Game loop
    while (!WindowShouldClose())
    {
        // Home Screen
        while (!game_running && !game_over && !WindowShouldClose()) {
            // Update
            if (IsKeyDown(KEY_SPACE)) {
                game_running = true;
            }
            // Draw
            BeginDrawing();

                ClearBackground(RAYWHITE);
                DrawText("Press SPACE to start!", 10, 10, 20, DARKGRAY);

            EndDrawing();
        }

        // Main game loop
        while (game_running && !game_over && !WindowShouldClose())
        {
            // Update
            if (IsKeyDown(KEY_SPACE) && !isJumping) {
                PlayerVelocity.y = -JUMP_FORCE;
                isJumping = true;
            }

            PlayerVelocity.y += GRAVITY;
            PlayerPosition.y += PlayerVelocity.y;
            EnemyPosition.x += -3.0f;

            if (PlayerPosition.y >= 225) {
                PlayerPosition.y = 225;
                PlayerVelocity.y = 0;
                isJumping = false;
            }

            if (CollisionDetection(PlayerPosition, EnemyPosition)) {
                game_over = true;
            }

            if (OffScreen(EnemyPosition)) {
                // Teleport the enemy off the user's view to the right side of the screen
                EnemyPosition.x = screenWidth + 50; // Offset it by 50 pixels to ensure it's off-screen
            }

            // Increment score (frames elapsed)
            framesCounter++;
            if (framesCounter / 60 > score) {
                score = framesCounter / 60; // Convert frames to seconds
            }

            // Draw
            BeginDrawing();

                ClearBackground(RAYWHITE);

                DrawCircleV(PlayerPosition, 50, MAROON);
                DrawCircleV((Vector2){ EnemyPosition.x, EnemyPosition.y + 25 }, 25, BLACK);

                DrawText(TextFormat("Score: %d", score), 10, 10, 20, BLACK);

            EndDrawing();
        }

        // Game over screen
        while (game_over && !WindowShouldClose()) {
            // Update
            if (IsKeyDown(KEY_ENTER)) {
                // Reset game state
                game_running = false;
                game_over = false;
                PlayerPosition = (Vector2){ (float)screenWidth / 2, (float)screenHeight / 2 };
                EnemyPosition = (Vector2){ (float)screenWidth + 50, (float)screenHeight / 2 };
                framesCounter = 0; // Reset frames counter
                score = 0; // Reset score
            }
            // Draw
            BeginDrawing();

                ClearBackground(RAYWHITE);
                DrawText("GAME OVER! Press ENTER to restart", 10, 10, 20, DARKGRAY);
                DrawText(TextFormat("Score: %d", score), 10, 40, 20, DARKGRAY);

            EndDrawing();
        }
    }

    // De-Initialization
    CloseWindow();

    return 0;
}
