//
// Created by Yvan Janssens on 19/05/2023.
//

#ifndef TN5250_SDLTERM_H
#define TN5250_SDLTERM_H

#include "tn5250-private.h"

#define SDL_TERMINAL_H 24
#define SDL_TERMINAL_W 80



extern Tn5250Terminal /*@null@*/ /*@only@*/ *tn5250_sdl_terminal_new(void);

static void sdl_terminal_init(Tn5250Terminal *This) /*@modifies This@*/;

static void sdl_terminal_term(Tn5250Terminal *This) /*@modifies This@*/;

static void sdl_terminal_destroy(Tn5250Terminal /*@only@*/ *This);

static int sdl_terminal_width(Tn5250Terminal *This);

static int sdl_terminal_height(Tn5250Terminal *This);

static int sdl_terminal_flags(Tn5250Terminal *This);

static void sdl_terminal_update(Tn5250Terminal *This,
                                   Tn5250Display *display) /*@modifies This@*/;

static void sdl_terminal_update_indicators(Tn5250Terminal *This,
                                              Tn5250Display *display) /*@modifies This@*/;

static int sdl_terminal_waitevent(Tn5250Terminal *This) /*@modifies This@*/;

static int sdl_terminal_getkey(Tn5250Terminal *This) /*@modifies This@*/;

static int sdl_terminal_get_esc_key(Tn5250Terminal *This, int is_esc) /*@modifies This@*/;

static void sdl_terminal_beep(Tn5250Terminal *This);

static int sdl_terminal_is_ruler(Tn5250Terminal *This, Tn5250Display *display, int x, int y);

void sdl_terminal_loop(Tn5250Terminal * This);

SDL_bool sdl_terminal_draw(Tn5250Terminal * This);

#endif //TN5250_SDLTERM_H
