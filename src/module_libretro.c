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
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[LUA] draw_string called: (%d, %d, %s, 0x%04X)\n",
               x, y, str, color);
    }
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
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[LUA] draw_square called: (%d, %d, %d, %d, 0x%04X)\n",
               x, y, width, height, color);
    }
    return 0;
}

// Lua function: lr.get_square_pos()
static int lua_get_square_pos(lua_State *L) {
    lua_pushinteger(L, square_x);
    lua_pushinteger(L, square_y);
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[LUA] get_square_pos called: (%d, %d)\n", square_x, square_y);
    }
    return 2; // Return x, y
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
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[LUA] set_square_pos called: (%d, %d)\n", x, y);
    }
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
    if (log_cb) {
        log_cb(RETRO_LOG_INFO, "[LUA] get_input called: port=%u, device=%u, id=%u, state=%d\n",
               port, device, id, state);
    }
    return 1;
}

// Table of functions to register
static const struct luaL_Reg libretro_funcs[] = {
    {"draw_string", lua_draw_string},
    {"draw_square", lua_draw_square},
    {"get_square_pos", lua_get_square_pos},
    {"set_square_pos", lua_set_square_pos},
    {"get_input", lua_get_input},
    {NULL, NULL}
};

// Table of constants to register
static void set_constants(lua_State *L) {
    lua_pushinteger(L, COLOR_WHITE);
    lua_setfield(L, -2, "COLOR_WHITE");
    lua_pushinteger(L, COLOR_RED);
    lua_setfield(L, -2, "COLOR_RED");
    lua_pushinteger(L, WIDTH);
    lua_setfield(L, -2, "WIDTH");
    lua_pushinteger(L, HEIGHT);
    lua_setfield(L, -2, "HEIGHT");
    // Add input constants
    lua_pushinteger(L, RETRO_DEVICE_JOYPAD);
    lua_setfield(L, -2, "DEVICE_JOYPAD");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_UP);
    lua_setfield(L, -2, "JOYPAD_UP");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_DOWN);
    lua_setfield(L, -2, "JOYPAD_DOWN");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_LEFT);
    lua_setfield(L, -2, "JOYPAD_LEFT");
    lua_pushinteger(L, RETRO_DEVICE_ID_JOYPAD_RIGHT);
    lua_setfield(L, -2, "JOYPAD_RIGHT");
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