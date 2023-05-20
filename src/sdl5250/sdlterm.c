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
    SDL_Renderer *renderer;
    SDL_Window *window;
    SDL_Texture *text;
    SDL_Rect textRect;
    pthread_t drawThread;
} ;

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
    TTF_Font *font = TTF_OpenFont("3270-Regular.ttf", 18);
    if(!font) {
        printf("Unable to load font: '3270-Regular.ttf'!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }

    SDL_Color textColor = { 0x00, 0xFF, 0x00, 0xFF };
    SDL_Color textBackgroundColor = { 0x00, 0x00, 0x00, 0xFF };


    SDL_Surface *fontRenderTestSurface = TTF_RenderText_Shaded(font, "0", textColor, textBackgroundColor);
    if(!fontRenderTestSurface) {
        printf("Unable to render text surface!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
        return NULL;
    }
    x = fontRenderTestSurface->w * 80;
    y = fontRenderTestSurface->h * 26;


    r->data->window = SDL_CreateWindow( "sdl5250",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           x, y, SDL_WINDOW_SHOWN );

    r->data->renderer = SDL_CreateRenderer( r->data->window, -1, SDL_RENDERER_ACCELERATED );



    SDL_Surface *textSurface = TTF_RenderText_Shaded(font, "01234567890123456789012345678901234567890123456789012345678901234567890123456789", textColor, textBackgroundColor);

    if(!textSurface) {
        printf("Unable to render text surface!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
    } else {
        // Create texture from surface pixels
        r->data->text = SDL_CreateTextureFromSurface(r->data->renderer, textSurface);
        if(!r->data->text) {
            printf("Unable to create texture from rendered text!\n"
                   "SDL2 Error: %s\n", SDL_GetError());
            return 0;
        }

        // Get text dimensions
        r->data->textRect.w = textSurface->w;
        r->data->textRect.h = textSurface->h;

        SDL_FreeSurface(textSurface);
    }

    r->data->textRect.x = 0;
    r->data->textRect.y = 0;
    printf("new sdl5250 term done\n");



    return r;
}

void sdl_terminal_init(Tn5250Terminal *This) {

}

// terminate sdlterm
void sdl_terminal_term(Tn5250Terminal /*@unused@*/ *This) {

}

void sdl_terminal_destroy(Tn5250Terminal *This) {
    SDL_DestroyRenderer( This->data->renderer );
    SDL_DestroyWindow( This->data->window );
    SDL_Quit();
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
    printf("sdl_term_update\n");
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
    SDL_RenderCopy(This->data->renderer, This->data->text, NULL, &This->data->textRect);

    // Update screen
    SDL_RenderPresent(This->data->renderer);

    return SDL_FALSE;
}