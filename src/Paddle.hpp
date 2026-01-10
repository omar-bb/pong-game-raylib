#ifndef PADDLE_HPP
#define PADDLE_HPP

#include "constants.hpp"
#include <raylib.h>

class Paddle {
public:
  static const int DEFAULT_HEIGHT = 60;
  static const int DEFAULT_WIDTH = 10;
  static constexpr float DEFAULT_SPEED = 5.f;

private:
  Rectangle paddle_r;
  const float speed;
  Color color;

public:
  // constr. of the paddle class
  Paddle(Vector2 pos_, Color color_ = BLACK);

  // drawing the paddle onto the screen
  void Draw();

  // movement handling function for the paddle
  void HandleMovement(KeyboardKey up, KeyboardKey down);

  // get rect of paddle
  Rectangle get_rect();
};

#endif
