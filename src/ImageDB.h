#pragma once

#include <string>
#include <iostream>
#include <unordered_map>

#include "SDL Image/include/SDL_image.h"

#include "glm/glm.hpp"

class ImageDB
{
public:
	static inline std::unordered_map<std::string, SDL_Texture*> loaded_images;
	static inline std::unordered_map<std::string, glm::ivec2> loaded_images_size;

	static SDL_Texture* GetImage(const std::string& image_name);
	static glm::ivec2 GetImageSize(const std::string& image_name);

	static void DrawUI(std::string image_name, float x, float y);
	static void DrawUIEx(std::string image_name, float x, float y, float r, float g, float b, float a, float sorting_order);

	static void Draw(std::string image_name, float x, float y);
	static void DrawEx(std::string image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order);

	// Can be placed in Renderer
	static void DrawPixel(float x, float y, float r, float g, float b, float a);

	ImageDB() {
		// Need Config and Renderer initialized
	}
};

