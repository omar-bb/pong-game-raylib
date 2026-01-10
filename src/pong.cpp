#include "Ball.hpp"
#include "GameManager.hpp"
#include "Paddle.hpp"
#include "constants.hpp"
#include "raylib.h"
#include "utils.hpp"

#define RAYGUI_IMPLEMENTATION
#include "../lib/raygui.h"

int main(void) {

  GameManager gm;

  // line thickness
  float lineThick = 20.0f;

  // for dashed line
  Vector2 lineStartPos_p = {(SCREEN_WIDTH - lineThick) / 2.0f, 0};
  Vector2 lineEndPos_p = {(SCREEN_WIDTH - lineThick) / 2.0f, SCREEN_HEIGHT};

  // the default player 1 paddle's position
  Vector2 pos_1 = {10, SCREEN_WIDTH / 2.0f};
  // the default player 2 paddle's position
  Vector2 pos_2 = {SCREEN_WIDTH - 2 * 10, SCREEN_WIDTH / 2.0f};

  // the default ball's position
  Vector2 pos_b = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

  // paddle and ball's color
  Color b_color = (Color){0x35, 0x31, 0x32, 0xFF}; // 35, 31, 32
  Color y_color = (Color){0xF3, 0xDF, 0xA2, 0xFF}; // 243, 223, 162

  // initialize the two paddle objects
  Paddle paddle_p1(pos_1, b_color);
  Paddle paddle_p2(pos_2, b_color);

  // initialize the ball object
  Ball ball_p(pos_b, b_color);

  // initialize the window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "pong game test game");

  // set the window's fps
  SetTargetFPS(FPS);

  // the state of the game
  GameState state = GameState::MENU;

  const int game_over_menu_x = 100;
  const int game_over_menu_y = 100;

  int text_w_t = 0;

  while (!WindowShouldClose()) {

    switch (state) {
    case GameState::MENU:
      BeginDrawing();
      ClearBackground(y_color);
      GuiSetStyle(DEFAULT, TEXT_SIZE, 30);
      if (GuiButton((Rectangle){(SCREEN_WIDTH / 2) - 100,
                                (SCREEN_HEIGHT / 2) - 25, 200, 50},
                    "Play")) {
        state = GameState::PLAYING;
      }
      EndDrawing();
      break;
    // game state PAUSED handled inside the PLAYING game state
    case GameState::GAMEOVER:
    case GameState::PAUSED:
    case GameState::PLAYING:

      if (IsKeyPressed(KEY_P) && state != GameState::PAUSED) {
        state = GameState::PAUSED;
      } else if (IsKeyPressed(KEY_P)) {
        state = GameState::PLAYING;
      }

      if (gm.getScoreP1() == 10 || gm.getScoreP2() == 10) {
        state = GameState::GAMEOVER;
      }

      BeginDrawing();
      // clear background
      ClearBackground(y_color);

      // draw the dashed line
      DrawVerticalDashedLine(lineStartPos_p, lineEndPos_p, lineThick, b_color);

      if (state == GameState::PAUSED) {

        DrawRectangle(10, SCREEN_HEIGHT - 40, SCREEN_WIDTH - 180, 30, y_color);
        DrawRectangleLines(10, SCREEN_HEIGHT - 40, SCREEN_WIDTH - 180, 30,
                           b_color);
        DrawText("Paused, press P to unpause the game", 20, SCREEN_HEIGHT - 35,
                 20, b_color);
      } else if (state == GameState::GAMEOVER) {
        DrawRectangle(game_over_menu_x, game_over_menu_y,
                      SCREEN_WIDTH - 2 * game_over_menu_x,
                      SCREEN_HEIGHT - 2 * game_over_menu_y, y_color);
        text_w_t = MeasureText("Player 1 WON!", 40);
        DrawText("Player 1 WON!", SCREEN_WIDTH / 2 - text_w_t / 2,
                 SCREEN_HEIGHT / 2 - 50, 40, b_color);
        DrawRectangleLinesEx((Rectangle){game_over_menu_x, game_over_menu_y,
                                         SCREEN_WIDTH - 2 * game_over_menu_x,
                                         SCREEN_HEIGHT - 2 * game_over_menu_y},
                             5, b_color);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
        if (GuiButton(
                (Rectangle){game_over_menu_x + 10, SCREEN_HEIGHT / 2 + 100,
                            (SCREEN_WIDTH - 2 * game_over_menu_x) - 20, 30},
                "Exit Game")) {
          CloseWindow();
        }
        if (GuiButton((Rectangle){game_over_menu_x + 10,
                                  (SCREEN_HEIGHT / 2 + 100) - 35,
                                  (SCREEN_WIDTH - 2 * game_over_menu_x) - 20,
                                  30},
                      "Play Again")) {

          gm.setScoreP1(0);
          gm.setScoreP2(0);
          state = GameState::PLAYING;
        }
      } else {
        // handle the movmeent of the two paddles
        paddle_p1.HandleMovement(KEY_UP, KEY_DOWN);
        paddle_p2.HandleMovement(KEY_Q, KEY_A);
        // handle the ball movement
        ball_p.Move(paddle_p1.get_rect(), paddle_p2.get_rect(), gm);
      }

      // draw the ball and the two paddles
      ball_p.Draw();
      paddle_p1.Draw();
      paddle_p2.Draw();

      // draw score on the screen
      gm.DrawScore(b_color);
      EndDrawing();
      break;
    }
  }
}
