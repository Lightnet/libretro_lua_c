# libretro_lua_c

# License: MIT

# Information:
  Sample test Libretro Core for lua 5.4 script to use opengl and inputs.

  Note this using library as libretro core for RetroArch Lancher. Like libretro_lua_c.dll or libretro_lua_c.so. It can be place in core folder to run it.

  By creating lua script to build libretro core api is idea. To able to debug faster. But there should be limited.

# Example:
  The script has the simple draw text. As well input control for square move around to test.

# Required:
- msys64
- cmake

# retroarch folder:
```
- core
     - libretro_lua_c.dll
     - script.lua
```

## 
- Windows ???.
- msys64 tested.

# run cmd:
```
retroarch.exe --verbose -L cores/libretro_lua_c.dll
```

# load core:
```
libretro_lua_c.dll
```

If there should be Start Core button once the core is loaded.

# Credits:
 * Grok 3.0
 * https://nnarain.github.io/2017/07/13/GameboyCore-as-a-libretro-core!.html
 * https://bitbucket.org/Themaister/libretro-gl/src/master/
 * https://github.com/nnarain/gameboycore-retro
 * https://github.com/libretro/libretro-samples
 * https://www.libretro.com/index.php/making-of-craft-core/
 * https://github.com/libretro/Craft/blob/master/libretro/libretro.c
 * https://github.com/RobLoach/libretro-dolphin-launcher/tree/master
