//
// Created by Yvan Janssens on 19/05/2023.
//

#define _TN5250_TERMINAL_PRIVATE_DEFINED
#include "sdlterm.h"

Tn5250Terminal *tn5250_sdl_terminal_new() {
    Tn5250Terminal *r = tn5250_new(Tn5250Terminal, 1);
    if (r == NULL)
        return NULL;

    r->data = tn5250_new(struct _SDL5250TerminalPrivate, 1);
    if (r->data == NULL) {
        free(r);
        return NULL;
    }

    r->conn_fd = -1;
    r->init = sdl_terminal_init;
    r->term = sdl_terminal_term;
    r->destroy = sdl_terminal_destroy;
    r->width = sdl_terminal_width;
    r->height = sdl_terminal_height;
    r->flags = sdl_terminal_flags;
    r->update = sdl_terminal_update;
    r->update_indicators = sdl_terminal_update_indicators;
    r->waitevent = sdl_terminal_waitevent;
    r->getkey = sdl_terminal_getkey;
    r->beep = sdl_terminal_beep;
    r->config = NULL;
    return r;
}

void sdl_terminal_init(Tn5250Terminal *This) {

}

// terminate sdlterm
void sdl_terminal_term(Tn5250Terminal /*@unused@*/ *This) {

}

void sdl_terminal_destroy(Tn5250Terminal *This) {

}

int sdl_terminal_width(Tn5250Terminal /*@unused@*/ *This) {
    return 0;
}

int sdl_terminal_height(Tn5250Terminal /*@unused@*/ *This) {
    return 0;
}

int sdl_terminal_flags(Tn5250Terminal /*@unused@*/ *This) {
    int f = 0;
    return f;
}

void sdl_terminal_update(Tn5250Terminal *This, Tn5250Display *display) {

}

int sdl_terminal_is_ruler(Tn5250Terminal *This,
                                    Tn5250Display *display, int x, int y) {

    if (x == tn5250_display_cursor_x(display)) {
        return 1;
    }
    if (y == tn5250_display_cursor_y(display)) {
        return 1;
    }

    return 0;
}

void sdl_terminal_update_indicators(Tn5250Terminal /*@unused@*/ *This, Tn5250Display *display) {

}


int sdl_terminal_waitevent(Tn5250Terminal *This) {
    return 0;
}

void sdl_terminal_beep(Tn5250Terminal *This) {
    TN5250_LOG (("SDL: beep\n"));
}

int sdl_terminal_getkey(Tn5250Terminal * This) {
    return 0;
}