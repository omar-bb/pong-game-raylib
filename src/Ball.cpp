#include "Ball.hpp"

// TODO: change back vel_x(DEFAULT_SPEED) to vel_x(START_SPEED)
Ball::Ball(Vector2 pos_t, Color color_t) : vel_x(START_SPEED), vel_y(0.0f) {
  ball_r.width = DEFAULT_WIDTH;
  ball_r.height = DEFAULT_HEIGHT;
  ball_r.x = pos_t.x;
  ball_r.y = pos_t.y;

  color = color_t;

  // temp
  // TODO: delete
  vel_x_t = 1.0f;
  vel_y_t = 1.0f;
  DEFAULT_SPEED = 10.f;
}

void Ball::setPos(float x, float y) {
  ball_r.x = x;
  ball_r.y = y;
}

void Ball::setVel(float vel_x_p, float vel_y_p) {
  vel_x = vel_x_p;
  vel_y = vel_y_p;
}

// temp
// TODO: delete
void Ball::setDefaultSpeed(float df_speed) { DEFAULT_SPEED = df_speed; }

void Ball::Draw() { DrawRectangleRec(ball_r, color); }

void Ball::Spawn(bool right_p) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937_64 rng(seed); // Use Mersenne Twister engine for high quality
  std::uniform_int_distribution<int> dist1(-45, 45);
  // generate the random number
  int random_ang = dist1(rng); // make it float

  // between 120 and screenHeight - 120 (inclusive)
  int lowerBound2 = 120;
  int upperBound2 = SCREEN_HEIGHT - 120;

  std::uniform_int_distribution<int> dist2(lowerBound2, upperBound2);
  // Generate the random number
  int random_pos = dist2(rng); // make it float

  setPos(SCREEN_WIDTH / 2.f, (float)random_pos);
  if (!right_p)
    setVel(START_SPEED * -cos(DEG2RAD * random_ang),
           START_SPEED * sin(DEG2RAD * random_ang));
  else
    setVel(START_SPEED * cos(DEG2RAD * random_ang),
           START_SPEED * sin(DEG2RAD * random_ang));
}

// movement handling function for the paddle
void Ball::Move(Rectangle r1, Rectangle r2, GameStateManager &gsm) {

  if (CheckCollisionRecs(ball_r, r1)) {
    TraceLog(LOG_INFO,
             TextFormat("ball pos: %f, paddle_pos: %f, ball_prev_pos: %f",
                        ball_r.x + ball_r.width, r1.x, prev_pos.x));
    if (r1.x + r1.width <= prev_pos.x) {

      ball_r.x = r1.x + ball_r.width;
      // DrawText("Collision Detected", 10, 10, 17, DARKBLUE);
      // get the ball's center
      double ball_center = ball_r.y + (ball_r.height / 2);
      // get the paddle's center
      double paddle_center = r1.y + (r1.height / 2);

      // get a ratio of how much the ball's center is close to the paddle's
      // center
      double perc = (ball_center - paddle_center) / (r1.height / 2);
      perc = std::clamp(perc, (double)-1, (double)1);

      // calculate how much the angle should be depending of the location of
      // the ball's center
      double perc_ang = perc * MAX_BOUNCE_ANGLE;
      TraceLog(LOG_INFO,
               TextFormat("perc: %f, perc_ang: %f°", perc, perc_ang)); // log
      // update the velocities
      // vel_x = DEFAULT_SPEED * cos(DEG2RAD * perc_ang);
      // vel_y = DEFAULT_SPEED * sin(DEG2RAD * perc_ang);
      vel_x_t = cos(DEG2RAD * perc_ang);
      vel_y_t = sin(DEG2RAD * perc_ang);
    }
  } else if (CheckCollisionRecs(ball_r, r2)) {
    // log the ball's right corner y coordinate and the paddle's left corner x
    // coordinate
    TraceLog(LOG_INFO,
             TextFormat("ball pos: %f, paddle_pos: %f, ball_prev_pos: %f",
                        ball_r.x + ball_r.width, r2.x, prev_pos.x));
    if (prev_pos.x + ball_r.width <= r2.x) {

      ball_r.x = r2.x - ball_r.width;
      // DrawText("Collision Detected", 10, 10, 17, DARKBLUE);
      // get the ball's center
      double ball_center = ball_r.y + (ball_r.height / 2);
      // get the paddle's center
      double paddle_center = r2.y + (r2.height / 2);
      // get a ratio of how much the ball's center is close to the paddle's
      // center
      double perc = (ball_center - paddle_center) / (r2.height / 2);
      // calculate how much the angle should be depending of the location of
      // the ball's center
      double perc_ang = perc * MAX_BOUNCE_ANGLE;
      TraceLog(LOG_INFO, TextFormat("perc: %f, perc_ang: %f°", perc, perc_ang));
      // update the velocities
      // vel_x = DEFAULT_SPEED * -cos(DEG2RAD * perc_ang);
      // vel_y = DEFAULT_SPEED * sin(DEG2RAD * perc_ang);
      vel_x_t = -cos(DEG2RAD * perc_ang);
      vel_y_t = sin(DEG2RAD * perc_ang);
    }
  } else if (ball_r.y < 0) {
    // vel_y *= -1;
    vel_y_t *= -1.f;
  } else if (ball_r.y + ball_r.height > 480) {
    // vel_y *= -1;
    vel_y_t *= -1.f;
  } else if (ball_r.x < -150) {
    gsm.incScoreP2();
    this->Spawn(true);
  } else if (ball_r.x > SCREEN_WIDTH + 150) {
    gsm.incScoreP1();
    this->Spawn();
  }

  vel_x = DEFAULT_SPEED * vel_x_t;
  vel_y = DEFAULT_SPEED * vel_y_t;

  // DrawText("No collision detected", 10, 10, 17, DARKBLUE);

  // move the ball
  prev_pos.x = ball_r.x;
  prev_pos.y = ball_r.y;

  ball_r.x += vel_x;
  ball_r.y += vel_y;
}
