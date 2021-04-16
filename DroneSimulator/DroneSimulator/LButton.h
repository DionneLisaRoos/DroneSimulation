//==============================================================
// Filename : LButton.h
// Authors : Dionne Ariëns and Marnick Los.
// Version : -
// License : -
// Description : This class is used in the GUI class and holds information about the buttons that are created in the GUI.
//==============================================================

#pragma once
#include <SDL.h>

class LButton
{
public:
	// set initial parameters to 0
	LButton() : pushed(false) {
		buttonInfo.x = 0;
		buttonInfo.y = 0;
		buttonInfo.w = 0;
		buttonInfo.h = 0;
	}

	~LButton() {}

	// Set position and size of the button 
	void setPosition(int x, int y, int w, int h) {
		buttonInfo.x = x - 10;
		buttonInfo.y = y - 10;
		buttonInfo.w = w + 20;
		buttonInfo.h = h + 20;
	}

	// This function is executed when a SDL event is detected. 
	void handleEvent(SDL_Event* e) {
		// Only do something when the mousebutton is detected to be pushed down
		if (e->type == SDL_MOUSEBUTTONDOWN)
		{
			// get the x and y position of the mouse 
			int x, y;
			SDL_GetMouseState(&x, &y);

			// Check whether the mouse position is inside the button rectangle
			if ((x < buttonInfo.x) || (x > (buttonInfo.x + buttonInfo.w)) || (y < buttonInfo.y) || (y > (buttonInfo.y + buttonInfo.h))) return;

			// If the mouse position is inside the button, set the pushed boolean high
			this->pushed = true;
		}
	}

	// Returns whether the button had been pressed
	bool isPushed() { return pushed; }

	// Clears the pushed button flag so it can be pressed again
	void clearPushedFlag() { pushed = false; }

	// retuns the position and size of the button.
	SDL_Rect getPosition() { return buttonInfo; }

private:
	// Private object and variable
	SDL_Rect buttonInfo;
	bool pushed;
};