#include "raylib.h"
#include <iostream>

const int SCREEN_WIDTH  = 1280;
const int SCREEN_HEIGHT = 720;
const int SPEED = 800;

int leftScore = 0;
int rightScore = 0;

enum class PlayerType {
    LEFT,
    RIGHT
};

class Player {
private:
    Rectangle rect;
    PlayerType type;

public:
    Player(PlayerType t) : type(t)
    {
        rect.width  = 15;
        rect.height = 200;

        if(type == PlayerType::LEFT)
        {
            rect.x = 50;
            rect.y = SCREEN_HEIGHT / 2 - 100;
        }
        else
        {
            rect.x = SCREEN_WIDTH - 65;
            rect.y = SCREEN_HEIGHT / 2 - 100;
        }
    }

    void move(float dt)
    {
        if(type == PlayerType::LEFT)
        {
            if(IsKeyDown(KEY_W))
                rect.y -= SPEED * dt;

            if(IsKeyDown(KEY_S))
                rect.y += SPEED * dt;
        }
        else
        {
            if(IsKeyDown(KEY_UP))
                rect.y -= SPEED * dt;

            if(IsKeyDown(KEY_DOWN))
                rect.y += SPEED * dt;
        }

        // Keep paddle inside screen
        if(rect.y < 0)
            rect.y = 0;

        if(rect.y + rect.height > SCREEN_HEIGHT)
            rect.y = SCREEN_HEIGHT - rect.height;
    }

    void draw() const
    {
        DrawRectangleRec(rect, GREEN);
    }

    Rectangle getRect() const
    {
        return rect;
    }
};

class Ball {
private:
    Vector2 position;
    float radius;
    float speedX;
    float speedY;

public:
    Ball(float r)
        : radius(r), speedX(-400.f), speedY(300.f)
    {
        position.x = SCREEN_WIDTH / 2;
        position.y = SCREEN_HEIGHT / 2;
    }

    void move(float dt, bool collision)
    {
        if(collision)
            speedX *= -1;

        if(position.y - radius <= 0 ||
           position.y + radius >= SCREEN_HEIGHT)
        {
            speedY *= -1;
        }

        position.x += speedX * dt;
        position.y += speedY * dt;
    }

    void draw() const
    {
        DrawCircleV(position, radius, WHITE);
    }

    bool leftOOB()
    {
        if(position.x + radius < 0)
        {
            rightScore++;
            reset();
            return true;
        }

        return false;
    }

    bool rightOOB()
    {
        if(position.x - radius > SCREEN_WIDTH)
        {
            leftScore++;
            reset();
            return true;
        }

        return false;
    }

    void reset()
    {
        position.x = SCREEN_WIDTH / 2;
        position.y = SCREEN_HEIGHT / 2;

        speedX *= -1;
        speedY = 300.f;
    }

    Vector2 getPosition() const
    {
        return position;
    }

    float getRadius() const
    {
        return radius;
    }
};

bool checkCollision(const Player& leftPlayer,
                    const Player& rightPlayer,
                    const Ball& ball)
{
    return CheckCollisionCircleRec(
               ball.getPosition(),
               ball.getRadius(),
               leftPlayer.getRect()
           ) ||

           CheckCollisionCircleRec(
               ball.getPosition(),
               ball.getRadius(),
               rightPlayer.getRect()
           );
}

void printScore()
{
    std::cout << "Left Player  : " << leftScore << '\n';
    std::cout << "Right Player : " << rightScore << '\n';
    std::cout << "========================\n";
}

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Pong");

    SetTargetFPS(60);

    Player leftPlayer(PlayerType::LEFT);
    Player rightPlayer(PlayerType::RIGHT);

    Ball ball(20);

    while(!WindowShouldClose())
    {
        float dt = GetFrameTime();

        bool collision =
            checkCollision(leftPlayer, rightPlayer, ball);

        ball.move(dt, collision);

        leftPlayer.move(dt);
        rightPlayer.move(dt);

        if(ball.leftOOB())
            printScore();

        if(ball.rightOOB())
            printScore();

        BeginDrawing();

        ClearBackground(BLACK);

        ball.draw();
        leftPlayer.draw();
        rightPlayer.draw();

        DrawText(
            TextFormat("%d", leftScore),
            SCREEN_WIDTH / 4,
            40,
            60,
            WHITE
        );

        DrawText(
            TextFormat("%d", rightScore),
            3 * SCREEN_WIDTH / 4,
            40,
            60,
            WHITE
        );

        EndDrawing();
    }

    CloseWindow();

    return 0;
}