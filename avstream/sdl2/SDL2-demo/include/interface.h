#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

int load_image();     // 02
int handle_event();   // 03
class Interface04 {
 public:
  Interface04();
  ~Interface04();

  SDL_Surface *LoadSurface(const char *path);
  int LoadMedias();
  int Run();

 private:
  enum {
    KEY_PRESS_SURFACE_DEFAULT,
    KEY_PRESS_SURFACE_UP,
    KEY_PRESS_SURFACE_DOWN,
    KEY_PRESS_SURFACE_LEFT,
    KEY_PRESS_SURFACE_RIGHT,
    KEY_PRESS_SURFACE_TOTAL
  };

  SDL_Window *window_;
  SDL_Surface *screenSurface_;
  SDL_Surface *surfaceList_[KEY_PRESS_SURFACE_TOTAL];
};  // 04
int load_png(); // 06
int render_and_texture(); // 07
int draw_render(); //08
int view_port(); // 09
int color_key();  // 10
int clip_render(); // 11
int color_module(); // 12
int alpha_blend(); //13
int rotation_and_flipping(); //15
int font(); //16

enum LButtonSprite {
  BUTTON_SPRITE_MOUSE_OUT = 0,
  BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
  BUTTON_SPRITE_MOUSE_DOWN = 2,
  BUTTON_SPRITE_MOUSE_UP = 3,
  BUTTON_SPRITE_TOTAL = 4
};  // 17
class LButton {
 public:
  void setPosition(int tx, int ty, int rx, int ry);
  void handleEvent(SDL_Event *event);

  SDL_Rect textureRect, renderRect;
};  // 17
int mouse_event();  // 17

int key_status(); // 18
int sound();  // 21

#endif