#ifndef BALL_HPP
#define BALL_HPP

#include "GameStateManager.hpp"
#include "constants.hpp"
#include <random>
#include <raylib.h>

class Ball {
public:
  static const int DEFAULT_HEIGHT = 10;
  static const int DEFAULT_WIDTH = 10;
  // static constexpr float DEFAULT_SPEED = 5.f;
  static constexpr float START_SPEED = 3.f;
  // static float DEFAULT_SPEED;

private:
  Rectangle ball_r;
  // ball's position and previous position
  Vector2 pos;
  Vector2 prev_pos;
  // ball's velocities
  float vel_x;
  float vel_y;

  // temp
  // TODO: delete these!
  double vel_x_t;
  double vel_y_t;
  float DEFAULT_SPEED;

  Color color;

public:
  // class constructor
  Ball(Vector2 pos_t, Color color_t);

  // set position of ball
  void setPos(float x, float y);
  // set the velocities
  void setVel(float vel_x_p, float vel_y_p);
  // set default speed
  // TODO: delete
  void setDefaultSpeed(float df_speed);

  // drawing the ball onto the screen
  void Draw();

  // spawn the ball at a random location in the middle
  void Spawn(bool right_p = false);

  // movement handling function for the paddle
  void Move(Rectangle r1, Rectangle r2, GameStateManager &gsm);
};

#endif
