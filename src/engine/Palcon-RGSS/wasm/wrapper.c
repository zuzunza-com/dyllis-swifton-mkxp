/**
 * Palcon-RGSS C언어 Wrapper
 * 
 * WASM 빌드를 위한 C언어 인터페이스 래퍼입니다.
 * 이 파일은 RGSS 엔진의 C API를 JavaScript에서 호출 가능하도록 래핑합니다.
 */

#include <emscripten.h>
#include <stdlib.h>
#include <string.h>
#include "../src/binding/binding.h"
#include "../src/platform/win32-emscripten.h"
#include "../src/sharedstate.h"

// RGSS 엔진 상태
typedef struct {
    int width;
    int height;
    void* render_buffer;
    int initialized;
    ScriptBinding* binding;
    SharedState* sharedState;
} RGSSState;

static RGSSState g_rgss_state = {0};

/**
 * RGSS 엔진 초기화
 * 
 * @param width 화면 너비
 * @param height 화면 높이
 * @return 성공 시 0, 실패 시 -1
 */
EMSCRIPTEN_KEEPALIVE
int rgss_init(int width, int height) {
    if (g_rgss_state.initialized) {
        return -1; // 이미 초기화됨
    }

    g_rgss_state.width = width;
    g_rgss_state.height = height;
    
    // 렌더링 버퍼 할당 (RGBA8888)
    size_t buffer_size = width * height * 4;
    g_rgss_state.render_buffer = malloc(buffer_size);
    if (!g_rgss_state.render_buffer) {
        return -1; // 메모리 할당 실패
    }
    
    memset(g_rgss_state.render_buffer, 0, buffer_size);
    
    // Initialize SharedState (contains graphics, audio, input, filesystem)
    g_rgss_state.sharedState = new SharedState();
    
    // TASK: Initialize Ruby binding (MRI or mruby)
    // This should be done after SharedState is initialized
    // Ruby binding initialization happens when scripts are executed
    g_rgss_state.binding = scriptBinding;
    
    g_rgss_state.initialized = 1;
    
    return 0;
}

/**
 * RGSS 엔진 종료
 */
EMSCRIPTEN_KEEPALIVE
void rgss_cleanup() {
    if (!g_rgss_state.initialized) {
        return;
    }
    
    // Cleanup SharedState
    if (g_rgss_state.sharedState) {
        delete g_rgss_state.sharedState;
        g_rgss_state.sharedState = NULL;
    }
    
    // TASK: Cleanup Ruby binding
    // Ruby binding cleanup happens automatically when Ruby interpreter is cleaned up
    g_rgss_state.binding = NULL;
    
    if (g_rgss_state.render_buffer) {
        free(g_rgss_state.render_buffer);
        g_rgss_state.render_buffer = NULL;
    }
    
    g_rgss_state.initialized = 0;
}

/**
 * RGSS 스크립트 실행
 * 
 * @param script_path 스크립트 파일 경로
 * @return 성공 시 0, 실패 시 -1
 */
EMSCRIPTEN_KEEPALIVE
int rgss_execute_script(const char* script_path) {
    if (!g_rgss_state.initialized) {
        return -1; // 엔진이 초기화되지 않음
    }
    
    // TASK: Load and execute RGSS script
    if (g_rgss_state.binding && g_rgss_state.binding->execute) {
        g_rgss_state.binding->execute();
    }
    
    return 0;
}

/**
 * RGSS 게임 데이터 로드
 * 
 * @param data 게임 데이터 포인터
 * @param size 데이터 크기
 * @return 성공 시 0, 실패 시 -1
 */
EMSCRIPTEN_KEEPALIVE
int rgss_load_game_data(const void* data, int size) {
    if (!g_rgss_state.initialized) {
        return -1;
    }
    
    // TASK: Parse game data (ZIP file)
    // TASK: Extract scripts and resources
    // TASK: Load into filesystem
    if (g_rgss_state.sharedState) {
        // Mount ZIP file to filesystem
        // For now, assume data is already mounted
        // Future: Extract ZIP contents and mount to /game/data
        g_rgss_state.sharedState->filesystem().init("/game/data");
    }
    
    return 0;
}

/**
 * RGSS 프레임 업데이트
 * 
 * @return 다음 프레임까지 대기 시간 (밀리초)
 */
EMSCRIPTEN_KEEPALIVE
int rgss_update() {
    if (!g_rgss_state.initialized) {
        return -1;
    }
    
    // Update game systems
    if (g_rgss_state.sharedState) {
        g_rgss_state.sharedState->graphics().update();
        g_rgss_state.sharedState->audio().update();
        g_rgss_state.sharedState->input().update();
    }
    
    // TASK: Update game logic (script execution)
    // Script execution is handled by Ruby binding's update loop
    // Graphics, audio, and input updates are done above
    
    return 16; // 60 FPS (16ms)
}

/**
 * RGSS 렌더링 버퍼 가져오기
 * 
 * @return 렌더링 버퍼 포인터 (RGBA8888 형식)
 */
EMSCRIPTEN_KEEPALIVE
void* rgss_get_render_buffer() {
    return g_rgss_state.render_buffer;
}

/**
 * RGSS 입력 처리
 * 
 * @param key_code 키 코드
 * @param pressed 눌림 여부 (1: 눌림, 0: 떼어짐)
 */
EMSCRIPTEN_KEEPALIVE
void rgss_handle_input(int key_code, int pressed) {
    if (!g_rgss_state.initialized) {
        return;
    }
    
    // Update input state
    if (g_rgss_state.sharedState) {
        g_rgss_state.sharedState->input().setKeyState(key_code, pressed ? true : false);
    }
}
