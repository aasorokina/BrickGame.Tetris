#ifndef TETRIS_FRONTEND_H
#define TETRIS_FRONTEND_H

#include "../../brick_game/tetris/backend/tetris_backend.h"
#include "../../brick_game/tetris/tetris.h"

// start points for the playing field
#define F_Y_START 2
#define F_X_START 3

// tetramino cells
#define CELL "[]"
#define CELL_SIZE strlen(CELL)

// custom colors
#define COLOR_ORANGE 8
#define COLOR_YELLOW_ 9
#define COLOR_VIOLET 10

// color pairs
#define RED_P 1
#define ORANGE_P 8
#define YELLOW_P 9
#define GREEN_P 2
#define CYAN_P 6
#define BLUE_P 4
#define VIOLET_P 10

typedef struct {
  Tetramino fig1;
  Tetramino fig2;
  Tetramino fig3;
  Tetramino fig4;
  Tetramino fig5;
  Tetramino fig6;
  Tetramino fig7;
  Tetramino fig8;
} Start_screen_figures;

void ncurses_init();
void init_colors();
Start_screen_figures* get_screen_figures();
void init_start_screen_figures();
void print_playing_field_frame();
void print_main_frame();
void print_field(GameInfo_t game);
void print_box(int top_y, int bottom_y, int left_x, int right_x);
void print_stats(GameInfo_t game);
void print_tetramino(Tetramino figure);
void print_next(Tetramino figure, int y, int x);
void print_start_screen();
void print_pause();
void print_game_screen(GameInfo_t game);
void print_game_over(GameInfo_t game);

#endif