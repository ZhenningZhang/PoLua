#pragma once

#include <string>
#include <SDL_ttf.h>

#include "TextDB.h"
#include "ImageDB.h"

class RenderRequests
{
	// Nothing here, can I delete it?
};

struct TextRenderRequest
{
	std::string text_content;
	int x;
	int y;
	TTF_Font* font;
	SDL_Color font_color;

	TextRenderRequest(const std::string& text_content, int x, int y, std::string font_name, int font_size, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
		: text_content(text_content), x(x), y(y)
	{
		font = TextDB::GetFont(font_name, font_size);
		font_color = { r, g, b, a };
	}
};

struct UIRenderRequest
{
	SDL_Texture* image;
	SDL_Rect destination_rect;
	SDL_Color color = { 255, 255, 255, 255 };
	int sorting_order = 0;

	// Ugly codes...
	UIRenderRequest(
		std::string image_name, 
		int x, int y, 
		uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255,
		int sorting_order = 0
	)
		: sorting_order(sorting_order)
	{
		image = ImageDB::GetImage(image_name);

		glm::ivec2 image_size = ImageDB::GetImageSize(image_name);
		destination_rect = { x, y, image_size.x, image_size.y };

		color = { r, g, b, a };
		this->sorting_order = sorting_order;
	}

	bool operator<(const UIRenderRequest& other) const
	{
		return sorting_order < other.sorting_order;
	}
};

struct ImageDrawRequest
{
	std::string image_name;
	float x, y;
	int rotation_degrees;
	float scale_x, scale_y;
	float pivot_x, pivot_y;
	int r, g, b, a;
	int sorting_order;

	// I don't know, is this syntax good?
	ImageDrawRequest(
		std::string image_name,
		float x, float y,
		int rotation_degrees = 0,
		float scale_x = 1.0f, float scale_y = 1.0f,
		float pivot_x = 0.5f, float pivot_y = 0.5f,
		uint8_t r = 255, uint8_t g = 255, uint8_t b = 255, uint8_t a = 255,
		int sorting_order = 0
	)
		: image_name(image_name), x(x), y(y), rotation_degrees(rotation_degrees),
		scale_x(scale_x), scale_y(scale_y), pivot_x(pivot_x), pivot_y(pivot_y),
		r(r), g(g), b(b), a(a), sorting_order(sorting_order) {}

	bool operator<(const ImageDrawRequest& other) const
	{
		return sorting_order < other.sorting_order;
	}
};

struct DrawPixelRequest
{
	int x, y;
	uint8_t r, g, b, a;

	DrawPixelRequest(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) : x(x), y(y), r(r), g(g), b(b), a(a) {}
};