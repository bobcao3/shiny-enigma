Scene_Message = Util.class(Scene)

function Scene_Message:init()
	Scene.init(self)
	self.lines[0] = Line.new("Message")
	self:provide_help("Space = Return\nHello, world!")
	function self.on:update()
		if Input.triggered(44) then
			SceneManager.back("123")
		end
	end
end
