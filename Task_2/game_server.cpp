/*This source code copyrighted by Lazy Foo' Productions (2004-2022)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, vectors, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include<bits/stdc++.h>
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>
#include<iostream>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>

#include "health_server.h"

using namespace std;

//generating random points for task of plantation.

int total=0;
bool finalwindow=false;

bool tplant1=false;
bool tplant2=false;
bool tplant3=false;

int ip1;
int ip2;
int ip3;

int x_plant1[]={527,713};
int x_plant2[]={221,859};
int x_plant3[]={757,835};

int y_plant1[]={646,103};
int y_plant2[]={295,768};
int y_plant3[]={201,363};

bool twater1=false;
bool twater2=false;
bool twater3=false;

int iw1;
int iw2;
int iw3;

int x_water1[]={644,203};
int x_water2[]={1042,872};
int x_water3[]={685,1240};

int y_water1[]={181,394};
int y_water2[]={768,661};
int y_water3[]={765,120};

bool tsolar1=false;
bool tsolar2=false;
bool tsolar3=false;

int is1;
int is2;
int is3;

int x_solar1[]={527,266};
int x_solar2[]={856,998};
int x_solar3[]={487,1093};

int y_solar1[]={271,658};
int y_solar2[]={555,124};
int y_solar3[]={96,174};

bool tbin1=false;
bool tbin2=false;
bool tbin3=false;

int ib1;
int ib2;
int ib3;

int x_bin1[]={656,224};
int x_bin2[]={1210,1139};
int x_bin3[]={1015,37};

int y_bin1[]={286,508};
int y_bin2[]={753,664};
int y_bin3[]={450,360};

bool c_plant1=false;
bool c_plant2=false;
bool c_plant3=false;
bool c_bin1=false;
bool c_bin2=false;
bool c_bin3=false;
bool c_water1=false;
bool c_water2=false;
bool c_water3=false;
bool c_solar1=false;
bool c_solar2=false;
bool c_solar3=false;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;

//Button constants
const int BUTTON_WIDTH = 300;
const int BUTTON_HEIGHT = 200;
const int TOTAL_BUTTONS = 4;

void error(const char *msg){
	perror(msg);
	exit(1);
}

// enum LButtonSprite
// {
// 	BUTTON_SPRITE_MOUSE_OUT = 0,
// 	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
// 	BUTTON_SPRITE_MOUSE_DOWN = 2,
// 	BUTTON_SPRITE_MOUSE_UP = 3,
// 	BUTTON_SPRITE_TOTAL = 4
// };

//A circle stucture
struct Circle
{
	int x, y;
	int r;
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
		bool loadFromFile( std::string path );
		
		#if defined(SDL_TTF_MAJOR_VERSION)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

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
		void setPosition( int x, int y );

		//Handles mouse event
		char handleEvent( SDL_Event* e );
	
		//Shows button sprite
		void render();

	private:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		// LButtonSprite mCurrentSprite;
};

//The dot that will move around on the screen
class Dot
{
    public:
		//The dimensions of the dot
		static const int DOT_WIDTH = 20;
		static const int DOT_HEIGHT = 20;

		//Maximum axis velocity of the dot
		int DOT_VEL = 1;

		//Initializes the variables
		Dot( int x, int y );

		//Takes key presses and adjusts the dot's velocity
		char handleEvent( SDL_Event& e );

		//Moves the dot and checks collision
		void move( SDL_Rect& square, Circle& circle );

		//Shows the dot on the screen
		void render();

		//Gets collision circle
		Circle& getCollider();

		//The X and Y offsets of the dot
		int mPosX, mPosY;

    private:
		

		//The velocity of the dot
		int mVelX, mVelY;
		
		//Dot's collision circle
		Circle mCollider;

		//Moves the collision circle relative to the dot's offset
		void shiftColliders();
};

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

SDL_Texture* loadTexture(std::string path);

//Loads individual image
SDL_Surface* loadSurface( std::string path );

//Circle/Circle collision detector
bool checkCollision( Circle& a, Circle& b );

//Circle/Box collision detector
bool checkCollision( Circle& a, SDL_Rect& b );

//Calculates distance squared between two points
double distanceSquared( int x1, int y1, int x2, int y2 );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//Current displayed PNG image
SDL_Surface* gPNGSurface = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

Mix_Music *satpura = NULL;
Mix_Music *jwala = NULL;
Mix_Music *acadarea = NULL;
Mix_Music *amul = NULL;
Mix_Music *aravali = NULL;
Mix_Music *bharti = NULL;
Mix_Music *bike = NULL;
Mix_Music *ccd = NULL;
Mix_Music *centrallib = NULL;
Mix_Music *cyclee = NULL;
Mix_Music *d16 = NULL;
Mix_Music *dograhall = NULL;
Mix_Music *garden = NULL;
Mix_Music *girnar = NULL;
Mix_Music *hospital = NULL;
Mix_Music *kara = NULL;
Mix_Music *kumaon = NULL;
Mix_Music *lhc = NULL;
Mix_Music *maingr = NULL;
Mix_Music *nalandagr = NULL;
Mix_Music *nil = NULL;
Mix_Music *sbi = NULL;
Mix_Music *shiva = NULL;
Mix_Music *staffc = NULL;
Mix_Music *udai = NULL;
Mix_Music *vindy = NULL;
Mix_Music *zanskar = NULL;

SDL_Texture* gTexture = NULL;
SDL_Texture* winner = NULL;
SDL_Texture* startMenu = NULL;

SDL_Texture* plant1 = NULL;
SDL_Texture* plant2 = NULL;
SDL_Texture* plant3 = NULL;

SDL_Texture* bin1 = NULL;
SDL_Texture* bin2 = NULL;
SDL_Texture* bin3 = NULL;

SDL_Texture* solar1 = NULL;
SDL_Texture* solar2 = NULL;
SDL_Texture* solar3 = NULL;

SDL_Texture* water1 = NULL;
SDL_Texture* water2 = NULL;
SDL_Texture* water3 = NULL;

SDL_Texture* cplant1 = NULL;
SDL_Texture* cplant2 = NULL;
SDL_Texture* cplant3 = NULL;

SDL_Texture* cbin1 = NULL;
SDL_Texture* cbin2 = NULL;
SDL_Texture* cbin3 = NULL;

SDL_Texture* csolar1 = NULL;
SDL_Texture* csolar2 = NULL;
SDL_Texture* csolar3 = NULL;

SDL_Texture* cwater1 = NULL;
SDL_Texture* cwater2 = NULL;
SDL_Texture* cwater3 = NULL;


//Scene textures
LTexture gDotTexture;

//Mouse button sprites
// SDL_Rect gSpriteClips[ BUTTON_SPRITE_TOTAL ];
// LTexture gButtonSpriteSheetTexture;

//Buttons objects
LButton gButtons[ TOTAL_BUTTONS ]; 

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

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
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

	// mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition( int x, int y )
{
	mPosition.x = x;
	mPosition.y = y;
}

char LButton::handleEvent( SDL_Event* e )
{
	//If mouse event happened
	if( e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP )
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

        // cout<<x<<" "<<y<<endl;

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if( x < mPosition.x )
		{
			inside = false;
		}
		//Mouse is right of the button
		else if( x > mPosition.x + BUTTON_WIDTH )
		{
			inside = false;
		}
		//Mouse above the button
		else if( y < mPosition.y )
		{
			inside = false;
		}
		//Mouse below the button
		else if( y > mPosition.y + BUTTON_HEIGHT )
		{
			inside = false;
		}

		//Mouse is outside button
		if( !inside )
		{
			// mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}
		//Mouse is inside button
		else
		{
			char screen;
			if (e->type = SDL_MOUSEBUTTONUP && x>300 && x<800 && y>200 && y<600)
				return 'p';   // p - play buttton pressed
			else
				return 'n';   // n - no button pressed

			// Set mouse over sprite
			switch( e->type )
			{
				case SDL_MOUSEMOTION:
				// mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
				break;
			
				case SDL_MOUSEBUTTONDOWN:
				// mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
				break;
				
				case SDL_MOUSEBUTTONUP:
				// mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
				break;
			}
		}
	}
}
	
void LButton::render()
{
	//Show current button sprite
	// gButtonSpriteSheetTexture.render( mPosition.x, mPosition.y, &gSpriteClips[ mCurrentSprite ] );
}

Dot::Dot( int x, int y )
{
    //Initialize the offsets
    mPosX = x;
    mPosY = y;

	//Set collision circle size
	mCollider.r = DOT_WIDTH / 2;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

	//Move collider relative to the circle
	shiftColliders();
}

bool cycle = false;
bool motor = false;
bool audio = false;

char Dot::handleEvent( SDL_Event& e )
{

	if (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=415 && mPosX <= 474 && mPosY >= 226 && mPosY <= 297))){
		audio = true;
		Mix_PlayMusic(satpura,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=35 && mPosX <= 120 && mPosY >= 75 && mPosY <= 165))){
		audio = true;
		Mix_PlayMusic(jwala,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=783 && mPosX <= 911 && mPosY >= 128 && mPosY <= 228))){
		audio = true;
		Mix_PlayMusic(acadarea,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=864 && mPosX <= 891 && mPosY >= 424 && mPosY <= 447))){
		audio = true;
		Mix_PlayMusic(amul,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=25 && mPosX <= 120 && mPosY >= 240 && mPosY <= 334))){
		audio = true;
		Mix_PlayMusic(aravali,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=600 && mPosX <= 754 && mPosY >= 329 && mPosY <= 459))){
		audio = true;
		Mix_PlayMusic(bharti,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

    else if (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && (((mPosX >=537 && mPosX <= 577 && mPosY >= 514 && mPosY <= 561)||(mPosX >=142 && mPosX <= 189 && mPosY >= 664 && mPosY <= 714)||(mPosX >=1132 && mPosX <= 1181 && mPosY >= 298 && mPosY <= 348)))){
		audio = true;
		Mix_PlayMusic(bike,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=869 && mPosX <= 917 && mPosY >= 253 && mPosY <= 298))){
		audio = true;
		Mix_PlayMusic(ccd,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=915 && mPosX <= 963 && mPosY >= 341 && mPosY <= 409))){
		audio = true;
		Mix_PlayMusic(centrallib,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && (((mPosX >=148 && mPosX <= 196 && mPosY >= 214 && mPosY <= 265)||(mPosX >=614 && mPosX <= 650 && mPosY >= 511 && mPosY <= 547||(mPosX >=113 && mPosX <= 1176 && mPosY >= 66 && mPosY <= 116))))){
		audio = true;
		Mix_PlayMusic(cyclee,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=145 && mPosX <= 186 && mPosY >= 301 && mPosY <= 341))){
		audio = true;
		Mix_PlayMusic(d16,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=992 && mPosX <= 1150 && mPosY >= 222 && mPosY <= 291))){
		audio = true;
		Mix_PlayMusic(dograhall,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=1189 && mPosX <= 1236 && mPosY >=172 && mPosY <= 232))){
		audio = true;
		Mix_PlayMusic(garden,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=537 && mPosX <= 601 && mPosY >= 119 && mPosY <= 200))){
		audio = true;
		Mix_PlayMusic(girnar,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=487 && mPosX <= 581 && mPosY >= 339 && mPosY <= 429))){
		audio = true;
		Mix_PlayMusic(hospital,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=25 && mPosX <= 120 && mPosY >= 410 && mPosY <= 504))){
		audio = true;
		Mix_PlayMusic(kara,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=176 && mPosX <= 231 && mPosY >= 97 && mPosY <= 165))){
		audio = true;
		Mix_PlayMusic(kumaon,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=1146 && mPosX <= 1208 && mPosY >= 436 && mPosY <= 489))){
		audio = true;
		Mix_PlayMusic(lhc,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=564 && mPosX <= 876 && mPosY >= 606 && mPosY <= 686))){
		audio = true;
		Mix_PlayMusic(maingr,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=135 && mPosX <= 261 && mPosY >= 469 && mPosY <= 605))){
		audio = true;
		Mix_PlayMusic(nalandagr,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=25 && mPosX <= 120 && mPosY >= 567 && mPosY <= 670))){
		audio = true;
		Mix_PlayMusic(nil,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=1052 && mPosX <= 1118 && mPosY >= 368 && mPosY <= 436))){
		audio = true;
		Mix_PlayMusic(sbi,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=286 && mPosX <= 353 && mPosY >= 287 && mPosY <= 361))){
		audio = true;
		Mix_PlayMusic(shiva,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=950 && mPosX <= 1000 && mPosY >= 521 && mPosY <= 575))){
		audio = true;
		Mix_PlayMusic(staffc,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=35 && mPosX <= 120 && mPosY >= 75 && mPosY <= 165))){
		audio = true;
		Mix_PlayMusic(udai,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=287 && mPosX <= 346 && mPosY >= 130 && mPosY <= 304))){
		audio = true;
		Mix_PlayMusic(vindy,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}
	
	else if  (audio == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN && ((mPosX >=361 && mPosX <= 410 && mPosY >= 381 && mPosY <= 442))){
		audio = true;
		Mix_PlayMusic(zanskar,0);
	}
	else if (audio == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		audio = false;
		Mix_HaltMusic();
	}

	//end all;

	if(finalwindow && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN){
		close();
	}

	//rendering options

	if(!c_plant1 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_plant1[ip1]-30&& mPosX <= x_plant1[ip1]+30 && mPosY >= y_plant1[ip1]-30&& mPosY <= y_plant1[ip1]+30))){
		c_plant1=true;
		total++;
	}

	if(!c_plant2 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_plant2[ip2]-30&& mPosX <= x_plant2[ip2]+30 && mPosY >= y_plant2[ip2]-30&& mPosY <= y_plant2[ip2]+30))){
		c_plant2=true;
		total++;
	}

	if(!c_plant3 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_plant3[ip3]-30&& mPosX <= x_plant3[ip3]+30 && mPosY >= y_plant3[ip3]-30&& mPosY <= y_plant3[ip3]+30))){
		c_plant3=true;
		total++;
	}

	if(!c_bin1 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_bin1[ib1]-30&& mPosX <= x_bin1[ib1]+30 && mPosY >= y_bin1[ib1]-30&& mPosY <= y_bin1[ib1]+30))){
		c_bin1=true;
		total++;
	}

	if(!c_bin2 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_bin2[ib2]-30&& mPosX <= x_bin2[ib2]+30 && mPosY >= y_bin2[ib2]-30&& mPosY <= y_bin2[ib2]+30))){
		c_bin2=true;
		total++;
	}

	if(!c_bin3 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_bin3[ib3]-30&& mPosX <= x_bin3[ib3]+30 && mPosY >= y_bin3[ib3]-30&& mPosY <= y_bin3[ib3]+30))){
		c_bin3=true;
		total++;
	}

	if(!c_solar1 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_solar1[is1]-30&& mPosX <= x_solar1[is1]+30 && mPosY >= y_solar1[is1]-30&& mPosY <= y_solar1[is1]+30))){
		c_solar1=true;
		total++;
	}

	if(!c_solar2 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_solar2[is2]-30&& mPosX <= x_solar2[is2]+30 && mPosY >= y_solar2[is2]-30&& mPosY <= y_solar2[is2]+30))){
		c_solar2=true;
		total++;
	}

	if(!c_solar3 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_solar3[is3]-30&& mPosX <= x_solar3[is3]+30 && mPosY >= y_solar3[is3]-30&& mPosY <= y_solar3[is3]+30))){
		c_solar3=true;
		total++;
	}

	if(!c_water1 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_water1[iw1]-30&& mPosX <= x_water1[iw1]+30 && mPosY >= y_water1[iw1]-30&& mPosY <= y_water1[iw1]+30))){
		c_water1=true;
		total++;
	}

	if(!c_water2 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_water2[iw2]-30&& mPosX <= x_water2[iw2]+30 && mPosY >= y_water2[iw2]-30&& mPosY <= y_water2[iw2]+30))){
		c_water2=true;
		total++;
	}

	if(!c_water3 && ((e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_t)&&(mPosX >=x_water3[iw3]-30&& mPosX <= x_water3[iw3]+30 && mPosY >= y_water3[iw3]-30&& mPosY <= y_water3[iw3]+30))){
		c_water3=true;
		total++;
	}


	// Velocity Changing methods
    if (cycle == false && motor == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE && ((mPosX >=147 && mPosX <= 200 && mPosY >= 208 && mPosY <= 268) || (mPosX >=610 && mPosX <= 652 && mPosY >= 503 && mPosY <= 554) || (mPosX >=1126 && mPosX <= 63 && mPosY >= 1183 && mPosY <= 124))){
        cycle = true;
        DOT_VEL = 3;
    }
	else if (cycle == false && motor == false && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE && ((mPosX >=141 && mPosX <= 188 && mPosY >= 659 && mPosY <= 722) || (mPosX >=533 && mPosX <= 577 && mPosY >= 507 && mPosY <= 562) || (mPosX >1132 && mPosX <= 1183 && mPosY >= 294 && mPosY <= 356))){
        motor = true;
        DOT_VEL = 4;
    }
    else if (cycle == true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE && ((mPosX >=147 && mPosX <= 200 && mPosY >= 208 && mPosY <= 268) || (mPosX >=610 && mPosX <= 652 && mPosY >= 503 && mPosY <= 554) || (mPosX >=1126 && mPosX <= 63 && mPosY >= 1183 && mPosY <= 124))){
        cycle = false;
        DOT_VEL = 1;
    }
    else if (motor = true && e.type == SDL_KEYDOWN && e.key.repeat == 0 && e.key.keysym.sym == SDLK_SPACE && ((mPosX >=141 && mPosX <= 188 && mPosY >= 659 && mPosY <= 722) || (mPosX >=533 && mPosX <= 577 && mPosY >= 507 && mPosY <= 562) || (mPosX >1132 && mPosX <= 1183 && mPosY >= 294 && mPosY <= 356))){
        motor = false;
        DOT_VEL = 1;
    }
	else{

	}

    //If a key was pressed
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_DOWN: mVelY += DOT_VEL; break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
            case SDLK_UP: mVelY += DOT_VEL; break;
            case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
	if (e.type == SDL_KEYUP && e.key.repeat == 0 && e.key.keysym.sym == SDLK_RETURN)
		return 'p';   // play button == enter button
	else	  
		return 'n';  //no button == any other button
}

void Dot::move( SDL_Rect& square, Circle& circle )
{
    //Move the dot left or right
    mPosX += mVelX;
	shiftColliders();

    //If the dot collided or went too far to the left or right
	if( ( mPosX - mCollider.r < 0 ) || ( mPosX + mCollider.r > SCREEN_WIDTH ) || checkCollision( mCollider, square )  )  //|| checkCollision( mCollider, circle 
    {
        //Move back
        mPosX -= mVelX;
		shiftColliders();
    }

    //Move the dot up or down
    mPosY += mVelY;
	shiftColliders();

    //If the dot collided or went too far up or down
    if( ( mPosY - mCollider.r < 0 ) || ( mPosY + mCollider.r > SCREEN_HEIGHT ) || checkCollision( mCollider, square )  ) //|| checkCollision( mCollider, circle 
    {
        //Move back
        mPosY -= mVelY;
		shiftColliders();
    }
}

void Dot::render()
{
    //Show the dot
	gDotTexture.render( mPosX - mCollider.r, mPosY - mCollider.r );
}

Circle& Dot::getCollider()
{
	return mCollider;
}

void Dot::shiftColliders()
{
	//Align collider to center of dot
	mCollider.x = mPosX;
	mCollider.y = mPosY;
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				else
			    {
				    //Get window surface
				    gScreenSurface = SDL_GetWindowSurface( gWindow );
			    }

				//Initialize SDL_mixer
                if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                {
                    printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
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

    gTexture = loadTexture("iitd-map.jpg");
	winner = loadTexture("player1.png");
	startMenu = loadTexture("play.jpg");

	plant1 = loadTexture("planting.jpeg");
	plant2 = loadTexture("planting.jpeg");
	plant3 = loadTexture("planting.jpeg");

	bin1 = loadTexture("overflow_garbage.jpeg");
	bin2 = loadTexture("overflow_garbage.jpeg");
	bin3 = loadTexture("overflow_garbage.jpeg");

	solar1 = loadTexture("elec_down.jpeg");
	solar2 = loadTexture("elec_down.jpeg");
	solar3 = loadTexture("elec_down.jpeg");

	water1 = loadTexture("run_water.jpeg");
	water2 = loadTexture("run_water.jpeg");
	water3 = loadTexture("run_water.jpeg");

	cplant1 = loadTexture("planted.jpeg");
	cplant2 = loadTexture("planted.jpeg");
	cplant3 = loadTexture("planted.jpeg");

	cbin1 = loadTexture("proper_garbage.jpeg");
	cbin2 = loadTexture("proper_garbage.jpeg");
	cbin3 = loadTexture("proper_garbage.jpeg");

	csolar1 = loadTexture("elec_up.png");
	csolar2 = loadTexture("elec_up.png");
	csolar3 = loadTexture("elec_up.png");

	cwater1 = loadTexture("closed_water.jpeg");
	cwater2 = loadTexture("closed_water.jpeg");
	cwater3 = loadTexture("closed_water.jpeg");

	
satpura = Mix_LoadMUS("satpura.mp3");
	if (satpura == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	jwala = Mix_LoadMUS("jwala.mp3");
	if (jwala == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	acadarea = Mix_LoadMUS("acadarea.mp3");
	if (acadarea == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	aravali = Mix_LoadMUS("aravali.mp3");
	if (aravali == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	bharti = Mix_LoadMUS("bharti.mp3");
	if (bharti == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	bike = Mix_LoadMUS("bike.mp3");
	if (bike == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	amul = Mix_LoadMUS("amul.mp3");
	if (amul == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	ccd = Mix_LoadMUS("ccd.mp3");
	if (ccd == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	centrallib = Mix_LoadMUS("centrallib.mp3");
	if (centrallib == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	cyclee = Mix_LoadMUS("cyclee.mp3");
	if (cyclee == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	d16 = Mix_LoadMUS("d16.mp3");
	if (d16 == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	dograhall = Mix_LoadMUS("dograhall.mp3");
	if (dograhall == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	garden = Mix_LoadMUS("garden.mp3");
	if (garden == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	girnar = Mix_LoadMUS("girnar.mp3");
	if (girnar == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	hospital = Mix_LoadMUS("hospital.mp3");
	if (hospital == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	kara = Mix_LoadMUS("kara.mp3");
	if (kara == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	kumaon = Mix_LoadMUS("kumaon.mp3");
	if (kumaon == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	lhc = Mix_LoadMUS("lhc.mp3");
	if (lhc == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	maingr = Mix_LoadMUS("maingr.mp3");
	if (maingr == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	nalandagr = Mix_LoadMUS("nalandagr.mp3");
	if (nalandagr == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

	nil = Mix_LoadMUS("nil.mp3");
	if (nil == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	sbi = Mix_LoadMUS("sbi.mp3");
	if (sbi == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	shiva = Mix_LoadMUS("shiva.mp3");
	if (shiva == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	staffc = Mix_LoadMUS("staffc.mp3");
	if (staffc == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	udai = Mix_LoadMUS("udai.mp3");
	if (udai == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	vindy = Mix_LoadMUS("vindy.mp3");
	if (vindy == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}
	
	zanskar = Mix_LoadMUS("zanskar.mp3");
	if (zanskar == NULL){
		cout<<"MUSIC ERROR!!! "<<endl;
		success = false;
	}

    //Load sprites
	// if( !gButtonSpriteSheetTexture.loadFromFile( "17_mouse_events/button.png" ) )
	// {
	// 	printf( "Failed to load button sprite texture!\n" );
	// 	success = false;
	// // }
	// else
	// {
	// 	//Set sprites
	// 	for( int i = 0; i < BUTTON_SPRITE_TOTAL; ++i )
	// 	{
	// 		gSpriteClips[ i ].x = 0;
	// 		gSpriteClips[ i ].y = i * 200;
	// 		gSpriteClips[ i ].w = BUTTON_WIDTH;
	// 		gSpriteClips[ i ].h = BUTTON_HEIGHT;
	// 	}

		//Set buttons in corners
		gButtons[ 0 ].setPosition( 0, 0 );
		gButtons[ 1 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, 0 );
		gButtons[ 2 ].setPosition( 0, SCREEN_HEIGHT - BUTTON_HEIGHT );
		gButtons[ 3 ].setPosition( SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT );
	// }


    //Load PNG surface
	// gPNGSurface = loadSurface( "iitd-map.jpg" );
	// if( gPNGSurface == NULL )
	// {
	// 	printf( "Failed to load PNG image!\n" );
	// 	success = false;
	// }

	//Load dot texture
	if( !gDotTexture.loadFromFile( "dot.bmp" ) )
	{
		printf( "Failed to load dot texture!\n" );
		success = false;
	}

	return success;
}

int loadScreen(SDL_Texture* screen){
	bool quit = false;
	//Event handler
	SDL_Event e;

	Dot checkDot( Dot::DOT_WIDTH / 2, Dot::DOT_HEIGHT / 2 );

	while(!quit){
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
						goto maingame;
					}

					//Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
						char button = checkDot.handleEvent(e);
						if (button == 'p')
							goto maingame;
					}

					//Handle input for the dot
					// dot.handleEvent( e , dot);
				}
				SDL_RenderClear( gRenderer );
				SDL_RenderCopy( gRenderer, screen, NULL, NULL ); 
				SDL_RenderPresent( gRenderer );
			}
			maingame:	SDL_DestroyTexture(screen);
			screen = NULL;
			if (e.type == SDL_QUIT)
				return -1;
			else
				return 0;
}

void close()
{

    //Free loaded images
	// gButtonSpriteSheetTexture.free();

	//Free loaded images
	gDotTexture.free();

	Mix_FreeMusic(jwala);
	jwala = NULL;

	Mix_FreeMusic(satpura);
	satpura = NULL;

	Mix_FreeMusic(acadarea);
	acadarea = NULL;

	Mix_FreeMusic(amul);
	amul = NULL;
	
	Mix_FreeMusic(aravali);
	aravali = NULL;
	
	Mix_FreeMusic(bharti);
	bharti = NULL;

	Mix_FreeMusic(bike);
	bike = NULL;
	
	Mix_FreeMusic(ccd);
	ccd = NULL;
	
	Mix_FreeMusic(centrallib);
	centrallib = NULL;
	
	Mix_FreeMusic(cyclee);
	cyclee = NULL;
	
	Mix_FreeMusic(d16);
	d16 = NULL;
	
	Mix_FreeMusic(dograhall);
	dograhall = NULL;
	
	Mix_FreeMusic(garden);
	garden = NULL;
	
	Mix_FreeMusic(girnar);
	girnar = NULL;
	
	Mix_FreeMusic(hospital);
	hospital = NULL;
	
	Mix_FreeMusic(kara);
	kara = NULL;
	
	Mix_FreeMusic(kumaon);
	kumaon = NULL;
	
	Mix_FreeMusic(lhc);
	lhc = NULL;
	
	Mix_FreeMusic(maingr);
	maingr = NULL;
	
	Mix_FreeMusic(nalandagr);
	nalandagr = NULL;
	
	Mix_FreeMusic(nil);
	nil = NULL;
	
	Mix_FreeMusic(sbi);
	sbi = NULL;
	
	Mix_FreeMusic(shiva);
	shiva = NULL;
	
	Mix_FreeMusic(staffc);
	staffc = NULL;
	
	Mix_FreeMusic(udai);
	udai = NULL;
	
	Mix_FreeMusic(vindy);
	vindy = NULL;

	Mix_FreeMusic(zanskar);
	zanskar = NULL;

    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

	SDL_DestroyTexture(winner);
    winner = NULL;

	SDL_DestroyTexture(plant1);
    plant1 = NULL;

	SDL_DestroyTexture(plant2);
    plant2 = NULL;

	SDL_DestroyTexture(plant3);
    plant3 = NULL;

	SDL_DestroyTexture(bin1);
    bin1 = NULL;

	SDL_DestroyTexture(bin2);
    bin2 = NULL;

	SDL_DestroyTexture(bin3);
    bin3 = NULL;

	SDL_DestroyTexture(solar1);
    solar1 = NULL;

	SDL_DestroyTexture(solar2);
    solar2 = NULL;

	SDL_DestroyTexture(solar3);
    solar3 = NULL;

	SDL_DestroyTexture(water1);
    water1 = NULL;

	SDL_DestroyTexture(water2);
    water2 = NULL;

	SDL_DestroyTexture(water3);
    water3 = NULL;

	SDL_DestroyTexture(cplant1);
    cplant1 = NULL;

	SDL_DestroyTexture(cplant2);
    cplant2 = NULL;

	SDL_DestroyTexture(cplant3);
    cplant3 = NULL;

	SDL_DestroyTexture(cbin1);
    cbin1 = NULL;

	SDL_DestroyTexture(cbin2);
    cbin2 = NULL;

	SDL_DestroyTexture(cbin3);
    cbin3 = NULL;

	SDL_DestroyTexture(csolar1);
    csolar1 = NULL;

	SDL_DestroyTexture(csolar2);
    csolar2 = NULL;

	SDL_DestroyTexture(csolar3);
    csolar3 = NULL;

	SDL_DestroyTexture(cwater1);
    cwater1 = NULL;

	SDL_DestroyTexture(cwater2);
    cwater2 = NULL;

	SDL_DestroyTexture(cwater3);
    cwater3 = NULL;

    // //Free loaded image
	// SDL_FreeSurface( gPNGSurface );
	// gPNGSurface = NULL;

	// //Destroy window	
	// SDL_DestroyRenderer( gRenderer );
	// SDL_DestroyWindow( gWindow );
	// gWindow = NULL;
	// gRenderer = NULL;

	// //Quit SDL subsystems
	// IMG_Quit();
	// SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
        if( newTexture == NULL )
        {
            printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

bool checkCollision( Circle& a, Circle& b )
{
	//Calculate total radius squared
	int totalRadiusSquared = a.r + b.r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

    //If the distance between the centers of the circles is less than the sum of their radii
    if( distanceSquared( a.x, a.y, b.x, b.y ) < ( totalRadiusSquared ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

bool checkCollision( Circle& a, SDL_Rect& b )
{
    //Closest point on collision box
    int cX, cY;

    //Find closest x offset
    if( a.x < b.x )
    {
        cX = b.x;
    }
    else if( a.x > b.x + b.w )
    {
        cX = b.x + b.w;
    }
    else
    {
        cX = a.x;
    }

    //Find closest y offset
    if( a.y < b.y )
    {
        cY = b.y;
    }
    else if( a.y > b.y + b.h )
    {
        cY = b.y + b.h;
    }
    else
    {
        cY = a.y;
    }

    //If the closest point is inside the circle
    if( distanceSquared( a.x, a.y, cX, cY ) < a.r * a.r )
    {
        //This box and the circle have collided
        return true;
    }

    //If the shapes have not collided
    return false;
}

double distanceSquared( int x1, int y1, int x2, int y2 )
{
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

SDL_Surface* loadSurface( std::string path )
{
	//The final optimized image
	SDL_Surface* optimizedSurface = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Convert surface to screen format
		optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, 0 );
		if( optimizedSurface == NULL )
		{
			printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return optimizedSurface;
}


int main( int argc, char* args[] )
{
	if (argc < 2 || argc > 3)
	{
		cerr << "Usage: " << args[0] << " <game-port> [health-port]" << endl;
		return 1;
	}

	char* endptr = NULL;
	errno = 0;
	long game_port = strtol(args[1], &endptr, 10);
	if (errno != 0 || endptr == args[1] || *endptr != '\0' || game_port <= 0 || game_port > 65535)
	{
		cerr << "Invalid game port: " << args[1] << endl;
		return 1;
	}

	long health_port = HealthServer::DEFAULT_PORT;
	if (argc == 3)
	{
		errno = 0;
		endptr = NULL;
		health_port = strtol(args[2], &endptr, 10);
		if (errno != 0 || endptr == args[2] || *endptr != '\0' || health_port < 0 || health_port > 65535)
		{
			cerr << "Invalid health port: " << args[2] << endl;
			return 1;
		}
	}
	if (game_port == health_port)
	{
		cerr << "Health port must be different from game port: " << health_port << endl;
		return 1;
	}

	HealthServer healthServer;
	string health_error;
	if (!healthServer.start(static_cast<uint16_t>(health_port), &health_error))
	{
		cerr << "Failed to start health listener on port " << health_port << ": " << health_error << endl;
		return 1;
	}

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//The dot that will be moving around on the screen
			Dot dot( Dot::DOT_WIDTH / 2, Dot::DOT_HEIGHT / 2 );
			Dot mydot( SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 );

			//Set the wall
			SDL_Rect wall;
			wall.x = 300;
			wall.y = 40;
			wall.w = 4;
			wall.h = 4;

			int state = loadScreen(startMenu);
			if (state == -1)
				goto end;

			int sockfd, newsockfd, portno, pid;
    		socklen_t length_client;
    		struct sockaddr_in address_server, address_client;			
				sockfd = socket(AF_INET, SOCK_STREAM, 0);
				if (sockfd < 0)
				{
					perror("ERROR opening socket");
					goto end;
				}
				int optval = 1 ;
				if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(int)) == -1) {
					perror("setsockopt") ;
					::close(sockfd);
					goto end;
				}
				bzero((char *) &address_server, sizeof(address_server));
				portno = static_cast<int>(game_port);
				address_server.sin_family = AF_INET;
				address_server.sin_addr.s_addr = INADDR_ANY;
				address_server.sin_port = htons(portno);
				if (bind(sockfd, (struct sockaddr *) &address_server, sizeof(address_server)) < 0)
				{
					perror("ERROR on binding");
					::close(sockfd);
					goto end;
				}
				if (listen(sockfd,5) < 0)
				{
					perror("ERROR on listen");
					::close(sockfd);
					goto end;
				}
				healthServer.setReady(true);
				length_client = sizeof(address_client);
				cout<<"Server accept"<<endl;
				newsockfd = accept(sockfd, (struct sockaddr *) &address_client, &length_client);
				if (newsockfd < 0) 
				{
					perror("ERROR on accept");
					healthServer.setReady(false);
					::close(sockfd);
					goto end;
				}

				
			
			//While application is running
			while( !quit )
			{

				int n,x,y;
					int buffer[256];
					bzero(buffer,256);
					n = read(newsockfd,buffer,8);
					mydot.mPosX = buffer[0];
					mydot.mPosY = buffer[1];
					if (n < 0) 
					{
						perror("ERROR reading from socket");
						healthServer.setReady(false);
						::close(newsockfd);
						::close(sockfd);
						goto end;
					}
					buffer[1] = dot.mPosY;
					n = write(newsockfd,buffer,8);
					if (n < 0) 
					{
						perror("ERROR writing to socket");
						healthServer.setReady(false);
						::close(newsockfd);
						::close(sockfd);
						goto end;
					}
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					//Handle input for the dot
					dot.handleEvent( e );

                    //Handle button events
					for( int i = 0; i < TOTAL_BUTTONS; ++i )
					{
						gButtons[ i ].handleEvent( &e );
					}
				}

                //Render buttons
				for( int i = 0; i < TOTAL_BUTTONS; ++i )
				{
					gButtons[ i ].render();
				}

                //Apply the PNG image
				// SDL_BlitSurface( gPNGSurface, NULL, gScreenSurface, NULL );

                //Update the surface
				// SDL_UpdateWindowSurface( gWindow );

				//Move the dot and check collision
				dot.move( wall, mydot.getCollider() );

				//Clear screen

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				
				SDL_RenderClear( gRenderer );
                SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);

				if(!tplant1){
					ip1=rand()%2;
					tplant1=true;
				}
				if(!tplant2){
					ip2=rand()%2;
					tplant2=true;
				}
				if(!tplant3){
					ip3=rand()%2;
					tplant3=true;
				}

				if(!tbin1){
					ib1=rand()%2;
					tbin1=true;
				}
				if(!tbin2){
					ib2=rand()%2;
					tbin2=true;
				}
				if(!tbin3){
					ib3=rand()%2;
					tbin3=true;
				}

				if(!tsolar1){
					is1=rand()%2;
					tsolar1=true;
				}
				if(!tsolar2){
					is2=rand()%2;
					tsolar2=true;
				}
				if(!tsolar3){
					is3=rand()%2;
					tsolar3=true;
				}

				if(!twater1){
					iw1=rand()%2;
					twater1=true;
				}
				if(!twater2){
					iw2=rand()%2;
					twater2=true;
				}
				if(!twater3){
					iw3=rand()%2;
					twater3=true;
				}

				if(!c_plant1){
				SDL_Rect rp1={x_plant1[ip1],y_plant1[ip1],50,50};
				SDL_RenderCopy(gRenderer,plant1,NULL,&rp1);
				}

				if(!c_plant2){
				SDL_Rect rp2={x_plant2[ip2],y_plant2[ip2],50,50};
				SDL_RenderCopy(gRenderer,plant2,NULL,&rp2);
				}

				if(!c_plant3){
				SDL_Rect rp3={x_plant3[ip3],y_plant3[ip3],50,50};
				SDL_RenderCopy(gRenderer,plant3,NULL,&rp3);
				}

				if(!c_water1){
				SDL_Rect rw1={x_water1[iw1],y_water1[iw1],50,50};
				SDL_RenderCopy(gRenderer,water1,NULL,&rw1);
				}

				if(!c_water2){
				SDL_Rect rw2={x_water2[iw2],y_water2[iw2],50,50};
				SDL_RenderCopy(gRenderer,water2,NULL,&rw2);
				}

				if(!c_water3){
				SDL_Rect rw3={x_water3[iw3],y_water3[iw3],50,50};
				SDL_RenderCopy(gRenderer,water3,NULL,&rw3);
				}

				if(!c_solar1){
				SDL_Rect rs1={x_solar1[is1],y_solar1[is1],50,50};
				SDL_RenderCopy(gRenderer,solar1,NULL,&rs1);
				}

				if(!c_solar2){
				SDL_Rect rs2={x_solar2[is2],y_solar2[is2],50,50};
				SDL_RenderCopy(gRenderer,solar2,NULL,&rs2);
				}

				if(!c_solar3){
				SDL_Rect rs3={x_solar3[is3],y_solar3[is3],50,50};
				SDL_RenderCopy(gRenderer,solar3,NULL,&rs3);
				}

				if(!c_bin1){
				SDL_Rect rb1={x_bin1[ib1],y_bin1[ib1],50,50};
				SDL_RenderCopy(gRenderer,bin1,NULL,&rb1);
				}

				if(!c_bin2){
				SDL_Rect rb2={x_bin2[ib2],y_bin2[ib2],50,50};
				SDL_RenderCopy(gRenderer,bin2,NULL,&rb2);
				}

				if(!c_bin3){
				SDL_Rect rb3={x_bin3[ib3],y_bin3[ib3],50,50};
				SDL_RenderCopy(gRenderer,bin3,NULL,&rb3);
				}

				if(c_plant1){
					SDL_Rect cp1={x_plant1[ip1],y_plant1[ip1],50,50};
					SDL_RenderCopy(gRenderer,cplant1,NULL,&cp1);
				}

				if(c_plant2){
					SDL_Rect cp2={x_plant2[ip2],y_plant2[ip2],50,50};
					SDL_RenderCopy(gRenderer,cplant2,NULL,&cp2);
				}

				if(c_plant3){
					SDL_Rect cp3={x_plant3[ip3],y_plant3[ip3],50,50};
					SDL_RenderCopy(gRenderer,cplant3,NULL,&cp3);
				}

				if(c_bin1){
					SDL_Rect cb1={x_bin1[ib1],y_bin1[ib1],50,50};
					SDL_RenderCopy(gRenderer,cbin1,NULL,&cb1);
				}

				if(c_bin2){
					SDL_Rect cb2={x_bin2[ib2],y_bin2[ib2],50,50};
					SDL_RenderCopy(gRenderer,cbin2,NULL,&cb2);
				}

				if(c_bin3){
					SDL_Rect cb3={x_bin3[ib3],y_bin3[ib3],50,50};
					SDL_RenderCopy(gRenderer,cbin3,NULL,&cb3);
				}

				if(c_solar1){
					SDL_Rect cs1={x_solar1[is1],y_solar1[is1],50,50};
					SDL_RenderCopy(gRenderer,csolar1,NULL,&cs1);
				}

				if(c_solar2){
					SDL_Rect cs2={x_solar2[is2],y_solar2[is2],50,50};
					SDL_RenderCopy(gRenderer,csolar2,NULL,&cs2);
				}

				if(c_solar3){
					SDL_Rect cs3={x_solar3[is3],y_solar3[is3],50,50};
					SDL_RenderCopy(gRenderer,csolar3,NULL,&cs3);
				}

				if(c_water1){
					SDL_Rect cw1={x_water1[iw1],y_water1[iw1],50,50};
					SDL_RenderCopy(gRenderer,cwater1,NULL,&cw1);
				}

				if(c_water2){
					SDL_Rect cw2={x_water2[iw2],y_water2[iw2],50,50};
					SDL_RenderCopy(gRenderer,cwater2,NULL,&cw2);
				}

				if(c_water3){
					SDL_Rect cw3={x_water3[iw3],y_water3[iw3],50,50};
					SDL_RenderCopy(gRenderer,cwater3,NULL,&cw3);
				}

				if(total==12){
					SDL_Rect fin={500,300,400,400};
					SDL_RenderCopy(gRenderer,winner,NULL,&fin);
					finalwindow=true;
				}

				//Render wall
				// SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );		 //roads
				// SDL_RenderDrawRect( gRenderer, &wall );
				
				//Render dots
				dot.render();
				// mydot.render();

                // SDL_RenderCopy(gRenderer,gTexture,NULL,NULL);

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
			healthServer.setReady(false);
			::close(newsockfd);
			::close(sockfd);
		}
	}

	//Free resources and close SDL
	end:
	healthServer.setReady(false);
	healthServer.stop();
	close();
	return 0;
}
