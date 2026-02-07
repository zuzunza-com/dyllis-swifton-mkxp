/*
** emscripten-bridge.c
**
** Emscripten Bridge for Palcon-RGSS
** Provides JavaScript interface for WASM module
**
** Copyright (C) 2025 Palcon Team
*/

#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/fetch.h>
#include <string.h>
#include <stdlib.h>

// Forward declarations
extern int rgss_init(int width, int height);
extern void rgss_cleanup(void);
extern int rgss_load_game_data(const void* data, int size);
extern int rgss_update(void);
extern void* rgss_get_render_buffer(void);
extern void rgss_handle_input(int key_code, int pressed);

/**
 * Initialize RGSS engine
 */
EMSCRIPTEN_KEEPALIVE
int palcon_rgss_init(int width, int height)
{
    return rgss_init(width, height);
}

/**
 * Cleanup RGSS engine
 */
EMSCRIPTEN_KEEPALIVE
void palcon_rgss_cleanup(void)
{
    rgss_cleanup();
}

/**
 * Load game data from URL
 */
EMSCRIPTEN_KEEPALIVE
void palcon_rgss_load_game_from_url(const char* url)
{
    emscripten_fetch_attr_t attr;
    emscripten_fetch_attr_init(&attr);
    strcpy(attr.requestMethod, "GET");
    attr.attributes = EMSCRIPTEN_FETCH_LOAD_TO_MEMORY;
    
    attr.onsuccess = [](emscripten_fetch_t *fetch) {
        if (fetch->data && fetch->numBytes > 0) {
            rgss_load_game_data(fetch->data, fetch->numBytes);
        }
        emscripten_fetch_close(fetch);
    };
    
    attr.onerror = [](emscripten_fetch_t *fetch) {
        printf("Failed to load game from URL: %s\n", fetch->url);
        emscripten_fetch_close(fetch);
    };
    
    emscripten_fetch(&attr, url);
}

/**
 * Load game data from ArrayBuffer
 */
EMSCRIPTEN_KEEPALIVE
int palcon_rgss_load_game_data(const void* data, int size)
{
    return rgss_load_game_data(data, size);
}

/**
 * Update game frame
 */
EMSCRIPTEN_KEEPALIVE
int palcon_rgss_update(void)
{
    return rgss_update();
}

/**
 * Get render buffer pointer
 */
EMSCRIPTEN_KEEPALIVE
void* palcon_rgss_get_render_buffer(void)
{
    return rgss_get_render_buffer();
}

/**
 * Handle input event
 */
EMSCRIPTEN_KEEPALIVE
void palcon_rgss_handle_input(int key_code, int pressed)
{
    rgss_handle_input(key_code, pressed);
}

/**
 * Setup canvas for rendering
 */
EMSCRIPTEN_KEEPALIVE
void palcon_rgss_setup_canvas(const char* canvas_id, int width, int height)
{
    EM_ASM({
        var canvas = document.getElementById(UTF8ToString($0));
        if (canvas) {
            canvas.width = $1;
            canvas.height = $2;
        }
    }, canvas_id, width, height);
}
