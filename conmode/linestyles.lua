return {
	normal = {
		color = {r = 255, g = 255, b = 255},
		draw_background = function (i)
			Graphics.tile(Line.get_rect(i), 0, {x = 32, y = 0, w = 8, h = 8})
		end,
	},
	null = {
		color = {r = 0, g = 0, b = 0},
		draw_background = function (i)
			Graphics.tile(Line.get_rect(i), 0, {x = 40, y = 0, w = 8, h = 8})
		end,
	},
	selected = {
		color = {r = 45, g = 54, b = 9},
		draw_background = function (i)
			local patch = {
				texture = 0,
				x = 17, y = 0, w = 5, h = 5,
				t = 2, r = 2, b = 2, l = 2
			}
			Graphics.draw_9patch(Line.get_rect(i), patch)
		end,
	},
	help = {
		color = {r = 147, g = 236, b = 147},
		draw_background = function (i)
			Graphics.copy(Line.get_rect(i), 0, {x = 48, y = 0, w = 1, h = 1})
		end,
	},
	title = {
		color = {r = 147, g = 236, b = 147},
		draw_background = function (i)
			Graphics.copy(Line.get_rect(i), 0, {x = 48, y = 0, w = 1, h = 1})
		end,
	},
}