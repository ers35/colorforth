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
#define MAX_IMAGE 1000
#define MAX_KEY 300
#define MAX_SOUND 300

int sdl_done = 0;
///* int sdl_mouse_button_1 = 0; */
///* int sdl_mouse_button_2 = 0; */
///* int sdl_mouse_button_3 = 0; */
///* int sdl_mouse_button_4 = 0; */
///* int sdl_mouse_button_5 = 0; */
int sdl_mouse_x = 0;
int sdl_mouse_y = 0;

int tab_mouse_button[3];
int tab_keys[MAX_KEY];

int image_count = -1;
SDL_Texture * tab_images[MAX_IMAGE];

int sound_count = -1;
Mix_Chunk * tab_sounds[MAX_SOUND];

SDL_Window * main_screen;
SDL_Renderer *main_renderer;
static TTF_Font * font = NULL;


// /*===========================================================
//  *		Open and close functions
//  ============================================================*/
void
sdl_init(struct state *s) {
//void sdl_init (int width, int height, int fullscreen) {
  POP3();
  int width = p3;
  int height = p2;
  int fullscreen = p1;

	Uint32 videoflags = 0;
	int i;

	sdl_done = 0;

	char *title = "TODO: Move this into a word";

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
	if (SDL_Init(SDL_INIT_AUDIO) < 0)
		{
			fprintf(stderr,
					"\nWarning: I could not initialize audio!\n"
					"The Simple DirectMedia error that occured was:\n"
					"%s\n\n", SDL_GetError());
		}

	/* if (Mix_OpenAudio(22050, AUDIO_S16, 2, 512) < 0) */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
		{
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

void
require_sdl_fn(struct state *state)
{
  if (initialized) return;

  define_primitive_extension(state, SDL_INIT_HASH,        ENTRY_NAME("sdl/init"), sdl_init);
  define_primitive_extension(state, SDL_CLOSE_HASH,       ENTRY_NAME("sdl/close"), sdl_close);

  initialized = 1;
}

#endif /* __SDL */
