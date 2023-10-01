#include "vex.h"
#include <functional>

using namespace vex;
using ButtonCallback = std::function<void()>;

char * screenWindow;
bool screenDebounce;
int updateScreen();
void drawText(char text[], int x, int y, fontType font, char * fillColor, char * textColor);