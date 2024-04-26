#include "Input.h"

void Input::Init()
{
	// Init all keys to UP state
	for (int code = SDL_SCANCODE_UNKNOWN; code < SDL_NUM_SCANCODES; code++)
		keyboard_states[static_cast<SDL_Scancode>(code)] = INPUT_STATE_UP;
}

void Input::ProcessEvent(const SDL_Event& e)
{
	// This is basically a state machine
	if (e.type == SDL_KEYDOWN) 
	{
		keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_DOWN;
		just_became_down_scancodes.push_back(e.key.keysym.scancode);
	}

	else if (e.type == SDL_KEYUP) 
	{
		keyboard_states[e.key.keysym.scancode] = INPUT_STATE_JUST_BECAME_UP;
		just_became_up_scancodes.push_back(e.key.keysym.scancode);
	}

	else if (e.type == SDL_MOUSEBUTTONDOWN) 
	{
		mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_DOWN;
		just_became_down_buttons.push_back(e.button.button);
	}

	else if (e.type == SDL_MOUSEBUTTONUP) 
	{
		mouse_button_states[e.button.button] = INPUT_STATE_JUST_BECAME_UP;
		just_became_up_buttons.push_back(e.button.button);
	}

	else if (e.type == SDL_MOUSEMOTION)
	{
		mouse_position.x = static_cast<float>(e.motion.x);
		mouse_position.y = static_cast<float>(e.motion.y);
	}

	else if (e.type == SDL_MOUSEWHEEL)
		mouse_scroll_this_frame = e.wheel.preciseY;
}

void Input::LateUpdate()
{
	for (const SDL_Scancode& code : just_became_down_scancodes)
		keyboard_states[code] = INPUT_STATE_DOWN;
	just_became_down_scancodes.clear();

	for (const SDL_Scancode& code : just_became_up_scancodes)
		keyboard_states[code] = INPUT_STATE_UP;
	just_became_up_scancodes.clear();

	for (int button : just_became_down_buttons)
		mouse_button_states[button] = INPUT_STATE_DOWN;
	just_became_down_buttons.clear();

	for (int button : just_became_up_buttons)
		mouse_button_states[button] = INPUT_STATE_UP;
	just_became_up_buttons.clear();

	mouse_scroll_this_frame = 0.0f;
}

bool Input::GetKey(SDL_Scancode keycode)
{
	return keyboard_states[keycode] == INPUT_STATE_DOWN || keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyDown(SDL_Scancode keycode)
{
	return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetKeyUp(SDL_Scancode keycode)
{
	return keyboard_states[keycode] == INPUT_STATE_JUST_BECAME_UP;
}

glm::vec2 Input::GetMousePosition()
{
	return mouse_position;
}

bool Input::GetMouseButton(int button)
{
	return mouse_button_states[button] == INPUT_STATE_DOWN || mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonDown(int button)
{
	return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_DOWN;
}

bool Input::GetMouseButtonUp(int button)
{
	return mouse_button_states[button] == INPUT_STATE_JUST_BECAME_UP;
}

float Input::GetMouseScrollDelta()
{
	return mouse_scroll_this_frame;
}

SDL_Scancode Input::GetScancodeFromName(std::string key)
{
	// Unknown Key will never be true (not getting them from actual input)
	SDL_Scancode scancode = SDL_GetScancodeFromName(key.c_str());

	if (key == "lshift")
		scancode = SDL_SCANCODE_LSHIFT;
	if (key == "rshift")
		scancode = SDL_SCANCODE_RSHIFT;
	if (key == "lctrl")
		scancode = SDL_SCANCODE_LCTRL;
	if (key == "rctrl")
		scancode = SDL_SCANCODE_RCTRL;
	if (key == "lalt")
		scancode = SDL_SCANCODE_LALT;
	if (key == "ralt")
		scancode = SDL_SCANCODE_RALT;
	if (key == "enter")
		scancode = SDL_SCANCODE_RETURN;

	return scancode;
}
