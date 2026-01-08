#include "Ball.hpp"
#include "GameStateManager.hpp"
#include "Paddle.hpp"
#include "utils.hpp"
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

  // initialize the two paddle objects
  Paddle paddle_p1(pos_1, pb_color);
  Paddle paddle_p2(pos_2, pb_color);

  // initialize the ball object
  Ball ball_p(pos_b, pb_color);

  // initialize the window
  InitWindow(screenWidth, screenHeight, "pong game test game");

  // set the window's fps
  SetTargetFPS(FPS);

  while (!WindowShouldClose()) {

    // handle the movmeent of the two paddles
    paddle_p1.HandleMovement(KEY_UP, KEY_DOWN);
    paddle_p2.HandleMovement(KEY_Q, KEY_A);
    // handle the ball movement
    ball_p.Move(paddle_p1.get_rect(), paddle_p2.get_rect(), gsm);

    // drawing part
    BeginDrawing();

    // set background color
    ClearBackground(bg_color);

    // draw the ball and the two paddles
    ball_p.Draw();
    paddle_p1.Draw();
    paddle_p2.Draw();

    // draw the dashed line
    DrawVerticalDashedLine(lineStartPos_p, lineEndPos_p, lineThick);

    // draw score on the screen
    gsm.DrawScore();

    EndDrawing();
  }
}
