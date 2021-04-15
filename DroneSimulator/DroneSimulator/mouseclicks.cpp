/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>

//Screen dimension constants
const int SCREEN_WIDTH1 = 640;
const int SCREEN_HEIGHT1 = 400;

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

enum class Images
{
	IMAGE_DRONE = 0,
	IMAGE_DRONE_AND_CARGO = 1,
	IMAGE_TITLE = 2,
	IMAGE_TEXT_DRONE = 3,
	IMAGE_TEXT_DRONE_AND_CARGO = 4,
	IMAGE_KEYBOARD = 5,
	IMAGE_CSV_FILE = 6,
	IMAGE_TEXT_KEYBOARD = 7,
	IMAGE_TEXT_CSV_FILE = 8,
	IMAGE_FILENAME = 9,
	TOTAL_IMAGES = 10
};

//Texture wrapper class
class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

#if defined(SDL_TTF_MAJOR_VERSION)d
	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Renders texture at given point
	void renderr(int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

	//Gets image dimensions
	int getWidth();
	int getHeight();

	SDL_Texture* mTexture;

private:
	//The actual hardware texture
	

	//Image dimensions
	int mWidth;
	int mHeight;
};

//The mouse button
class LButton
{
public:
	//Initializes internal variables
	LButton();

	//Sets top left position
	void setPosition(int x, int y);

	//Handles mouse event
	void handleEvent(SDL_Event* e);

	bool isPushed() { return pushed; }
	void clearPushedFlag() { pushed = false; }

	//Shows button sprite
	//void render();

private:
	//Top left position
	SDL_Point mPosition;
	bool pushed;

	//Currently used global sprite
	LButtonSprite mCurrentSprite;
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Mouse button sprites
SDL_Rect gImages[static_cast<int>(Images::TOTAL_IMAGES)];
LTexture gDrone;
LTexture gDroneWithCargo;
LTexture gTitle;
LTexture gtextDrone;
LTexture gtextDroneWithCargo;
LTexture gKeyboard;
LTexture gCSVfile;
LTexture gtextKeyboard;
LTexture gtextCSVfile;
LTexture gfileName;

//Buttons objects
LButton gButtons[2];

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

bool LTexture::loadFromFile(std::string path)
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//TODO: uncommand, setcolorkey. 
		//Color key image
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	//Modulate texture rgb
	SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
	//Set blending function
	SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::renderr(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;
	pushed = false;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
	mPosition.x = x;
	mPosition.y = y;
}

void LButton::handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEBUTTONDOWN)
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
			//mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			if (e->type == SDL_MOUSEBUTTONDOWN) {
				//std::cout << "button pushed\n" << this->mCurrentSprite << std::endl;
				this->pushed = true;
			}
			//Set mouse over sprite
			/*switch (e->type)
			{
			case SDL_MOUSEMOTION:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;

			case SDL_MOUSEBUTTONDOWN:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;

			case SDL_MOUSEBUTTONUP:
				mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}*/
		}
	}
}

void render(bool choice)
{
	if (!choice) {
		SDL_RenderCopyEx(gRenderer, gDrone.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_DRONE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gDroneWithCargo.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gtextDrone.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gTitle.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TITLE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gtextDroneWithCargo.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)], 0.0, NULL, SDL_FLIP_NONE);
	}
	else {
		SDL_RenderCopyEx(gRenderer, gCSVfile.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_CSV_FILE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gKeyboard.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_KEYBOARD)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gtextCSVfile.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gTitle.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TITLE)], 0.0, NULL, SDL_FLIP_NONE);
		SDL_RenderCopyEx(gRenderer, gtextKeyboard.mTexture, NULL, &gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)], 0.0, NULL, SDL_FLIP_NONE);
	}
	
}

SDL_Rect fillImagePosition(int x, int y, LTexture texture) {
	SDL_Rect im;
	im.x = x;
	im.y = y;
	im.w = texture.getWidth();
	im.h = texture.getHeight();

	return im;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH1, SCREEN_HEIGHT1, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprites
	if (!gDrone.loadFromFile("drone.png")) printf("Failed to load button sprite texture!\n");
	if (!gDroneWithCargo.loadFromFile("DroneAndCargo.png")) printf("Failed to load button sprite texture!\n");
	if (!gKeyboard.loadFromFile("Keyboard.png")) printf("Failed to load button sprite texture!\n");
	if (!gCSVfile.loadFromFile("CSV.png")) printf("Failed to load button sprite texture!\n");
	if (!gtextDrone.loadFromFile("DroneChoice.png")) printf("Failed to load button sprite texture!\n");
	if (!gtextDroneWithCargo.loadFromFile("DroneWithCargoChoice.png")) printf("Failed to load button sprite texture!\n");
	if (!gTitle.loadFromFile("Title.png")) printf("Failed to load button sprite texture!\n");
	if (!gtextKeyboard.loadFromFile("KeyboardInputChoice.png")) printf("Failed to load button sprite texture!\n");
	if (!gtextCSVfile.loadFromFile("InputFileChoice.png")) printf("Failed to load button sprite texture!\n");
	if (!gfileName.loadFromFile("FileName.png")) printf("Failed to load button sprite texture!\n");
	//{
		//printf("Failed to load button sprite texture!\n");
		//success = false;
	//}
	//else
	//{
		//Set sprites

	gImages[static_cast<int>(Images::IMAGE_DRONE)] = fillImagePosition(60, 180, gDrone);
	//gImages[static_cast<int>(Images::IMAGE_DRONE)].x = 60;
	//gImages[static_cast<int>(Images::IMAGE_DRONE)].y = 180;
	//gImages[static_cast<int>(Images::IMAGE_DRONE)].w = gDrone.getWidth();
	//gImages[static_cast<int>(Images::IMAGE_DRONE)].h = gDrone.getHeight();

	gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)] = fillImagePosition(380, 180, gDroneWithCargo);
	/*gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)].x = 380;
	gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)].y = 180;
	gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)].w = gDroneWithCargo.getWidth();
	gImages[static_cast<int>(Images::IMAGE_DRONE_AND_CARGO)].h = gDroneWithCargo.getHeight();*/

	gImages[static_cast<int>(Images::IMAGE_TITLE)] = fillImagePosition(200, 10, gTitle);
	//gImages[static_cast<int>(Images::IMAGE_TITLE)].x = 200;
	//gImages[static_cast<int>(Images::IMAGE_TITLE)].y = 10;
	//gImages[static_cast<int>(Images::IMAGE_TITLE)].w = gTitle.getWidth();
	//gImages[static_cast<int>(Images::IMAGE_TITLE)].h = gTitle.getHeight();

	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)] = fillImagePosition(120, 120, gtextDrone);
	/*gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)].x = 120;
	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)].y = 120;
	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)].w = gtextDrone.getWidth();
	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE)].h = gtextDrone.getHeight();*/

	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)] = fillImagePosition(400, 120, gtextDroneWithCargo);
	/*gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)].x = 400;
	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)].y = 120;
	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)].w = gtextDroneWithCargo.getWidth();
	gImages[static_cast<int>(Images::IMAGE_TEXT_DRONE_AND_CARGO)].h = gtextDroneWithCargo.getHeight();*/

	gImages[static_cast<int>(Images::IMAGE_CSV_FILE)] = fillImagePosition(60, 180, gCSVfile);
	//gImages[static_cast<int>(Images::IMAGE_CSV_FILE)].x = 60;
	//gImages[static_cast<int>(Images::IMAGE_CSV_FILE)].y = 180;
	//gImages[static_cast<int>(Images::IMAGE_CSV_FILE)].w = gCSVfile.getWidth();
	//gImages[static_cast<int>(Images::IMAGE_CSV_FILE)].h = gCSVfile.getHeight();

	gImages[static_cast<int>(Images::IMAGE_KEYBOARD)] = fillImagePosition(380, 180, gKeyboard);
	/*gImages[static_cast<int>(Images::IMAGE_KEYBOARD)].x = 380;
	gImages[static_cast<int>(Images::IMAGE_KEYBOARD)].y = 180;
	gImages[static_cast<int>(Images::IMAGE_KEYBOARD)].w = gKeyboard.getWidth();
	gImages[static_cast<int>(Images::IMAGE_KEYBOARD)].h = gKeyboard.getHeight();*/

	gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)] = fillImagePosition(60, 120, gtextCSVfile);
	/*gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)].x = 60;
	gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)].y = 120;
	gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)].w = gtextCSVfile.getWidth();
	gImages[static_cast<int>(Images::IMAGE_TEXT_CSV_FILE)].h = gtextCSVfile.getHeight();*/

	gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)] = fillImagePosition(450, 120, gtextKeyboard);
	/*gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)].x = 450;
	gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)].y = 120;
	gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)].w = gtextKeyboard.getWidth();
	gImages[static_cast<int>(Images::IMAGE_TEXT_KEYBOARD)].h = gtextKeyboard.getHeight();*/


		/*for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i)
		{
			gImages [i].x = 0;
			gImage[i].y = i * 200;
			gImages[i].w = BUTTON_WIDTH;
			gImages[i].h = BUTTON_HEIGHT;
		}*/

		//Set buttons in corners
		gButtons[0].setPosition(60, 180);
		gButtons[1].setPosition(380, 180);
	//}

	return success;
}

void close()
{
	//Free loaded images
	gDrone.free();
	gDroneWithCargo.free();
	gtextCSVfile.free();
	gtextDrone.free();
	gtextDroneWithCargo.free();
	gtextKeyboard.free();
	gKeyboard.free();
	gCSVfile.free();
	gTitle.free();
	gfileName.free();

	//Destroy window	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char* args[])
{
	bool choice = false;
	//Start up SDL and create window
	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media
		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while (!quit)
			{
				//Handle events on queue
				while (SDL_PollEvent(&e) != 0)
				{
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}

					//Handle button events
					for (int i = 0; i < 2; ++i)
					{
						gButtons[i].handleEvent(&e);
					}
				}

				if (gButtons[0].isPushed() && choice) {
					std::cout << "first button pushed again\n";
					quit = true;
				}
				else if (gButtons[1].isPushed() && choice) {
					std::cout << "second button pushed again\n";
					quit = true;
				}
				else if (gButtons[0].isPushed()) {
					std::cout << "first button pushed\n";
					gButtons[0].clearPushedFlag();
					choice = true;
				}
				else if (gButtons[1].isPushed()) {
					std::cout << "second button pushed\n";
					gButtons[1].clearPushedFlag();
					choice = true;
				}

				

				//Clear screen
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				//Render buttons
				render(choice);
				/*for (int i = 0; i < TOTAL_BUTTONS; ++i)
				{
					gButtons[i].render();
				}*/

				//Update screen
				SDL_RenderPresent(gRenderer);
			}

		}
	}

	//Free resources and close SDL
	close();

	return 0;
}