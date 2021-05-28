#include <cstdio>
#include "interface.h"
#include "defer.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

int load_image() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("SDL init failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  if (surface == nullptr) {
    printf("get surface from window failed, err: %s\n", SDL_GetError());
    return -3;
  }

  SDL_Surface *image = SDL_LoadBMP("/data/avstream/sdl/image/x.bmp");
  if (image == nullptr) {
    printf("load bmp file failed, err: %s\n", SDL_GetError());
    return -4;
  }
  defer (SDL_FreeSurface(image));

  SDL_BlitSurface(image, NULL, surface, NULL);
  SDL_UpdateWindowSurface(window);
  SDL_Delay(5000);

  return 0;
}
int handle_event() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("SDL init failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  if (surface == nullptr) {
    printf("get surface from window failed, err: %s\n", SDL_GetError());
    return -3;
  }

  SDL_Surface *image = SDL_LoadBMP("/data/avstream/sdl/image/x.bmp");
  if (image == nullptr) {
    printf("load bmp file failed, err: %s\n", SDL_GetError());
    return -4;
  }
  defer (SDL_FreeSurface(image));

  SDL_Event event;
  bool quit = false;
  while (!quit) {
    // cpu full
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        printf("recv quit event\n");
      }
    }
    printf("-------\n");

    SDL_BlitSurface(image, NULL, surface, NULL);
    SDL_UpdateWindowSurface(window);
  }

  return 0;
}

Interface04::Interface04() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("SDL init failed, err: %s\n", SDL_GetError());
    exit(-1);
  }

  window_ = SDL_CreateWindow("SDL Tutorial",
                             SDL_WINDOWPOS_UNDEFINED,
                             SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH,
                             SCREEN_HEIGHT,
                             SDL_WINDOW_SHOWN);
  if (window_ == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    exit(-2);
  }

  screenSurface_ = SDL_GetWindowSurface(window_);
  if (screenSurface_ == nullptr) {
    printf("get surface from window failed, err: %s\n", SDL_GetError());
    exit(-3);
  }
}
Interface04::~Interface04() {
  SDL_Quit();
  SDL_DestroyWindow(window_);
  for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
    SDL_FreeSurface(surfaceList_[i]);
  }
}
SDL_Surface *Interface04::LoadSurface(const char *path) {
  SDL_Surface *surface = SDL_LoadBMP(path);
  if (surface == nullptr) {
    printf("load image failed, err: %s\n", SDL_GetError());
    return nullptr;
  }
  return surface;
}
int Interface04::LoadMedias() {
  surfaceList_[KEY_PRESS_SURFACE_DEFAULT] = LoadSurface("/data/avstream/sdl/image/04/press.bmp");
  surfaceList_[KEY_PRESS_SURFACE_UP] = LoadSurface("/data/avstream/sdl/image/04/up.bmp");
  surfaceList_[KEY_PRESS_SURFACE_DOWN] = LoadSurface("/data/avstream/sdl/image/04/down.bmp");
  surfaceList_[KEY_PRESS_SURFACE_LEFT] = LoadSurface("/data/avstream/sdl/image/04/left.bmp");
  surfaceList_[KEY_PRESS_SURFACE_RIGHT] = LoadSurface("/data/avstream/sdl/image/04/right.bmp");
  return 0;
}
int Interface04::Run() {
  SDL_Event event;
  bool quit = false;

  SDL_Surface *currentSurface = surfaceList_[KEY_PRESS_SURFACE_DEFAULT];
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
      } else if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_UP:currentSurface = surfaceList_[KEY_PRESS_SURFACE_UP];
            break;
          case SDLK_DOWN:currentSurface = surfaceList_[KEY_PRESS_SURFACE_DOWN];
            break;
          case SDLK_LEFT:currentSurface = surfaceList_[KEY_PRESS_SURFACE_LEFT];
            break;
          case SDLK_RIGHT:currentSurface = surfaceList_[KEY_PRESS_SURFACE_RIGHT];
            break;
          default:currentSurface = surfaceList_[KEY_PRESS_SURFACE_DEFAULT];
            break;
        }
      }
    }

    SDL_BlitSurface(currentSurface, NULL, screenSurface_, NULL);
    SDL_UpdateWindowSurface(window_);
  }

  return 0;
}

int load_png() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Surface *surface = SDL_GetWindowSurface(window);
  if (surface == nullptr) {
    printf("get surface failed, err: %s\n", SDL_GetError());
    return -3;
  }

  SDL_Surface *image_surface = IMG_Load("/data/avstream/sdl/image/06/loaded.png");
  if (image_surface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(image_surface));

  SDL_BlitSurface(image_surface, NULL, surface, NULL);
  SDL_UpdateWindowSurface(window);
  SDL_Delay(5000);

  return 0;
}
int render_and_texture() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *image_surface = IMG_Load("/data/avstream/sdl/image/06/loaded.png");
  if (image_surface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(image_surface));

  SDL_Texture *texture = SDL_CreateTextureFromSurface(render, image_surface);
  if (texture == nullptr) {
    printf("create texture failde, err: %s\n", SDL_GetError());
    return -8;
  }
  defer(SDL_DestroyTexture(texture));

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }

    SDL_RenderClear(render);
    SDL_RenderCopy(render, texture, nullptr, nullptr);
    SDL_RenderPresent(render);
  }

  return 0;
}
int draw_render() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);
    SDL_RenderClear(render);

    SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_SetRenderDrawColor(render, 0xff, 0x00, 0x00, 0xff);
    SDL_RenderFillRect(render, &fillRect);

    SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
    SDL_SetRenderDrawColor(render, 0x00, 0xFF, 0x00, 0xFF);
    SDL_RenderDrawRect(render, &outlineRect);

    SDL_SetRenderDrawColor(render, 0x00, 0x00, 0xFF, 0xFF);
    SDL_RenderDrawLine(render, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

    SDL_RenderPresent(render);
  }

  return 0;
}
int view_port() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *image_surface = IMG_Load("/data/avstream/sdl/image/06/loaded.png");
  if (image_surface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(image_surface));

  SDL_Texture *texture = SDL_CreateTextureFromSurface(render, image_surface);
  if (texture == nullptr) {
    printf("create texture failde, err: %s\n", SDL_GetError());
    return -8;
  }
  defer(SDL_DestroyTexture(texture));

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }

    SDL_RenderClear(render);

    SDL_Rect leftTop = {0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_RenderSetViewport(render, &leftTop);
    SDL_RenderCopy(render, texture, nullptr, nullptr);

    SDL_Rect rightTop = {SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
    SDL_RenderSetViewport(render, &rightTop);
    SDL_RenderCopy(render, texture, nullptr, nullptr);

    SDL_Rect bottom = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderSetViewport(render, &bottom);
    SDL_RenderCopy(render, texture, nullptr, nullptr);

    SDL_RenderPresent(render);
  }

  return 0;
}
int color_key() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  // create background texture
  SDL_Surface *backgrond_surface = IMG_Load("/data/avstream/sdl/image/10/background.png");
  if (backgrond_surface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(backgrond_surface));

  SDL_Texture *background_texture = SDL_CreateTextureFromSurface(render, backgrond_surface);
  if (background_texture == nullptr) {
    printf("create texture failde, err: %s\n", SDL_GetError());
    return -8;
  }
  defer(SDL_DestroyTexture(background_texture));

  // create foo texture
  SDL_Surface *foo_surface = IMG_Load("/data/avstream/sdl/image/10/foo.png");
  if (foo_surface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(foo_surface));

  // 把color调整到背景色，就可以做到背景透明
  SDL_SetColorKey(foo_surface, SDL_TRUE, SDL_MapRGB(foo_surface->format, 0x00, 0xff, 0xff));
  SDL_Texture *foo_texture = SDL_CreateTextureFromSurface(render, foo_surface);
  if (foo_texture == nullptr) {
    printf("create texture failde, err: %s\n", SDL_GetError());
    return -8;
  }
  defer(SDL_DestroyTexture(foo_texture));

  // get background & foo length
  int backgroundWidth = backgrond_surface->w, backgroundHeight = backgrond_surface->h;
  int fooWidth = foo_surface->w, fooHeight = foo_surface->h;

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }

    SDL_RenderClear(render);

    SDL_Rect background = {0, 0, backgroundWidth, backgroundHeight};
    SDL_RenderCopy(render, background_texture, nullptr, &background);

    SDL_Rect foo = {240, 190, fooWidth, fooHeight};
    SDL_RenderCopy(render, foo_texture, nullptr, &foo);

    SDL_RenderPresent(render);
  }

  return 0;
}
int clip_render() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *dots_surface = IMG_Load("/data/avstream/sdl/image/11/dots.png");
  if (dots_surface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(dots_surface));

  SDL_SetColorKey(dots_surface, SDL_TRUE, SDL_MapRGB(dots_surface->format, 0x00, 0xff, 0xff));
  SDL_Texture *dots_texture = SDL_CreateTextureFromSurface(render, dots_surface);
  if (dots_texture == nullptr) {
    printf("create texture failde, err: %s\n", SDL_GetError());
    return -8;
  }
  defer(SDL_DestroyTexture(dots_texture));

  // dots position
  SDL_Rect leftTopTextureRect = {0, 0, 100, 100};
  SDL_Rect rightTopTextureRect = {100, 0, 100, 100};
  SDL_Rect leftBottomTextureRect = {0, 100, 100, 100};
  SDL_Rect rightBottomTextureRect = {100, 100, 100, 100};

  // render position
  SDL_Rect leftTopRenderRect = {0, 0, 100, 100};
  SDL_Rect rightTopRenderRect = {SCREEN_WIDTH - 100, 0, 100, 100};
  SDL_Rect leftBottomRenderRect = {0, SCREEN_HEIGHT - 100, 100, 100};
  SDL_Rect rightBottomRenderRect = {SCREEN_WIDTH - 100, SCREEN_HEIGHT - 100, 100, 100};

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderClear(render);

    SDL_RenderCopy(render, dots_texture, &leftTopTextureRect, &leftTopRenderRect);
    SDL_RenderCopy(render, dots_texture, &rightTopTextureRect, &rightTopRenderRect);
    SDL_RenderCopy(render, dots_texture, &leftBottomTextureRect, &leftBottomRenderRect);
    SDL_RenderCopy(render, dots_texture, &rightBottomTextureRect, &rightBottomRenderRect);

    SDL_RenderPresent(render);
  }

  return 0;
}
int color_module() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *image_surface = IMG_Load("/data/avstream/sdl/image/12/colors.png");
  if (image_surface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(image_surface));

  SDL_Texture *texture = SDL_CreateTextureFromSurface(render, image_surface);
  if (texture == nullptr) {
    printf("create texture from surface failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_DestroyTexture(texture));

  int32_t red = 255, green = 255, blue = 255;
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:quit = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_q:red += 32;
              break;
            case SDLK_w:green += 32;
              break;
            case SDLK_e:blue += 32;
              break;
            case SDLK_a:red -= 32;
              break;
            case SDLK_s:green -= 32;
              break;
            case SDLK_d:blue -= 32;
              break;
          }
          break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(render);

    SDL_SetTextureColorMod(texture, red, green, blue);

    SDL_Rect rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    SDL_RenderCopy(render, texture, nullptr, &rect);

    SDL_RenderPresent(render);
  }

  return 0;
}
int alpha_blend() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *backgroundSurface = IMG_Load("/data/avstream/sdl/image/13/fadein.png");
  if (backgroundSurface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(backgroundSurface));

  SDL_Texture *backgroundTexture = SDL_CreateTextureFromSurface(render, backgroundSurface);
  if (backgroundTexture == nullptr) {
    printf("create texture from surface failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_DestroyTexture(backgroundTexture));

  SDL_Surface *moduledSurface = IMG_Load("/data/avstream/sdl/image/15/arrow.png");
  if (moduledSurface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(moduledSurface));

  SDL_Texture *moduledTexture = SDL_CreateTextureFromSurface(render, moduledSurface);
  if (moduledTexture == nullptr) {
    printf("create texture from surface failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_DestroyTexture(moduledTexture));

  int32_t alpha = 255;
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:quit = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_w: alpha = alpha + 32 > 255 ? 255 : alpha + 32;
              break;
            case SDLK_s:alpha = alpha - 32 < 0 ? 0 : alpha - 32;
              break;
          }
          break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(render);

    SDL_RenderCopy(render, backgroundTexture, nullptr, nullptr);
    SDL_SetTextureAlphaMod(moduledTexture, alpha);
    SDL_RenderCopy(render, moduledTexture, nullptr, nullptr);

    SDL_RenderPresent(render);
  }

  return 0;
}
int rotation_and_flipping() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *arrowSurface = IMG_Load("/data/avstream/sdl/image/15/arrow.png");
  if (arrowSurface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(arrowSurface));

  SDL_Texture *arrowTexture = SDL_CreateTextureFromSurface(render, arrowSurface);
  if (arrowTexture == nullptr) {
    printf("create texture from surface failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_DestroyTexture(arrowTexture));

  int32_t alpha = 255;
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:quit = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_w: alpha = alpha + 32 > 255 ? 255 : alpha + 32;
              break;
            case SDLK_s:alpha = alpha - 32 < 0 ? 0 : alpha - 32;
              break;
          }
          break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(render);

    SDL_RenderCopyEx(render, arrowTexture, nullptr, nullptr, 270, nullptr, SDL_FLIP_NONE);
    SDL_RenderPresent(render);
  }

  return 0;
}
int font() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  if (TTF_Init() == -1) {
    printf("init ttf failed, err: %s\n", TTF_GetError());
    return -1;
  }
  defer(TTF_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  TTF_Font *font = TTF_OpenFont("/data/avstream/sdl/image/16/lazy.ttf", 28);
  if (font == nullptr) {
    printf("open ttf handler failed, err: %s\n", TTF_GetError());
    return -1;
  }
  defer(TTF_CloseFont(font));

  SDL_Surface *surface = TTF_RenderText_Solid(font, "hello world!", SDL_Color{0, 0, 0});
  if (surface == nullptr) {
    printf("create text surface failed, err: %s\n", TTF_GetError());
    return -1;
  }
  defer(SDL_FreeSurface(surface));

  SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
  if (texture == nullptr) {
    printf("create texture failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_DestroyTexture(texture));

  int32_t alpha = 255;
  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:quit = true;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_w: alpha = alpha + 32 > 255 ? 255 : alpha + 32;
              break;
            case SDLK_s:alpha = alpha - 32 < 0 ? 0 : alpha - 32;
              break;
          }
          break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(render);

    SDL_Rect renderRect = {100, 100, 300, 100};
    SDL_RenderCopyEx(render, texture, nullptr, &renderRect, 0, nullptr, SDL_FLIP_NONE);
    SDL_RenderPresent(render);
  }

  return 0;
}

void LButton::setPosition(int tx, int ty, int rx, int ry) {
  textureRect.x = tx;
  textureRect.y = ty;
  renderRect.x = rx;
  renderRect.y = ry;
  textureRect.w = renderRect.w = BUTTON_WIDTH;
  textureRect.h = renderRect.h = BUTTON_HEIGHT;
}
void LButton::handleEvent(SDL_Event *event) {
  int x, y;
  SDL_GetMouseState(&x, &y);
  bool inside =
      (x < renderRect.x || x > renderRect.x + BUTTON_WIDTH || y < renderRect.y || y > renderRect.y + BUTTON_HEIGHT)
      ? false : true;
  int mCurrentSprite = inside ? BUTTON_SPRITE_MOUSE_UP : BUTTON_SPRITE_MOUSE_OUT;
  if (inside) {
    switch (event->type) {
      case SDL_MOUSEMOTION:mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
        break;
      case SDL_MOUSEBUTTONDOWN:mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
        break;
      case SDL_MOUSEBUTTONUP:mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
        break;
    }
  }

  switch (mCurrentSprite) {
    case BUTTON_SPRITE_MOUSE_OUT:textureRect.y = 0;
      break;
    case BUTTON_SPRITE_MOUSE_OVER_MOTION:textureRect.y = BUTTON_HEIGHT * 1;
      break;
    case BUTTON_SPRITE_MOUSE_DOWN:textureRect.y = BUTTON_HEIGHT * 2;
      break;
    case BUTTON_SPRITE_MOUSE_UP:textureRect.y = BUTTON_HEIGHT * 3;
      break;
  }
}
int mouse_event() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *buttonSurface = IMG_Load("/data/avstream/sdl/image/17/button.png");
  if (buttonSurface == nullptr) {
    printf("load image surface failed, err: %s\n", SDL_GetError());
    return -7;
  }
  defer(SDL_FreeSurface(buttonSurface));

  SDL_Texture *buttonTexture = SDL_CreateTextureFromSurface(render, buttonSurface);
  if (buttonTexture == nullptr) {
    printf("create texture from surface failed, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_DestroyTexture(buttonTexture));

  // set button list
  LButton buttonList[TOTAL_BUTTONS];
  buttonList[0].setPosition(0, 0, 0, 0);
  buttonList[1].setPosition(0, 0, SCREEN_WIDTH - BUTTON_WIDTH, 0);
  buttonList[2].setPosition(0, 0, 0, SCREEN_HEIGHT - BUTTON_HEIGHT);
  buttonList[3].setPosition(0, 0, SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:quit = true;
          break;
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONUP:
        case SDL_MOUSEBUTTONDOWN:
          for (int i = 0; i < TOTAL_BUTTONS; i++) {
            buttonList[i].handleEvent(&event);
          }
          break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(render);

    for (int i = 0; i < TOTAL_BUTTONS; i++) {
      SDL_RenderCopy(render, buttonTexture, &buttonList[i].textureRect, &buttonList[i].renderRect);
    }
    SDL_RenderPresent(render);
  }

  return 0;
}

int key_status() {
  if (SDL_Init(SDL_INIT_VIDEO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  // press
  SDL_Surface *pressSurface = IMG_Load("/data/avstream/sdl/image/18/press.png");
  if (pressSurface == nullptr) {
    printf("load image failed, err: %s\n", IMG_GetError());
    return -4;
  }
  defer(SDL_FreeSurface(pressSurface));

  SDL_Texture *pressTexture = SDL_CreateTextureFromSurface(render, pressSurface);
  if (pressTexture == nullptr) {
    printf("create texture failed, err: %s\n", SDL_GetError());
    return -5;
  }
  defer(SDL_DestroyTexture(pressTexture));

  // left
  SDL_Surface *leftSurface = IMG_Load("/data/avstream/sdl/image/18/left.png");
  if (leftSurface == nullptr) {
    printf("load image failed, err: %s\n", IMG_GetError());
    return -4;
  }
  defer(SDL_FreeSurface(leftSurface));

  SDL_Texture *leftTexture = SDL_CreateTextureFromSurface(render, leftSurface);
  if (leftTexture == nullptr) {
    printf("create texture failed, err: %s\n", SDL_GetError());
    return -5;
  }
  defer(SDL_DestroyTexture(leftTexture));

  // right
  SDL_Surface *rightSurface = IMG_Load("/data/avstream/sdl/image/18/right.png");
  if (rightSurface == nullptr) {
    printf("load image failed, err: %s\n", IMG_GetError());
    return -4;
  }
  defer(SDL_FreeSurface(rightSurface));

  SDL_Texture *rightTexture = SDL_CreateTextureFromSurface(render, rightSurface);
  if (rightTexture == nullptr) {
    printf("create texture failed, err: %s\n", SDL_GetError());
    return -5;
  }
  defer(SDL_DestroyTexture(rightTexture));

  // up
  SDL_Surface *upSurface = IMG_Load("/data/avstream/sdl/image/18/up.png");
  if (upSurface == nullptr) {
    printf("load image failed, err: %s\n", IMG_GetError());
    return -4;
  }
  defer(SDL_FreeSurface(upSurface));

  SDL_Texture *upTexture = SDL_CreateTextureFromSurface(render, upSurface);
  if (upTexture == nullptr) {
    printf("create texture failed, err: %s\n", SDL_GetError());
    return -5;
  }
  defer(SDL_DestroyTexture(upTexture));

  // down
  SDL_Surface *downSurface = IMG_Load("/data/avstream/sdl/image/18/down.png");
  if (downSurface == nullptr) {
    printf("load image failed, err: %s\n", IMG_GetError());
    return -4;
  }
  defer(SDL_FreeSurface(downSurface));

  SDL_Texture *downTexture = SDL_CreateTextureFromSurface(render, downSurface);
  if (downSurface == nullptr) {
    printf("create texture failed, err: %s\n", SDL_GetError());
    return -5;
  }
  defer(SDL_DestroyTexture(downTexture));

  bool quit = false;
  SDL_Event event;
  SDL_Texture *texture;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }

      const Uint8 *keys = SDL_GetKeyboardState(nullptr);
      if (keys[SDL_SCANCODE_1]) {
        texture = upTexture;
      } else if (keys[SDL_SCANCODE_2]) {
        texture = downTexture;
      } else if (keys[SDL_SCANCODE_3]) {
        texture = leftTexture;
      } else if (keys[SDL_SCANCODE_4]) {
        texture = rightTexture;
      } else {
        texture = pressTexture;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(render);

    SDL_RenderCopy(render, texture, nullptr, nullptr);
    SDL_RenderPresent(render);
  }

  return 0;
}
int sound() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
    printf("init sdl video fialde, err: %s\n", SDL_GetError());
    return -1;
  }
  defer(SDL_Quit());

  if (!IMG_Init(IMG_INIT_PNG)) {
    printf("init image process failde, err: %s\n", SDL_GetError());
    return -2;
  }
  defer(IMG_Quit());

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    printf("open audio failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(Mix_CloseAudio());

  SDL_Window *window = SDL_CreateWindow("SDL Tutorial",
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED,
                                        SCREEN_WIDTH,
                                        SCREEN_HEIGHT,
                                        SDL_WINDOW_SHOWN);
  if (window == nullptr) {
    printf("create sdl window failed, err: %s\n", SDL_GetError());
    return -2;
  }
  defer (SDL_DestroyWindow(window));

  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (render == nullptr) {
    printf("init renderer failed, err: %s\n", SDL_GetError());
    return -3;
  }
  defer(SDL_DestroyRenderer(render));

  SDL_Surface *surface = IMG_Load("/data/avstream/sdl/image/21/prompt.png");
  if (surface == nullptr) {
    printf("load image failed, err: %s\n", IMG_GetError());
    return -4;
  }
  defer (SDL_FreeSurface(surface));

  SDL_Texture *texture = SDL_CreateTextureFromSurface(render, surface);
  if (texture == nullptr) {
    printf("create texture failed, err: %s\n", SDL_GetError());
    return -4;
  }
  defer(SDL_DestroyTexture(texture));

  // load sound
  Mix_Music *music = Mix_LoadMUS("/data/avstream/sdl/image/21/medium.wav");
  if (music == nullptr) {
    printf("load music failed, err: %s\n", Mix_GetError());
    return -1;
  }
  defer(Mix_FreeMusic(music));

  Mix_Chunk *scratch = Mix_LoadWAV("/data/avstream/sdl/image/21/scratch.wav");
  if (scratch == nullptr) {
    printf("load media failed, err: %s\n", Mix_GetError());
    return -1;
  }
  defer(Mix_FreeChunk(scratch));

  Mix_Chunk *high = Mix_LoadWAV("/data/avstream/sdl/image/21/high.wav");
  if (high == nullptr) {
    printf("load media failed, err: %s\n", Mix_GetError());
    return -1;
  }
  defer(Mix_FreeChunk(high));

  Mix_Chunk *medium = Mix_LoadWAV("/data/avstream/sdl/image/21/medium.wav");
  if (medium == nullptr) {
    printf("load media failed, err: %s\n", Mix_GetError());
    return -1;
  }
  defer(Mix_FreeChunk(medium));

  Mix_Chunk *low = Mix_LoadWAV("/data/avstream/sdl/image/21/low.wav");
  if (scratch == nullptr) {
    printf("load media failed, err: %s\n", Mix_GetError());
    return -1;
  }
  defer(Mix_FreeChunk(low));

  bool quit = false;
  SDL_Event event;
  while (!quit) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        quit = true;
        break;
      }

      if (event.type != SDL_KEYDOWN) {
        continue;
      }

      switch (event.key.keysym.sym) {
        case SDLK_1:Mix_PlayChannel(-1, high, 0);
          break;
        case SDLK_2:Mix_PlayChannel(-1, medium, 0);
          break;
        case SDLK_3:Mix_PlayChannel(-1, low, 0);
          break;
        case SDLK_4:Mix_PlayChannel(-1, scratch, 0);
          break;
        case SDLK_0:Mix_HaltMusic();
          break;
        case SDLK_9:
          if (Mix_PlayingMusic() == 0) {
            Mix_PlayMusic(music, -1);
          } else {
            if (Mix_PausedMusic() == 1) {
              Mix_ResumeMusic();
            } else {
              Mix_PauseMusic();
            }
          }
          break;
      }
    }

    SDL_SetRenderDrawColor(render, 0xff, 0xff, 0xff, 0xff);
    SDL_RenderClear(render);

    SDL_RenderCopy(render, texture, nullptr, nullptr);
    SDL_RenderPresent(render);
  }

  return 0;
}

