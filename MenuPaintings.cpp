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
 *  Name: MenuPaintings.cpp
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震)
 *  Description: 小课题Outspacer菜单的绘制实现 
 */
#include "MenuPaintings.h"

int BmpRes::load ( char* filename )
{
    if (state) delete[] data;
    int tmp;
    std::ifstream picfile (filename, std::ios::binary);
    if (picfile.good())
    {
       picfile.seekg (0x22,std::ios::beg);
       picfile.read ((char*)&size,4);
       picfile.seekg (0xa,std::ios::beg);
       picfile.read ((char*)&tmp,4);
       picfile.seekg (tmp,std::ios::beg);
       data = new char[size];
       picfile.read (data,size);
       picfile.close();
       state = 1;
    }
    else state = 0;
    return state;
}

int BmpRes::release ()
{
    if (!state) return 0;
    delete[] data;
    state = 0;
    return 0;
}

BmpRes::~BmpRes ()
{
         if (state) delete[] data;
}

void LoadResBackground ( char *filename )
{
     glGenTextures ( 1 , &texBackground );
     image.load ( filename );
     glBindTexture ( GL_TEXTURE_2D, texBackground );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
     glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
     glTexImage2D ( GL_TEXTURE_2D, 0, 3, 512, 256, 0, GL_RGB,
                    GL_UNSIGNED_BYTE, image.data );
     image.release ();
}

void LoadResMenu ( char *filename , int num )
{
     texID = new unsigned [num];
     glGenTextures ( num , texID );
     image.load ( filename );
     for ( int tmp1 = 0; tmp1 < num; ++ tmp1 )
     {
         glBindTexture ( GL_TEXTURE_2D, texID[tmp1] );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
         glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
         glTexImage2D ( GL_TEXTURE_2D, 0, 3, PIC_MENU_WIDTH, PIC_MENU_HEIGHT, 0, GL_RGB,
                        GL_UNSIGNED_BYTE, image.data + (tmp1 * PIC_MENU_W_3MUL_H) );
     }
     image.release ();
}

void LoadResMenuOptions ()
{
     texID = new unsigned [NUM_CASE_SUB_MENU_Options];
     glGenTextures ( NUM_CASE_SUB_MENU_Options , texID );
     image.load ( BMP_SUBMENU_Options );
     for ( int tmp1 = 0; tmp1 < NUM_CASE_SUB_MENU_Options; ++ tmp1 )
     {
         glBindTexture ( GL_TEXTURE_2D, texID[tmp1] );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
         glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
         glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
         if ( tmp1 == 0 && control_mode )
         {
              glTexImage2D ( GL_TEXTURE_2D, 0, 3, PIC_MENU_WIDTH, PIC_MENU_HEIGHT, 0, GL_RGB,
                             GL_UNSIGNED_BYTE, image.data + ( ( tmp1 + control_mode + 3 ) * PIC_MENU_W_3MUL_H) );
         }
         else if ( tmp1 == 2 && cheat_mode )
         {
              glTexImage2D ( GL_TEXTURE_2D, 0, 3, PIC_MENU_WIDTH, PIC_MENU_HEIGHT, 0, GL_RGB,
                             GL_UNSIGNED_BYTE, image.data + ( ( tmp1 + 4 ) * PIC_MENU_W_3MUL_H) );
         }
         else
         {
              glTexImage2D ( GL_TEXTURE_2D, 0, 3, PIC_MENU_WIDTH, PIC_MENU_HEIGHT, 0, GL_RGB,
                             GL_UNSIGNED_BYTE, image.data + ( tmp1 * PIC_MENU_W_3MUL_H) );
         }
     }
     image.release ();
}

void LoadResText ( char *filename )
{
     texID = new unsigned;
     glGenTextures ( 1 , texID );
     image.load ( filename );
     glBindTexture ( GL_TEXTURE_2D, *texID );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
     glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
     glTexImage2D ( GL_TEXTURE_2D, 0, 4, 512, 512, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, image.data );
     image.release ();
}

void LoadResFontKeyBoard ()
{
     glGenTextures ( 1 , &texFont );
     image.load ( BMP_FONT_KeyBoard );
     glBindTexture ( GL_TEXTURE_2D, texFont );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
     glTexParameterf ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
     glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
     glTexImage2D ( GL_TEXTURE_2D, 0, 4, 512, 256, 0, GL_RGBA,
                    GL_UNSIGNED_BYTE, image.data );
     image.release ();
}

void DeleteResBackground ()
{
     glDeleteTextures ( 1 , &texBackground );
}

void DeleteResMenu ( int num )
{
     glDeleteTextures ( num , texID );
     delete[] texID;
}

void DeleteResMenuOptions ()
{
     glDeleteTextures ( NUM_CASE_SUB_MENU_Options , texID );
     delete[] texID;
}

void DeleteResText ()
{
     glDeleteTextures ( 1 , texID );
     delete texID;
}

void DeleteResFontKeyBoard () //need new idea
{
     glDeleteTextures ( 1 , &texFont );
}

void DrawMenu ( int num )
{
     DrawBackground ();
     
     float tmpA1 = 180. / float (num);
     float tmpD1 = ( SHOW_MENU_HALF_HEIGHT ) / tan ( M_PI / float (num) ) * now_alpha;
     
     for ( int tmp1 = 0; tmp1 < num; ++ tmp1 )
     {
         glLoadIdentity ();
         glTranslatef ( 0, 0, - SHOW_MENU_DISTANCE - tmpD1 );
         glRotatef ( ( now_item - 1 - float (tmp1) ) * tmpA1 * 2 ,-1, 0, 0 );
         
         glBindTexture ( GL_TEXTURE_2D, texID[tmp1] );
         glBegin ( GL_QUADS );
         glTexCoord2f ( 0, 0 );
         glVertex3f ( - now_alpha * SHOW_MENU_HALF_WIDTH, - now_alpha * SHOW_MENU_HALF_HEIGHT, tmpD1 );
         glTexCoord2f ( 1, 0 );
         glVertex3f (   now_alpha * SHOW_MENU_HALF_WIDTH, - now_alpha * SHOW_MENU_HALF_HEIGHT, tmpD1 );
         glTexCoord2f ( 1, 1 );
         glVertex3f (   now_alpha * SHOW_MENU_HALF_WIDTH,   now_alpha * SHOW_MENU_HALF_HEIGHT, tmpD1 );
         glTexCoord2f ( 0, 1 );
         glVertex3f ( - now_alpha * SHOW_MENU_HALF_WIDTH,   now_alpha * SHOW_MENU_HALF_HEIGHT, tmpD1 );
         glEnd ();
     }
     glLoadIdentity ();
}

void DrawText ()
{
     DrawBackground ();
     glBindTexture ( GL_TEXTURE_2D, *texID );
     glBegin ( GL_QUADS );
     glTexCoord2f ( 0, 0 );
     glVertex4f ( - now_alpha * SHOW_TEXT_HALF_WIDTH, - now_alpha * SHOW_TEXT_HALF_WIDTH, - SHOW_MENU_DISTANCE, now_alpha );
     glTexCoord2f ( 1, 0 );
     glVertex4f (   now_alpha * SHOW_TEXT_HALF_WIDTH, - now_alpha * SHOW_TEXT_HALF_WIDTH, - SHOW_MENU_DISTANCE, now_alpha );
     glTexCoord2f ( 1, 1 );
     glVertex4f (   now_alpha * SHOW_TEXT_HALF_WIDTH,   now_alpha * SHOW_TEXT_HALF_WIDTH, - SHOW_MENU_DISTANCE, now_alpha );
     glTexCoord2f ( 0, 1 );
     glVertex4f ( - now_alpha * SHOW_TEXT_HALF_WIDTH,   now_alpha * SHOW_TEXT_HALF_WIDTH, - SHOW_MENU_DISTANCE, now_alpha );
     glEnd ();
}

void DrawBackground ()
{
     glColor4f ( 1, 1, 1, 1 );
     glLoadIdentity ();
     glBindTexture ( GL_TEXTURE_2D, texBackground );
     glBegin ( GL_QUADS );
     glTexCoord2f ( 0, 0 );
     glVertex3f ( - VIEW_BACK2, - VIEW_BACK, - VIEW_BACK );
     glTexCoord2f ( 1, 0 );
     glVertex3f (   VIEW_BACK2, - VIEW_BACK, - VIEW_BACK );
     glTexCoord2f ( 1, 1 );
     glVertex3f (   VIEW_BACK2,   VIEW_BACK, - VIEW_BACK );
     glTexCoord2f ( 0, 1 );
     glVertex3f ( - VIEW_BACK2,   VIEW_BACK, - VIEW_BACK );
     glEnd ();
}
