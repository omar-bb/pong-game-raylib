#include <algorithm>
#include <math.h>
#include <random>
#include <raylib.h>

#define FPS 60
#define MAX_BOUNCE_ANGLE 45

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

class GameStateManager {
private:
  int score_p1;
  int score_p2;

  bool is_paused;

public:
  GameStateManager() {
    // initialize the scores
    score_p1 = 0;
    score_p2 = 0;

    is_paused = false;
  }

  // set the score of the first player
  void setScoreP1(int sc_p) { score_p1 = sc_p; }
  // set the score of the second player
  void setScoreP2(int sc_p) { score_p2 = sc_p; }

  // get the first player's score
  int getScoreP1() const { return score_p1; }
  // get the second player's score
  int getScoreP2() const { return score_p2; }

  // increment by one the first player's score
  void incScoreP1() { score_p1++; }
  // increment by one the second player's score
  void incScoreP2() { score_p2++; }

  void DrawScore() {
    int textWidthS1 = MeasureText(TextFormat("%d", getScoreP1()), 40);
    int gap = 30;
    DrawText(TextFormat("%d", getScoreP2()), (SCREEN_WIDTH / 2) + gap, 10, 40,
             BLACK);

    DrawText(TextFormat("%d", getScoreP1()),
             SCREEN_WIDTH - ((SCREEN_WIDTH / 2 + gap) + textWidthS1), 10, 40,
             BLACK);
  }
};

class Paddle {
public:
  static const int DEFAULT_HEIGHT = 60;
  static const int DEFAULT_WIDTH = 10;
  static constexpr float DEFAULT_SPEED = 10.f;

private:
  Rectangle paddle_r;
  const float speed;
  Color color;

public:
  // constr. of the paddle class
  Paddle(Vector2 pos_t, Color color_t = BLACK) : speed(DEFAULT_SPEED) {
    paddle_r.width = DEFAULT_WIDTH;
    paddle_r.height = DEFAULT_HEIGHT;
    paddle_r.x = pos_t.x;
    paddle_r.y = pos_t.y;

    color = color_t;
  }

  // drawing the paddle onto the screen
  void Draw() { DrawRectangleRec(paddle_r, color); }

  // movement handling function for the paddle
  void HandleMovement(KeyboardKey up, KeyboardKey down) {
    if (IsKeyDown(up) && paddle_r.y > 0)
      // move the paddle up
      paddle_r.y -= speed;
    if (IsKeyDown(down) && paddle_r.y + paddle_r.height < 480)
      // move the paddle down
      paddle_r.y += speed;
  }

  // get rect of paddle
  Rectangle get_rect() { return paddle_r; }
};

class Ball {
public:
  static const int DEFAULT_HEIGHT = 10;
  static const int DEFAULT_WIDTH = 10;
  static constexpr float DEFAULT_SPEED = 10.f;
  static constexpr float START_SPEED = 3.f;

private:
  Rectangle ball_r;
  // ball's position and previous position
  Vector2 pos;
  Vector2 prev_pos;
  // ball's velocities
  float vel_x;
  float vel_y;

  Color color;

public:
  Ball(Vector2 pos_t, Color color_t) : vel_x(DEFAULT_SPEED), vel_y(0.0f) {
    ball_r.width = DEFAULT_WIDTH;
    ball_r.height = DEFAULT_HEIGHT;
    ball_r.x = pos_t.x;
    ball_r.y = pos_t.y;

    color = color_t;
  }

  // set position of ball
  void setPos(float x, float y) {
    ball_r.x = x;
    ball_r.y = y;
  }

  // set the velocities
  void setVel(float vel_x_p, float vel_y_p) {
    vel_x = vel_x_p;
    vel_y = vel_y_p;
  }

  // drawing the ball onto the screen
  void Draw() { DrawRectangleRec(ball_r, color); }

  void Spawn(bool right_p = false) {
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
  void Move(Rectangle r1, Rectangle r2, GameStateManager &gsm) {
    if (CheckCollisionRecs(ball_r, r1)) {
      TraceLog(LOG_INFO,
               TextFormat("ball pos: %f, paddle_pos: %f, ball_prev_pos: %f",
                          ball_r.x + ball_r.width, r1.x, prev_pos.x));
      if (r1.x + r1.width <= prev_pos.x) {

        ball_r.x = r1.x + ball_r.width;
        DrawText("Collision Detected", 10, 10, 17, DARKBLUE);
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
        vel_x = DEFAULT_SPEED * cos(DEG2RAD * perc_ang);
        vel_y = DEFAULT_SPEED * sin(DEG2RAD * perc_ang);
      }
    } else if (CheckCollisionRecs(ball_r, r2)) {
      // log the ball's right corner y coordinate and the paddle's left corner x
      // coordinate
      TraceLog(LOG_INFO,
               TextFormat("ball pos: %f, paddle_pos: %f, ball_prev_pos: %f",
                          ball_r.x + ball_r.width, r2.x, prev_pos.x));
      if (prev_pos.x + ball_r.width <= r2.x) {

        ball_r.x = r2.x - ball_r.width;
        DrawText("Collision Detected", 10, 10, 17, DARKBLUE);
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
        TraceLog(LOG_INFO,
                 TextFormat("perc: %f, perc_ang: %f°", perc, perc_ang));
        // update the velocities
        vel_x = DEFAULT_SPEED * -cos(DEG2RAD * perc_ang);
        vel_y = DEFAULT_SPEED * sin(DEG2RAD * perc_ang);
      }
    } else if (ball_r.y < 0) {
      vel_y *= -1;
    } else if (ball_r.y + ball_r.height > 480) {
      vel_y *= -1;
    } else if (ball_r.x < -150) {
      gsm.incScoreP2();
      this->Spawn(true);
    } else if (ball_r.x > SCREEN_WIDTH + 150) {
      gsm.incScoreP1();
      this->Spawn();
    }

    DrawText("No collision detected", 10, 10, 17, DARKBLUE);

    // move the ball
    prev_pos.x = ball_r.x;
    prev_pos.y = ball_r.y;

    ball_r.x += vel_x;
    ball_r.y += vel_y;
  }
};

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

  while (!WindowShouldClose()) {
    // if (IsKeyPressed(KEY_R))
    //   ball_p.Spawn(true);
    // else if (IsKeyPressed(KEY_T))
    //   ball_p.Spawn(false);
    // TraceLog(LOG_TRACE, "This is a test");

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

    ///////// testing purposes
    // DrawLine(0.f, 120.f, screenWidth, 120.f, BLACK);
    // DrawLine(0.f, screenHeight - 120.f, screenWidth, screenHeight - 120.f,
    //          BLACK);
    /////////

    EndDrawing();
  }
}
