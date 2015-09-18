//Copied from http://www.gamedev.net/page/resources/_/technical/game-programming/sdl--fonts-r1953
//Modified and added to by Joseph Dykstra
//2013-07-23
//Version 0.9.79 Beta

//#define   ALPHA_CAPABLE
#define CONVERT_FUNCTIONS
#define		NEW_FUNCTIONS		
#define		SET_FUNCTIONS		
#define   APPLY_FUNCTIONS		
#define   OTHER_FUNCTIONS		

#ifndef _SDL_H
#include "SDL/SDL.h"
#endif

#ifdef _SDL_H

#ifndef _EASYSDL_H
#define _EASYSDL_H


#ifdef NEW_FUNCTIONS
//           ___
//  |\   |  |     |      |
//  | \  |  |___  |  /\  |
//  |  \ |  |     | /  \ |
//  |   \|  |___  |/    \|

//Make a new rectangle
SDL_Rect EZ_new_rect(int INx,int INy, int INw,int INh)
	{
	SDL_Rect outRect;
	outRect.x = INx;
	outRect.y = INy;
	outRect.w = INw;
	outRect.h = INh;
	return(outRect);
	}


//Make a new SDL_Color
SDL_Color EZ_new_color(int INr, int INg, int INb)
	{
	SDL_Color tempColor;
	tempColor.r			= INr;
	tempColor.g			= INg;
	tempColor.b			= INb;
	tempColor.unused	= 0;
	return (tempColor);
	}


Uint32 EZ_new_Uint32(int INr, int INg, int INb)
	{
	Uint32 outColor=0;
	outColor += INr<<16;
	outColor += INg<<8;
	outColor += INb;
	return outColor;
	}


#ifdef APLPA_CAPABLE
//Make a new SDL_Color with alpha
SDL_Color EZ_new_colorA(int INr, int INg, int INb, int INa)
	{
	SDL_Color tempColor;
	tempColor.r		= INr;
	tempColor.g		= INg;
	tempColor.b		= INb;
	tempColor.unused	= INa;
	return (tempColor);
	}
#endif //ALPHA
#endif //NEW FUNCTIONS



#ifdef SET_FUNCTIONS
//   __    ___  _____
//  /  \  |       |
//  \__   |___    |
//     \  |       |
//  \__/  |___    |

//Set coordinates of inputted rectangle
void EZ_set_rect(SDL_Rect *INrect, int INx,int INy, int INw,int INh)
	{
	INrect->x = INx;
	INrect->y = INy;
	INrect->w = INw;
	INrect->h = INh;
	}



//Set a SDL_Color
void EZ_set_color(SDL_Color *INcolor, int INr, int INg, int INb)
	{
	INcolor->r		= INr;
	INcolor->g		= INg;
	INcolor->b		= INb;
	INcolor->unused	= 0;
	}


#ifdef ALPHA_CAPABLE
//Set a SDL_Color with alpha
void EZ_set_colorA(SDL_Color INcolor, int INr, int INg, int INb, int INa)
	{
	INcolor.r		= INr;
	INcolor.g		= INg;
	INcolor.b		= INb;
	INcolor.unused	= INa;
	}
#endif //APLHA_FUNCTIONS
#endif //SET



#ifdef CONVERT_FUNCTIONS
SDL_Color EZ_Uint32_to_SDL(Uint32 INcolor)
	{
	char tempR,tempG,tempB;
	tempR = INcolor>>16;
	tempG = INcolor>>8;
	tempB = INcolor;
	SDL_Color outColor;
	EZ_set_color(&outColor,tempR,tempG,tempB);
	return outColor;
	}

Uint32 EZ_SDL_to_Uint32(SDL_Color INcolor)
	{
	Uint32 outColor=0;
	outColor += INcolor.r<<16;
	outColor += INcolor.g<<8;
	outColor += INcolor.b;
	return outColor;
	}
#endif //CONVERT_FUNCTIONS



#ifdef APPLY_FUNCTIONS
//             _    _
//	   /\     | \  | \  |     \   /
//	  /__\    |_/  |_/  |      \ /
//	 /    \   |    |    |       |
//  /      \  |    |    |___    |

//Apply a rectangle to the surface with an inputted rectangle and color
int EZ_apply_rect(SDL_Surface *INdest, SDL_Rect INrect, Uint32 INcolor)
	{
	if( INdest == NULL )
		return (-1);
	else
		return (SDL_FillRect(INdest, &INrect, INcolor));
	}


#ifdef ALPHA_CAPABLE
//Apply a rectangle to the surface with all custom inputs (include alpha)
int EZ_apply_rect_customA(SDL_Surface *INdest, SDL_Rect INrect, int INr, int INg, int INb, int INa)
	{
	if( INdest == NULL )
		{
		return(-1);
		}
	else
		{
		SDL_Rect rct;
		rct.x = INx;
		rct.y = INy;
		rct.w = INw;
		rct.h = INh;
		return( SDL_FillRect( INdest, &rct, SDL_MapRGBA(INdest->format,INr,INg,INb,INa) ) );
		}
	}
#endif //ALPHA


#ifdef _SDL_TTF_H
//Apply text to the surface with inputted x, and y
int EZ_apply_text(SDL_Surface *INdest, char *INtext, TTF_Font *INfont, SDL_Rect INrect, SDL_Color INcolor)
	{
	SDL_Surface *tempMessage = NULL;
	tempMessage = TTF_RenderText_Solid(INfont, INtext, INcolor);

	int ret=SDL_BlitSurface(tempMessage, NULL, INdest, &INrect);
	SDL_FreeSurface(tempMessage);
	return ret;
	}
#endif	// _SDL_TTF_H

#endif //APPLY_FUNCTIONS



#ifdef OTHER_FUNCTIONS
//wait for the exit [x] button to be clicked
void EZ_waitForExit()
	{
	SDL_Event tempEvent;
	char quit = 0;
	while( quit == 0 )
		while( SDL_PollEvent( &tempEvent ) )
			if( tempEvent.type == SDL_QUIT )
				quit = 1;
	}

char EZ_checkForExit()
	{
	char ret=0;
	SDL_Event tempEvent;
	while( SDL_PollEvent( &tempEvent ) )
		if( tempEvent.type == SDL_QUIT )
			ret=1;
	return ret;
	}

#endif //OTHER_FUNCTIONS


#endif	// _EASYSDL_H

#endif	// _SDL_H



