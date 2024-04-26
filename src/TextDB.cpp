#include "TextDB.h"
#include "Renderer.h"

void TextDB::Draw(
	const std::string& text_content,
	int x, int y,
	std::string font_name, int font_size,
	float r, float g, float b, float a)
{
	Renderer::text_render_requests.emplace(text_content, x, y, font_name, font_size, r, g, b, a);
}

void TextDB::LoadFontPath(std::string font_name)
{
	std::filesystem::path font_path("resources/fonts/" + font_name + ".ttf");

	if (!std::filesystem::exists(font_path)) 
	{
		std::cout << "error: font " << font_name << " missing";
		exit(0);
	}

	font_paths[font_name] = font_path;
}

TTF_Font* TextDB::GetFont(std::string font_name, int font_size)
{
	// First time using the font
	if (font_paths.find(font_name) == font_paths.end())
		LoadFontPath(font_name);

	// Not have corresponding font and size
	if (fonts[font_name][font_size] == nullptr)
		fonts[font_name][font_size] = TTF_OpenFont(font_paths[font_name].string().c_str(), font_size);

	return fonts[font_name][font_size];
}
