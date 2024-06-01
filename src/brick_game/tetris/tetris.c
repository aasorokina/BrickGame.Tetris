#include "tetris.h"

int main(void) {
  ncurses_init();
  game_loop();
  endwin();

  return 0;
}

/**
 * The main game loop that runs the Tetris game.
 */
void game_loop() {
  GameInfo_t *game = updateCurrentState();
  stats_init(game);
  while (game->state != EXIT_STATE) {
    erase();
    print_game_screen(*game);
    userInput(get_action(getch()), 0);
    refresh();
  }
}
