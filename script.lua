local lr = require("libretro")

local square_color = lr.COLOR_RED -- Default color

function update()
    -- Draw "Hello World"
    lr.draw_string(50, 50, "Hello World", lr.COLOR_WHITE)
    
    -- Get current square position
    local x, y = lr.get_square_pos()
    
    -- Keyboard input (WASD)
    if lr.get_key(lr.KEY_W) then
        y = y - 1
    end
    if lr.get_key(lr.KEY_S) then
        y = y + 1
    end
    if lr.get_key(lr.KEY_A) then
        x = x - 1
    end
    if lr.get_key(lr.KEY_D) then
        x = x + 1
    end
    
    -- Gamepad input (buttons)
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_UP) then
        y = y - 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_DOWN) then
        y = y + 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_LEFT) then
        x = x - 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_RIGHT) then
        x = x + 1
    end
    
    -- Gamepad analog input (left stick)
    local left_x = lr.get_analog(0, lr.ANALOG_LEFT, lr.ANALOG_X)
    local left_y = lr.get_analog(0, lr.ANALOG_LEFT, lr.ANALOG_Y)
    -- Scale analog input (-32768 to 32767) to pixel movement (e.g., ±1 pixel per frame)
    x = x + math.floor(left_x / 8192) -- Adjust sensitivity (8192 ~ 1/4 of max)
    y = y + math.floor(left_y / 8192)
    
    -- Mouse input
    local mouse = lr.get_mouse()
    if mouse.x ~= 0 or mouse.y ~= 0 then
        x = x + mouse.x -- Relative mouse movement
        y = y + mouse.y
    end
    if mouse.left then
        square_color = lr.COLOR_RED
    end
    if mouse.right then
        square_color = lr.COLOR_GREEN
    end
    if mouse.middle then
        square_color = lr.COLOR_BLUE
    end
    
    -- Clamp position to screen bounds (20x20 square)
    x = math.max(0, math.min(x, lr.WIDTH - 20))
    y = math.max(0, math.min(y, lr.HEIGHT - 20))
    
    -- Update square position
    lr.set_square_pos(x, y)
    
    -- Draw square
    lr.draw_square(x, y, 20, 20, square_color)
    
    -- Display square position and color
    local color_name = (square_color == lr.COLOR_RED and "Red") or
                       (square_color == lr.COLOR_GREEN and "Green") or
                       (square_color == lr.COLOR_BLUE and "Blue") or "Unknown"
    lr.draw_string(50, 70, string.format("Square at: (%d, %d), Color: %s", x, y, color_name), lr.COLOR_WHITE)
end