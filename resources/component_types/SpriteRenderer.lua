SpriteRenderer = {
	sprite = "???",
	r = 255,
	g = 255,
	b = 255,
	a = 255,
	sorting_order = 0,

	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
	end,

	OnUpdate = function(self)
		local magnet = self.actor:GetComponent("Magnet")
		if magnet ~= nil then
			if self.sprite == "box1" then
				if magnet.active == true then
					self.r = 0
					self.g = 102
					self.b = 255
				end
				if magnet.active == false then
					self.r = 0
					self.g = 0
					self.b = 153
				end
			end
			if self.sprite == "box2" then
				if magnet.active == true then
					self.r = 255
					self.g = 80
					self.b = 80
				end
				if magnet.active == false then
					self.r = 128
					self.g = 0
					self.b = 0
				end
			end
		end
		local pos = self.rb:GetPosition()
		local rot_degrees = self.rb:GetRotation()
		local scale_x = self.rb:GetWidth()
		local scale_y = self.rb:GetHeight()
		Image.DrawEx(self.sprite, pos.x, pos.y, rot_degrees, scale_x, scale_y, 0.5, 0.5, self.r, self.g, self.b, self.a, self.sorting_order)
	end
}

