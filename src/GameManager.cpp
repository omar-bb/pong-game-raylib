#include "GameManager.hpp"

GameManager::GameManager() {
  // initialize the scores
  score_p1 = 0;
  score_p2 = 0;

  is_paused = false;
}

// set the score of the first player
void GameManager::setScoreP1(int sc_p) { score_p1 = sc_p; }
// set the score of the second player
void GameManager::setScoreP2(int sc_p) { score_p2 = sc_p; }

// get the first player's score
int GameManager::getScoreP1() const { return score_p1; }
// get the second player's score
int GameManager::getScoreP2() const { return score_p2; }

// increment by one the first player's score
void GameManager::incScoreP1() { score_p1 += 10; }
// increment by one the second player's score
void GameManager::incScoreP2() { score_p2 += 10; }

void GameManager::DrawScore(Color color_) {
  int textWidthS1 = MeasureText(TextFormat("%d", getScoreP1()), 40);
  int gap = 30;
  DrawText(TextFormat("%d", getScoreP2()), (SCREEN_WIDTH / 2) + gap, 10, 40,
           color_);

  DrawText(TextFormat("%d", getScoreP1()),
           SCREEN_WIDTH - ((SCREEN_WIDTH / 2 + gap) + textWidthS1), 10, 40,
           color_);
}
