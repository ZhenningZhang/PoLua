#pragma once

#include "glm/glm.hpp"

class Camera
{
public:
	// HW6 Continuous World
	static inline glm::vec2 camera_position;
	static inline float zoom_factor = 1.0f;

	static void SetPosition(float x, float y)
	{
		camera_position.x = x;
		camera_position.y = y;
	}

	static float GetPositionX()
	{
		return camera_position.x;
	}

	static float GetPositionY()
	{
		return camera_position.y;
	}

	static void SetZoom(float zoom_factor_target)
	{
		zoom_factor = zoom_factor_target;
	}

	static float GetZoom()
	{
		return zoom_factor;
	}
};

