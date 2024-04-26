#include "Renderer.h"

#include "Config.h"
#include "Camera.h"

void Renderer::InitializeWindow()
{
	sdl_window = Helper::SDL_CreateWindow498(
		Config::game_title.c_str(),
		100, 100,
		Config::x_resolution, Config::y_resolution,
		SDL_WINDOW_SHOWN
	);
}

void Renderer::InitializeRenderer(SDL_RendererFlags flags)
{
	sdl_renderer = Helper::SDL_CreateRenderer498(sdl_window, -1, flags);
}

void Renderer::ClearByColor()
{
	SDL_SetRenderDrawColor(
		sdl_renderer,
		Config::clear_color_r,
		Config::clear_color_g,
		Config::clear_color_b,
		Config::clear_color_a
	);
	SDL_RenderClear(sdl_renderer);
}

void Renderer::DrawText(const TextRenderRequest& text)
{
	SDL_Surface* surface = TTF_RenderText_Solid(text.font, text.text_content.c_str(), text.font_color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

	//SDL_FreeSurface(surface);

	SDL_Rect textRect = { text.x, text.y, surface->w, surface->h };
	SDL_RenderCopy(sdl_renderer, texture, NULL, &textRect);
}

void Renderer::DrawTexts()
{
	while (!text_render_requests.empty())
	{
		DrawText(text_render_requests.front());
		text_render_requests.pop();
	}
}

void Renderer::RenderUI()
{
	std::stable_sort(ui_render_requests.begin(), ui_render_requests.end());

	for (const UIRenderRequest& ui_render_request : ui_render_requests)
	{
		SDL_SetTextureColorMod(ui_render_request.image, ui_render_request.color.r, ui_render_request.color.g, ui_render_request.color.b);
		SDL_SetTextureAlphaMod(ui_render_request.image, ui_render_request.color.a);

		SDL_RenderCopy(sdl_renderer, ui_render_request.image, NULL, &ui_render_request.destination_rect);

		SDL_SetTextureAlphaMod(ui_render_request.image, 255);
		SDL_SetTextureColorMod(ui_render_request.image, 255, 255, 255);
	}

	ui_render_requests.clear();
}

void Renderer::RenderImages()
{
	std::stable_sort(image_draw_requests.begin(), image_draw_requests.end());

	SDL_RenderSetScale(sdl_renderer, Camera::zoom_factor, Camera::zoom_factor);

	for (const ImageDrawRequest& request : image_draw_requests)
	{
		const int pixels_per_meter = 100;
		glm::vec2 final_rendering_position = glm::vec2(request.x, request.y) - Camera::camera_position;

		SDL_Texture* tex = ImageDB::GetImage(request.image_name);
		SDL_Rect tex_rect;
		SDL_QueryTexture(tex, NULL, NULL, &tex_rect.w, &tex_rect.h);

		float x_scale = std::abs(request.scale_x);
		float y_scale = std::abs(request.scale_y);

		tex_rect.w *= x_scale;
		tex_rect.h *= y_scale;

		SDL_Point pivot_point = { static_cast<int>(request.pivot_x * tex_rect.w), static_cast<int>(request.pivot_y * tex_rect.h) };

		tex_rect.x = static_cast<int>(final_rendering_position.x * pixels_per_meter + Config::x_resolution * 0.5f / Camera::zoom_factor - pivot_point.x);
		tex_rect.y = static_cast<int>(final_rendering_position.y * pixels_per_meter + Config::y_resolution * 0.5f / Camera::zoom_factor - pivot_point.y);
	
		SDL_SetTextureColorMod(tex, request.r, request.g, request.b);
		SDL_SetTextureAlphaMod(tex, request.a);

		Helper::SDL_RenderCopyEx498(0, "", sdl_renderer, tex, NULL, &tex_rect, request.rotation_degrees, &pivot_point, SDL_FLIP_NONE);

		SDL_SetTextureColorMod(tex, 255, 255, 255);
		SDL_SetTextureAlphaMod(tex, 255);
	}

	SDL_RenderSetScale(sdl_renderer, 1, 1);

	image_draw_requests.clear();
}

void Renderer::RenderPixels()
{
	SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_BLEND);

	for (const DrawPixelRequest& draw_pixel_request : draw_pixel_requests)
	{
		SDL_SetRenderDrawColor(sdl_renderer, draw_pixel_request.r, draw_pixel_request.g, draw_pixel_request.b, draw_pixel_request.a);
		SDL_RenderDrawPoint(sdl_renderer, draw_pixel_request.x, draw_pixel_request.y);
	}

	SDL_SetRenderDrawBlendMode(sdl_renderer, SDL_BLENDMODE_NONE);

	SDL_SetRenderDrawColor(
		sdl_renderer,
		Config::clear_color_r,
		Config::clear_color_g,
		Config::clear_color_b,
		Config::clear_color_a
	);

	draw_pixel_requests.clear();
}
