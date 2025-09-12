# libretro_lua_c

# License: MIT

# Features:
- draw_string
- draw_square
- get_square_pos (testing)
- set_square_pos (testing)
- get_input
- get_key
- get_mouse
- get_analog
- 

## Table of constants:
- COLOR_WHITE
- COLOR_RED
- COLOR_GREEN
- COLOR_BLUE
- RETRO_DEVICE_JOYPAD
- RETRO_DEVICE_KEYBOARD
- RETRO_DEVICE_MOUSE
- RETRO_DEVICE_ANALOG
- RETRO_DEVICE_ID_JOYPAD_UP
- RETRO_DEVICE_ID_JOYPAD_DOWN
- RETRO_DEVICE_ID_JOYPAD_LEFT
- RETRO_DEVICE_ID_JOYPAD_RIGHT
- RETRO_DEVICE_ID_MOUSE_X
- RETRO_DEVICE_ID_MOUSE_Y
- RETRO_DEVICE_ID_MOUSE_LEFT
- RETRO_DEVICE_ID_MOUSE_RIGHT
- RETRO_DEVICE_ID_MOUSE_MIDDLE
- RETRO_DEVICE_ID_MOUSE_WHEELUP
- RETRO_DEVICE_ID_MOUSE_WHEELDOWN
- RETRO_DEVICE_INDEX_ANALOG_LEFT
- RETRO_DEVICE_INDEX_ANALOG_RIGHT
- RETRO_DEVICE_ID_ANALOG_X
- RETRO_DEVICE_ID_ANALOG_Y
- 

# Information:
  Sample test Libretro Core for lua 5.4 script to use opengl and inputs.

  Note this using library as libretro core for RetroArch Lancher. Like libretro_lua_c.dll or libretro_lua_c.so. It can be place in core folder to run it.

  By creating lua script to build libretro core api is idea. To able to debug faster. But there should be limited.

# Example:
  The script has the simple draw text. As well input control for square move around to test.

# Required:
- msys64
- cmake

# Retroarch folder:
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
