#pragma once

#include <vector>

#include "SDL.h"
#include "Helper.h"
#include "glm/glm.hpp"

#include "RenderRequests.h"

class Renderer
{
public:
	static inline SDL_Window* sdl_window;
	static inline SDL_Renderer* sdl_renderer;

	static inline std::queue<TextRenderRequest> text_render_requests;
	static inline std::vector<UIRenderRequest> ui_render_requests;
	static inline std::vector<ImageDrawRequest> image_draw_requests;
	static inline std::vector<DrawPixelRequest> draw_pixel_requests;

	static void ClearByColor();

	static void DrawText(const TextRenderRequest& text);
	static void DrawTexts();

	static void RenderUI();
	static void RenderImages();
	static void RenderPixels();


	Renderer() {
		// Only need Config initialized
		SDL_Init(SDL_INIT_VIDEO);
		InitializeWindow();
		InitializeRenderer(SDL_RENDERER_PRESENTVSYNC);
	}

private:
	static void InitializeWindow();
	static void InitializeRenderer(SDL_RendererFlags flags);
};

