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
 *  Name: MenuItems.h
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震)
 *  Description: 小课题Menu的宏定义 
 */
#ifndef _MENU_

#define CASE_MENU 0
#define CASE_GAMEtime 1
#define CASE_GAMEfight 2
#define CASE_GAMEfree 3

#define NUM_CASE_MENU 7
#define CASE_MENU_Continue 1
#define CASE_MENU_StartGame 2
#define CASE_MENU_Options 3
#define CASE_MENU_Help 4
#define CASE_MENU_Demo 5
#define CASE_MENU_About 6
#define CASE_MENU_Exit 7

#define NUM_CASE_SUB_MENU_Continue 5
#define CASE_SUB_MENU_Continue_LastOne 0x11
#define CASE_SUB_MENU_Continue_Saving1 0x21
#define CASE_SUB_MENU_Continue_Saving2 0x31
#define CASE_SUB_MENU_Continue_Saving3 0x41
#define CASE_SUB_MENU_Continue_Return  0x51

#define NUM_CASE_SUB_MENU_StartGame 4
#define CASE_SUB_MENU_StartGame_TimeMode     0x12
#define CASE_SUB_MENU_StartGame_FightingMode 0x22
#define CASE_SUB_MENU_StartGame_FreeMode     0x32
#define CASE_SUB_MENU_StartGame_Return       0x42

#define NUM_CASE_SUB_MENU_StartGame_TimeMode 5
#define CASE_SUB_MENU_StartGame_TimeMode_Level1  0x112
#define CASE_SUB_MENU_StartGame_TimeMode_Level2  0x212
#define CASE_SUB_MENU_StartGame_TimeMode_Level3  0x312
#define CASE_SUB_MENU_StartGame_TimeMode_DIYmap  0x412
#define CASE_SUB_MENU_StartGame_TimeMode_Return  0x512

#define NUM_CASE_SUB_MENU_StartGame_FightingMode 5
#define CASE_SUB_MENU_StartGame_FightingMode_Level1  0x122
#define CASE_SUB_MENU_StartGame_FightingMode_Level2  0x222
#define CASE_SUB_MENU_StartGame_FightingMode_Level3  0x322
#define CASE_SUB_MENU_StartGame_FightingMode_DIYmap  0x422
#define CASE_SUB_MENU_StartGame_FightingMode_Return  0x522

#define NUM_CASE_SUB_MENU_StartGame_FreeMode 5
#define CASE_SUB_MENU_StartGame_FreeMode_Level1  0x132
#define CASE_SUB_MENU_StartGame_FreeMode_Level2  0x232
#define CASE_SUB_MENU_StartGame_FreeMode_Level3  0x332
#define CASE_SUB_MENU_StartGame_FreeMode_DIYmap  0x432
#define CASE_SUB_MENU_StartGame_FreeMode_Return  0x532

#define NUM_CASE_SUB_MENU_Options 4
#define CASE_SUB_MENU_Options_ControlMode   0x13 //mode1/mode2/mode3
#define CASE_SUB_MENU_Options_Keyboard      0x23
#define CASE_SUB_MENU_Options_Cheating      0x33 //on/off
#define CASE_SUB_MENU_Options_Return        0x43

#define NUM_CASE_SUB_MENU_Options_KeyBoard 0xb //need new idea
#define CASE_SUB_MENU_Options_KeyBoard_Pusher1     0x123
#define CASE_SUB_MENU_Options_KeyBoard_Pusher2     0x223
#define CASE_SUB_MENU_Options_KeyBoard_Pusher3     0x323
#define CASE_SUB_MENU_Options_KeyBoard_Pusher4     0x423
#define CASE_SUB_MENU_Options_KeyBoard_Pusher5     0x523
#define CASE_SUB_MENU_Options_KeyBoard_Pusher6     0x623
#define CASE_SUB_MENU_Options_KeyBoard_Controller1 0x723
#define CASE_SUB_MENU_Options_KeyBoard_Controller2 0x823
#define CASE_SUB_MENU_Options_KeyBoard_Weapon1     0x923
#define CASE_SUB_MENU_Options_KeyBoard_Weapon2     0xa23
#define CASE_SUB_MENU_Options_KeyBoard_Return      0xb23

#define CASE_SUB_MENU_Help_Return  0x14
#define CASE_SUB_MENU_Demo_Return  0x15
#define CASE_SUB_MENU_About_Return 0x16

#define NUM_Options_ControlMode 3//???

#define CASE_WIN   0x18
#define CASE_LOSE  0x28
#define CASE_ERROR 0x38

#define PIC_MENU_WIDTH 256
#define PIC_MENU_HEIGHT 64
#define PIC_MENU_W_3MUL_H 49152 // 3 * PIC_MENU_WIDTH * PIC_MENU_HEIGHT
#define VIEW_FRONT 1
#define VIEW_FRONT2 2 // 2 * VIEW_FRONT
#define VIEW_BACK 10
#define VIEW_BACK2 20 // 2 * VIEW_BACK
#define SHOW_MENU_DISTANCE 2.1
#define SHOW_MENU_HALF_WIDTH 2.0
#define SHOW_MENU_HALF_HEIGHT 0.5
#define SHOW_TEXT_HALF_WIDTH 2.0

#define _MENU_
#endif

#ifndef _BMPFILE_

#define BMP_BACKGROUND_MENU  "Res/Menu_Background.bmp" //1
#define BMP_BACKGROUND_GAME  "Res/Game_Background.bmp" //1
#define BMP_MENU  "Res/Pic_Menu.bmp" //NUM_CASE_MENU
#define BMP_SUBMENU_Continue  "Res/Pic_SubMenu_Continue.bmp" //NUM_CASE_SUB_MENU_Continue
#define BMP_SUBMENU_StartGame "Res/Pic_SubMenu_StartGame.bmp"//NUM_CASE_SUB_MENU_StartGame
#define BMP_SUBMENU_Options   "Res/Pic_SubMenu_Options.bmp"  //NUM_CASE_SUB_MENU_Options
#define BMP_TEXT_Help  "Res/Pic_SubMenu_Help.bmp" //1
#define BMP_TEXT_Demo  "Res/Pic_SubMenu_Demo.bmp" //1 //todo
#define BMP_TEXT_About "Res/Pic_SubMenu_About.bmp"//1
#define BMP_SUBMENU_StartGame_TimeMode      "Res/Pic_SubMenu_StartGame_TimeMode.bmp"     //NUM_CASE_SUB_MENU_StartGame_TimeMode
#define BMP_SUBMENU_StartGame_FightingMode  "Res/Pic_SubMenu_StartGame_FightingMode.bmp" //NUM_CASE_SUB_MENU_StartGame_FightingMode
#define BMP_SUBMENU_StartGame_FreeMode      "Res/Pic_SubMenu_StartGame_FreeMode.bmp"     //NUM_CASE_SUB_MENU_StartGame_FreeMode
#define BMP_TEXT_KeyBoard  "Res/Pic_SubMenu_Options_KeyBoard.bmp" //1
#define BMP_FONT_KeyBoard  "Res/Pic_Font_KeyBoard.bmp" //1
#define BMP_TEXT_WIN   "Res/Pic_Win.bmp"
#define BMP_TEXT_LOSE  "Res/Pic_Lose.bmp"
#define BMP_TEXT_ERROR "Res/Pic_Error.bmp"

#define _BMPFILE_
#endif
