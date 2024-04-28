SceneSwitcher = {
    next_scene = "",

    OnStart = function (self)
        Debug.Log(self.next_scene)
        self.rb = self.actor:GetComponent("Rigidbody")
        self.compass = Actor.Find("compass"):GetComponent("Rigidbody")
    end,

    OnUpdate = function(self)
        if Vector2.Distance(self.rb:GetPosition(), self.compass:GetPosition()) < 0.01 then
            Scene.Load(self.next_scene)
        end
    end
    
    -- it just cannot use OnCollisionEnter things
    -- OnCollisionEnter = function(self, collision)
    --     Debug.Log(111)

    -- 	if collision.other:GetName() == "compass" and self.next_scene ~= "" then
    -- 		Scene.Load(self.next_scene)
    -- 	end
    -- end
}