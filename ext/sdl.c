// The author disclaims copyright to this source code.
#include "colorforth.h"
#include "cf-stdio.h"

#ifdef __SDL

#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>


static char initialized = 0;


///* Please, edit this for your project */
#define MAX_KEY 300

int sdl_mouse_x = 0;
int sdl_mouse_y = 0;

int tab_mouse_button[3];
int tab_keys[MAX_KEY];

SDL_Window * main_screen;
SDL_Renderer *main_renderer;
TTF_Font * font = NULL;


// /*===========================================================
//  *   Open and close functions
//  ============================================================*/
void
sdl_init(struct state *s) {
  POP3();
  int width = p3;
  int height = p2;
  int fullscreen = p1;

  Uint32 videoflags = 0;
  int i;

  char *title = "My SDL Windows";

  if ( IMG_Init(IMG_INIT_PNG) < 0 ) {
    fprintf(stderr, "Couldn't initialize SDL Image: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(IMG_Quit);

  if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 ) {
    fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }
  atexit(SDL_Quit);

  if ( TTF_Init() < 0 ) {
    fprintf(stderr, "Couldn't initialize TTF: %s\n", SDL_GetError());
    exit(2);
  }
  atexit(TTF_Quit);

  if (fullscreen == 1) {
    videoflags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
  }

  if ((main_screen = SDL_CreateWindow("Atlast SDL",
                     SDL_WINDOWPOS_UNDEFINED,
                     SDL_WINDOWPOS_UNDEFINED,
                     width, height,
                     videoflags)) == NULL) {
    fprintf(stderr, "Couldn't set %dx%d video mode: %s\n", width, height, SDL_GetError());
    exit(2);
  }

  SDL_SetWindowTitle (main_screen, title);
  SDL_SetWindowInputFocus(main_screen);

  if ((main_renderer = SDL_CreateRenderer(main_screen, -1, 0)) == NULL) {
    fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
    exit(2);
  }

  /* Sound initialisation */
  if (SDL_Init(SDL_INIT_AUDIO) < 0) {
    fprintf(stderr,
            "\nWarning: I could not initialize audio!\n"
            "The Simple DirectMedia error that occured was:\n"
            "%s\n\n", SDL_GetError());
  }

  /* if (Mix_OpenAudio(22050, AUDIO_S16, 2, 512) < 0) */
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
    fprintf(stderr,
            "\nWarning: I could not set up audio for 44100 Hz "
            "16-bit stereo.\n"
            "The Simple DirectMedia error that occured was:\n"
            "%s\n\n", SDL_GetError());
  }
  Mix_Volume(-1, 20 * MIX_MAX_VOLUME / 100);

  // allocate mixing channels
  Mix_AllocateChannels(100);

  for (i = 0; i < MAX_KEY; i++) {
    tab_keys[i] = 0;
  }

  for (i = 0; i < 3; i++) {
    tab_mouse_button[i] = 0;
  }
}

void sdl_close (struct state *s) {
  SDL_DestroyRenderer(main_renderer);
  SDL_DestroyWindow(main_screen);

  if ( !(font == NULL)) {
    TTF_CloseFont(font);
    font = NULL;
  }

  Mix_CloseAudio();

  IMG_Quit();
  SDL_Quit();
}

void sdl_delay (struct state *s) {
  POP1();
  SDL_Delay(p1);
}

void sdl_show_cursor (struct state *s) {
  SDL_ShowCursor (SDL_ENABLE);
}

void sdl_hide_cursor (struct state *s) {
  SDL_ShowCursor (SDL_DISABLE);
}

// /*===========================================================
//  *   Input functions
//  ============================================================*/
void set_tab_keys_from_keysym(int keysym, int value) {
  switch(keysym) {
    case SDLK_ESCAPE:        tab_keys[27] = value; break;
    case SDLK_UP:            tab_keys[28] = value; break;
    case SDLK_DOWN:          tab_keys[29] = value; break;
    case SDLK_LEFT:          tab_keys[30] = value; break;
    case SDLK_RIGHT:         tab_keys[31] = value; break;
    case SDLK_RETURN:        tab_keys[32] = value; break;
    case SDLK_SPACE:         tab_keys[33] = value; break;
    case SDLK_TAB:           tab_keys[34] = value; break;
    case SDLK_LCTRL:         tab_keys[35] = value; break;
    case SDLK_KP_DIVIDE:     tab_keys[36] = value; break;
    case SDLK_KP_MULTIPLY:   tab_keys[37] = value; break;
    case SDLK_KP_MINUS:      tab_keys[38] = value; break;
    case SDLK_KP_PLUS:       tab_keys[39] = value; break;
    case SDLK_KP_ENTER:      tab_keys[40] = value; break;
    case SDLK_KP_1:          tab_keys[41] = value; break;
    case SDLK_KP_2:          tab_keys[42] = value; break;
    case SDLK_KP_3:          tab_keys[43] = value; break;
    case SDLK_KP_4:          tab_keys[44] = value; break;
    case SDLK_KP_5:          tab_keys[45] = value; break;
    case SDLK_KP_6:          tab_keys[46] = value; break;
    case SDLK_KP_7:          tab_keys[47] = value; break;
    case SDLK_KP_8:          tab_keys[48] = value; break;
    case SDLK_KP_9:          tab_keys[49] = value; break;
    case SDLK_KP_0:          tab_keys[50] = value; break;
    case SDLK_KP_PERIOD:     tab_keys[51] = value; break;
  }
}

void analyze_event (SDL_Event * event) {
  switch( event->type ) {
    case SDL_QUIT : tab_keys[100] = 1; break;

    case SDL_MOUSEBUTTONDOWN :
      if (event->button.button == SDL_BUTTON_LEFT) tab_mouse_button[0] = 1;
      if (event->button.button == SDL_BUTTON_MIDDLE) tab_mouse_button[1] = 1;
      if (event->button.button == SDL_BUTTON_RIGHT) tab_mouse_button[2] = 1;
      break;

    case SDL_MOUSEBUTTONUP :
      if (event->button.button == SDL_BUTTON_LEFT) tab_mouse_button[0] = 0;
      if (event->button.button == SDL_BUTTON_MIDDLE) tab_mouse_button[1] = 0;
      if (event->button.button == SDL_BUTTON_RIGHT) tab_mouse_button[2] = 0;
      break;

    case SDL_KEYDOWN :
      printf ("In C- Key press: %s %d 0x%02X\n", SDL_GetKeyName(event->key.keysym.sym), event->key.keysym.sym, event->key.keysym.scancode);

      if (event->key.keysym.sym >= SDLK_a && event->key.keysym.sym <= SDLK_z) {
        tab_keys[event->key.keysym.sym - SDLK_a] = 1;
      } else {
        set_tab_keys_from_keysym(event->key.keysym.sym, 1);
      }
      break;

    case SDL_KEYUP :
      printf ("In C- Key release: %s %d 0x%02X\n", SDL_GetKeyName(event->key.keysym.sym), event->key.keysym.sym, event->key.keysym.scancode);
      if (event->key.keysym.sym >= SDLK_a && event->key.keysym.sym <= SDLK_z) {
        tab_keys[event->key.keysym.sym - SDLK_a] = 0;
      } else {
        set_tab_keys_from_keysym(event->key.keysym.sym, 0);
      }
      break;

    default:
      break;
  }
}

void sdl_poll_event (struct state *s) {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    analyze_event(&event);
  }

  SDL_GetMouseState (&sdl_mouse_x, &sdl_mouse_y);
}

void sdl_present (struct state *s) {
  SDL_RenderPresent(main_renderer);
}


void sdl_key_press (struct state *s) {
  POP1();
  PUSH1(tab_keys[p1]);
}

void sdl_mouse_button (struct state *s) {
  POP1();
  PUSH1(tab_mouse_button[p1 - 1]);
}

void sdl_get_mouse_x (struct state *s) {
  PUSH1(sdl_mouse_x);
}

void sdl_get_mouse_y (struct state *s) {
  PUSH1(sdl_mouse_y);
}

void sdl_background (struct state *s) {
  POP3();
  SDL_SetRenderDrawColor(main_renderer, (int)p3, (int)p2, (int)p1, 255);
  SDL_RenderClear(main_renderer);
}

// /*===========================================================
//  *   Image functions
//  ============================================================*/
void
sdl_load_image (struct state *s) {
  POP1();
  char * filename = CFSTRING2C(p1);
  SDL_Surface * surf = NULL;

  printf ("In C - Opening: %s\n", filename);

  surf = IMG_Load (filename);
  if (surf == NULL) {
    fprintf(stderr, "Can't load image %s - %s\n", filename, SDL_GetError());
    PUSH1(-1);
    return;
  }

  SDL_Texture *texture = SDL_CreateTextureFromSurface(main_renderer, surf);
  SDL_FreeSurface(surf);

  if (texture == NULL) {
    fprintf(stderr, "Can't load image %s - %s\n", filename, SDL_GetError());
    PUSH1(-1);
    return;
  }

  PUSH1((cell) texture);
}

void
sdl_put_image (struct state *s) {
  POP3();
  int x = p2, y = p1;
  SDL_Texture * texture = (SDL_Texture *) p3;
  SDL_Rect dstrect;
  int w, h;

  SDL_QueryTexture(texture, NULL, NULL, &w, &h);
  dstrect.x = x; dstrect.y = y;
  dstrect.w = w; dstrect.h = h;
  SDL_RenderCopy(main_renderer, texture, NULL, &dstrect);
}


// /*===========================================================
//  *   Text functions
//  ============================================================*/
void
sdl_open_font (struct state *s) {
  POP2();
  char * filename = CFSTRING2C(p2);
  int ptsize = p1;

  if ( !(font == NULL)) {
    TTF_CloseFont(font);
  }

  font = TTF_OpenFont(filename, ptsize);
  if ( font == NULL ) {
    fprintf(stderr, "Couldn't load %d pt font from %s: %s\n",
        ptsize, filename, SDL_GetError());
    PUSH1(-1);
    return;
  }
  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);

  PUSH1(0);
}

void
sdl_put_text (struct state *s) {
  POP4();
  char * string = CFSTRING2C(p4);
  int x = p3, y = p2;
  int forecol = p1;

  SDL_Color fg;
  /* SDL_Color bg; */
  SDL_Surface *text;
  SDL_Rect dstrect;
  SDL_Texture *texture;

  fg.r = (forecol >> 16) & 0xff;
  fg.g = (forecol >> 8) & 0xff;
  fg.b = forecol & 0xff;

  text = TTF_RenderUTF8_Solid(font, string, fg);

  if ( text != NULL ) {
    dstrect.x = x;
    dstrect.y = y;
    dstrect.w = text->w;
    dstrect.h = text->h;
    texture = SDL_CreateTextureFromSurface(main_renderer, text);
    SDL_RenderCopy(main_renderer, texture, NULL, &dstrect);
    SDL_FreeSurface(text);
  }
}

void
sdl_text_size (struct state *s) {
  POP1();
  char * string = CFSTRING2C(p1);

  SDL_Surface *text;
  SDL_Color fg;

  text = TTF_RenderUTF8_Solid(font, string, fg);

  if ( text != NULL ) {
    PUSH2(text->w, text->h);
    SDL_FreeSurface(text);
  } else {
    PUSH2(-1, -1);
  }
}

//,-----
//| Sound part
//`-----
void
sdl_play_sound(struct state *s) {
  POP1();
  Mix_PlayChannel(-1, (Mix_Chunk *) p1, 0);
}

void
sdl_halt_sound(struct state *s) {
   Mix_HaltChannel(-1);
 }

void
sdl_set_sound_volume(struct state *s) {
  POP1();
  Mix_Volume(-1, p1 * MIX_MAX_VOLUME / 100);
}

void
sdl_load_sound (struct state *s) {
  POP1();
  char * filename = CFSTRING2C(p1);

  const Mix_Chunk * snd = Mix_LoadWAV(filename);
  if (snd == NULL) {
    fprintf(stderr,
        "\nError: I could not load the sound file:\n"
        "%s\n"
        "The Simple DirectMedia error that occured was:\n"
            "%s\n\n", filename, SDL_GetError());
    PUSH1(-1);
    return;
  }

  PUSH1((cell) snd);
}

void
sdl_get_ticks (struct state *s) {
  PUSH1(SDL_GetTicks ());
}

void
require_sdl_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, SDL_INIT_HASH,              ENTRY_NAME("sdl/init"), sdl_init);
  define_primitive_extension(state, SDL_CLOSE_HASH,             ENTRY_NAME("sdl/close"), sdl_close);
  define_primitive_extension(state, SDL_DELAY_HASH,             ENTRY_NAME("sdl/delay"), sdl_delay);
  define_primitive_extension(state, SDL_SHOW_CURSOR_HASH,       ENTRY_NAME("sdl/show-cursor"), sdl_show_cursor);
  define_primitive_extension(state, SDL_HIDE_CURSOR_HASH,       ENTRY_NAME("sdl/hide-cursor"), sdl_hide_cursor);
  define_primitive_extension(state, SDL_POLL_EVENT_HASH,        ENTRY_NAME("sdl/poll-event"), sdl_poll_event);
  define_primitive_extension(state, SDL_PRESENT_HASH,           ENTRY_NAME("sdl/present"), sdl_present);
  define_primitive_extension(state, SDL_KEY_PRESS_HASH,         ENTRY_NAME("sdl/key?"), sdl_key_press);
  define_primitive_extension(state, SDL_MOUSE_BUTTON_HASH,      ENTRY_NAME("sdl/button?"), sdl_mouse_button);
  define_primitive_extension(state, SDL_GET_MOUSE_X_HASH,       ENTRY_NAME("sdl/mouse-x@"), sdl_get_mouse_x);
  define_primitive_extension(state, SDL_GET_MOUSE_Y_HASH,       ENTRY_NAME("sdl/mouse-y@"), sdl_get_mouse_y);
  define_primitive_extension(state, SDL_BACKGROUND_HASH,        ENTRY_NAME("sdl/background"), sdl_background);

  define_primitive_extension(state, SDL_OPEN_FONT_HASH,         ENTRY_NAME("sdl/open-font"), sdl_open_font);
  define_primitive_extension(state, SDL_PUT_TEXT_HASH,          ENTRY_NAME("sdl/put-text"), sdl_put_text);
  define_primitive_extension(state, SDL_GET_TEXT_SIZE_HASH,     ENTRY_NAME("sdl/text-size@"), sdl_text_size);

  define_primitive_extension(state, SDL_LOAD_IMAGE_HASH,        ENTRY_NAME("sdl/load-image"), sdl_load_image);
  define_primitive_extension(state, SDL_PUT_IMAGE_HASH,         ENTRY_NAME("sdl/put-image"), sdl_put_image);

  define_primitive_extension(state, SDL_PLAY_SOUND_HASH,        ENTRY_NAME("sdl/play-sound"), sdl_play_sound);
  define_primitive_extension(state, SDL_HALT_SOUND_HASH,        ENTRY_NAME("sdl/halt-sound"), sdl_halt_sound);
  define_primitive_extension(state, SDL_VOLUME_HASH,            ENTRY_NAME("sdl/volume!"), sdl_set_sound_volume);
  define_primitive_extension(state, SDL_LOAD_SOUND_HASH,        ENTRY_NAME("sdl/load-sound"), sdl_load_sound);

  define_primitive_extension(state, SDL_GET_TICKS_HASH,         ENTRY_NAME("sdl/ticks@"), sdl_get_ticks);

  initialized = 1;
}

#endif /* __SDL */
