local lr = require("libretro")

local square1_color = lr.COLOR_RED -- Square 1 (left stick, keyboard, mouse)
local square2_color = lr.COLOR_GREEN -- Square 2 (right stick)
local square1_visible = true -- Visibility toggle
local square2_visible = true
local colors = {lr.COLOR_RED, lr.COLOR_GREEN, lr.COLOR_BLUE}
local square1_color_index = 1 -- Tracks color cycle
local square2_color_index = 2

function update()
    -- Draw "Hello World"
    lr.draw_string(10, 10, "Hello World", lr.COLOR_WHITE)
    
    -- Get square positions
    local x1, y1 = lr.get_square_pos()
    local x2, y2 = lr.get_square2_pos()
    
    -- Square 1: Keyboard input (WASD)
    if lr.get_key(lr.KEY_W) then
        y1 = y1 - 1
    end
    if lr.get_key(lr.KEY_S) then
        y1 = y1 + 1
    end
    if lr.get_key(lr.KEY_A) then
        x1 = x1 - 1
    end
    if lr.get_key(lr.KEY_D) then
        x1 = x1 + 1
    end
    
    -- Square 1: Gamepad input (buttons)
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_UP) then
        y1 = y1 - 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_DOWN) then
        y1 = y1 + 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_LEFT) then
        x1 = x1 - 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_RIGHT) then
        x1 = x1 + 1
    end
    
    -- Square 1: Gamepad analog input (left stick)
    local left_x = lr.get_analog(0, lr.ANALOG_LEFT, lr.ANALOG_X)
    local left_y = lr.get_analog(0, lr.ANALOG_LEFT, lr.ANALOG_Y)
    x1 = x1 + math.floor(left_x / 8192)
    y1 = y1 + math.floor(left_y / 8192)
    
    -- Square 2: Gamepad analog input (right stick)
    local right_x = lr.get_analog(0, lr.ANALOG_RIGHT, lr.ANALOG_X)
    local right_y = lr.get_analog(0, lr.ANALOG_RIGHT, lr.ANALOG_Y)
    x2 = x2 + math.floor(right_x / 8192)
    y2 = y2 + math.floor(right_y / 8192)
    
    -- Mouse input (affects square 1)
    local mouse = lr.get_mouse()
    if mouse.x ~= 0 or mouse.y ~= 0 then
        x1 = x1 + mouse.x
        y1 = y1 + mouse.y
    end
    if mouse.left then
        square1_color = lr.COLOR_RED
        square1_color_index = 1
    end
    if mouse.right then
        square1_color = lr.COLOR_GREEN
        square1_color_index = 2
    end
    if mouse.middle then
        square1_color = lr.COLOR_BLUE
        square1_color_index = 3
    end
    
    -- Gamepad buttons: Square, Cross, Triangle, Circle, L1, R1, L2, R2, L3, R3, Start, Select
    local square_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_SQUARE)
    local cross_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_CROSS)
    local triangle_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_TRIANGLE)
    local circle_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_CIRCLE)
    local l1_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_L1)
    local r1_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_R1)
    local l2_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_L2)
    local r2_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_R2)
    local l3_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_L3)
    local r3_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_R3)
    local start_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_START)
    local select_btn = lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_SELECT)
    
    -- Cycle colors
    if triangle_btn then
        square1_color_index = (square1_color_index % 3) + 1
        square1_color = colors[square1_color_index]
    end
    if square_btn then
        square2_color_index = (square2_color_index % 3) + 1
        square2_color = colors[square2_color_index]
    end
    
    -- Toggle visibility
    if l3_btn then
        square1_visible = not square1_visible
    end
    if r3_btn then
        square2_visible = not square2_visible
    end
    if select_btn then
        square1_visible = not square1_visible
        square2_visible = not square2_visible
    end
    
    -- Reset positions
    if l2_btn then
        x1, y1 = 0, 0
    end
    if r2_btn then
        x2, y2 = 0, 0
    end
    if start_btn then
        x1, y1 = 0, 0
        x2, y2 = 0, 0
    end
    
    -- Clamp positions (20x20 squares)
    x1 = math.max(0, math.min(x1, lr.WIDTH - 20))
    y1 = math.max(0, math.min(y1, lr.HEIGHT - 20))
    x2 = math.max(0, math.min(x2, lr.WIDTH - 20))
    y2 = math.max(0, math.min(y2, lr.HEIGHT - 20))
    
    -- Update square positions
    lr.set_square_pos(x1, y1)
    lr.set_square2_pos(x2, y2)
    
    -- Draw squares (if visible)
    if square1_visible then
        lr.draw_square(x1, y1, 20, 20, square1_color)
    end
    if square2_visible then
        lr.draw_square(x2, y2, 20, 20, square2_color)
    end
    
    -- Display positions, colors, and button states
    local color1_name = (square1_color == lr.COLOR_RED and "Red") or
                        (square1_color == lr.COLOR_GREEN and "Green") or
                        (square1_color == lr.COLOR_BLUE and "Blue") or "Unknown"
    local color2_name = (square2_color == lr.COLOR_RED and "Red") or
                        (square2_color == lr.COLOR_GREEN and "Green") or
                        (square2_color == lr.COLOR_BLUE and "Blue") or "Unknown"
    lr.draw_string(10, 30, string.format("S1 (Left Stick): (%d, %d), Color: %s, Visible: %s", 
                                         x1, y1, color1_name, square1_visible), lr.COLOR_WHITE)
    lr.draw_string(10, 50, string.format("S2 (Right Stick): (%d, %d), Color: %s, Visible: %s", 
                                         x2, y2, color2_name, square2_visible), lr.COLOR_WHITE)
    -- Vertical button list with PS4 symbols
    lr.draw_string(10, 90, string.format("square: %d", square_btn and 1 or 0), lr.COLOR_WHITE) -- □
    lr.draw_string(10, 100, string.format("cross: %d", cross_btn and 1 or 0), lr.COLOR_WHITE) -- ×
    lr.draw_string(10, 110, string.format("triangle: %d", triangle_btn and 1 or 0), lr.COLOR_WHITE) -- △
    lr.draw_string(10, 120, string.format("circle: %d", circle_btn and 1 or 0), lr.COLOR_WHITE) -- ○
    lr.draw_string(10, 130, string.format("L1: %d", l1_btn and 1 or 0), lr.COLOR_WHITE)
    lr.draw_string(10, 140, string.format("R1: %d", r1_btn and 1 or 0), lr.COLOR_WHITE)
    lr.draw_string(10, 150, string.format("L2: %d", l2_btn and 1 or 0), lr.COLOR_WHITE)
    lr.draw_string(10, 160, string.format("R2: %d", r2_btn and 1 or 0), lr.COLOR_WHITE)
    lr.draw_string(10, 170, string.format("L3: %d", l3_btn and 1 or 0), lr.COLOR_WHITE)
    lr.draw_string(10, 180, string.format("R3: %d", r3_btn and 1 or 0), lr.COLOR_WHITE)
    lr.draw_string(10, 190, string.format("Start: %d", start_btn and 1 or 0), lr.COLOR_WHITE)
    lr.draw_string(10, 200, string.format("Select: %d", select_btn and 1 or 0), lr.COLOR_WHITE)
end