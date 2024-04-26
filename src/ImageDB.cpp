#include "ImageDB.h"

#include "Renderer.h"

SDL_Texture* ImageDB::GetImage(const std::string& image_name)
{
	// If Image is not loaded, load it.
	if (loaded_images.find(image_name) == loaded_images.end())
	{
		std::string image_path = "resources/images/" + image_name + ".png";

		if (!std::filesystem::exists(image_path)) 
		{
			std::cout << "error: missing image " << image_name;
			exit(0);
		}

		loaded_images[image_name] = IMG_LoadTexture(Renderer::sdl_renderer, image_path.c_str());
	}
	
	return loaded_images[image_name];
}

glm::ivec2 ImageDB::GetImageSize(const std::string& image_name)
{
	// Assume the image does exist

	// If this is the first time to get image size, find it
	if (loaded_images_size.find(image_name) == loaded_images_size.end())
	{
		glm::ivec2 image_size = glm::ivec2(0, 0);
		SDL_QueryTexture(loaded_images[image_name], NULL, NULL, &image_size.x, &image_size.y);

		loaded_images_size[image_name] = image_size;
	}

	return loaded_images_size[image_name];
}

void ImageDB::DrawUI(std::string image_name, float x, float y)
{
	Renderer::ui_render_requests.emplace_back(image_name, x, y);
}

void ImageDB::DrawUIEx(std::string image_name, float x, float y, float r, float g, float b, float a, float sorting_order)
{
	Renderer::ui_render_requests.emplace_back(image_name, x, y, r, g, b, a, sorting_order);
}

// Note: If Draw is called first, then the actor is destroyed, then it only reflects in the next frame
void ImageDB::Draw(std::string image_name, float x, float y)
{
	Renderer::image_draw_requests.emplace_back(image_name, x, y);
}

void ImageDB::DrawEx(std::string image_name, float x, float y, float rotation_degrees, float scale_x, float scale_y, float pivot_x, float pivot_y, float r, float g, float b, float a, float sorting_order)
{
	Renderer::image_draw_requests.emplace_back(image_name, x, y, rotation_degrees, scale_x, scale_y, pivot_x, pivot_y, r, g, b, a, sorting_order);
}

void ImageDB::DrawPixel(float x, float y, float r, float g, float b, float a)
{
	Renderer::draw_pixel_requests.emplace_back(x, y, r, g, b, a);
}

