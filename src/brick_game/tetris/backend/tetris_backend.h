#ifndef TETRIS_BACKEND_H
#define TETRIS_BACKEND_H

#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

// game parameters
#define HEIGHT 20
#define WIDTH 10
#define LEVEL_MAX 10
#define LEVEL_MIN 1
#define SPEED_MIN 900

// user input keys
#define ESCAPE_KEY 'q'
#define ENTER_KEY 10
#define SPACE_KEY 32
#define PAUSE_KEY 'p'

// custom colors
#define COLOR_ORANGE 8
#define COLOR_YELLOW_ 9
#define COLOR_VIOLET 10

// game states
typedef enum {
  START = 0,
  SPAWN,
  MOVING,
  SHIFTING,
  ATTACHING,
  GAMEOVER,
  PAUSE,
  EXIT_STATE
} GameState_t;

// tetramino figure
typedef struct {
  int view[4][4];
  int x;
  int y;
  char type;
  int rows;
  int cols;
} Tetramino;

// user actions
typedef enum {
  Start,
  Pause,
  Terminate,
  Left,
  Right,
  Up,
  Down,
  Action
} UserAction_t;

// main game information
typedef struct {
  int field[HEIGHT][WIDTH];
  Tetramino next;
  Tetramino current;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  long long timer;
  GameState_t state;
} GameInfo_t;

GameInfo_t *updateCurrentState();
UserAction_t get_action(int user_input);
void userInput(UserAction_t action, bool hold);

void start_state_actions(GameInfo_t *game, UserAction_t action);
void spawn_state_actions(GameInfo_t *game);
void moving_state_actions(GameInfo_t *game, UserAction_t action);
void shifting_state_actions(GameInfo_t *game);
void attaching_state_actions(GameInfo_t *game);
void gameover_state_actions(GameInfo_t *game, UserAction_t action);
void pause_state_actions(GameInfo_t *game, UserAction_t action);

void stats_init(GameInfo_t *game);
long long int get_time();
void reset_field();

void reset_figure(Tetramino *figure);
void generate_figure(Tetramino *figure);
void spawn_figure();
void moving_left();
void moving_right();
void moving_down();
void rotate_figure();
void set_figure_on_field();

int leaving_field();
int collision();
int figure_overlay();

int remove_lines(int *lines);
void shift_lines(int line);

void calculate_score();
void set_level();
void save_high_score(int high_score);
int load_high_score();

#endif