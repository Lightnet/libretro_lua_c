local lr = require("libretro")

function update()
    -- Draw "Hello World"
    lr.draw_string(50, 50, "Hello World", lr.COLOR_WHITE)
    
    -- Get current square position
    local x, y = lr.get_square_pos()
    
    -- Move square based on input
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_RIGHT) then
        x = x + 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_LEFT) then
        x = x - 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_DOWN) then
        y = y + 1
    end
    if lr.get_input(0, lr.DEVICE_JOYPAD, lr.JOYPAD_UP) then
        y = y - 1
    end
    
    -- Update square position
    lr.set_square_pos(x, y)
    
    -- Draw square (20x20, red, matching original behavior)
    lr.draw_square(x, y, 20, 20, lr.COLOR_RED)
    
    -- Display square position
    lr.draw_string(50, 70, string.format("Square at: (%d, %d)", x, y), lr.COLOR_WHITE)
end