Mover = {

	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
		self.rb:AddForce(Vector2(400, -400))
	end
}

