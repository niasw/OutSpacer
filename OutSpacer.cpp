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
 *  Name: OutSpacer.cpp
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震) 
 *  Description: 小课题Outspacer主程序 
 */

#include "OutSpacerConfig.h"

#define _SMOOTH_ //the following variables is external (in MenuPaintings.h)
float now_item = 0; //the current view, tend to the 'to_item'
int to_item = 1; //the chosen item, lead the 'now_item', to move the menu smoothly
float now_alpha = 0; //when proceed a menu, change alpha to create a smooth view
int to_alpha = 1; //the value alpha towards
int wait_time = 0; //(this is not external)[set wait time]

#define _GAMEVARIABLES_ //the following variables is external (in SpaceObjects.h, GameProcedure.h)
int Time = 0;
int Level;
double MapRadius;
double DstPx; //TimeMode
double DstPy; //TimeMode
//SpaceShip * mainship; see (^)
//Fort * bossfort; see (^) //FightingMode
double ViewSpx; //(this is not external)[when mainship crashed, the view spot]
double ViewSpy; //(this is not external)[when mainship crashed, the view spot]
double ViewSpa; //(this is not external)[when mainship crashed, the view direction]

#define _BMPRES_ //the following variables is external (in MenuPaintings.h)
//BmpRes image; see (&)
unsigned texBackground;
unsigned texFont;
unsigned *texID;
//GLUquadricObj* QuadTmp; see (*) // external in SpaceObjects.h

#define _GAMEOPTIONS_ //the following variables is external (in MenuPaintings.h)
float slope_angle = 30; //angle of the viewing direction
int cheat_mode = 0; //whether cheating mode is on
char key[0xb] = { 'w', 's', 'a', 'd', 'j', 'k', 'l', ';', ',', '.', '\0' }; //key settings
int keystate[0x7]; //key states
int control_mode = 0; //controlling mode

#define _GAMESTATES_ //the following variables is external (in MenuPaintings.h, GameProcedure.h, SpaceObjects.h)
int gamestate = 0;     //0 for menu, 1 for TimeMode, 2 for FightingMode, 3 for FreeMode

#include "MenuItems.h"
#include "SpaceObjects.h"
#include "SpecialAffect.h"
#include "MenuPaintings.h"
#include "GameProcedure.h"

BmpRes image; // (&)
SpaceShip * mainship; // (^)
Fort * bossfort; // (^)
GLUquadricObj* QuadTmp; // (*)

#ifndef _MATH_
#include <cmath>
#define _MATH_
#endif

#ifndef _CONIO_
#include <iostream>
#define _CONIO_
#endif

#ifndef _CONIO_FILE_
#include <fstream>
#define _CONIO_FILE_
#endif

#ifndef _OPENGL_
#include <GL/glut.h>
#define _OPENGL_
#endif

int now_menu = 0;      //menu and submenu param
int to_menu = 0;       //when choosing an item, save your choice into the to_menu,
                       //and copy it to now_menu when animation finishes

static void reshape ( int width, int height );
static void display ();
static void keyboard ( unsigned char key, int x, int y );
static void keyup ( unsigned char key, int x, int y );
static void idle ();
static void timer ( int value );
void BeforeExitMenu ();
void BeforeExitGame ();
void BacktoMenu (); // save data, initial the projection matrix as menu mode, destroy all
void LoadOptions ();
void SaveOptions ();
void SaveTime ();
void DrawObj ();
void AffectObj (); //Update Star,BiStar,BlackHole,Planet
void MoveObj (); //Update SpaceShip,Fort
void CrashObj (); //Update Missile,Bullet,FuelBox,ExplosiveBox
void PassObj (); //Update Gas,Explosion
void KeyControl ();
void DrawAttributes ();

#define _LIGHT_ //the following variables is external (in SpaceObjects.h)
GLfloat light_ambient_menu[]  = { 0.1, 0.1, 0.1, 0.0 };
GLfloat light_diffuse_menu[]  = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular_menu[] = { 0.8, 0.8, 0.8, 1.0 };
GLfloat light_position_menu1[]= {-3.0, 2.0, 5.0, 1.0 };
GLfloat light_position_menu2[]= {-3.0, 2.0,-5.0, 1.0 };

GLfloat mat_ambient_menu[]    = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_diffuse_menu[]    = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular_menu[]   = { 0.9, 0.9, 0.9, 1.0 };
GLfloat mat_emission_menu[]   = { 0.0, 0.0, 0.0, 1.0 };
GLfloat high_shininess_menu[] = { 100.0 };

GLfloat mat_ambient_Star[]    = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_diffuse_Star[]    = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular_Star[]   = { 0.1, 0.1, 0.1, 1.0 };
GLfloat mat_emission_Star[]   = { 0.9, 0.9, 0.7, 1.0 };
GLfloat high_shininess_Star[] = { 100.0 };

GLfloat mat_ambient_Obj[]    = { 0.7, 0.7, 0.7, 1.0 };
GLfloat mat_diffuse_Obj[]    = { 0.8, 0.8, 0.8, 1.0 };
GLfloat mat_specular_Obj[]   = { 0.9, 0.9, 0.9, 1.0 };
GLfloat mat_emission_Obj[]   = { 0.0, 0.0, 0.0, 0.0 };
GLfloat high_shininess_Obj[] = { 100.0 };

//--------------Main Procedure------------
int main ( int argc, char *argv[] )
{
    glutInit ( &argc, argv );
    glutInitDisplayMode ( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition ( 5, 5 );
    glutInitWindowSize ( 1200, 700 );
    glutCreateWindow ( argv[0] );
//    glutFullScreen ();
    
    glutReshapeFunc( reshape );
    glutDisplayFunc ( display );
    glutKeyboardFunc ( keyboard );
    glutKeyboardUpFunc ( keyup );

    glutIgnoreKeyRepeat (1);

    glutIdleFunc ( idle );
    glutTimerFunc ( FreshTime, timer, 0 );
    
    glClearColor ( 0, 0, 0, 1 );
    
    glPolygonMode ( GL_FRONT_AND_BACK, GL_FILL );

    glCullFace( GL_BACK );
//    glEnable ( GL_CULL_FACE );
  glShadeModel (GL_SMOOTH);

    glEnable ( GL_DEPTH_TEST );
    glDepthFunc ( GL_LEQUAL );

    glEnable ( GL_LIGHT0 );
//    glEnable ( GL_NORMALIZE );
    glEnable ( GL_COLOR_MATERIAL );
    glEnable ( GL_LIGHTING );

    glEnable ( GL_BLEND );
    glBlendFunc ( GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA  );

    glEnable ( GL_LINE_SMOOTH );
    glHint ( GL_LINE_SMOOTH_HINT , GL_FASTEST );

    glEnable ( GL_POLYGON_SMOOTH );
    glHint ( GL_POLYGON_SMOOTH_HINT , GL_FASTEST );

    glLightfv ( GL_LIGHT0, GL_AMBIENT,  light_ambient_menu  );
    glLightfv ( GL_LIGHT0, GL_DIFFUSE,  light_diffuse_menu  );
    glLightfv ( GL_LIGHT0, GL_SPECULAR, light_specular_menu );
    glLightfv ( GL_LIGHT0, GL_POSITION, light_position_menu1 );

    glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_menu    );
    glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_menu    );
    glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_menu   );
    glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_menu );

    LoadResBackground ( BMP_BACKGROUND_MENU );
    LoadResMenu ( BMP_MENU , NUM_CASE_MENU );
    LoadResFontKeyBoard ();
    QuadTmp = gluNewQuadric();
    glEnable ( GL_TEXTURE_2D );

    glutMainLoop ();
    return EXIT_SUCCESS;
}

static void reshape ( int width, int height )
{
     const double ar = double ( width ) / double ( height );
     
     glViewport ( 0, 0, width, height );
     
     glMatrixMode ( GL_PROJECTION );
     glLoadIdentity ();
     glFrustum ( -ar, ar, -1, 1, VIEW_FRONT, VIEW_BACK );
     
     glMatrixMode ( GL_MODELVIEW );
     glLoadIdentity ();
}

static void display ()
{
     glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
     switch ( gamestate )
     {
            case CASE_GAMEtime:
                 //ShowTime();
            case CASE_GAMEfight:
            case CASE_GAMEfree:
                 glLoadIdentity();
                 glTranslated(0,0,- SHOW_SHIP_DISTANCE);
                 glRotated(slope_angle,1,0,0);
                 if (mainship)
                 {
                    glRotated( - mainship->dAgw,0,1,0);
                    glTranslated( - mainship->dPx, 0, - mainship->dPy );
                 }
                 else
                 {
                    glRotated( - ViewSpa,0,1,0);
                    glTranslated( - ViewSpx, 0, - ViewSpy );
                 }
                 
                 glEnable(GL_TEXTURE_2D);
                 gluQuadricTexture ( QuadTmp, GL_TRUE );
                 glBindTexture(GL_TEXTURE_2D,texBackground);
                 glPushMatrix();
                  glColor3f(1,1,1);
                  glDepthMask ( GL_FALSE );
                  glRotated(90,1,0,0);
                  gluSphere( QuadTmp, MapRadius * 2, 100, 100 );
                  glDepthMask ( GL_TRUE );
                 glPopMatrix();
                 glDisable(GL_TEXTURE_2D);

                 glPushMatrix();
                  glColor3f(MapEdgeColor);
                  glRotated(90,1,0,0);
                  glTranslatef(0,0,- float(MapEdgeH)/2);
                  gluCylinder( QuadTmp, MapRadius, MapRadius, MapEdgeH, 100, 100 );
                 glPopMatrix();
                 
                 if (gamestate==CASE_GAMEtime)
                 {
                    glPushMatrix();
                     glColor4f(DstColor,0.6);
                     glTranslatef(DstPx,0,DstPy);
                     glutSolidSphere( DstRadius, 30, 30 );
                    glPopMatrix();
                 }
                 
                 DrawObj ();
                 if (mainship) DrawAttributes();
                 break;
            case CASE_MENU:
                 switch ( now_menu )
                 {
                        case CASE_MENU_Exit:
                             BeforeExitMenu ();
                             exit (0);
                        case CASE_MENU:
                             DrawMenu ( NUM_CASE_MENU );
                             break;
                        case CASE_MENU_Continue:
                             DrawMenu ( NUM_CASE_SUB_MENU_Continue );
                             break;
                        case CASE_MENU_StartGame:
                             DrawMenu ( NUM_CASE_SUB_MENU_StartGame );
                             break;
                        case CASE_MENU_Options:
                             DrawMenu ( NUM_CASE_SUB_MENU_Options );
                             break;
                        case CASE_MENU_Help:
                        case CASE_MENU_Demo://todo
                        case CASE_MENU_About:
                        case CASE_WIN:
                        case CASE_LOSE:
                        case CASE_ERROR:     
                             DrawText ();
                             break;
                        case CASE_SUB_MENU_StartGame_TimeMode:
                             DrawMenu ( NUM_CASE_SUB_MENU_StartGame_TimeMode );
                             break;
                        case CASE_SUB_MENU_StartGame_FightingMode:
                             DrawMenu ( NUM_CASE_SUB_MENU_StartGame_FightingMode );
                             break;
                        case CASE_SUB_MENU_StartGame_FreeMode:
                             DrawMenu ( NUM_CASE_SUB_MENU_StartGame_FreeMode );
                             break;
                        case CASE_SUB_MENU_Options_Keyboard:
                             //todo
                             break;
                 }
                 break;
     }
     glutSwapBuffers ();
}

static void keyboard ( unsigned char key, int x, int y )
{
     switch ( gamestate )
     {
            case CASE_GAMEtime:
            case CASE_GAMEfight:
            case CASE_GAMEfree:
                 if ( key == 27 ) //Exit
                 {
                      BeforeExitGame ();
                      exit(0);
                 }
                 else if ( key == 8 ) //BackSpace
                 {
                      BacktoMenu ();
                      LoadResMenu ( BMP_MENU , NUM_CASE_MENU );
                      to_menu = 0;
                      now_menu = 0;
                 }
                 else if ( key == ::key[0x0] ) //Key0 Pressed
                 {
                      ::keystate[0x0] = 1;
                 }
                 else if ( key == ::key[0x1] ) //Key1 Pressed
                 {
                      ::keystate[0x1] = 1;
                 }
                 else if ( key == ::key[0x2] ) //Key2 Pressed
                 {
                      ::keystate[0x2] = 1;
                 }
                 else if ( key == ::key[0x3] ) //Key3 Pressed
                 {
                      ::keystate[0x3] = 1;
                 }
                 else if ( key == ::key[0x4] ) //Key4 Pressed
                 {
                      ::keystate[0x4] = 1;
                 }
                 else if ( key == ::key[0x5] ) //Key5 Pressed
                 {
                      ::keystate[0x5] = 1;
                 }
                 else if ( key == ::key[0x6] ) //Key6 Pressed
                 {
                      ::keystate[0x6] = 1;
                 }
                 else if ( key == ::key[0x7] ) //fire missile(to improve)
                 {
                      //to improve
                 }
                 else if ( key == ::key[0x8] ) //Enhanse Jet
                 {
                      mainship->EnhanceJet();
                 }
                 else if ( key == ::key[0x9] ) //Reduce Jet
                 {
                      mainship->ReduceJet();
                 }
                 else if ( key == 'n' && slope_angle < 60 ) //slope_angle+
                 {
                      slope_angle += 5;
                 }
                 else if ( key == 'm' && slope_angle > 5 ) //slope_angle-
                 {
                      slope_angle -= 5;
                 }
                 break;
            case CASE_MENU:
              if ( now_menu == to_menu )
              {
                 switch ( now_menu )
                 {
                        case CASE_MENU_Exit:
                             BeforeExitMenu ();
                             exit(0);
                        case CASE_MENU:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 'w':
                                         to_item += 1;
                                         if ( to_item > NUM_CASE_MENU )
                                         {
                                              to_item -= (NUM_CASE_MENU);
                                              now_item -= float (NUM_CASE_MENU);
                                         }
                                         break;
                                    case 's':
                                         to_item -= 1;
                                         if ( to_item <= 0 )
                                         {
                                              to_item += (NUM_CASE_MENU);
                                              now_item += float (NUM_CASE_MENU);
                                         }
                                         break;
                                    case 13: // Enter
                                         to_alpha = 0;
                                         to_menu = to_item;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_MENU_Continue:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 'w':
                                         to_item += 1;
                                         if ( to_item > NUM_CASE_SUB_MENU_Continue )
                                         {
                                              to_item -= (NUM_CASE_SUB_MENU_Continue);
                                              now_item -= float (NUM_CASE_SUB_MENU_Continue);
                                         }
                                         break;
                                    case 's':
                                         to_item -= 1;
                                         if ( to_item <= 0 )
                                         {
                                              to_item += (NUM_CASE_SUB_MENU_Continue);
                                              now_item += float (NUM_CASE_SUB_MENU_Continue);
                                         }
                                         break;
                                    case 13: // Enter
                                         to_alpha = 0;
                                         to_menu += ( to_item << 4 );
                                         wait_time = 21;
                                         if ( to_menu == CASE_SUB_MENU_Continue_Return ) to_menu = 0;
                                         break;
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu = 0;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_MENU_StartGame:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 'w':
                                         to_item += 1;
                                         if ( to_item > NUM_CASE_SUB_MENU_StartGame )
                                         {
                                              to_item -= (NUM_CASE_SUB_MENU_StartGame);
                                              now_item -= float (NUM_CASE_SUB_MENU_StartGame);
                                         }
                                         break;
                                    case 's':
                                         to_item -= 1;
                                         if ( to_item <= 0 )
                                         {
                                              to_item += (NUM_CASE_SUB_MENU_StartGame);
                                              now_item += float (NUM_CASE_SUB_MENU_StartGame);
                                         }
                                         break;
                                    case 13: // Enter
                                         to_alpha = 0;
                                         to_menu += ( to_item << 4 );
                                         wait_time = 21;
                                         if ( to_menu == CASE_SUB_MENU_StartGame_Return ) to_menu = 0;
                                         break;
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu = 0;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_MENU_Options:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 'w':
                                         to_item += 1;
                                         if ( to_item > NUM_CASE_SUB_MENU_Options )
                                         {
                                              to_item -= (NUM_CASE_SUB_MENU_Options);
                                              now_item -= float (NUM_CASE_SUB_MENU_Options);
                                         }
                                         break;
                                    case 's':
                                         to_item -= 1;
                                         if ( to_item <= 0 )
                                         {
                                              to_item += (NUM_CASE_SUB_MENU_Options);
                                              now_item += float (NUM_CASE_SUB_MENU_Options);
                                         }
                                         break;
                                    case 13: // Enter
                                         to_alpha = 0;
                                         to_menu += ( to_item << 4 );
                                         wait_time = 21;
                                         if ( to_menu == CASE_SUB_MENU_Options_Return ) to_menu = 0;
                                         break;
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu = 0;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_MENU_Help:
                        case CASE_MENU_About:
                        case CASE_WIN:
                        case CASE_LOSE:
                        case CASE_ERROR:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 13: // Enter
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu = 0;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_MENU_Demo:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 13: // Enter
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu = 0;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_SUB_MENU_StartGame_TimeMode:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 'w':
                                         to_item += 1;
                                         if ( to_item > NUM_CASE_SUB_MENU_StartGame_TimeMode )
                                         {
                                              to_item -= (NUM_CASE_SUB_MENU_StartGame_TimeMode);
                                              now_item -= float (NUM_CASE_SUB_MENU_StartGame_TimeMode);
                                         }
                                         break;
                                    case 's':
                                         to_item -= 1;
                                         if ( to_item <= 0 )
                                         {
                                              to_item += (NUM_CASE_SUB_MENU_StartGame_TimeMode);
                                              now_item += float (NUM_CASE_SUB_MENU_StartGame_TimeMode);
                                         }
                                         break;
                                    case 13: // Enter
                                         to_alpha = 0;
                                         to_menu += ( to_item << 8 );
                                         wait_time = 21;
                                         if ( to_menu == CASE_SUB_MENU_StartGame_TimeMode_Return ) to_menu &= 0xf;
                                         break;
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu &= 0xf;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_SUB_MENU_StartGame_FightingMode:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 'w':
                                         to_item += 1;
                                         if ( to_item > NUM_CASE_SUB_MENU_StartGame_FightingMode )
                                         {
                                              to_item -= (NUM_CASE_SUB_MENU_StartGame_FightingMode);
                                              now_item -= float (NUM_CASE_SUB_MENU_StartGame_FightingMode);
                                         }
                                         break;
                                    case 's':
                                         to_item -= 1;
                                         if ( to_item <= 0 )
                                         {
                                              to_item += (NUM_CASE_SUB_MENU_StartGame_FightingMode);
                                              now_item += float (NUM_CASE_SUB_MENU_StartGame_FightingMode);
                                         }
                                         break;
                                    case 13: // Enter
                                         to_alpha = 0;
                                         to_menu += ( to_item << 8 );
                                         wait_time = 21;
                                         if ( to_menu == CASE_SUB_MENU_StartGame_FightingMode_Return ) to_menu &= 0xf;
                                         break;
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu &= 0xf;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_SUB_MENU_StartGame_FreeMode:
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 'w':
                                         to_item += 1;
                                         if ( to_item > NUM_CASE_SUB_MENU_StartGame_FreeMode )
                                         {
                                              to_item -= (NUM_CASE_SUB_MENU_StartGame_FreeMode);
                                              now_item -= float (NUM_CASE_SUB_MENU_StartGame_FreeMode);
                                         }
                                         break;
                                    case 's':
                                         to_item -= 1;
                                         if ( to_item <= 0 )
                                         {
                                              to_item += (NUM_CASE_SUB_MENU_StartGame_FreeMode);
                                              now_item += float (NUM_CASE_SUB_MENU_StartGame_FreeMode);
                                         }
                                         break;
                                    case 13: // Enter
                                         to_alpha = 0;
                                         to_menu += ( to_item << 8 );
                                         wait_time = 21;
                                         if ( to_menu == CASE_SUB_MENU_StartGame_FreeMode_Return ) to_menu &= 0xf;
                                         break;
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu &= 0xf;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                        case CASE_SUB_MENU_Options_Keyboard://todo
                             switch ( key )
                             {
                                    case 27: // Esc
                                         BeforeExitMenu ();
                                         exit (0);
                                    case 13: // Enter
                                    case 8: // Backspace
                                         to_alpha = 0;
                                         to_menu = 0;
                                         wait_time = 21;
                                         break;
                             }
                             break;
                             //todo
                 }
                 break;
              }
     }
}

static void keyup ( unsigned char key, int x, int y )
{
    switch ( gamestate )
    {
        case CASE_GAMEtime:
        case CASE_GAMEfight:
        case CASE_GAMEfree:
                 if ( key == ::key[0x0] ) //Key0 Released
                 {
                      ::keystate[0x0] = 0;
                 }
                 else if ( key == ::key[0x1] ) //Key1 Released
                 {
                      ::keystate[0x1] = 0;
                 }
                 else if ( key == ::key[0x2] ) //Key2 Released
                 {
                      ::keystate[0x2] = 0;
                 }
                 else if ( key == ::key[0x3] ) //Key3 Released
                 {
                      ::keystate[0x3] = 0;
                 }
                 else if ( key == ::key[0x4] ) //Key4 Released
                 {
                      ::keystate[0x4] = 0;
                 }
                 else if ( key == ::key[0x5] ) //Key5 Released
                 {
                      ::keystate[0x5] = 0;
                 }
                 else if ( key == ::key[0x6] ) //Key6 Released
                 {
                      ::keystate[0x6] = 0;
                 }
    }
}

static void idle ()
{
     switch ( gamestate )
     {
            case CASE_GAMEtime:

            case CASE_GAMEfree:
                 break;                 
            case CASE_MENU:
                 glutPostRedisplay ();
                 break;
     }
}

static void timer ( int value )
{
     switch ( gamestate )
     {
            case CASE_GAMEtime:
                 ++ Time;
            case CASE_GAMEfight:
            case CASE_GAMEfree:
                 AffectObj ();
                 if ( !wait_time )
                 {
                    KeyControl ();
                    if ( cheat_mode )
                    {
                         mainship->dMf = Sp_FuelMax;
                         mainship->dMe = Sp_ExplMax;
                    }
                    if ( mainship->HP <= 0 )
                    {
                         wait_time = 100;
                         ViewSpx = mainship->dPx;
                         ViewSpy = mainship->dPy;
                         ViewSpa = mainship->dAgw;
                         mainship = 0;
                         to_menu = CASE_LOSE;
                         LoadResText ( BMP_TEXT_LOSE );
                    }
                    switch (gamestate)
                    {
                        case CASE_GAMEtime:
                             if ( (mainship->dPx - DstPx)*(mainship->dPx - DstPx) +
                                  (mainship->dPy - DstPy)*(mainship->dPy - DstPy) < DstRadius*DstRadius )
                             {
                                wait_time = 100;
                                to_menu = CASE_WIN;
                                SaveTime ();
                                LoadResText ( BMP_TEXT_WIN );
                             }
                             break;
                        case CASE_GAMEfight:
                             if ( bossfort->Fq <= 0 )
                             {
                                wait_time = 100;
                                bossfort = 0;
                                to_menu = CASE_WIN;
                                LoadResText ( BMP_TEXT_WIN );
                             }
                             break;
                    }
                 }
                 MoveObj ();
                 CrashObj ();
                 PassObj ();
                 if ( wait_time==1 )
                 {
                      BacktoMenu ();
                      wait_time = 2;
                 }
                 if ( wait_time > 0 ) -- wait_time;
                 glutPostRedisplay ();
                 glutTimerFunc ( FreshTime, timer, 0 );
                 break;
            case CASE_MENU:
                 now_item += ( float (to_item) - now_item ) / 3.;
                 now_alpha += ( float (to_alpha) - now_alpha ) / 5;
                 if ( wait_time == 1 )
                 {
                                switch ( now_menu )
                                {
                                       case CASE_MENU:
                                            DeleteResMenu ( NUM_CASE_MENU );
                                            break;
                                       case CASE_MENU_Continue:
                                            DeleteResMenu ( NUM_CASE_SUB_MENU_Continue );
                                            break;
                                       case CASE_MENU_StartGame:
                                            DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame );
                                            break;
                                       case CASE_MENU_Options:
                                            DeleteResMenuOptions ();
                                            break;
                                       case CASE_MENU_Help:
                                       case CASE_MENU_Demo:
                                       case CASE_MENU_About:
                                       case CASE_WIN:
                                       case CASE_LOSE:
                                       case CASE_ERROR:
                                            DeleteResText ();
                                            break;
                                       case CASE_SUB_MENU_StartGame_TimeMode:
                                            DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame_TimeMode );
                                            break;
                                       case CASE_SUB_MENU_StartGame_FightingMode:
                                            DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame_FightingMode );
                                            break;
                                       case CASE_SUB_MENU_StartGame_FreeMode:
                                            DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame_FreeMode );
                                            break;
                                       case CASE_SUB_MENU_Options_Keyboard:
                                            DeleteResText ();
                                            //todo(save ini file)
                                            break;
                                       case -1: //from Game
                                            //nothing to be done
                                            break;
                                }
                                int judge = 1;
                                switch ( to_menu )
                                {
                                       case CASE_MENU:
                                            to_item = 1;
                                            LoadResMenu ( BMP_MENU , NUM_CASE_MENU );
                                            break;
                                       case CASE_MENU_Continue:
                                            to_item = 1;
                                            LoadResMenu ( BMP_SUBMENU_Continue , NUM_CASE_SUB_MENU_Continue );
                                            break;
                                       case CASE_MENU_StartGame:
                                            to_item = 1;
                                            LoadResMenu ( BMP_SUBMENU_StartGame , NUM_CASE_SUB_MENU_StartGame );
                                            break;
                                       case CASE_MENU_Options:
                                            to_item = 1;
                                            LoadResMenuOptions ();
                                            break;
                                       case CASE_MENU_Help:
                                            LoadResText ( BMP_TEXT_Help );
                                            break;
                                       case CASE_MENU_Demo://todo
                                            LoadResText ( BMP_TEXT_Demo );
                                            break;
                                       case CASE_MENU_About:
                                            LoadResText ( BMP_TEXT_About );
                                            break;
                                       case CASE_SUB_MENU_Continue_LastOne:
                                            if (LoadSav ( SAV_LASTONE ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_Continue_Saving1:
                                            if (LoadSav ( SAV_SAVING1 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_Continue_Saving2:
                                            if (LoadSav ( SAV_SAVING2 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_Continue_Saving3:
                                            if (LoadSav ( SAV_SAVING3 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_TimeMode:
                                            to_item = 1;
                                            LoadResMenu ( BMP_SUBMENU_StartGame_TimeMode , NUM_CASE_SUB_MENU_StartGame_TimeMode );
                                            break;
                                       case CASE_SUB_MENU_StartGame_FightingMode:
                                            to_item = 1;
                                            LoadResMenu ( BMP_SUBMENU_StartGame_FightingMode , NUM_CASE_SUB_MENU_StartGame_FightingMode );
                                            break;
                                       case CASE_SUB_MENU_StartGame_FreeMode:
                                            to_item = 1;
                                            LoadResMenu ( BMP_SUBMENU_StartGame_FreeMode , NUM_CASE_SUB_MENU_StartGame_FreeMode );
                                            break;
                                       case CASE_SUB_MENU_Options_ControlMode:
                                            ++ control_mode;
                                            control_mode %= NUM_Options_ControlMode;
                                            LoadResMenuOptions ();
                                            to_menu = now_menu;
                                            break;
                                       case CASE_SUB_MENU_Options_Keyboard:
                                            to_item = 1;
                                            LoadResText ( BMP_TEXT_KeyBoard );
                                            //todo
                                            break;
                                       case CASE_SUB_MENU_Options_Cheating:
                                            cheat_mode = 1 - cheat_mode;
                                            LoadResMenuOptions ();
                                            to_menu = now_menu;
                                            break;
                                       case CASE_SUB_MENU_StartGame_TimeMode_Level1:
                                            if (LoadMap ( MAP_TIME1 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_TimeMode_Level2:
                                            if (LoadMap ( MAP_TIME2 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_TimeMode_Level3:
                                            if (LoadMap ( MAP_TIME3 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_TimeMode_DIYmap:
                                            if (LoadMap ( MAP_TIMEDIY ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FightingMode_Level1:
                                            if (LoadMap ( MAP_FIGHT1 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FightingMode_Level2:
                                            if (LoadMap ( MAP_FIGHT2 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FightingMode_Level3:
                                            if (LoadMap ( MAP_FIGHT3 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FightingMode_DIYmap:
                                            if (LoadMap ( MAP_FIGHTDIY ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FreeMode_Level1:
                                            if (LoadMap ( MAP_FREE1 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FreeMode_Level2:
                                            if (LoadMap ( MAP_FREE2 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FreeMode_Level3:
                                            if (LoadMap ( MAP_FREE3 ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                       case CASE_SUB_MENU_StartGame_FreeMode_DIYmap:
                                            if (LoadMap ( MAP_FREEDIY ))
                                            {
                                               wait_time = 21;
                                               to_menu = CASE_ERROR;
                                               LoadResText ( BMP_TEXT_ERROR );
                                               now_menu = -1;
                                               judge = 0;
                                            }
                                            else
                                            {
                                                DeleteResBackground ();
                                                LoadResBackground ( BMP_BACKGROUND_GAME );
                                                ProceedGame ();
                                            }
                                            break;
                                }
                                to_alpha = 1;
                                if (judge) now_menu = to_menu;
                 }
                 if ( wait_time > 0 ) -- wait_time;
                 glutPostRedisplay ();
                 glutTimerFunc ( FreshTime, timer, 0 );
                 break;
     }
}

void DrawObj ()
{
     for ( Node<Star>::cur = Node<Star>::beg; Node<Star>::cur != 0; Node<Star>::cur = Node<Star>::cur->next )
     {
         Node<Star>::cur->obj->Draw();
     }
     for ( Node<BiStar>::cur = Node<BiStar>::beg; Node<BiStar>::cur != 0; Node<BiStar>::cur = Node<BiStar>::cur->next )
     {
         Node<BiStar>::cur->obj->Draw();
     }
     for ( Node<BlackHole>::cur = Node<BlackHole>::beg; Node<BlackHole>::cur != 0; Node<BlackHole>::cur = Node<BlackHole>::cur->next )
     {
         Node<BlackHole>::cur->obj->Draw();
     }
     for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
     {
         Node<Planet>::cur->obj->Draw();
     }
     for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
     {
         Node<SpaceShip>::cur->obj->Draw();
     }
     for ( Node<Fort>::cur = Node<Fort>::beg; Node<Fort>::cur != 0; Node<Fort>::cur = Node<Fort>::cur->next )
     {
         Node<Fort>::cur->obj->Draw();
     }
     for ( Node<Missile>::cur = Node<Missile>::beg; Node<Missile>::cur != 0; Node<Missile>::cur = Node<Missile>::cur->next )
     {
         Node<Missile>::cur->obj->Draw();
     }
     for ( Node<Bullet>::cur = Node<Bullet>::beg; Node<Bullet>::cur != 0; Node<Bullet>::cur = Node<Bullet>::cur->next )
     {
         Node<Bullet>::cur->obj->Draw();
     }
     for ( Node<FuelBox>::cur = Node<FuelBox>::beg; Node<FuelBox>::cur != 0; Node<FuelBox>::cur = Node<FuelBox>::cur->next )
     {
         Node<FuelBox>::cur->obj->Draw();
     }
     for ( Node<ExplosiveBox>::cur = Node<ExplosiveBox>::beg; Node<ExplosiveBox>::cur != 0; Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next )
     {
         Node<ExplosiveBox>::cur->obj->Draw();
     }
     for ( Node<Station>::cur = Node<Station>::beg; Node<Station>::cur != 0; Node<Station>::cur = Node<Station>::cur->next )
     {
         Node<Station>::cur->obj->Draw();
     }
     for ( Node<Gas>::cur = Node<Gas>::beg; Node<Gas>::cur != 0; Node<Gas>::cur = Node<Gas>::cur->next )
     {
         Node<Gas>::cur->obj->Draw();
     }
     for ( Node<Explosion>::cur = Node<Explosion>::beg; Node<Explosion>::cur != 0; Node<Explosion>::cur = Node<Explosion>::cur->next )
     {
         Node<Explosion>::cur->obj->Draw();
     }
}

void AffectObj ()
{
     for ( Node<Star>::cur = Node<Star>::beg; Node<Star>::cur != 0; Node<Star>::cur = Node<Star>::cur->next )
     {
         for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
         {
             Node<Star>::cur->obj->Affect ( Node<Planet>::cur->obj );
         }
         for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
         {
             Node<Star>::cur->obj->Affect ( Node<SpaceShip>::cur->obj );
         }
         for ( Node<Missile>::cur = Node<Missile>::beg; Node<Missile>::cur != 0; Node<Missile>::cur = Node<Missile>::cur->next )
         {
             Node<Star>::cur->obj->Affect ( Node<Missile>::cur->obj );
         }
         for ( Node<Bullet>::cur = Node<Bullet>::beg; Node<Bullet>::cur != 0; Node<Bullet>::cur = Node<Bullet>::cur->next )
         {
             Node<Star>::cur->obj->Affect ( Node<Bullet>::cur->obj );
         }
         for ( Node<FuelBox>::cur = Node<FuelBox>::beg; Node<FuelBox>::cur != 0; Node<FuelBox>::cur = Node<FuelBox>::cur->next )
         {
             Node<Star>::cur->obj->Affect ( Node<FuelBox>::cur->obj );
         }
         for ( Node<ExplosiveBox>::cur = Node<ExplosiveBox>::beg; Node<ExplosiveBox>::cur != 0; Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next )
         {
             Node<Star>::cur->obj->Affect ( Node<ExplosiveBox>::cur->obj );
         }
    }
    for ( Node<BiStar>::cur = Node<BiStar>::beg; Node<BiStar>::cur != 0; Node<BiStar>::cur = Node<BiStar>::cur->next )
    {
        Node<BiStar>::cur->obj->Update();
        for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
        {
            Node<BiStar>::cur->obj->Affect ( Node<Planet>::cur->obj );
        }
        for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
        {
            Node<BiStar>::cur->obj->Affect ( Node<SpaceShip>::cur->obj );
        }
        for ( Node<Missile>::cur = Node<Missile>::beg; Node<Missile>::cur != 0; Node<Missile>::cur = Node<Missile>::cur->next )
        {
            Node<BiStar>::cur->obj->Affect ( Node<Missile>::cur->obj );
        }
        for ( Node<Bullet>::cur = Node<Bullet>::beg; Node<Bullet>::cur != 0; Node<Bullet>::cur = Node<Bullet>::cur->next )
        {
            Node<BiStar>::cur->obj->Affect ( Node<Bullet>::cur->obj );
        }
        for ( Node<FuelBox>::cur = Node<FuelBox>::beg; Node<FuelBox>::cur != 0; Node<FuelBox>::cur = Node<FuelBox>::cur->next )
        {
            Node<BiStar>::cur->obj->Affect ( Node<FuelBox>::cur->obj );
        }
        for ( Node<ExplosiveBox>::cur = Node<ExplosiveBox>::beg; Node<ExplosiveBox>::cur != 0; Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next )
        {
            Node<BiStar>::cur->obj->Affect ( Node<ExplosiveBox>::cur->obj );
        }
    }
    for ( Node<BlackHole>::cur = Node<BlackHole>::beg; Node<BlackHole>::cur != 0; Node<BlackHole>::cur = Node<BlackHole>::cur->next )
    {
        for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
        {
            Node<BlackHole>::cur->obj->Affect ( Node<Planet>::cur->obj );
        }
        for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
        {
            Node<BlackHole>::cur->obj->Affect ( Node<SpaceShip>::cur->obj );
        }
        for ( Node<Missile>::cur = Node<Missile>::beg; Node<Missile>::cur != 0; Node<Missile>::cur = Node<Missile>::cur->next )
        {
            Node<BlackHole>::cur->obj->Affect ( Node<Missile>::cur->obj );
        }
        for ( Node<Bullet>::cur = Node<Bullet>::beg; Node<Bullet>::cur != 0; Node<Bullet>::cur = Node<Bullet>::cur->next )
        {
            Node<BlackHole>::cur->obj->Affect ( Node<Bullet>::cur->obj );
        }
        for ( Node<FuelBox>::cur = Node<FuelBox>::beg; Node<FuelBox>::cur != 0; Node<FuelBox>::cur = Node<FuelBox>::cur->next )
        {
            Node<BlackHole>::cur->obj->Affect ( Node<FuelBox>::cur->obj );
        }
        for ( Node<ExplosiveBox>::cur = Node<ExplosiveBox>::beg; Node<ExplosiveBox>::cur != 0; Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next )
        {
            Node<BlackHole>::cur->obj->Affect ( Node<ExplosiveBox>::cur->obj );
        }
    }
    for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
    {
        Node<Planet>::cur->obj->Update();
        for ( Node<Planet>* tmp = Node<Planet>::cur->next ; tmp != 0; tmp = tmp->next )
        {
            Node<Planet>::cur->obj->Affect ( tmp->obj );
        }
        for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
        {
            Node<Planet>::cur->obj->Affect ( Node<SpaceShip>::cur->obj );
        }
        for ( Node<Missile>::cur = Node<Missile>::beg; Node<Missile>::cur != 0; Node<Missile>::cur = Node<Missile>::cur->next )
        {
            Node<Planet>::cur->obj->Affect ( Node<Missile>::cur->obj );
        }
        for ( Node<Bullet>::cur = Node<Bullet>::beg; Node<Bullet>::cur != 0; Node<Bullet>::cur = Node<Bullet>::cur->next )
        {
            Node<Planet>::cur->obj->Affect ( Node<Bullet>::cur->obj );
        }
        for ( Node<FuelBox>::cur = Node<FuelBox>::beg; Node<FuelBox>::cur != 0; Node<FuelBox>::cur = Node<FuelBox>::cur->next )
        {
            Node<Planet>::cur->obj->Affect ( Node<FuelBox>::cur->obj );
        }
        for ( Node<ExplosiveBox>::cur = Node<ExplosiveBox>::beg; Node<ExplosiveBox>::cur != 0; Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next )
        {
            Node<Planet>::cur->obj->Affect ( Node<ExplosiveBox>::cur->obj );
        }
    }
}

void MoveObj ()
{
     for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; )
     {
         if (Node<SpaceShip>::cur->obj->HP <= 0)
         {
            new Node<Explosion>(new Explosion( Node<SpaceShip>::cur->obj->dPx, Node<SpaceShip>::cur->obj->dPy,
                                               Sp_ExplosionV, 0 ) );
            Node<SpaceShip> *tmp = Node<SpaceShip>::cur;
            Node<SpaceShip>::cur = Node<SpaceShip>::cur->next;
            delete tmp;
         }
         else
         {
            Node<SpaceShip>::cur->obj->Update();
            Node<SpaceShip>::cur = Node<SpaceShip>::cur->next;
         }
     }
     for ( Node<Fort>::cur = Node<Fort>::beg; Node<Fort>::cur != 0; )
     {
         if (Node<Fort>::cur->obj->Fq <= 0)
         {
            new Node<Explosion>(new Explosion( Node<Fort>::cur->obj->dPx, Node<Fort>::cur->obj->dPy,
                                               Ft_ExplosionV, 0 ) );
            Node<Fort> *tmp = Node<Fort>::cur;
            Node<Fort>::cur = Node<Fort>::cur->next;
            delete tmp;
         }
         else
         {
            Node<Fort>::cur->obj->Update();
            Node<Fort>::cur = Node<Fort>::cur->next;
         }
     }
}

void CrashObj ()
{
     int over = 0;
     for ( Node<Missile>::cur = Node<Missile>::beg; Node<Missile>::cur != 0; )
     {
         Node<Missile>::cur->obj->Update();
         for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
         {
             if ( Node<Missile>::cur->obj->Damage ( Node<SpaceShip>::cur->obj ) )
             {
                Node<Missile> *tmp = Node<Missile>::cur;
                Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Fort>::cur = Node<Fort>::beg; Node<Fort>::cur != 0; Node<Fort>::cur = Node<Fort>::cur->next )
         {
             if ( Node<Missile>::cur->obj->Damage ( Node<Fort>::cur->obj ) )
             {
                Node<Missile> *tmp = Node<Missile>::cur;
                Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Missile>* tmp1 = Node<Missile>::cur->next ; tmp1 != 0; tmp1 = tmp1->next )
         {
             if ( Node<Missile>::cur->obj->Damage ( tmp1 ) )
             {
                Node<Missile> *tmp = Node<Missile>::cur;
                Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp;
                
                if ( Node<Missile>::cur == tmp1 ) Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp1;
                   
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Star>::cur = Node<Star>::beg; Node<Star>::cur != 0; Node<Star>::cur = Node<Star>::cur->next )
         {
             if ( Node<Missile>::cur->obj->Crash ( Node<Star>::cur->obj ) )
             {
                Node<Missile> *tmp = Node<Missile>::cur;
                Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<BiStar>::cur = Node<BiStar>::beg; Node<BiStar>::cur != 0; Node<BiStar>::cur = Node<BiStar>::cur->next )
         {
             if ( Node<Missile>::cur->obj->Crash ( Node<BiStar>::cur->obj ) )
             {
                Node<Missile> *tmp = Node<Missile>::cur;
                Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<BlackHole>::cur = Node<BlackHole>::beg; Node<BlackHole>::cur != 0; Node<BlackHole>::cur = Node<BlackHole>::cur->next )
         {
             if ( Node<Missile>::cur->obj->Crash ( Node<BlackHole>::cur->obj ) )
             {
                Node<Missile> *tmp = Node<Missile>::cur;
                Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
         {
             if ( Node<Missile>::cur->obj->Crash ( Node<Planet>::cur->obj ) )
             {
                Node<Missile> *tmp = Node<Missile>::cur;
                Node<Missile>::cur = Node<Missile>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         Node<Missile>::cur = Node<Missile>::cur->next;
     }
     for ( Node<Bullet>::cur = Node<Bullet>::beg; Node<Bullet>::cur != 0; )
     {
         for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
         {
             if ( Node<Bullet>::cur->obj->Damage ( Node<SpaceShip>::cur->obj ) )
             {
                Node<Bullet> *tmp = Node<Bullet>::cur;
                Node<Bullet>::cur = Node<Bullet>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Fort>::cur = Node<Fort>::beg; Node<Fort>::cur != 0; Node<Fort>::cur = Node<Fort>::cur->next )
         {
             if ( Node<Bullet>::cur->obj->Damage ( Node<Fort>::cur->obj ) )
             {
                Node<Bullet> *tmp = Node<Bullet>::cur;
                Node<Bullet>::cur = Node<Bullet>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Missile>::cur = Node<Missile>::beg; Node<Missile>::cur != 0; Node<Missile>::cur = Node<Missile>::cur->next )
         {
             if ( Node<Bullet>::cur->obj->Damage ( Node<Missile>::cur ) )
             {
                Node<Bullet> *tmp = Node<Bullet>::cur;
                Node<Bullet>::cur = Node<Bullet>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Star>::cur = Node<Star>::beg; Node<Star>::cur != 0; Node<Star>::cur = Node<Star>::cur->next )
         {
             if ( Node<Bullet>::cur->obj->Crash ( Node<Star>::cur->obj ) )
             {
                Node<Bullet> *tmp = Node<Bullet>::cur;
                Node<Bullet>::cur = Node<Bullet>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<BiStar>::cur = Node<BiStar>::beg; Node<BiStar>::cur != 0; Node<BiStar>::cur = Node<BiStar>::cur->next )
         {
             if ( Node<Bullet>::cur->obj->Crash ( Node<BiStar>::cur->obj ) )
             {
                Node<Bullet> *tmp = Node<Bullet>::cur;
                Node<Bullet>::cur = Node<Bullet>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
             over = 0;
             continue;
         }
         for ( Node<BlackHole>::cur = Node<BlackHole>::beg; Node<BlackHole>::cur != 0; Node<BlackHole>::cur = Node<BlackHole>::cur->next )
         {
             if ( Node<Bullet>::cur->obj->Crash ( Node<BlackHole>::cur->obj ) )
             {
                Node<Bullet> *tmp = Node<Bullet>::cur;
                Node<Bullet>::cur = Node<Bullet>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
         {
             if ( Node<Bullet>::cur->obj->Crash ( Node<Planet>::cur->obj ) )
             {
                Node<Bullet> *tmp = Node<Bullet>::cur;
                Node<Bullet>::cur = Node<Bullet>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         if ( Node<Bullet>::cur->obj->Crash())
         {
            Node<Bullet> *tmp = Node<Bullet>::cur;
            Node<Bullet>::cur = Node<Bullet>::cur->next;
            delete tmp;
            break;
         }
         Node<Bullet>::cur->obj->Update();
         Node<Bullet>::cur = Node<Bullet>::cur->next;
     }
     for ( Node<FuelBox>::cur = Node<FuelBox>::beg; Node<FuelBox>::cur != 0; )
     {
         for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
         {
             if ( Node<FuelBox>::cur->obj->Fill ( Node<SpaceShip>::cur->obj ) )
             {
                Node<FuelBox> *tmp = Node<FuelBox>::cur;
                Node<FuelBox>::cur = Node<FuelBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Star>::cur = Node<Star>::beg; Node<Star>::cur != 0; Node<Star>::cur = Node<Star>::cur->next )
         {
             if ( Node<FuelBox>::cur->obj->Crash ( Node<Star>::cur->obj ) )
             {
                Node<FuelBox> *tmp = Node<FuelBox>::cur;
                Node<FuelBox>::cur = Node<FuelBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<BiStar>::cur = Node<BiStar>::beg; Node<BiStar>::cur != 0; Node<BiStar>::cur = Node<BiStar>::cur->next )
         {
             if ( Node<FuelBox>::cur->obj->Crash ( Node<BiStar>::cur->obj ) )
             {
                Node<FuelBox> *tmp = Node<FuelBox>::cur;
                Node<FuelBox>::cur = Node<FuelBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<BlackHole>::cur = Node<BlackHole>::beg; Node<BlackHole>::cur != 0; Node<BlackHole>::cur = Node<BlackHole>::cur->next )
         {
             if ( Node<FuelBox>::cur->obj->Crash ( Node<BlackHole>::cur->obj ) )
             {
                Node<FuelBox> *tmp = Node<FuelBox>::cur;
                Node<FuelBox>::cur = Node<FuelBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
         {
             if ( Node<FuelBox>::cur->obj->Crash ( Node<Planet>::cur->obj ) )
             {
                Node<FuelBox> *tmp = Node<FuelBox>::cur;
                Node<FuelBox>::cur = Node<FuelBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         Node<FuelBox>::cur->obj->Update();
         Node<FuelBox>::cur = Node<FuelBox>::cur->next;
     }
     for ( Node<ExplosiveBox>::cur = Node<ExplosiveBox>::beg; Node<ExplosiveBox>::cur != 0; )
     {
         for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
         {
             if ( Node<ExplosiveBox>::cur->obj->Fill ( Node<SpaceShip>::cur->obj ) )
             {
                 Node<ExplosiveBox> *tmp = Node<ExplosiveBox>::cur;
                 Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next;
                 delete tmp;
                 over = 1;
                 break;
              }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Star>::cur = Node<Star>::beg; Node<Star>::cur != 0; Node<Star>::cur = Node<Star>::cur->next )
         {
             if ( Node<ExplosiveBox>::cur->obj->Crash ( Node<Star>::cur->obj ) )
             {
                Node<ExplosiveBox> *tmp = Node<ExplosiveBox>::cur;
                Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<BiStar>::cur = Node<BiStar>::beg; Node<BiStar>::cur != 0; Node<BiStar>::cur = Node<BiStar>::cur->next )
         {
             if ( Node<ExplosiveBox>::cur->obj->Crash ( Node<BiStar>::cur->obj ) )
             {
                Node<ExplosiveBox> *tmp = Node<ExplosiveBox>::cur;
                Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<BlackHole>::cur = Node<BlackHole>::beg; Node<BlackHole>::cur != 0; Node<BlackHole>::cur = Node<BlackHole>::cur->next )
         {
             if ( Node<ExplosiveBox>::cur->obj->Crash ( Node<BlackHole>::cur->obj ) )
             {
                Node<ExplosiveBox> *tmp = Node<ExplosiveBox>::cur;
                Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next;
                delete tmp;
                over = 1;
                break;
             }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         for ( Node<Planet>::cur = Node<Planet>::beg; Node<Planet>::cur != 0; Node<Planet>::cur = Node<Planet>::cur->next )
         {
            if ( Node<ExplosiveBox>::cur->obj->Crash ( Node<Planet>::cur->obj ) )
            {
               Node<ExplosiveBox> *tmp = Node<ExplosiveBox>::cur;
               Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next;
               delete tmp;
               over = 1;
               break;
            }
         }
         if ( over )
         {
            over = 0;
            continue;
         }
         Node<ExplosiveBox>::cur->obj->Update();
         Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next;
     }
}

void PassObj ()
{
     for ( Node<Gas>::cur = Node<Gas>::beg; Node<Gas>::cur != 0; )
     {
         if ( Node<Gas>::cur->obj->nowTime >= 10 )
         {
            Node<Gas> *tmp = Node<Gas>::cur;
            Node<Gas>::cur = Node<Gas>::cur->next;
            delete tmp;
         }
         else
         {
             Node<Gas>::cur->obj->Update();
             Node<Gas>::cur = Node<Gas>::cur->next;
         }
     }
     for ( Node<Explosion>::cur = Node<Explosion>::beg; Node<Explosion>::cur != 0; )
     {
         if ( Node<Explosion>::cur->obj->nowTime >= 20 )
         {
            Node<Explosion> *tmp = Node<Explosion>::cur;
            Node<Explosion>::cur = Node<Explosion>::cur->next;
            delete tmp;
         }
         else
         {
             Node<Explosion>::cur->obj->Update();
             Node<Explosion>::cur = Node<Explosion>::cur->next;
         }
     }
     for ( Node<Station>::cur = Node<Station>::beg; Node<Station>::cur != 0; Node<Station>::cur = Node<Station>::cur->next )
     {
         for ( Node<SpaceShip>::cur = Node<SpaceShip>::beg; Node<SpaceShip>::cur != 0; Node<SpaceShip>::cur = Node<SpaceShip>::cur->next )
             Node<Station>::cur->obj->Refresh(Node<SpaceShip>::cur->obj);
     }
}

void KeyControl ()
{
     switch (control_mode)
     {
            case 0: //Mode1
                 if (keystate[0x0]==1) //Front
                 {
                    mainship->UseJet(0);
                 }
                 if (keystate[0x1]==1) //Back
                 {
                    mainship->UseJet(2);
                 }
                 if (keystate[0x2]==1) //Left
                 {
                    mainship->UseJet(1);
                 }
                 if (keystate[0x3]==1) //Right
                 {
                    mainship->UseJet(3);
                 }
                 if (keystate[0x4]==1) //Spin1
                 {
                    mainship->SpinWeapon1();
                 }
                 if (keystate[0x5]==1) //Spin2
                 {
                    mainship->SpinWeapon2();
                 }
                 if (keystate[0x6]==1) //fire bullet
                 {
                    mainship->FireBullet();
                 }
                 break;
            case 1: //Mode2
                 if (keystate[0x0]==1) //Front
                 {
                    mainship->UseJet(0);
                 }
                 if (keystate[0x1]==1) //Back
                 {
                    mainship->UseJet(2);
                 }
                 if (keystate[0x2]==1) //SpinCtrl1
                 {
                    mainship->SpinCtrl1();
                 }
                 if (keystate[0x3]==1) //SpinCtrl2
                 {
                    mainship->SpinCtrl2();
                 }
                 if (keystate[0x4]==1) //Spin1
                 {
                    mainship->SpinWeapon1();
                 }
                 if (keystate[0x5]==1) //Spin2
                 {
                    mainship->SpinWeapon2();
                 }
                 if (keystate[0x6]==1) //fire bullet
                 {
                    mainship->FireBullet();
                 }
                 break;
            case 2: //Mode3
                 //todo
                 break;
     }
}

void DrawAttributes ()
{
     glLoadIdentity();
     glDisable(GL_LIGHTING);
     double r = double(glutGet(GLUT_SCREEN_HEIGHT)) / double(glutGet(GLUT_SCREEN_WIDTH));
     r-=0.12;
     glColor4f(1,0,0,0.5);
     glBegin(GL_QUADS);
      glVertex3f(-0.8,r,-VIEW_GAME_FRONT-0.1);
      glVertex3f(-0.8,r-0.05,-VIEW_GAME_FRONT-0.1);
      glVertex3f(float(mainship->HP)/Sp_HPmax*1.6-0.8,r-0.05,-VIEW_GAME_FRONT-0.1);
      glVertex3f(float(mainship->HP)/Sp_HPmax*1.6-0.8,r,-VIEW_GAME_FRONT-0.1);
     glEnd();
     glColor4f(0,1,1,0.5);
     glBegin(GL_QUADS);
      glVertex3f(-0.8,r-0.05,-VIEW_GAME_FRONT-0.1);
      glVertex3f(-0.8,r-0.1,-VIEW_GAME_FRONT-0.1);
      glVertex3f(float(mainship->dMf)/Sp_FuelMax*1.6-0.8,r-0.1,-VIEW_GAME_FRONT-0.1);
      glVertex3f(float(mainship->dMf)/Sp_FuelMax*1.6-0.8,r-0.05,-VIEW_GAME_FRONT-0.1);
     glEnd();
     glColor4f(1,1,0,0.5);
     glBegin(GL_QUADS);
      glVertex3f(-0.8,r-0.1,-VIEW_GAME_FRONT-0.1);
      glVertex3f(-0.8,r-0.15,-VIEW_GAME_FRONT-0.1);
      glVertex3f(float(mainship->dMe)/Sp_ExplMax*1.6-0.8,r-0.15,-VIEW_GAME_FRONT-0.1);
      glVertex3f(float(mainship->dMe)/Sp_ExplMax*1.6-0.8,r-0.1,-VIEW_GAME_FRONT-0.1);
     glEnd();
     glEnable(GL_LIGHTING);
     glLoadIdentity();
     glLightfv ( GL_LIGHT0, GL_POSITION, light_position_menu1 );
     glTranslated(0,0,- SHOW_SHIP_DISTANCE);
     glRotated(1,0,0, slope_angle);
     glRotated(0,1,0, - mainship->dAgw);
     glTranslated( - mainship->dPx, 0, - mainship->dPy );
}

void BeforeExitMenu ()
{
     switch ( now_menu )
     {
            case CASE_MENU:
                 DeleteResMenu ( NUM_CASE_MENU );
                 break;
            case CASE_MENU_Continue:
                 DeleteResMenu ( NUM_CASE_SUB_MENU_Continue );
                 break;
            case CASE_MENU_StartGame:
                 DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame );
                 break;
            case CASE_MENU_Options:
                 DeleteResMenuOptions ();
                 break;
            case CASE_MENU_Help:
            case CASE_MENU_Demo:
            case CASE_MENU_About:
            case CASE_WIN:
            case CASE_LOSE:
            case CASE_ERROR:
                 DeleteResText ();
                 break;
            case CASE_SUB_MENU_StartGame_TimeMode:
                 DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame_TimeMode );
                 break;
            case CASE_SUB_MENU_StartGame_FightingMode:
                 DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame_FightingMode );
                 break;
            case CASE_SUB_MENU_StartGame_FreeMode:
                 DeleteResMenu ( NUM_CASE_SUB_MENU_StartGame_FreeMode );
                 break;
            case CASE_SUB_MENU_Options_Keyboard:
                 DeleteResText ();
                 break;
     }
     DeleteResBackground ();
     DeleteResFontKeyBoard ();
}

void BeforeExitGame ()
{
     if (mainship && (gamestate!=CASE_GAMEfight || bossfort)) Save ( SAV_LASTONE );
     DeleteResBackground ();
     DeleteAllObjs ();
}

void BacktoMenu ()
{
     BeforeExitGame();
     double ar = double(glutGet(GLUT_SCREEN_WIDTH)) / double(glutGet(GLUT_SCREEN_HEIGHT));
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glFrustum(-ar,ar,-1,1,VIEW_FRONT,VIEW_BACK);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     LoadResBackground ( BMP_BACKGROUND_MENU );
     glEnable(GL_TEXTURE_2D);
     glColor4f(1,1,1,1);
     glLightfv ( GL_LIGHT0, GL_POSITION, light_position_menu2 );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_menu    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_menu    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_menu   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission_menu   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_menu );
     now_item = 0;
     to_item = 1;
     now_alpha = 0;
     to_alpha = 1;
     wait_time = 0;
     Time = 0;
     gamestate = 0;
     now_menu = -1;//from GAME
}

void LoadOptions ()
{
     //todo
}

void SaveOptions ()
{
     //todo
}

void SaveTime ()//to improve
{
     std::ofstream fileout( REC_TIME );
     fileout<<"Time="<<FreshTime*Time<<"ms;"<<std::endl;
     fileout.close();
}
