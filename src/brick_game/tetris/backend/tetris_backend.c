#include "tetris_backend.h"

/**
 * Init game start values and clear game field.
 * @param game Main game structure.
 */
void stats_init(GameInfo_t *game) {
  reset_field();
  generate_figure(&game->next);
  game->score = 0;
  game->high_score = load_high_score();
  game->level = LEVEL_MIN;
  game->speed = SPEED_MIN;
  game->pause = 0;
  game->timer = get_time();
  game->state = START;
}

/**
 * Convert a user input key code to the corresponding user action.
 * @param user_input The key code of the user input.
 * @return The user action corresponding to the input key code.
 */
UserAction_t get_action(int user_input) {
  UserAction_t res = -1;
  switch (user_input) {
    case KEY_DOWN:
      res = Down;
      break;
    case KEY_LEFT:
      res = Left;
      break;
    case KEY_RIGHT:
      res = Right;
      break;
    case SPACE_KEY:
      res = Action;
      break;
    case PAUSE_KEY:
      res = Pause;
      break;
    case ESCAPE_KEY:
      res = Terminate;
      break;
    case ENTER_KEY:
      res = Start;
      break;
  }
  return res;
}

/**
 * This is a finite state machine realisation based on switch-case statement.
 *
 * Each game state corresponds to one of state-case, where begin functions,
 * handling user action. After processing it makes some action and switches
 * game state to the next one.
 *
 * @param action The user action to be processed.
 * @param hold Indicates whether the user is holding down the action button.
 */
void userInput(UserAction_t action, bool hold) {
  GameInfo_t *game = updateCurrentState();

  switch (game->state) {
    case START:
      start_state_actions(game, action);
      break;

    case SPAWN:
      spawn_state_actions(game);
      break;

    case MOVING:
      moving_state_actions(game, action);
      break;

    case SHIFTING:
      shifting_state_actions(game);
      break;

    case ATTACHING:
      attaching_state_actions(game);
      break;

    case GAMEOVER:
      gameover_state_actions(game, action);
      break;

    case PAUSE:
      pause_state_actions(game, action);
      break;

    default:
      break;
  }
  (void)hold;
}

/**
 * Process user action in START game state and switch game state to the
 * next one.
 */
void start_state_actions(GameInfo_t *game, UserAction_t action) {
  switch (action) {
    case Start:
      game->state = SPAWN;
      break;
    case Terminate:
      game->state = EXIT_STATE;
      break;
    default:
      game->state = START;
      break;
  }
}

/**
 * Create new Tetramino figure and switch game state to the
 * next one.
 */
void spawn_state_actions(GameInfo_t *game) {
  spawn_figure();
  if (figure_overlay()) {
    while (figure_overlay()) {
      game->current.y--;
    }
    game->state = GAMEOVER;
  } else
    game->state = MOVING;
}

/**
 * Process user action in MOVING game state and switch game state to the
 * next one.
 */
void moving_state_actions(GameInfo_t *game, UserAction_t action) {
  switch (action) {
    case Left:
      moving_left();
      break;
    case Right:
      moving_right();
      break;
    case Down:
      while (((collision() & 0b100) != 4)) {
        moving_down();
      }
      break;
    case Action:
      rotate_figure();
      break;
    case Terminate:
      game->state = EXIT_STATE;
      break;
    case Pause:
      game->state = PAUSE;
      game->pause = 1;
      break;
    default:
      break;
  }
  if ((get_time() - game->timer) >= game->speed) {
    game->timer = get_time();
    game->state = SHIFTING;
  }
}

/**
 * Shifting Tetramino figure down, check down collision and switch game state to
 * the next state.
 */
void shifting_state_actions(GameInfo_t *game) {
  moving_down();
  game->state = ((collision() & 0b100) == 4) ? ATTACHING : MOVING;
}

/**
 * Set current figure on the game field, calculate the score, level, and switch
 * game state to the SPAWN state.
 */
void attaching_state_actions(GameInfo_t *game) {
  set_figure_on_field();
  calculate_score();
  set_level();
  game->state = SPAWN;
}

/**
 * Process user action in GAMEOVER game state and switch game state to the
 * next one.
 */
void gameover_state_actions(GameInfo_t *game, UserAction_t action) {
  switch (action) {
    case Start:
      reset_figure(&game->next);
      stats_init(game);
      game->state = SPAWN;
      break;
    case Terminate:
      game->state = EXIT_STATE;
      break;
    default:
      break;
  }
}

/**
 * Process user action in PAUSE game state and switch game state to the
 * next one.
 */
void pause_state_actions(GameInfo_t *game, UserAction_t action) {
  switch (action) {
    case Pause:
      game->pause = 0;
      game->state = MOVING;
      game->timer = get_time();
      break;
    case Terminate:
      game->state = EXIT_STATE;
      break;
    default:
      break;
  }
}

/**
 * Return a pointer to current game state.
 */
GameInfo_t *updateCurrentState() {
  static GameInfo_t game = {0};
  return &game;
}

/**
 * Generate a random Tetramino figure for the Tetris game.
 * @param figure Pointer to the Tetramino struct to be filled with the generated
 * figure.
 */
void generate_figure(Tetramino *figure) {
  int number = rand() % 7;
  figure->rows = 3;
  figure->cols = 3;
  if (number == 0) {
    figure->rows = 2;
    figure->cols = 4;
    for (int i = 0; i < 4; i++) figure->view[1][i] = COLOR_RED;
    figure->type = 'I';
  }
  if (number == 1) {
    figure->rows = 2;
    figure->cols = 2;
    for (int i = 1; i < 3; i++) figure->view[0][i] = COLOR_YELLOW_;
    for (int i = 1; i < 3; i++) figure->view[1][i] = COLOR_YELLOW_;
    figure->type = 'O';
  }
  if (number == 2) {
    figure->view[0][2] = COLOR_BLUE;
    for (int j = 0; j < 3; j++) figure->view[1][j] = COLOR_BLUE;
    figure->type = 'L';
  }
  if (number == 3) {
    figure->view[0][0] = COLOR_GREEN;
    for (int j = 0; j < 3; j++) figure->view[1][j] = COLOR_GREEN;
    figure->type = 'J';
  }
  if (number == 4) {
    for (int i = 1; i < 3; i++) figure->view[0][i] = COLOR_CYAN;
    for (int i = 0; i < 2; i++) figure->view[1][i] = COLOR_CYAN;
    figure->type = 'S';
  }
  if (number == 5) {
    figure->view[0][1] = COLOR_ORANGE;
    for (int i = 0; i < 3; i++) figure->view[1][i] = COLOR_ORANGE;
    figure->type = 'T';
  }
  if (number == 6) {
    for (int i = 0; i < 2; i++) figure->view[0][i] = COLOR_VIOLET;
    for (int i = 1; i < 3; i++) figure->view[1][i] = COLOR_VIOLET;
    figure->type = 'Z';
  }
}

/**
 * Move Tetramino figure to the left on playing field.
 */
void moving_left() {
  GameInfo_t *game = updateCurrentState();
  if ((collision() & 0b010) != 2) game->current.x--;
  if (leaving_field()) game->current.x++;
}

/**
 * Move Tetramino figure to the right on playing field.
 */
void moving_right() {
  GameInfo_t *game = updateCurrentState();
  if ((collision() & 0b001) != 1) game->current.x++;
  if (leaving_field()) game->current.x--;
}

/**
 * Move Tetramino figure down on playing field.
 */
void moving_down() {
  GameInfo_t *game = updateCurrentState();
  if (!leaving_field() && (collision() & 0b100) != 4) game->current.y++;
}

/**
 * Return the current time in milliseconds since the Unix epoch.
 * @return The current time in milliseconds since the Unix epoch.
 */
long long int get_time() {
  struct timeval now;
  gettimeofday(&now, NULL);
  return (long long)now.tv_sec * 1000 + now.tv_usec / 1000;
}

/**
 * Rotate current Tetramino figure.
 */
void rotate_figure() {
  GameInfo_t *game = updateCurrentState();
  int temp_view[4][4] = {0};
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp_view[i][j] = game->current.view[i][j];
    }
  }
  if (game->current.type != 'O' && game->current.type != 'I') {
    for (int i = 0; i < game->current.rows; i++) {
      for (int j = 0; j < game->current.cols; j++)
        game->current.view[i][j] = temp_view[game->current.cols - 1 - j][i];
    }
  }
  if (game->current.type == 'I' && game->current.y >= 0) {
    for (int i = 0; i < 4; i++) {
      int temp = game->current.view[1][i];
      game->current.view[1][i] = game->current.view[i][1];
      game->current.view[i][1] = temp;
    }
    int cols_temp = game->current.cols;
    game->current.cols = game->current.rows;
    game->current.rows = cols_temp;
  }

  if (leaving_field() == 1)
    if ((collision() & 0b001) != 1) game->current.x++;
  if (leaving_field() == 2) {
    if ((collision() & 0b010) != 2) game->current.x--;
    if (game->current.type == 'I')
      if ((collision() & 0b010) != 2) game->current.x--;
  }

  if (figure_overlay() || leaving_field()) {
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) game->current.view[i][j] = temp_view[i][j];
    }
  }
}

/**
 * Check if the current Tetramino is leaving the game field.
 * @return 0 - figure is not leaving the field, 1 - figure is leaving to
 * the left, 2 - figure is leaving to the right, or 3 - figure is leaving from
 * the bottom.
 */
int leaving_field() {
  GameInfo_t *game = updateCurrentState();
  int leave = 0;
  int x = game->current.x;
  int y = game->current.y;
  for (int i = 0; i < 4; i++, y++) {
    for (int j = 0; j < 4; j++, x++) {
      if (game->current.view[i][j] != 0 && (x < 0))
        leave = 1;
      else if (game->current.view[i][j] != 0 && (x > WIDTH - 1))
        leave = 2;
      else if (game->current.view[i][j] != 0 && (y > HEIGHT - 1))
        leave = 3;
    }
    x = game->current.x;
  }
  return leave;
}

/**
 * Set current figure on the game field.
 */
void set_figure_on_field() {
  GameInfo_t *game = updateCurrentState();
  int x = game->current.x;
  int y = game->current.y;
  for (int i = 0; i < 4; i++, y++) {
    for (int j = 0; j < 4; j++, x++) {
      if (game->current.view[i][j] != 0)
        game->field[y][x] = game->current.view[i][j];
    }
    x = game->current.x;
  }
}

/**
 * Reset the game field to an empty state.
 */
void reset_field() {
  GameInfo_t *game = updateCurrentState();
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game->field[i][j] = 0;
    }
  }
}

/**
 * Reset the Tetramino figure to an empty state.
 * @param figure The Tetramino figure to reset.
 */
void reset_figure(Tetramino *figure) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      figure->view[i][j] = 0;
    }
  }
}

/**
 * Spawn a new figure for the Tetris game.
 */
void spawn_figure() {
  GameInfo_t *game = updateCurrentState();
  reset_figure(&game->current);
  game->current = game->next;

  game->current.x = WIDTH / 2 - 2;
  game->current.y = game->current.type == 'I' ? -1 : 0;

  reset_figure(&game->next);
  generate_figure(&game->next);
}

/**
 * Remove any full lines from the game field and shift remaining lines
 * down.
 * @param lines A pointer to an integer that will be incremented for each line
 * removed.
 * @return 1 - any lines were removed, 0 - no lines were removed.
 */
int remove_lines(int *lines) {
  GameInfo_t *game = updateCurrentState();
  int removed = 0;
  for (int i = HEIGHT - 1; i >= 0; i--) {
    int full = 1;
    for (int j = 0; j < WIDTH; j++) {
      if (game->field[i][j] == 0) full = 0;
    }
    if (full) {
      shift_lines(i);
      *lines += 1;
      removed = 1;
    }
  }
  return removed;
}

/**
 * Shift all lines in the game field down by one line, starting from the given
 * line.
 * @param line The line index to start shifting from.
 */
void shift_lines(int line) {
  GameInfo_t *game = updateCurrentState();
  for (int i = line; i > 0; i--) {
    for (int j = 0; j < WIDTH; j++) {
      game->field[i][j] = game->field[i - 1][j];
    }
  }
}

/**
 * Calculate game score and update high score for the current game state.
 */
void calculate_score() {
  GameInfo_t *game = updateCurrentState();
  int lines = 0;
  while (remove_lines(&lines))
    ;
  switch (lines) {
    case 1:
      game->score += 100;
      break;
    case 2:
      game->score += 300;
      break;
    case 3:
      game->score += 700;
      break;
    case 4:
      game->score += 1500;
      break;
  }
  if (game->score > game->high_score) {
    game->high_score = game->score;
    save_high_score(game->high_score);
  }
}

/**
 * Check for collisions between current figure and game field in three
 * directions: down, left, and right.
 *
 * @return integer, whose first three bits contain information about collision.
 *         A bit mask on returned value indicating the type of collision
 * detected: 0b100 (4) - collision on the bottom; 0b010 (2) - collision on the
 * left side; 0b001 (1) - collision on the right side.
 */
int collision() {
  GameInfo_t *game = updateCurrentState();
  int collision = 0;
  int x = game->current.x;
  int y = game->current.y;
  for (int i = 0; i < 4; i++, y++) {
    for (int j = 0; j < 4; j++, x++) {
      if (game->current.view[i][j] != 0 &&
          (game->field[y + 1][x] != 0 || y == HEIGHT - 1))
        collision |= (1 << 2);
      if (game->current.view[i][j] != 0 && game->field[y][x - 1] != 0)
        collision |= (1 << 1);
      if (game->current.view[i][j] != 0 && game->field[y][x + 1] != 0)
        collision |= 1;
    }
    x = game->current.x;
  }
  return collision;
}

/**
 * Check if the current figure overlaps with any blocks in game field.
 * @return 1 - current figure overlaps with game field, 0 - figure does not
 * overlap game field.
 */
int figure_overlay() {
  GameInfo_t *game = updateCurrentState();
  int overlay = 0;
  int x = game->current.x;
  int y = game->current.y;
  for (int i = 0; i < 4; i++, y++) {
    for (int j = 0; j < 4; j++, x++) {
      if (game->current.view[i][j] != 0 && game->field[y][x] != 0) overlay = 1;
    }
    x = game->current.x;
  }
  return overlay;
}

/**
 * Update current game level and speed based on the player's score.
 */
void set_level() {
  GameInfo_t *game = updateCurrentState();
  game->level = (game->score / 600) + 1;
  if (game->level > LEVEL_MAX) game->level = LEVEL_MAX;
  game->speed = SPEED_MIN - (game->level * 80);
}

/**
 * Save the given high score to a file.
 * @param high_score The high score to be saved.
 */
void save_high_score(int high_score) {
  FILE *file = fopen("install/high_score.txt", "w");
  fprintf(file, "%d", high_score);
  fclose(file);
}

/**
 * Load the high score from a file.
 * @return The high score value read from the file.
 */
int load_high_score() {
  int high_score = 0;
  FILE *file = fopen("install/high_score.txt", "r");
  fscanf(file, "%d", &high_score);
  fclose(file);
  return high_score;
}