/*
 *  OutSpacer -- a 3D game as C++ class homework
 *  Copyright (C) 2010 Sun Sibai <niasw@pku.edu.cn> et al.
 *
 *  OutSpacer is free software: you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation, either version 3 of
 *  the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Name: MenuPaintings.h
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震)
 *  Description: 小课题Outspacer菜单的绘制 
 */
#include "GameAttributes.h"
#include "MenuItems.h"

#ifndef _MATH_
#define _MATH_
#include <cmath>
#endif

#ifndef _CONIO_
#define _CONIO_
#include <iostream>
#endif

#ifndef _CONIO_FILE_
#define _CONIO_FILE_
#include <fstream>
#endif

#ifndef _OPENGL_
#define _OPENGL_
#include <GL/glut.h>
#endif

#ifndef _SMOOTH_
extern float now_item ; //the current view, tend to the 'to_item'
extern int to_item ; //the chosen item, lead the 'now_item', to move the menu smoothly
extern float now_alpha ; //when proceed a menu, change alpha to create a smooth view
extern int to_alpha ; //the value alpha towards
#define _SMOOTH_
#endif

#ifndef _GAMEOPTIONS_
extern float slope_angle; //angle of the viewing direction
extern int cheat_mode; //whether cheating mode is on
extern char key[0xb]; //key settings
extern int control_mode; //controlling mode
#define _GAMEOPTIONS_
#endif

#ifndef _GAMESTATES_
extern int gamestate;     //0 for menu, 1 for game1, 2 for game2, ...
#define _GAMESTATES_
#endif

struct BmpRes
{
      char *data;
      int size; //number of bytes
      int state; //loaded for 1, unloaded for 0
      BmpRes (): state(0) {}
      ~BmpRes ();
      int load ( char* ); //return state
      int release (); //return (!state)
};

#ifndef _BMPRES_
extern BmpRes image;
extern unsigned texBackground;
extern unsigned texFont;
extern unsigned *texID;
#define _BMPRES_
#endif

void LoadResBackground ( char *filename );
void LoadResMenu ( char *filename , int num );
void LoadResText ( char *filename );
void LoadResMenuOptions ();
void LoadResFontKeyBoard ();
void DeleteResBackground ();
void DeleteResMenu ( int num );
void DeleteResText ();
void DeleteResMenuOptions ();
void DeleteResFontKeyBoard ();

void DrawMenu ( int num );
//num is the 'NUM_CASE', the total amount of items in the menu

void DrawText (); //About, Help
void DrawBackground ();
