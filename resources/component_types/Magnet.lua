Magnet = {
    active = false,

    OnStart = function(self)
        self.SpriteRenderer = self.actor:GetComponent("SpriteRenderer")
        self.rb = self.actor:GetComponent("Rigidbody")
        local screenLeft = self.rb:GetPosition():__sub(Vector2(self.rb:GetWidth() * 0.5, 0))
        local screenRight = self.rb:GetPosition():__add(Vector2(self.rb:GetWidth() * 0.5, 0))
        local screenUp = self.rb:GetPosition():__sub(Vector2(0, self.rb:GetHeight() * 0.5))
        local screenBottom = self.rb:GetPosition():__add(Vector2(0, self.rb:GetHeight() * 0.5))
        self.left = Application.WorldToScreen(screenLeft).x
        self.right = Application.WorldToScreen(screenRight).x
        self.up = Application.WorldToScreen(screenUp).y
        self.bottom = Application.WorldToScreen(screenBottom).y
    end,

    OnUpdate = function(self)
        if Input.GetMousePosition().x >= self.left and Input.GetMousePosition().x <= self.right and Input.GetMousePosition().y >= self.up and Input.GetMousePosition().y <= self.bottom then
            if Input.GetMouseButtonDown(1) then
                self.active = not self.active
            end
        end
    end
}