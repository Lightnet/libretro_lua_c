#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <libretro.h>
#include <stdint.h>
#include <string.h>
#include "font.h"
#include "lib.h"
#include "module_libretro.h"

// Color constants from lib.c
#define COLOR_WHITE 0xFFFF
#define COLOR_RED   0xF800
#define COLOR_GREEN 0x07E0
#define COLOR_BLUE  0x001F

// Lua function: lr.draw_string(x, y, str, color)
static int lua_draw_string(lua_State *L) {
    if (lua_gettop(L) != 4) {
        lua_pushstring(L, "draw_string expects 4 arguments: x, y, string, color");
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    const char *str = luaL_checkstring(L, 3);
    uint16_t color = (uint16_t)luaL_checkinteger(L, 4);
    if (x < 0 || y < 0) {
        lua_pushstring(L, "x and y must be non-negative");
        lua_error(L);
        return 0;
    }
    if (!str) {
        lua_pushstring(L, "string must not be nil");
        lua_error(L);
        return 0;
    }
    draw_string(x, y, str, color);
    // if (log_cb) {
    //     log_cb(RETRO_LOG_INFO, "[LUA] draw_string called: (%d, %d, %s, 0x%04X)\n",
    //            x, y, str, color);
    // }
    return 0;
}

// Lua function: lr.draw_square(x, y, width, height, color)
static int lua_draw_square(lua_State *L) {
    if (lua_gettop(L) != 5) {
        lua_pushstring(L, "draw_square expects 5 arguments: x, y, width, height, color");
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    int width = luaL_checkinteger(L, 3);
    int height = luaL_checkinteger(L, 4);
    uint16_t color = (uint16_t)luaL_checkinteger(L, 5);
    draw_square(x, y, width, height, color);
    // if (log_cb) {
    //     log_cb(RETRO_LOG_INFO, "[LUA] draw_square called: (%d, %d, %d, %d, 0x%04X)\n",
    //            x, y, width, height, color);
    // }
    return 0;
}

// Lua function: lr.get_square_pos()
static int lua_get_square_pos(lua_State *L) {
    lua_pushinteger(L, square_x);
    lua_pushinteger(L, square_y);
    // if (log_cb) {
        // log_cb(RETRO_LOG_INFO, "[LUA] get_square_pos called: (%d, %d)\n", square_x, square_y);
    // }
    return 2;
}

// Lua function: lr.set_square_pos(x, y)
static int lua_set_square_pos(lua_State *L) {
    if (lua_gettop(L) != 2) {
        lua_pushstring(L, "set_square_pos expects 2 arguments: x, y");
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    if (x < 0 || x > WIDTH - 20) x = (x < 0) ? 0 : WIDTH - 20;
    if (y < 0 || y > HEIGHT - 20) y = (y < 0) ? 0 : HEIGHT - 20;
    square_x = x;
    square_y = y;
    // if (log_cb) {
    //     log_cb(RETRO_LOG_INFO, "[LUA] set_square_pos called: (%d, %d)\n", x, y);
    // }
    return 0;
}


// Lua function: lr.get_square2_pos()
static int lua_get_square2_pos(lua_State *L) {
    lua_pushinteger(L, square2_x);
    lua_pushinteger(L, square2_y);
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[LUA] get_square2_pos called: (%d, %d)\n", square2_x, square2_y);
    }
    return 2;
}

// Lua function: lr.set_square2_pos(x, y)
static int lua_set_square2_pos(lua_State *L) {
    if (lua_gettop(L) != 2) {
        lua_pushstring(L, "set_square2_pos expects 2 arguments: x, y");
        lua_error(L);
        return 0;
    }
    int x = luaL_checkinteger(L, 1);
    int y = luaL_checkinteger(L, 2);
    if (x < 0 || x > WIDTH - 20) x = (x < 0) ? 0 : WIDTH - 20;
    if (y < 0 || y > HEIGHT - 20) y = (y < 0) ? 0 : HEIGHT - 20;
    square2_x = x;
    square2_y = y;
    // if (log_cb) {
    //     log_cb(RETRO_LOG_INFO, "[LUA] set_square2_pos called: (%d, %d)\n", x, y);
    // }
    return 0;
}

// Lua function: lr.get_input(port, device, id)
static int lua_get_input(lua_State *L) {
    if (lua_gettop(L) != 3) {
        lua_pushstring(L, "get_input expects 3 arguments: port, device, id");
        lua_error(L);
        return 0;
    }
    unsigned port = luaL_checkinteger(L, 1);
    unsigned device = luaL_checkinteger(L, 2);
    unsigned id = luaL_checkinteger(L, 3);
    int state = input_state_cb ? input_state_cb(port, device, 0, id) : 0;
    lua_pushboolean(L, state);
    // if (log_cb) {
    //     log_cb(RETRO_LOG_INFO, "[LUA] get_input called: port=%u, device=%u, id=%u, state=%d\n",
    //            port, device, id, state);
    // }
    return 1;
}

// Lua function: lr.get_key(key_id)
static int lua_get_key(lua_State *L) {
    if (lua_gettop(L) != 1) {
        lua_pushstring(L, "get_key expects 1 argument: key_id");
        lua_error(L);
        return 0;
    }
    unsigned key_id = luaL_checkinteger(L, 1);
    int state = input_state_cb ? input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, key_id) : 0;
    lua_pushboolean(L, state);
    // if (log_cb) {
        // log_cb(RETRO_LOG_INFO, "[LUA] get_key called: key_id=%u, state=%d\n", key_id, state);
    // }
    return 1;
}

// Lua function: lr.get_mouse()
static int lua_get_mouse(lua_State *L) {
    if (lua_gettop(L) != 0) {
        lua_pushstring(L, "get_mouse expects no arguments");
        lua_error(L);
        return 0;
    }
    int x = input_state_cb ? input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_X) : 0;
    int y = input_state_cb ? input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_Y) : 0;
    int left = input_state_cb ? input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_LEFT) : 0;
    int right = input_state_cb ? input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_RIGHT) : 0;
    int middle = input_state_cb ? input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_MIDDLE) : 0;
    int wheel = input_state_cb ? input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELUP) -
                                input_state_cb(0, RETRO_DEVICE_MOUSE, 0, RETRO_DEVICE_ID_MOUSE_WHEELDOWN) : 0;
    lua_newtable(L);
    lua_pushinteger(L, x);
    lua_setfield(L, -2, "x");
    lua_pushinteger(L, y);
    lua_setfield(L, -2, "y");
    lua_pushboolean(L, left);
    lua_setfield(L, -2, "left");
    lua_pushboolean(L, right);
    lua_setfield(L, -2, "right");
    lua_pushboolean(L, middle);
    lua_setfield(L, -2, "middle");
    lua_pushinteger(L, wheel);
    lua_setfield(L, -2, "wheel");
    // if (log_cb) {
    //     log_cb(RETRO_LOG_INFO, "[LUA] get_mouse called: x=%d, y=%d, left=%d, right=%d, middle=%d, wheel=%d\n",
    //            x, y, left, right, middle, wheel);
    // }
    return 1;
}

// Lua function: lr.get_analog(port, index, id)
static int lua_get_analog(lua_State *L) {
    if (lua_gettop(L) != 3) {
        lua_pushstring(L, "get_analog expects 3 arguments: port, index, id");
        lua_error(L);
        return 0;
    }
    unsigned port = luaL_checkinteger(L, 1);
    unsigned index = luaL_checkinteger(L, 2);
    unsigned id = luaL_checkinteger(L, 3);
    int value = input_state_cb ? input_state_cb(port, RETRO_DEVICE_ANALOG, index, id) : 0;
    lua_pushinteger(L, value);
    // if (log_cb) {
    //     log_cb(RETRO_LOG_INFO, "[LUA] get_analog called: port=%u, index=%u, id=%u, value=%d\n",
    //            port, index, id, value);
    // }
    return 1;
}

// Table of functions to register
static const struct luaL_Reg libretro_funcs[] = {
    {"draw_string", lua_draw_string},
    {"draw_square", lua_draw_square},
    {"get_square_pos", lua_get_square_pos},
    {"set_square_pos", lua_set_square_pos},
    {"get_square2_pos", lua_get_square2_pos},
    {"set_square2_pos", lua_set_square2_pos},
    {"get_input", lua_get_input},
    {"get_key", lua_get_key},
    {"get_mouse", lua_get_mouse},
    {"get_analog", lua_get_analog},
    {NULL, NULL}
};

// Table of constants to register
static void set_constants(lua_State *L) {
    lua_pushinteger(L, COLOR_WHITE);
    lua_setfield(L, -2, "COLOR_WHITE");
    lua_pushinteger(L, COLOR_RED);
    lua_setfield(L, -2, "COLOR_RED");
    lua_pushinteger(L, COLOR_GREEN);
    lua_setfield(L, -2, "COLOR_GREEN");
    lua_pushinteger(L, COLOR_BLUE);
    lua_setfield(L, -2, "COLOR_BLUE");
    lua_pushinteger(L, WIDTH);
    lua_setfield(L, -2, "WIDTH");
    lua_pushinteger(L, HEIGHT);
    lua_setfield(L, -2, "HEIGHT");
    lua_pushinteger(L, RETRO_DEVICE_JOYPAD);
    lua_setfield(L, -2, "DEVICE_JOYPAD");
    lua_pushinteger(L, RETRO_DEVICE_KEYBOARD);
    lua_setfield(L, -2, "DEVICE_KEYBOARD");
    lua_pushinteger(L, RETRO_DEVICE_MOUSE);
    lua_setfield(L, -2, "DEVICE_MOUSE");
    lua_pushinteger(L, RETRO_DEVICE_ANALOG);
    lua_setfield(L, -2, "DEVICE_ANALOG");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_UP);
    lua_setfield(L, -2, "JOYPAD_UP");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_DOWN);
    lua_setfield(L, -2, "JOYPAD_DOWN");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_LEFT);
    lua_setfield(L, -2, "JOYPAD_LEFT");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_RIGHT);
    lua_setfield(L, -2, "JOYPAD_RIGHT");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_X); // Square (□)
    lua_setfield(L, -2, "JOYPAD_SQUARE");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_A); // Circle (○)
    lua_setfield(L, -2, "JOYPAD_CIRCLE");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_B); // Cross (×)
    lua_setfield(L, -2, "JOYPAD_CROSS");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_Y); // Triangle (△)
    lua_setfield(L, -2, "JOYPAD_TRIANGLE");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_L); // L1
    lua_setfield(L, -2, "JOYPAD_L1");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_R); // R1
    lua_setfield(L, -2, "JOYPAD_R1");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_L2); // L2
    lua_setfield(L, -2, "JOYPAD_L2");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_R2); // R2
    lua_setfield(L, -2, "JOYPAD_R2");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_SELECT); // Select
    lua_setfield(L, -2, "JOYPAD_SELECT");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_START); // Start
    lua_setfield(L, -2, "JOYPAD_START");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_L3); // L3
    lua_setfield(L, -2, "JOYPAD_L3");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_R3); // R3
    lua_setfield(L, -2, "JOYPAD_R3");
    lua_pushinteger(L, RETROK_w);
    lua_setfield(L, -2, "KEY_W");
    lua_pushinteger(L, RETROK_a);
    lua_setfield(L, -2, "KEY_A");
    lua_pushinteger(L, RETROK_s);
    lua_setfield(L, -2, "KEY_S");
    lua_pushinteger(L, RETROK_d);
    lua_setfield(L, -2, "KEY_D");
    lua_pushinteger(L, RETRO_DEVICE_ID_MOUSE_X);
    lua_setfield(L, -2, "MOUSE_X");
    lua_pushinteger(L, RETRO_DEVICE_ID_MOUSE_Y);
    lua_setfield(L, -2, "MOUSE_Y");
    lua_pushinteger(L, RETRO_DEVICE_ID_MOUSE_LEFT);
    lua_setfield(L, -2, "MOUSE_LEFT");
    lua_pushinteger(L, RETRO_DEVICE_ID_MOUSE_RIGHT);
    lua_setfield(L, -2, "MOUSE_RIGHT");
    lua_pushinteger(L, RETRO_DEVICE_ID_MOUSE_MIDDLE);
    lua_setfield(L, -2, "MOUSE_MIDDLE");
    lua_pushinteger(L, RETRO_DEVICE_ID_MOUSE_WHEELUP);
    lua_setfield(L, -2, "MOUSE_WHEELUP");
    lua_pushinteger(L, RETRO_DEVICE_ID_MOUSE_WHEELDOWN);
    lua_setfield(L, -2, "MOUSE_WHEELDOWN");
    lua_pushinteger(L, RETRO_DEVICE_INDEX_ANALOG_LEFT);
    lua_setfield(L, -2, "ANALOG_LEFT");
    lua_pushinteger(L, RETRO_DEVICE_INDEX_ANALOG_RIGHT);
    lua_setfield(L, -2, "ANALOG_RIGHT");
    lua_pushinteger(L, RETRO_DEVICE_ID_ANALOG_X);
    lua_setfield(L, -2, "ANALOG_X");
    lua_pushinteger(L, RETRO_DEVICE_ID_ANALOG_Y);
    lua_setfield(L, -2, "ANALOG_Y");
}

// Module initialization function
int luaopen_libretro(lua_State *L) {
    luaL_newlib(L, libretro_funcs);
    set_constants(L);
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[LUA] libretro module initialized\n");
    }
    return 1;
}