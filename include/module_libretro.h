#ifndef MODULE_LIBRETRO_H
#define MODULE_LIBRETRO_H

#include <lua.h>

/* Lua module initialization function */
int luaopen_libretro(lua_State *L);

#endif /* MODULE_LIBRETRO_H */