#include "eadk.h"
#include <stddef.h>

/* Garde ces deux lignes identiques à celles de main.c dans
   github.com/numworks/epsilon-sample-app-c (nom de l'app et niveau d'API). */
const char eadk_app_name[] __attribute__((section(".rodata.eadk_app_name"))) = "Multijeux";
const uint32_t eadk_api_level __attribute__((section(".rodata.eadk_api_level"))) = 0;

/* Un jeu = une fonction qui boucle jusqu'à ce que le joueur appuie sur Retour,
   puis revient au menu. */
typedef struct { const char *name; void (*run)(void); } game_t;

#include "game_demo.c"
/* #include "game_run3.c"   <- inclus ici chaque nouveau jeu */

static const game_t games[] = {
  { "Demo carre", game_demo_run },
  /* { "Run 3", game_run3_run }, */
};
#define GAME_COUNT ((int)(sizeof(games) / sizeof(games[0])))

static void wait_release(void) {
  while (eadk_keyboard_scan() != 0) { eadk_timing_msleep(20); }
}

static void draw_menu(int sel) {
  eadk_display_push_rect_uniform((eadk_rect_t){0, 0, 320, 240}, eadk_color_white);
  eadk_display_draw_string("Choisis un jeu", (eadk_point_t){10, 8}, true, eadk_color_black, eadk_color_white);
  for (int i = 0; i < GAME_COUNT; i++) {
    int y = 40 + i * 22;
    eadk_color_t bg = (i == sel) ? 0x001F : eadk_color_white;
    eadk_color_t fg = (i == sel) ? eadk_color_white : eadk_color_black;
    eadk_display_push_rect_uniform((eadk_rect_t){0, (uint16_t)y, 320, 20}, bg);
    eadk_display_draw_string(games[i].name, (eadk_point_t){12, (uint16_t)(y + 2)}, false, fg, bg);
  }
  eadk_display_draw_string("OK : lancer", (eadk_point_t){10, 220}, false, eadk_color_black, eadk_color_white);
}

int main(int argc, char *argv[]) {
  int sel = 0;
  draw_menu(sel);
  while (1) {
    eadk_keyboard_state_t k = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(k, eadk_key_down) && sel < GAME_COUNT - 1) { sel++; draw_menu(sel); wait_release(); }
    else if (eadk_keyboard_key_down(k, eadk_key_up) && sel > 0)          { sel--; draw_menu(sel); wait_release(); }
    else if (eadk_keyboard_key_down(k, eadk_key_ok) || eadk_keyboard_key_down(k, eadk_key_exe)) {
      wait_release();
      games[sel].run();
      wait_release();
      draw_menu(sel);
    }
    eadk_timing_msleep(20);
  }
  return 0;
}
