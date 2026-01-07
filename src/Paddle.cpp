#include "Paddle.hpp"

// constr. of the paddle class
Paddle::Paddle(Vector2 pos_t, Color color_t) : speed(DEFAULT_SPEED) {
  paddle_r.width = DEFAULT_WIDTH;
  paddle_r.height = DEFAULT_HEIGHT;
  paddle_r.x = pos_t.x;
  paddle_r.y = pos_t.y;

  color = color_t;
}

// drawing the paddle onto the screen
void Paddle::Draw() { DrawRectangleRec(paddle_r, color); }

// movement handling function for the paddle
void Paddle::HandleMovement(KeyboardKey up, KeyboardKey down) {
  if (IsKeyDown(up) && paddle_r.y > 0)
    // move the paddle up
    paddle_r.y -= speed;
  if (IsKeyDown(down) && paddle_r.y + paddle_r.height < 480)
    // move the paddle down
    paddle_r.y += speed;
}

// get rect of paddle
Rectangle Paddle::get_rect() { return paddle_r; }
