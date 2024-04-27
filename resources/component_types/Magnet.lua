Magnet = {
    active = false,

    OnStart = function(self)
        self.SpriteRenderer = self.actor:GetComponent("SpriteRenderer")
        self.rb = self.actor:GetComponent("Rigidbody")
    end,

    OnUpdate = function(self)
        Debug.Log(Input.GetMousePosition().x)
        Debug.Log(self.rb:GetPosition().x)
    end
}