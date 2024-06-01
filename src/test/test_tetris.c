#include "test_tetris.h"

START_TEST(nothing) { ck_assert_int_eq(1, 1); }
END_TEST

Suite *test_suite(void) {
  Suite *s = suite_create(" ");
  TCase *tc_nothing = tcase_create(" ");
  tcase_add_test(tc_nothing, nothing);
  suite_add_tcase(s, tc_nothing);
  return s;
}

START_TEST(stats_test) {
  GameInfo_t *game = updateCurrentState();
  stats_init(game);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(game->field[i][j], 0);
    }
  }
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  ck_assert_int_eq(game->state, START);
  ck_assert_int_eq(game->score, 0);
  ck_assert_int_eq(game->level, 1);
  ck_assert_int_eq(game->pause, 0);

  save_high_score(50000);
  ck_assert_int_eq(load_high_score(), 50000);
  save_high_score(0);

  for (int i = 19; i > 15; i--) {
    for (int j = 0; j < WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  calculate_score();
  ck_assert_int_eq(game->score, 1500);
  set_level();
  ck_assert_int_eq(game->level, 3);
}
END_TEST

Suite *stats_test_suite(void) {
  Suite *s = suite_create("stats_test");
  TCase *tc_stats_test = tcase_create("stats_test");
  tcase_add_test(tc_stats_test, stats_test);
  suite_add_tcase(s, tc_stats_test);
  return s;
}

START_TEST(user_action_test) {
  ck_assert_int_eq(get_action(ESCAPE_KEY), Terminate);
  ck_assert_int_eq(get_action(ENTER_KEY), Start);
  ck_assert_int_eq(get_action(SPACE_KEY), Action);
  ck_assert_int_eq(get_action(PAUSE_KEY), Pause);
  ck_assert_int_eq(get_action(KEY_DOWN), Down);
  ck_assert_int_eq(get_action(KEY_LEFT), Left);
  ck_assert_int_eq(get_action(KEY_RIGHT), Right);
}
END_TEST

Suite *user_action_test_suite(void) {
  Suite *s = suite_create("user_action_test");
  TCase *tc_user_action_test = tcase_create("user_action_test");
  tcase_add_test(tc_user_action_test, user_action_test);
  suite_add_tcase(s, tc_user_action_test);
  return s;
}

START_TEST(generate_figure_test) {
  GameInfo_t *game = updateCurrentState();
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
  generate_figure(&game->next);
  ck_assert_int_eq(game->next.type != 0, 1);
  ck_assert_int_eq(game->next.cols != 0, 1);
  ck_assert_int_eq(game->next.rows != 0, 1);
}
END_TEST

Suite *generate_figure_test_suite(void) {
  Suite *s = suite_create("generate_figure_test");
  TCase *tc_generate_figure_test = tcase_create("generate_figure_test");
  tcase_add_test(tc_generate_figure_test, generate_figure_test);
  suite_add_tcase(s, tc_generate_figure_test);
  return s;
}

START_TEST(moving_figure_test) {
  GameInfo_t *game = updateCurrentState();
  generate_figure(&game->current);
  game->current.x = 3;
  game->current.y = 0;
  moving_state_actions(game, Left);
  moving_state_actions(game, Left);
  moving_state_actions(game, Right);
  moving_state_actions(game, Down);
  ck_assert_int_eq(game->current.x, 2);
  ck_assert_int_eq(game->current.y, 18);
  game->current.x = 9;
  game->current.y = 0;
  ck_assert_int_eq(leaving_field(), 2);
  game->current.x = -1;
  game->current.y = 0;
  ck_assert_int_eq(leaving_field(), 1);
  game->current.x = 3;
  game->current.y = 19;
  ck_assert_int_eq(leaving_field(), 3);
  game->current.x = 3;
  game->current.y = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game->field[i][j] = 1;
    }
  }
  ck_assert_int_eq(figure_overlay(), 1);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < WIDTH; j++) {
      game->field[i][j] = 0;
    }
  }
  ck_assert_int_eq(figure_overlay(), 0);
  for (int i = 0; i < WIDTH; i++) {
    game->field[2][i] = 1;
  }
  ck_assert_int_eq(collision() & 0b100, 4);
}
END_TEST

Suite *moving_figure_test_suite(void) {
  Suite *s = suite_create("moving_figure_test");
  TCase *tc_moving_figure_test = tcase_create("moving_figure_test");
  tcase_add_test(tc_moving_figure_test, moving_figure_test);
  suite_add_tcase(s, tc_moving_figure_test);
  return s;
}

START_TEST(rotate_figure_test) {
  GameInfo_t *game = updateCurrentState();
  reset_field();
  spawn_state_actions(game);
  reset_figure(&game->current);
  game->current.rows = 2;
  game->current.cols = 2;
  for (int i = 1; i < 3; i++) game->current.view[0][i] = COLOR_YELLOW_;
  for (int i = 1; i < 3; i++) game->current.view[1][i] = COLOR_YELLOW_;
  game->current.type = 'O';
  game->current.y = 0;
  rotate_figure();
  for (int i = 1; i < 3; i++)
    ck_assert_int_eq(game->current.view[0][i], COLOR_YELLOW_);
  for (int i = 1; i < 3; i++)
    ck_assert_int_eq(game->current.view[1][i], COLOR_YELLOW_);

  reset_figure(&game->current);
  game->current.rows = 2;
  game->current.cols = 4;
  for (int i = 0; i < 4; i++) game->current.view[1][i] = COLOR_RED;
  game->current.type = 'I';
  game->current.x = 0;
  game->current.y = 1;
  rotate_figure();
  for (int i = 0; i < 4; i++)
    ck_assert_int_eq(game->current.view[i][1], COLOR_RED);

  reset_figure(&game->current);
  game->current.rows = 3;
  game->current.cols = 3;
  game->current.view[0][2] = COLOR_BLUE;
  for (int i = 0; i < 3; i++) game->current.view[1][i] = COLOR_BLUE;
  game->current.type = 'T';
  rotate_figure();
  for (int i = 0; i < 3; i++)
    ck_assert_int_eq(game->current.view[i][1], COLOR_BLUE);
}
END_TEST

Suite *rotate_figure_test_suite(void) {
  Suite *s = suite_create("rotate_figure_test");
  TCase *tc_rotate_figure_test = tcase_create("rotate_figure_test");
  tcase_add_test(tc_rotate_figure_test, rotate_figure_test);
  suite_add_tcase(s, tc_rotate_figure_test);
  return s;
}

START_TEST(fsm_test) {
  GameInfo_t *game = updateCurrentState();
  game->state = START;
  userInput(Start, 0);
  ck_assert_int_eq(game->state, SPAWN);
  game->state = SPAWN;
  userInput(Left, 0);
  ck_assert_int_eq(game->state, MOVING);
  userInput(Pause, 0);
  ck_assert_int_eq(game->state, PAUSE);
  userInput(Pause, 0);
  ck_assert_int_eq(game->state, MOVING);
  game->state = ATTACHING;
  userInput(Right, 0);
  ck_assert_int_eq(game->state, SPAWN);
  game->state = SHIFTING;
  userInput(Down, 0);
  ck_assert_int_eq(game->state, ATTACHING);
  game->state = MOVING;
  userInput(Terminate, 0);
  ck_assert_int_eq(game->state, EXIT_STATE);
  game->state = GAMEOVER;
  userInput(Start, 0);
  ck_assert_int_eq(game->state, SPAWN);
}
END_TEST

Suite *fsm_test_suite(void) {
  Suite *s = suite_create("fsm_test");
  TCase *tc_fsm_test = tcase_create("fsm_test");
  tcase_add_test(tc_fsm_test, fsm_test);
  suite_add_tcase(s, tc_fsm_test);
  return s;
}

int main() {
  int n_failed = 0;
  Suite *suite = NULL;
  SRunner *sr = srunner_create(suite);

  Suite *suites[] = {test_suite(),
                     stats_test_suite(),
                     user_action_test_suite(),
                     generate_figure_test_suite(),
                     moving_figure_test_suite(),
                     rotate_figure_test_suite(),
                     fsm_test_suite(),
                     NULL};

  for (Suite **st = suites; *st != NULL; st++) srunner_add_suite(sr, *st);
  srunner_set_fork_status(sr, CK_NOFORK);

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  (void)n_failed;
  return 0;
}
