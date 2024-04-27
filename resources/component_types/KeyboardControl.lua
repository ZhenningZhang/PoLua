KeyboardControl = {

	OnStart = function(self)
		self.rb = self.actor:GetComponent("Rigidbody")
		self.frame = 0
		self.moving = false
	end,

	OnUpdate = function(self)
		local function updateObject(self, targetX, targetY, targetRotation, duration)
			local startTime = os.clock() -- Get current time
			print(startTime)
			local endTime = startTime + duration -- Calculate end time
			local startX, startY = self.rb:GetPosition().x, self.rb:GetPosition().y
			local startRotation = self.rb:GetRotation()

			while os.clock() < endTime do -- Loop until duration is reached
				local t = (os.clock() - startTime) / duration -- Calculate progress (0 to 1)
				self.rb:SetPosition(Vector2(startX + (targetX - startX) * t, startY + (targetY - startY) * t))
				print((Vector2(startX + (targetX - startX) * t, startY + (targetY - startY) * t)).x)
				self.rb:SetRotation(startRotation + (targetRotation - startRotation) * t)
				coroutine.yield() -- Yield to let other coroutines run
			end
			-- Ensure object reaches the target position and rotation exactly
			self.rb:SetPosition(Vector2(targetX, targetY))
			self.rb:SetRotation(targetRotation)
		end

		-- Coroutine to move and rotate the object
		local function moveAndRotate(self, targetX, targetY, targetRotation, duration)
			self.moving = true
			local startTime = os.clock()
			local endTime = startTime + duration
			local co = coroutine.create(updateObject)
			while os.clock() < endTime do
				local status, result = coroutine.resume(co, self, targetX, targetY, targetRotation, duration)
				if not status then
					print("Coroutine error:", result)
				end
				if status then
					print(result)
				end
			end
			self.moving = false
		end

		if Application.GetFrame() - self.frame >= 5 then
			self.frame = Application.GetFrame()
			if Input.GetKey("right") then
				local vel_x = self.rb:GetPosition().x;
				local vel_y = self.rb:GetPosition().y;
				local rotation = self.rb:GetRotation();
				moveAndRotate(self, vel_x + 0.5, vel_y, rotation + 90, 1)
			end

			if Input.GetKey("left") then
				self.rb:SetPosition(Vector2((vel_x - 0.5), vel_y))
				self.rb:SetRotation(self.rb:GetRotation() - 90)
			end
		end
	end
}