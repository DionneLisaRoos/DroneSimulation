#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "LTexture.h"

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 2;

enum class LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton(SDL_Rect SpriteClips, LTexture ButtonSpriteSheetTexture) {
		mPosition.x = 0;
		mPosition.y = 0;

		mCurrentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_OUT;
		gSpriteClips = SpriteClips;
		gButtonSpriteSheetTexture = ButtonSpriteSheetTexture;
	}

	//Sets top left position
	void setPosition(int x, int y) {
		mPosition.x = x;
		mPosition.y = y;
	}

	//Handles mouse event
	void handleEvent(SDL_Event* e) {
		//If mouse event happened
		if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
		{
			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//Check if mouse is in button
			bool inside = true;

			//Mouse is left of the button
			if (x < mPosition.x)
			{
				inside = false;
			}
			//Mouse is right of the button
			else if (x > mPosition.x + BUTTON_WIDTH)
			{
				inside = false;
			}
			//Mouse above the button
			else if (y < mPosition.y)
			{
				inside = false;
			}
			//Mouse below the button
			else if (y > mPosition.y + BUTTON_HEIGHT)
			{
				inside = false;
			}

			//Mouse is outside button
			if (!inside)
			{
				mCurrentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_OUT;
			}
			//Mouse is inside button
			else
			{
				//Set mouse over sprite
				switch (e->type)
				{
				case SDL_MOUSEMOTION:
					mCurrentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;

				case SDL_MOUSEBUTTONDOWN:
					mCurrentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN;
					break;

				case SDL_MOUSEBUTTONUP:
					mCurrentSprite = LButtonSprite::BUTTON_SPRITE_MOUSE_UP;
					break;
				}
			}
		}
	}

	//Shows button sprite
	void render() {
		//Show current button sprite
		gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
	}

private:
	//Top left position
	SDL_Point mPosition;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
	SDL_Rect gSpriteClips;
	LTexture gButtonSpriteSheetTexture;

};
