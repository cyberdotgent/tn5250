//
// Created by Yvan Janssens on 19/05/2023.
//


#include <SDL.h>
#include <SDL_ttf.h>


#include <stdlib.h>
#include <string.h>


int main( int argc, char *argv[] ) {


    int x = 640, y = 480;
    SDL_Color textColor           = { 0x00, 0xFF, 0x00, 0xFF };
    SDL_Color textBackgroundColor = { 0x00, 0x00, 0x00, 0xFF };


    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );
    TTF_Init();

    // calculate screen size based off font glyph
    TTF_Font *font = TTF_OpenFont("3270-Regular.ttf", 18);
    if(!font) {
        printf("Unable to load font: '3270-Regular.ttf'!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
        return 0;
    }
    SDL_Surface *fontRenderTestSurface = TTF_RenderText_Shaded(font, "0", textColor, textBackgroundColor);
    if(!fontRenderTestSurface) {
        printf("Unable to render text surface!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
        return -1;
    }
    x = fontRenderTestSurface->w * 80;
    y = fontRenderTestSurface->h * 26;


    SDL_Window *window = SDL_CreateWindow( "sdl5250",
                                           SDL_WINDOWPOS_UNDEFINED,
                                           SDL_WINDOWPOS_UNDEFINED,
                                           x, y, SDL_WINDOW_SHOWN );

    SDL_Renderer *renderer =
            SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );








    SDL_Texture *text = NULL;
    SDL_Rect textRect;

    SDL_Surface *textSurface = TTF_RenderText_Shaded(font, "01234567890123456789012345678901234567890123456789012345678901234567890123456789", textColor, textBackgroundColor);

    if(!textSurface) {
        printf("Unable to render text surface!\n"
               "SDL2_ttf Error: %s\n", TTF_GetError());
    } else {
        // Create texture from surface pixels
        text = SDL_CreateTextureFromSurface(renderer, textSurface);
        if(!text) {
            printf("Unable to create texture from rendered text!\n"
                   "SDL2 Error: %s\n", SDL_GetError());
            return 0;
        }

        // Get text dimensions
        textRect.w = textSurface->w;
        textRect.h = textSurface->h;

        SDL_FreeSurface(textSurface);
    }

    textRect.x = 0;
    textRect.y = 0;



    /* The quit flag.  We run the game loop until this flag becomes
     * true.
     */
    SDL_bool quit = SDL_FALSE;
    /* The event we're going to receive.  Initialised to all zeros.
     */
    SDL_Event e = { 0 };

    /* Here is a very simple game loop, with a soft framerate.  There is
     * a chance an individual frame will take longer, and this happens
     * predictably when the user presses the X button to close the
     * window, and the delay routine can oversleep because of operating
     * system scheduling.
     */
    while ( !quit ) {
        SDL_Event e;

        // Wait indefinitely for the next available event
        SDL_WaitEvent(&e);

        // User requests quit
        if(e.type == SDL_QUIT)
        {
            quit = SDL_TRUE;
        }

        // draw black background colour
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);

        // Clear screen
        SDL_RenderClear(renderer);

        // Set FG colour to green
        SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);


        // Draw text
        SDL_RenderCopy(renderer, text, NULL, &textRect);

        // Update screen
        SDL_RenderPresent(renderer);

    }

    /* Clean up after ourselves before we quit.
     */
    SDL_DestroyRenderer( renderer );
    SDL_DestroyWindow( window );
    SDL_Quit();
    return 0;
}
