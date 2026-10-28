#include "eadk.h"

/* Exemple de jeu : un carré qu'on déplace avec les flèches. Retour = quitter. */
void game_demo_run(void) {
  int x = 150, y = 110;
  while (1) {
    eadk_keyboard_state_t k = eadk_keyboard_scan();
    if (eadk_keyboard_key_down(k, eadk_key_back)) { return; }
    if (eadk_keyboard_key_down(k, eadk_key_left))  { x -= 4; }
    if (eadk_keyboard_key_down(k, eadk_key_right)) { x += 4; }
    if (eadk_keyboard_key_down(k, eadk_key_up))    { y -= 4; }
    if (eadk_keyboard_key_down(k, eadk_key_down))  { y += 4; }
    if (x < 0) x = 0; if (x > 300) x = 300;
    if (y < 0) y = 0; if (y > 220) y = 220;
    eadk_display_push_rect_uniform((eadk_rect_t){0, 0, 320, 240}, eadk_color_black);
    eadk_display_push_rect_uniform((eadk_rect_t){(uint16_t)x, (uint16_t)y, 20, 20}, 0xFFE0);
    eadk_display_wait_for_vblank();
  }
}
