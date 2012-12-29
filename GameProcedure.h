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
 *  Name: GameProcedure.h
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震)
 *  Description: 小课题Outspacer游戏过程中辅助函数 
 */
#ifndef _CONIO_
#define _CONIO_
#include <iostream>
#endif

#ifndef _CONIO_FILE_
#define _CONIO_FILE_
#include <fstream>
#endif

#include "MenuItems.h"
#include "SpaceObjects.h"

#ifndef _GAMEVARIABLES_
extern int Time;
extern int Level;
extern double MapRadius;
extern double DstPx;
extern double DstPy;
extern SpaceShip * mainship;
extern Fort * bossfort;
#define _GAMEVARIABLES_
#endif

#ifndef _GAMESTATES_
extern int gamestate;     //0 for menu, 1 for game1, 2 for game2, ...
#define _GAMESTATES_
#endif

int LoadMap ( char* filename ); //succeed for 0, fail for 1
int LoadSav ( char* filename ); //succeed for 0, fail for 1
void Save ( char* filename );
void ProceedGame (); // initial the projection matrix as game mode, disable texture
void DeleteAllObjs (); // delete all nodes
