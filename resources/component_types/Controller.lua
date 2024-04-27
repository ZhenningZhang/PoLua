Controller = {
	OnStart = function(self)
		for i = 1, 15 do
			local new_actor = Actor.Instantiate("Tile")
			local new_rb = new_actor:GetComponent("Rigidbody")
			new_rb:SetPosition(Vector2(-3.5 + (i-1) * 0.5, -1))
			local pos = new_rb:GetPosition()
			Debug.Log("Spawned actor is at (" .. pos.x .. "," .. pos.y .. ")")
		end
	end
}