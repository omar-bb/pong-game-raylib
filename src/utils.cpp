#include "utils.hpp"
#include "constants.hpp"

void DrawVerticalDashedLine(Vector2 lineStartPos_p, Vector2 lineEndPos_p,
                            float lineThick) {
  for (int i = 0; i < SCREEN_HEIGHT; i += 30) {
    lineStartPos_p = {SCREEN_WIDTH / 2.0f, (float)i};
    lineEndPos_p = {SCREEN_WIDTH / 2.0f, (float)(i + 20)};
    DrawLineEx(lineStartPos_p, lineEndPos_p, lineThick, BLACK);
  }
}
