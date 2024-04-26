#include "Engine.h"

void Engine::GameLoop() 
{
	Start();

	while (true) 
	{
		// Will mark all actors as newly added
		SceneDB::LoadScene();

		SDL_Event next_event;

		while (Helper::SDL_PollEvent498(&next_event)) 
		{
			// HW6 Test Suite #1 Input Management
			Input::ProcessEvent(next_event);

			if (next_event.type == SDL_QUIT)
				quit = true;
		}

		// This is the real Update()
		{
			SceneDB::ProcessAddedActors();

			for (std::shared_ptr<Actor> actor : SceneDB::actors_loop)
				actor->ProcessAddedComponents();

			SceneDB::OnUpdate();
			SceneDB::OnLateUpdate();

			for (std::shared_ptr<Actor> actor : SceneDB::actors_loop)
				actor->ProcessRemovedComponents();

			SceneDB::ProcessDestroyedActors();
		}

		Input::LateUpdate();
		Event::LateUpdate();

		// HW8: Step the Physics
		if (Rigidbody::world != nullptr)
			Rigidbody::world->Step(1.0f / 60.0f, 8, 3);

		// Clear the buffer
		Renderer::ClearByColor();

		// HW7: This Render order is fixed
		Renderer::RenderImages();
		Renderer::RenderUI();
		Renderer::DrawTexts();
		Renderer::RenderPixels();

		Helper::SDL_RenderPresent498(renderer.sdl_renderer);

		if (quit)
			exit(0);
	}
}

void Engine::Start() 
{
	// Initialization was done in ctors (see Engine.h)
	// As of HW6, there are no updates here
}