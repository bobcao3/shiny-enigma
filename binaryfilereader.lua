BinaryFileReader = Util.class()

function BinaryFileReader:init(filename)
	self.file = io.open(filename, "rb")
	if not self.file then
		error(string.format("error opening binary file %s", filename))
	end
end

function BinaryFileReader:close()
	self.file:close()
end

-- utilizing luajit 2 bit module
function BinaryFileReader:u8()
	return self.file:read(1):byte(1)
end

function BinaryFileReader:u16()
	local a, b = self.file:read(2):byte(1, 2)
	return bit.bor(a, bit.lshift(b, 8))
end

function BinaryFileReader:i32()
	local a, b, c, d = self.file:read(4):byte(1, 4)
	return bit.bor(a, bit.lshift(b, 8), bit.lshift(c, 16), bit.lshift(d, 24))
end