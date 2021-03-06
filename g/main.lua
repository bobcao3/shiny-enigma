Util.dofile 'widget'
Util.dofile 'widget_textbase'
Util.dofile 'widget_label'
Util.dofile 'widget_textbox'
Util.dofile 'widget_button'

fullscreen = false
w = Widget_Button.new()
w.rect.x = 100
w.rect.y = 100
w.rect.w = 80
w.set_text("CommandButton1")
w.set_color({r = 0, g = 128, b = 128})
ad = "Audio devices: "
for i, name in MIDIOut.devices() do
	ad = ad .. string.format("[%d] = %q; ", i, name)
	print(i, name)
end

x = 0
y = 100
msg = "就绪"
function draw_debug_line(i, s)
	if s == "" then return end
	t = Texture.render_text(0, s)
	t.set_color({r = 0, g = 0, b = 0})
	Graphics.copy({x = 0, y = Graphics.h - (8 - i) * WLH}, t, t.get_rect())
end

Input.text_start()

function on.update()
	w.update()
	if first_frame then
		first_frame = false
	end
	if Input.triggered(96) then
		mo = MIDIOut.open(1)
		msg = "打开MIDI输出设备#1"
	end
	if Input.triggered(97) then
		if mo then
			mo.send_short_message(0x90, 0x66, 0x7f)
			mo = nil
			msg = ""
		end
	end
	if Input.triggered(41) then
		os.exit()
	end
	if Input.triggered(68) then
		fullscreen = not fullscreen
		Window.set_fullscreen(fullscreen and "borderless" or "windowed")
	end
end

function on.paint()
	if x >= 60 then x = 0 end
	Graphics.stretch_copy(Graphics, Texture.stock[0], {x = 255, y = 255, w = 1, h = 1})
	Graphics.stretch_copy({x = x, y = y, w = 10, h = 10}, Texture.stock[0], {x = 0, y = 0, w = 1, h = 1})
	Graphics.tile({x = 300, y = 100, w = 10, h = 10}, Texture.stock[0], {x = 17, y = 0, w = 4, h = 4})
	w.draw()
	draw_debug_line(1, string.format("%q", Util.os_encoding_to_utf8("\xc2\xd2\xc2\xeb")))
	draw_debug_line(2, string.format("%d", utf8.len("你好", 1, 6)))
	draw_debug_line(3, Input.mods().ctrl and "ctrl" or "---")
	draw_debug_line(4, msg)
	draw_debug_line(5, ad)
	draw_debug_line(6, Util.rtp("main.lua"))
end
