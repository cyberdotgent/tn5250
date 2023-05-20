//
// Created by Yvan Janssens on 19/05/2023.
//

#define _TN5250_TERMINAL_PRIVATE_DEFINED
#include <SDL.h>
#include <SDL_ttf.h>
#include <pthread.h>
#include "sdlterm.h"

struct _Tn5250TerminalPrivate {
    int version;
    int glyph_h;
    int glyph_w;
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *terminalChars[25][80];
    SDL_Rect terminalCharsRect[25][80];
    TTF_Font *font;
} ;

SDL_Color textColor = { 0x00, 0xFF, 0x00, 0xFF };
SDL_Color textBackgroundColor = { 0x00, 0x00, 0x00, 0xFF };

Tn5250Terminal *tn5250_sdl_terminal_new() {
    printf("new sdl5250 term\n");

    Tn5250Terminal *r = tn5250_new(Tn5250Terminal, 1);
    int x = 640, y = 480;


    if (r == NULL)
        return NULL;

    r->data = tn5250_new(struct _Tn5250TerminalPrivate, 1);
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

    // calculate screen size based off font glyph
    r->data->font = TTF_OpenFont("3270-Regular.ttf", 18);
    if(!r->data->font) {
        printf("Unable to load font: '3270-Regular.ttf'!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }




    SDL_Surface *fontRenderTestSurface = TTF_RenderText_Shaded(r->data->font, "0", textColor, textBackgroundColor);
    if(!fontRenderTestSurface) {
        printf("Unable to render text surface!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }
    r->data->glyph_h = fontRenderTestSurface->h;
    r->data->glyph_w = fontRenderTestSurface->w;
    x = r->data->glyph_w * SDL_TERMINAL_W;
    y = r->data->glyph_h * SDL_TERMINAL_H + 1;



    r->data->window = SDL_CreateWindow( "sdl5250",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           x, y, SDL_WINDOW_SHOWN );

    r->data->renderer = SDL_CreateRenderer( r->data->window, -1, SDL_RENDERER_ACCELERATED );


    for (int char_y = 0; char_y < SDL_TERMINAL_H; char_y++) {
        for (int char_x = 0; char_x < SDL_TERMINAL_W; char_x++) {
            SDL_Surface *textSurface = TTF_RenderText_Shaded(r->data->font, "X", textColor, textBackgroundColor);
            if(!textSurface) {
                printf("Unable to render text surface!\n"
                       "SDL2_ttf Error: %s\n", TTF_GetError());
            } else {
                // Create texture from surface pixels
                r->data->terminalChars[char_y][char_x] = SDL_CreateTextureFromSurface(r->data->renderer, textSurface);
                if(!r->data->terminalChars[char_y][char_x]) {
                    printf("Unable to create texture from rendered text!\n"
                           "SDL2 Error: %s\n", SDL_GetError());
                    return 0;
                }

                // Get text dimensions
                r->data->terminalCharsRect[char_y][char_x].w = textSurface->w;
                r->data->terminalCharsRect[char_y][char_x].h = textSurface->h;
                r->data->terminalCharsRect[char_y][char_x].x = char_x * r->data->glyph_w;
                r->data->terminalCharsRect[char_y][char_x].y = char_y * r->data->glyph_h;

                SDL_FreeSurface(textSurface);
            }
        }
    }


    printf("new sdl5250 term done\n");

    return r;
}

void sdl_terminal_init(Tn5250Terminal *This) {
    printf("sdl_terminal_init\n");
    printf("sdl_terminal_init done\n");
}

// terminate sdlterm
void sdl_terminal_term(Tn5250Terminal /*@unused@*/ *This) {
    printf("sdl_terminal_term\n");
    printf("sdl_terminal_term done\n");
}

void sdl_terminal_destroy(Tn5250Terminal *This) {
    SDL_DestroyRenderer( This->data->renderer );
    SDL_DestroyWindow( This->data->window );
    SDL_Quit();
}

int sdl_terminal_width(Tn5250Terminal /*@unused@*/ *This) {
    printf("sdl_terminal_width\n");
    return SDL_TERMINAL_W + 1;
}

int sdl_terminal_height(Tn5250Terminal /*@unused@*/ *This) {
    printf("sdl_terminal_height\n");
    return SDL_TERMINAL_H + 1;
}

int sdl_terminal_flags(Tn5250Terminal /*@unused@*/ *This) {
    int f = 0;
    printf("sdl_terminal_flags\n");
    printf("sdl_terminal_flags done\n");
    return f;
}

void sdl_terminal_update(Tn5250Terminal *This, Tn5250Display *display) {
    printf("sdl_term_update\n");

    printf("curs x: %d y: %d\n", tn5250_display_cursor_x(display), tn5250_display_cursor_y(display) );

    for (int char_y = 0; char_y < SDL_TERMINAL_H; char_y++) {
        for (int char_x = 0; char_x < SDL_TERMINAL_W; char_x++) {
            char ebcdic_terminal_char = tn5250_display_char_at(display, char_y, char_x);
            char render_terminal_char = tn5250_char_map_to_local(tn5250_display_char_map (display), ebcdic_terminal_char);
            char render_buffer[10];

            if (ebcdic_terminal_char != 0x00)
                printf("x: %d y: %d ebcdic: %02X a: '%c'\n", char_x, char_y, ebcdic_terminal_char, render_terminal_char);

            if (render_terminal_char == 0x00)
                render_terminal_char = ' ';

            snprintf(render_buffer, 10, "%c", render_terminal_char );

            SDL_Surface *textSurface;
            if (sdl_terminal_is_ruler(This, display, char_x, char_y)) {
                // highlight pls
                textSurface = TTF_RenderText_Shaded(This->data->font, render_buffer, textBackgroundColor, textColor);
            } else {
                textSurface = TTF_RenderText_Shaded(This->data->font, render_buffer, textColor, textBackgroundColor);
            }
            if(!textSurface) {
                printf("Unable to render text surface!\n"
                       "SDL2_ttf Error: %s\n", TTF_GetError());
            } else {
                // Create texture from surface pixels
                This->data->terminalChars[char_y][char_x] = SDL_CreateTextureFromSurface(This->data->renderer, textSurface);
                if(!This->data->terminalChars[char_y][char_x]) {
                    printf("Unable to create texture from rendered text!\n"
                           "SDL2 Error: %s\n", SDL_GetError());
                    return;
                }

                // Get text dimensions
                This->data->terminalCharsRect[char_y][char_x].w = textSurface->w;
                This->data->terminalCharsRect[char_y][char_x].h = textSurface->h;
                This->data->terminalCharsRect[char_y][char_x].x = char_x * This->data->glyph_w;
                This->data->terminalCharsRect[char_y][char_x].y = char_y * This->data->glyph_h;

                SDL_FreeSurface(textSurface);
            }
        }
    }
}

int sdl_terminal_is_ruler(Tn5250Terminal *This,
                                    Tn5250Display *display, int x, int y) {

    if (x == tn5250_display_cursor_x(display) && y == tn5250_display_cursor_y(display)) {
        return 1;
    }

    return 0;
}

void sdl_terminal_update_indicators(Tn5250Terminal /*@unused@*/ *This, Tn5250Display *display) {
    printf("sdl_terminal_update_indicators\n");
    printf("sdl_terminal_update_indicators done\n");
}


int sdl_terminal_waitevent(Tn5250Terminal *This) {
    fd_set fdr;
    int result = 0;
    int sm;


    FD_ZERO(&fdr);

    FD_SET(0, &fdr);
    sm = 1;
    if (This->conn_fd >= 0) {
        FD_SET(This->conn_fd, &fdr);
        sm = This->conn_fd + 1;
    }

    select(sm, &fdr, NULL, NULL, NULL);

    if (FD_ISSET(0, &fdr))
        result |= TN5250_TERMINAL_EVENT_KEY;

    if (This->conn_fd >= 0 && FD_ISSET(This->conn_fd, &fdr))
        result |= TN5250_TERMINAL_EVENT_DATA;

    return result;
}

void sdl_terminal_beep(Tn5250Terminal *This) {
    printf("sdl_terminal_beep!\n");
}

int sdl_terminal_getkey(Tn5250Terminal * This) {
    printf("sdl_terminal_getkey\n");
    printf("sdl_terminal_getkey done\n");
    return 0;
}

void sdl_terminal_loop(Tn5250Terminal * This) {
    SDL_bool quit = SDL_FALSE;

    while ( !quit ) {
        quit = sdl_terminal_draw(This);
    }
}

SDL_bool sdl_terminal_draw(Tn5250Terminal * This) {
    SDL_Event e;

    // Wait indefinitely for the next available event
    SDL_WaitEvent(&e);

    // User requests quit
    if(e.type == SDL_QUIT)
    {
        return SDL_TRUE;
    }

    // draw black background colour
    SDL_SetRenderDrawColor(   This->data->renderer, 0x00, 0x00, 0x00, 0xFF);

    // Clear screen
    SDL_RenderClear(This->data->renderer);

    // Set FG colour to green
    SDL_SetRenderDrawColor(This->data->renderer, 0x00, 0xFF, 0x00, 0xFF);


    // Draw text
    for (int char_y = 0; char_y < SDL_TERMINAL_H; char_y++) {
        for (int char_x = 0; char_x < SDL_TERMINAL_W; char_x++) {
            SDL_RenderCopy(This->data->renderer, This->data->terminalChars[char_y][char_x], NULL,
                           &This->data->terminalCharsRect[char_y][char_x]);
        }
    }

    // Update screen
    SDL_RenderPresent(This->data->renderer);

    return SDL_FALSE;
}