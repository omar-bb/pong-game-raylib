#include "Ball.hpp"
#include "GameStateManager.hpp"
#include "Paddle.hpp"
#include <raylib.h>

int main(void) {

  GameStateManager gsm;

  // define the screen's width and height
  const int screenWidth = 640;
  const int screenHeight = 480;

  // line thickness
  float lineThick = 20.0f;

  // for dashed line
  Vector2 lineStartPos_p = {(screenWidth - lineThick) / 2.0f, 0};
  Vector2 lineEndPos_p = {(screenWidth - lineThick) / 2.0f, screenHeight};

  // the default player 1 paddle's position
  Vector2 pos_1 = {10, screenWidth / 2.0f};
  // the default player 2 paddle's position
  Vector2 pos_2 = {screenWidth - 2 * 10, screenWidth / 2.0f};

  // the default ball's position
  Vector2 pos_b = {screenWidth / 2.0f, screenHeight / 2.0f};

  // paddle and ball's color
  Color pb_color = (Color){0x35, 0x31, 0x32, 0xFF}; // 35, 31, 32
  Color bg_color = (Color){0xF3, 0xDF, 0xA2, 0xFF}; // 243, 223, 162

  // define the first player's paddle
  Paddle paddle_p1(pos_1, pb_color);
  Paddle paddle_p2(pos_2, pb_color);

  Ball ball_p(pos_b, pb_color);

  // initialize the window
  InitWindow(screenWidth, screenHeight, "pong game test game");

  // set the window's fps
  SetTargetFPS(FPS);

  // just for debugging purposes
  // int ball_speed = Ball::DEFAULT_SPEED;

  float df = 5.f;

  while (!WindowShouldClose()) {
    if (IsKeyPressed(KEY_R))
      df += 1;
    else if (IsKeyPressed(KEY_T))
      df -= 1;
    // TraceLog(LOG_TRACE, "This is a test");

    ball_p.setDefaultSpeed(df);

    paddle_p1.HandleMovement(KEY_UP, KEY_DOWN);
    paddle_p2.HandleMovement(KEY_Q, KEY_A);

    ball_p.Move(paddle_p1.get_rect(), paddle_p2.get_rect(), gsm);

    // drawing part
    BeginDrawing();
    ClearBackground(bg_color); // Set background color

    // draw the ball and the two paddles
    ball_p.Draw();
    paddle_p1.Draw();
    paddle_p2.Draw();
    // draw the dashed line
    for (int i = 0; i < screenHeight; i += 30) {
      lineStartPos_p = {screenWidth / 2.0f, (float)i};
      lineEndPos_p = {screenWidth / 2.0f, (float)(i + 20)};
      DrawLineEx(lineStartPos_p, lineEndPos_p, lineThick, BLACK);
    }

    gsm.DrawScore();

    // DrawText(TextFormat("Ball speed: %d", ball_speed), 10, 10, 17, DARKBLUE);

    ///////// testing purposes
    // DrawLine(0.f, 120.f, screenWidth, 120.f, BLACK);
    // DrawLine(0.f, screenHeight - 120.f, screenWidth, screenHeight - 120.f,
    //          BLACK);
    /////////

    EndDrawing();
  }
}
