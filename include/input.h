#pragma once

enum MOUSE_INPUT
{
	LEFT_BUTTON   = (1 << 0),
	MIDDLE_BUTTON = (1 << 1),
	RIGHT_BUTTON  = (1 << 2),

	MOUSE_MOVE    = (1 << 3),

	NUM_MOUSE_INPUT = 4
};
static_assert(NUM_MOUSE_INPUT < 8, "need additional bits for mouse input");

struct Input
{
	char mouseInput;
	char keyInput;
	int x;
	int y;
};

void Input_Init();
void Input_Deinit();

void Input_Update(const Input& input);
