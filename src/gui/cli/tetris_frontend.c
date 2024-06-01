#include "tetris_frontend.h"

/**
 * Init ncurses and set screen parameters.
 */
void ncurses_init() {
  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  srand(time(NULL));
  nodelay(stdscr, TRUE);
  init_colors();
  init_start_screen_figures();
}

/**
 * Print game screen based on the current game state in console.
 */
void print_game_screen(GameInfo_t game) {
  print_main_frame();
  switch (game.state) {
    case START:
      print_start_screen();
      break;
    case PAUSE:
      print_playing_field_frame();
      print_pause();
      print_field(game);
      print_stats(game);
      print_tetramino(game.current);
      break;
    case GAMEOVER:
      print_playing_field_frame();
      print_field(game);
      print_tetramino(game.current);
      print_game_over(game);
      break;
    default:
      print_playing_field_frame();
      print_field(game);
      print_stats(game);
      print_tetramino(game.current);
      break;
  }
}

void print_playing_field_frame() {
  print_box(1, F_Y_START + HEIGHT, 2, F_X_START + WIDTH * CELL_SIZE);
}

void print_main_frame() {
  print_box(0, F_Y_START + HEIGHT + 1, 0,
            F_X_START + (WIDTH * CELL_SIZE) * 2 + 6);
}

void print_start_screen() {
  Start_screen_figures *figures = get_screen_figures();
  mvprintw(8, ((F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) - 45) / 2 + 1,
           " _______ ______ _______ _____  _____  _____ ");
  mvprintw(9, ((F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) - 45) / 2 + 1,
           "|__   __|  ____|__   __|  __ \\|_   _|/ ____|");
  mvprintw(10, ((F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) - 45) / 2 + 1,
           "   | |  | |__     | |  | |__) | | | | (___  ");
  mvprintw(11, ((F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) - 45) / 2 + 1,
           "   | |  |  __|    | |  |  _  /  | |  \\___ \\ ");
  mvprintw(12, ((F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) - 45) / 2 + 1,
           "   | |  | |____   | |  | | \\ \\ _| |_ ____) |");
  mvprintw(13, ((F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) - 45) / 2 + 1,
           "   |_|  |______|  |_|  |_|  \\_\\_____|_____/ ");

  print_next(figures->fig1, 3,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 6 - 4);
  print_next(figures->fig2, 5,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 6 * 2 - 3);
  print_next(figures->fig3, 2,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 6 * 3 - 3);
  print_next(figures->fig4, 6,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 6 * 4 - 3);
  print_next(figures->fig5, 2,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 6 * 5 - 3);

  print_next(figures->fig6, 15,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 4 - 5);
  print_next(figures->fig7, 17,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 4 * 2 - 3);
  print_next(figures->fig8, 16,
             (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 4 * 3 - 2);

  attron(A_BLINK);
  mvprintw(HEIGHT, (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 2 - 9,
           "ENTER - start game");
  mvprintw(HEIGHT + 1, (F_X_START + (WIDTH * CELL_SIZE) * 2 + 6) / 2 - 9,
           "    q - exit");
  attroff(A_BLINK);
}

void print_pause() {
  attron(A_BLINK);
  mvprintw(F_Y_START + 12, F_X_START + WIDTH * CELL_SIZE + 3, "PAUSE");
  attroff(A_BLINK);
}

void print_box(int top_y, int bottom_y, int left_x, int right_x) {
  for (int i = top_y + 1; i < bottom_y; i++) {
    mvaddch(i, left_x, ACS_VLINE);
    mvaddch(i, right_x, ACS_VLINE);
  }
  for (int i = left_x + 1; i < right_x; i++) {
    mvaddch(top_y, i, ACS_HLINE);
    mvaddch(bottom_y, i, ACS_HLINE);
  }
  mvaddch(top_y, left_x, ACS_ULCORNER);
  mvaddch(top_y, right_x, ACS_URCORNER);
  mvaddch(bottom_y, left_x, ACS_LLCORNER);
  mvaddch(bottom_y, right_x, ACS_LRCORNER);
}

void print_field(GameInfo_t game) {
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      if (game.field[i][j] != 0) {
        attron(COLOR_PAIR(game.field[i][j]));
        mvprintw(F_Y_START + i, F_X_START + j * CELL_SIZE, CELL);
        attroff(COLOR_PAIR(game.field[i][j]));
      }
    }
  }
}

void print_tetramino(Tetramino figure) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (figure.view[i][j] != 0 && figure.y + i >= 0) {
        attron(COLOR_PAIR(figure.view[i][j]));
        mvprintw(F_Y_START + figure.y + i,
                 F_X_START + figure.x * CELL_SIZE + j * CELL_SIZE, CELL);
        attroff(COLOR_PAIR(figure.view[i][j]));
      }
    }
  }
}

void print_stats(GameInfo_t game) {
  mvprintw(F_Y_START, F_X_START + WIDTH * CELL_SIZE + 3, "SCORE: %d",
           game.score);
  mvprintw(F_Y_START + 2, F_X_START + WIDTH * CELL_SIZE + 3, "HIGH SCORE: %d",
           game.high_score);
  mvprintw(F_Y_START + 4, F_X_START + WIDTH * CELL_SIZE + 3, "LEVEL: %d",
           game.level);
  mvprintw(F_Y_START + 6, F_X_START + WIDTH * CELL_SIZE + 3, "NEXT:");
  print_next(game.next, F_Y_START + 8, F_X_START + WIDTH * CELL_SIZE + 3);
  mvprintw(F_Y_START + 15, F_X_START + WIDTH * CELL_SIZE + 3, "<   >  -  move");
  mvprintw(F_Y_START + 16, F_X_START + WIDTH * CELL_SIZE + 3, "  V    -  drop");
  mvprintw(F_Y_START + 17, F_X_START + WIDTH * CELL_SIZE + 3,
           "SPACE  -  rotate");
  mvprintw(F_Y_START + 18, F_X_START + WIDTH * CELL_SIZE + 3,
           "  p    -  pause");
  mvprintw(F_Y_START + 19, F_X_START + WIDTH * CELL_SIZE + 3, "  q    -  exit");
}

void print_next(Tetramino figure, int y, int x) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (figure.view[i][j] != 0) {
        attron(COLOR_PAIR(figure.view[i][j]));
        mvprintw(y + i, x + j * CELL_SIZE, CELL);
        attroff(COLOR_PAIR(figure.view[i][j]));
      }
    }
  }
}

void print_game_over(GameInfo_t game) {
  mvprintw(F_Y_START, F_X_START + WIDTH * CELL_SIZE + 3, "SCORE: %d",
           game.score);
  mvprintw(F_Y_START + 2, F_X_START + WIDTH * CELL_SIZE + 3, "HIGH SCORE: %d",
           game.high_score);
  attron(COLOR_PAIR(BLUE_P));
  mvprintw(6, F_X_START + WIDTH * CELL_SIZE + 3, "[GAME OVER]");
  attroff(COLOR_PAIR(BLUE_P));
  attron(COLOR_PAIR(ORANGE_P));
  mvprintw(8, F_X_START + WIDTH * CELL_SIZE + 3, "            ____  ");
  mvprintw(9, F_X_START + WIDTH * CELL_SIZE + 3, "         / >     >");
  mvprintw(10, F_X_START + WIDTH * CELL_SIZE + 3, "        |   _   _|");
  mvprintw(11, F_X_START + WIDTH * CELL_SIZE + 3, "        / == _x ==");
  mvprintw(12, F_X_START + WIDTH * CELL_SIZE + 3, "       /         |");
  mvprintw(13, F_X_START + WIDTH * CELL_SIZE + 3, "      /  \\      / ");
  mvprintw(14, F_X_START + WIDTH * CELL_SIZE + 3, "   / ￣|  |  |  | ");
  mvprintw(15, F_X_START + WIDTH * CELL_SIZE + 3, "  | (_￣\\__\\_)_) ");
  mvprintw(16, F_X_START + WIDTH * CELL_SIZE + 3, "   \\__)");
  attroff(COLOR_PAIR(ORANGE_P));

  attron(A_BLINK);
  mvprintw(18, F_X_START + WIDTH * CELL_SIZE + 3, "TRY AGAIN?");
  attroff(A_BLINK);
  mvprintw(20, F_X_START + WIDTH * CELL_SIZE + 3, "ENTER  -  YES");
  mvprintw(21, F_X_START + WIDTH * CELL_SIZE + 3, "  q    -  NO");
}

/**
 * Initialize the color palette and color pairs used in the Tetris game.
 */
void init_colors() {
  start_color();
  init_color(COLOR_ORANGE, 800, 429, 0);
  init_color(COLOR_YELLOW_, 900, 900, 0);
  init_color(COLOR_VIOLET, 442, 100, 800);
  init_pair(RED_P, COLOR_RED, COLOR_BLACK);
  init_pair(ORANGE_P, COLOR_ORANGE, COLOR_BLACK);
  init_pair(YELLOW_P, COLOR_YELLOW_, COLOR_BLACK);
  init_pair(GREEN_P, COLOR_GREEN, COLOR_BLACK);
  init_pair(CYAN_P, COLOR_CYAN, COLOR_BLACK);
  init_pair(BLUE_P, COLOR_BLUE, COLOR_BLACK);
  init_pair(VIOLET_P, COLOR_VIOLET, COLOR_BLACK);
}

/**
 * Return a pointer to the struct contains the figures to be displayed on the
 * start screen.
 */
Start_screen_figures *get_screen_figures() {
  static Start_screen_figures figures = {0};
  return &figures;
}

/**
 * Generate figures to be displayed on the start screen.
 */
void init_start_screen_figures() {
  Start_screen_figures *figures = get_screen_figures();
  generate_figure(&figures->fig1);
  generate_figure(&figures->fig2);
  generate_figure(&figures->fig3);
  generate_figure(&figures->fig4);
  generate_figure(&figures->fig5);
  generate_figure(&figures->fig6);
  generate_figure(&figures->fig7);
  generate_figure(&figures->fig8);
}