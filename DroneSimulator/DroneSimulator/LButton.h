//==============================================================
// Filename : LButton.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : 
//==============================================================

#pragma once
#include <SDL.h>
#include <SDL_image.h>

class LButton
{
public:
	LButton() : pushed(false) {
		buttonInfo.x = 0;
		buttonInfo.y = 0;
		buttonInfo.w = 0;
		buttonInfo.h = 0;
	}

	~LButton() {}

	void setPosition(int x, int y, int w, int h) {
		buttonInfo.x = x - 10;
		buttonInfo.y = y - 10;
		buttonInfo.w = w + 20;
		buttonInfo.h = h + 20;
	}

	void handleEvent(SDL_Event* e) {
		if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			int x, y;
			SDL_GetMouseState(&x, &y);

			if ((x < buttonInfo.x) || (x > (buttonInfo.x + buttonInfo.w)) || (y < buttonInfo.y) || (y > (buttonInfo.y + buttonInfo.h))) return;

			this->pushed = true;
		}
	}

	bool isPushed() { return pushed; }

	void clearPushedFlag() { pushed = false; }

	SDL_Rect getPosition() { return buttonInfo; }

private:
	SDL_Rect buttonInfo;
	bool pushed;
};