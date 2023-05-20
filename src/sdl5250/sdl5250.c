//
// Created by Yvan Janssens on 19/05/2023.
//


#include <SDL.h>
#include <SDL_ttf.h>


#include <stdlib.h>
#include <string.h>

#include <pthread.h>

#include "tn5250-private.h"
#include "sdlterm.h"


Tn5250Session *sess = NULL;
Tn5250Stream *stream = NULL;
Tn5250Terminal *term = NULL;
Tn5250Display *display = NULL;
Tn5250Config *config = NULL;

int main(int argc, char *argv[] ) {

    SDL_Init( SDL_INIT_VIDEO | SDL_INIT_EVENTS );
    TTF_Init();

    config = tn5250_config_new ();
    if (tn5250_config_load_default (config) == -1) {
        tn5250_config_unref(config);
        exit (1);
    }
    if (tn5250_config_parse_argv (config, argc, argv) == -1) {
        tn5250_config_unref(config);
    }

    tn5250_config_set(config, "host", "backpain.qseco.fr");

    stream = tn5250_stream_open (tn5250_config_get (config, "host"), config);
    printf("Connecting... ");
    if (stream == NULL) {
        printf("Unable to open communications stream!");
        return errno;
    }
    printf("connected!\n");

/* set up display terminal */

    display = tn5250_display_new ();
    display->sign_key_hack = 0;
    if (tn5250_display_config (display, config) == -1)
        return errno;

    term = tn5250_sdl_terminal_new();
    if (term == NULL)
        return errno;
    if (tn5250_terminal_config (term, config) == -1)
        return errno;


    tn5250_terminal_init(term);
    tn5250_display_set_terminal(display, term);


    sess = tn5250_session_new();
    tn5250_display_set_session(display, sess);
    term->conn_fd = tn5250_stream_socket_handle(stream);
    tn5250_session_set_stream(sess, stream);

    if (tn5250_session_config (sess, config) == -1)
        return errno;

    pthread_t * session_thread;
    pthread_create(session_thread, NULL, (void *(*)(void *)) tn5250_session_main_loop, sess);

    sdl_terminal_loop(term);

}