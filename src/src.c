#include "../include/src.h"
#include <ncurses.h>

void output() {
  initscr();
  clear();
  refresh();
  addstr("peenor");
  refresh();

  getch();
  endwin();
}
