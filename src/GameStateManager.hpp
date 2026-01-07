#ifndef GAMESTATEMANAGER_HPP
#define GAMESTATEMANAGER_HPP

#include "constants.hpp"
#include <raylib.h>

class GameStateManager {
private:
  int score_p1;
  int score_p2;

  bool is_paused;

public:
  GameStateManager();

  // set the score of the first player
  void setScoreP1(int sc_p);
  // set the score of the second player
  void setScoreP2(int sc_p);

  // get the first player's score
  int getScoreP1() const;
  // get the second player's score
  int getScoreP2() const;

  // increment by one the first player's score
  void incScoreP1();
  // increment by one the second player's score
  void incScoreP2();

  // function to draw score on the screen
  // TODO: make it a standalone function (shouldn't be in this class)
  void DrawScore();
};

#endif
