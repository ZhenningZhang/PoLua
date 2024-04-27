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
		local pos = self.rb:GetPosition()
		local rot_degrees = self.rb:GetRotation()
		local scale_x = self.rb:GetWidth()
		local scale_y = self.rb:GetHeight()
		Image.DrawEx(self.sprite, pos.x, pos.y, rot_degrees, scale_x, scale_y, 0.5, 0.5, self.r, self.g, self.b, self.a, self.sorting_order)
	end
}

