#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <filesystem>

#include "SDL_ttf.h"

class TextDB
{
public:
	static inline std::unordered_map<std::string, std::filesystem::path> font_paths;
	static inline std::unordered_map<std::string, std::unordered_map<int, TTF_Font*>> fonts;

	static void Draw(
		const std::string& text_content,
		int x, int y,
		std::string font_name, int font_size,
		float r, float g, float b, float a);

	static void LoadFontPath(std::string font_name);
	static TTF_Font* GetFont(std::string font_name, int font_size);
	
	TextDB() { TTF_Init(); }
};
