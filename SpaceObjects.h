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
 *  Name: SpaceObjects.h
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震) 
 *  Description: 小课题Object头文件 
 */
//前向引用声明 
class Planet;
class SpaceShip;
class Missile;
class Bullet;
class FuelBox;
class ExplosiveBox;
class Fort;

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

#ifndef _GAMEOPTIONS_
extern float slope_angle; //angle of the viewing direction
extern int cheat_mode; //whether cheating mode is on
extern char key[0xb]; //key settings
extern int control_mode; //controlling mode
#define _GAMEOPTIONS_
#endif

#ifndef _QUAD_
extern GLUquadricObj* QuadTmp;
#define _QUAD_
#endif

#ifndef _LIGHT_
extern GLfloat light_ambient_menu[];
extern GLfloat light_diffuse_menu[];
extern GLfloat light_specular_menu[];
extern GLfloat light_position_menu1[];
extern GLfloat light_position_menu2[];
extern GLfloat mat_ambient_menu[];
extern GLfloat mat_diffuse_menu[];
extern GLfloat mat_specular_menu[];
extern GLfloat high_shininess_menu[];
extern GLfloat mat_ambient_Star[];
extern GLfloat mat_diffuse_Star[];
extern GLfloat mat_specular_Star[];
extern GLfloat mat_emission_Star[];
extern GLfloat high_shininess_Star[];
extern GLfloat mat_ambient_Obj[];
extern GLfloat mat_diffuse_Obj[];
extern GLfloat mat_specular_Obj[];
extern GLfloat mat_emission_Obj[];
extern GLfloat high_shininess_Obj[];
#define _LIGHT_
#endif

#include "GameAttributes.h"
#include "MenuItems.h"

#ifndef _RANDOM_
//-----------------Double Random---------------
class srand_d
{
             static int seed;
             double x;
      public:
             srand_d();
             double rand();
             double normdev();
};
#define _RANDOM_
#endif

#ifndef _SPACEOBJS_
//-----------------List Nodes------------------
template <class Obj>
struct Node
{
       Obj * obj;
       Node<Obj> * last;
       Node<Obj> * next;
       
       static unsigned num;
       static Node<Obj> * beg;
       static Node<Obj> * cur;
       static Node<Obj> * end;

       Node ( Obj * newobj );
       ~Node ();
       static void ins( Obj * newobj ); //put newobj after cur
       static void del(); //delete cur, just like Backspace

   private:
       Node() {}
};
/*
How to use:
    new Node<Obj>( new Obj(构造函数参数) );
    ...
    Node<Obj> * cur;
    ...cur 指向了要删的 Obj 的 Node 时...
    delete cur;
*/

//-----------------------Class List Node-------------------------
template <class Obj>
unsigned Node<Obj>::num = 0;
template <class Obj>
Node<Obj> * Node<Obj>::beg = 0;
template <class Obj>
Node<Obj> * Node<Obj>::cur = 0;
template <class Obj>
Node<Obj> * Node<Obj>::end = 0;

template <class Obj>
Node<Obj>::Node ( Obj * newobj ) : obj(newobj), last(Node<Obj>::end), next(0)
{
     if ( Node::end ) Node::end->next = this;
     else Node::beg = this;
     Node::end = this;
     ++ Node::num;
}

template <class Obj>
Node<Obj>::~Node ()
{
     delete obj;
     if (last) last->next = next;
     else Node::beg = next;
     if (next) next->last = last;
     else Node::end = last;
     -- Node::num;
}

template <class Obj>
void Node<Obj>::ins( Obj * newobj )
{
     if ( Node::cur )
     {
        Node * tmp = Node::cur->next;
        Node::cur->next = new Node<Obj>;
        ( Node::cur->next )->last = Node::cur;
        Node::cur = Node::cur->next;
        Node::cur->obj = newobj;
        Node::cur->next = tmp;
        if (!tmp) Node::end = Node::cur;
     }
     else
     {
         Node * tmp = Node::beg;
         Node::beg = new Node<Obj>;
         Node::beg->last = 0;
         Node::beg->next = tmp;
         Node::beg->obj = newobj;
         Node::cur = Node::beg;
         if (tmp) tmp->last = Node::cur;
         else Node::end = Node::cur;
     }
     ++ Node::num;
}

template <class Obj>
void Node<Obj>::del()
{
     Node *tmp=Node::cur;
     if ( Node::cur )
     {
          Node::cur = Node::cur->last;
          delete tmp;
     }
}

//-----------------------Class Objects---------------------------
class Star //恒星 
{
    protected:
              double dM;       //质量 
              double dPx,dPy;  //坐标 
              double dR;       //半径 
    public:
        Star (double m, double px, double py, double r): dM(m), dPx(px), dPy(py), dR(r)  {}
        void Draw ();
        template <class Obj> void Affect ( Obj *obj );//calculate the acceleration
        
        friend class Planet;
        friend class SpaceShip;
        friend class Fort;
        friend class Bullet;
        friend class Missile;
        friend class FuelBox;
        friend class ExplosiveBox;
        friend void Save ( char * filename );
};

class BiStar //双星 
{
      protected:
                double dM1,dM2;  //质量 
                double dPCx,dPCy;//质心位置 
                double dR1,dR2;  //半径 
                double dAo;      //初始角度 
                double dD,dE;    //星星最远距离，离心率
                int direc;       //顺时针为-1,逆时针为1 
                int T;           //周期 ,单位为DeltaTime 

                double dA1,dB1,dA2,dB2;    //半长轴长，半短轴长（正负取决于direc） 构造时计算
                double omega;              //2π/T 构造时计算 ,单位为DeltaTime^-1 

                double dAg;         //偏近点角 
                int nowTime;        //作为是否需要校正的判据，每T/2时校正并归零。 
                double dP1x,dP1y;   //当前双星1位置 'Update'中更新
                double dP2x,dP2y;   //当前双星2位置 'Update'中更新
                double dAg_;        //预估偏近点角
                double dP1x_,dP1y_; //预估双星1位置 'Update'中更新
                double dP2x_,dP2y_; //预估双星2位置 'Update'中更新

      public:
             BiStar (double m1, double m2, double pcx, double pcy,
                     double r1, double r2, double ao, double ag,
                     double  d, double  e, int direction, int period);
             void Draw ();
             void Update ();                //Update the position and angle position (if it has)
             template <class Obj> void Affect ( Obj *obj );       //calculate the acceleration

             friend class Fort;
             friend class Bullet;
             friend class Missile;
             friend class FuelBox;
             friend class ExplosiveBox;
             friend void Save ( char * filename );
};

class BlackHole //用漩涡表示黑洞好了
{
      protected:
                double dM;
                double dPx,dPy;
                double dR;      //半径内算吸入
      public:
             BlackHole (double m, double px, double py, double r): dM(m), dPx(px), dPy(py), dR(r) {}
             void Draw ();
             template <class Obj> void Affect ( Obj *obj );       //calculate the acceleration

             friend class Bullet;
             friend class Missile;
             friend class FuelBox;
             friend class ExplosiveBox;
             friend void Save ( char * filename );
};

class Planet //行星
{
      protected:
                double dM;      //质量
                double dPx,dPy; //位置
                double dVx,dVy; //速度 
                double dR;      //半径
                
                double dPx_,dPy_; //预估位置 'Update'中更新 
                double dAx,dAy;   //当前位置加速度 'Affect'中更新 
                double dAx_,dAy_; //预估位置加速度 'Affect'中更新 
      public:
             Planet (double m, double px, double py, double vx, double vy, double r);
             void Draw ();
             void Update ();                //Update the position and angle position (if it has)
             template <class Obj> void Affect ( Obj *obj );       //calculate the acceleration
             
             friend class Star;
             friend class BiStar;
             friend class BlackHole;
             friend class Fort;
             friend class Bullet;
             friend class Missile;
             friend class FuelBox;
             friend class ExplosiveBox;
             friend void Save ( char * filename );
};

class SpaceShip //我们的飞船啦 ,all angle position begin with -z direction
{
      public:
              double dMB;                      //mass of the ball
              double dMb;                      //mass of the bar/board
              double dMw;                      //mass of the weapon
              double dJw;                      //moment inertia of the weapon
              double dR;                       //Radius of the ball
              double dLb, dHb;                 //size of the bar/board, L for HalfLength, H for HalfHeight
              
              double dJb;                      //moment inertia of the board, calculate this constant in the constructor

              double dPx,dPy;                  //position of the ball
              double dAg;                      //angle position of the bar/board
              double dAgw;                     //angle position of the weapon, move it can affect dAg (make sure dJw * dAgw + dJb * dAg = Const)
              double dVx,dVy;                  //velocity of the ball
              double dWg;                      //angle velocity of the bar/board
              double dAgc;                     //angle position of the direction you want to move, 'c' for controller
              int JetI;                        //the rate of acceleration you want to move, 'I' for intensity

              double dMf;                      //mass of the fuel
              double dMe;                      //mass of the explosive
              int HP;                          //HP of the ship

              double dPx_,dPy_,dAg_; //预估位置 'Update'中更新 
              double dAx,dAy,dAga;   //当前位置加速度 'Affect'中更新 ,Aga指角加速度（Angle Acceleration） 
              double dAx_,dAy_,dAga_;//预估位置加速度 'Affect'中更新 
              
           SpaceShip (double px, double py, double vx, double vy, double mf, double me,
                      int hp = Sp_HPmax, int jeti = Sp_JetImin,
                      double ag = 0, double wg = 0, double agw = 0, double agc = 0);
           void Draw ();
           void Update ();           //Update the position and angle position (if it has)
           void UseJet ( int ID );   //jet in the opposite direction of 'dAgc'
           void FireBullet ();       //create (bullet at initial speed), direction: dAgw
           void SpinCtrl1 ();        //spin the controller in the positive direction
           void SpinCtrl2 ();        //spin the controller in the negative direction
           void SpinWeapon1 ();      //spin the weapon in the positive direction
           void SpinWeapon2 ();      //spin the weapon in the negative direction
           void EnhanceJet ();       //boost the intensity of jet by one level
           void ReduceJet ();        //lower the intensity of jet by one level

//           int Crash ( Star * star );
//           int Crash ( BlackHole * blackhole );
//           int Crash ( Planet * planet );
//           int Crash ( BiStar * bistar );
};

class Fort //炮台 
{
      public:
              double dAo;                //angle of the normal direction (法向)
              double dR;                 //radius of the fort
              int Fq;                    //frequency of shooting, meanwhile, this is the HP of the Fort
              int AttachCate;            //what is the type of the object that the fort attaches to
                                         //1 for Star, 2 for BiStar, 4 for Planet, 5 for ...
                                         //0x02 for BiStar1, 0x12 for BiStar2, 0x22,0x32 for spin with the BiStar
              void * Attach;             //point at the object that the fort attaches to
              int AimCate;
              void * Aim;                //Fort always aims at "Aim"
              int nowTime;

              double dAg;                //angle of the fort, aims at "Aim"
              double Sin_Ag,Cos_Ag;
              double Sin_Ao,Cos_Ao;
              double dPx,dPy;            //As a middle data, will be Updated by 'Update'
              double dPx_,dPy_;          //As a middle data, will be Updated by 'Update'

           Fort (double ao, double r, int fq, int attachcate, void *attach, int aimcate, void *aim, int nowtime = 0);
           void Draw ();
           void Update ();               //Update the angle position by using AI
           void Fire ();
};

class Missile //导弹 
{
      protected:
                double dMB;         //Mass of the Ball
                double dMf;         //Mass of the Fuel
                double dR;          //Radius of the Missile
                double dPx,dPy;     //Position of the Missile
                double dVx,dVy;     //Velocity of the Missile
                double dAg;         //Angle position towards the "Aim"
                int AimCate;
                void * Aim;         //Missile always flies towards "Aim"
                double Sin_Ag,Cos_Ag;

                double dPx_,dPy_; //预估位置 'Update'中更新 
                double dAx,dAy;   //当前位置加速度 'Affect'中更新 
                double dAx_,dAy_; //预估位置加速度 'Affect'中更新 

      public:
             Missile (double px, double py, double vx, double vy, int aimcate, void *aim);
             void Draw ();
             void Update ();           //用AI更新喷射角 Update the position and angle position (if it has)
             int Damage ( SpaceShip * ship );
             int Damage ( Fort * fort );
             int Damage ( Node<Missile> * missile );
             int Crash ( Star * star );
             int Crash ( Planet * planet );
             int Crash ( BiStar * bistar );
             int Crash ( BlackHole * blackhole );

             friend class Star;
             friend class BiStar;
             friend class BlackHole;
             friend class Planet;
             friend class Bullet;
             friend void Save ( char * filename );
};

class Bullet //小圆炮 
{
      protected:
                double dMB;           //Mass of the Ball
                double dR;            //Radius of the Bullet
                double dPx,dPy;       //Position of the Bullet
                double dVx,dVy;       //Velocity of the Bullet

                double dPx_,dPy_; //预估位置 'Update'中更新 
                double dAx,dAy;   //当前位置加速度 'Affect'中更新 
                double dAx_,dAy_; //预估位置加速度 'Affect'中更新 
                
      public:
             Bullet (double px, double py, double vx, double vy);
             void Draw ();
             void Update ();          //Update the position and angle position (if it has)
             int Damage ( SpaceShip * ship );
             int Damage ( Fort* fort );
             int Damage ( Node<Missile> * missile );
             int Crash ( Star * star );
             int Crash ( Planet* planet );
             int Crash ( BiStar* bistar );
             int Crash ( BlackHole* blackhole );
             int Crash();

             friend class Star;
             friend class BiStar;
             friend class BlackHole;
             friend class Planet;
             friend void Save ( char * filename );
};

class FuelBox //能量补给箱 
{
      protected:
                double dMf;           //Mass of the fuel
                double dPx,dPy;       //Position of the FuelBox
                double dVx,dVy;       //Velocity of the FuelBox
                double dL;            //Size of the FuelBox

                double dPx_,dPy_; //预估位置 'Update'中更新 
                double dAx,dAy;   //当前位置加速度 'Affect'中更新 
                double dAx_,dAy_; //预估位置加速度 'Affect'中更新 
                
      public:
             FuelBox (double px, double py, double vx, double vy):
                     dMf(FuelBoxMass), dPx(px), dPy(py), dVx(vx), dVy(vy), dL(FuelBoxLength)
                     {}
             void Draw ();
             void Update ();           //Update the position and angle position (if it has)
             int Fill ( SpaceShip * ship );
             int Crash ( Star * star );
             int Crash ( Planet* planet );
             int Crash ( BiStar* bistar );
             int Crash ( BlackHole* blackhole );

             friend class Star;
             friend class BiStar;
             friend class BlackHole;
             friend class Planet;
             friend void Save ( char * filename );
};

class ExplosiveBox //火药补给箱 
{
      protected:
                double dMe;           //Mass of the Explosive
                double dPx,dPy;       //Position of the ExplosiveBox
                double dVx,dVy;       //Velocity of the ExplosiveBox
                double dL;            //Size of the ExplosiveBox

                double dPx_,dPy_; //预估位置 'Update'中更新 
                double dAx,dAy;   //当前位置加速度 'Affect'中更新 
                double dAx_,dAy_; //预估位置加速度 'Affect'中更新 
                
      public:
             ExplosiveBox (double px, double py, double vx, double vy):
                     dMe(ExplBoxMass), dPx(px), dPy(py), dVx(vx), dVy(vy), dL(ExplBoxLength)
                     {}
             void Draw ();
             void Update ();           //Update the position and angle position (if it has)
             int Fill ( SpaceShip * ship );
             int Crash ( Star * star );
             int Crash ( Planet* planet );
             int Crash ( BiStar* bistar );
             int Crash ( BlackHole* blackhole );

             friend class Star;
             friend class BiStar;
             friend class BlackHole;
             friend class Planet;
             friend void Save ( char * filename );
};

class Station //全能的空间站，自动补满能量和火药，还有存档功能 
{
      protected:
                double dPx,dPy;    //Position of the Station
                double dR;         //Radius of the Station
      public:
             Station ( double px, double py, double r ): dPx(px), dPy(py), dR(r)
             {}
             void Draw ();
             void Refresh ( SpaceShip * sp );               //补满能量和火药
             void SaveRecord (int No);                      //存档 

             friend void Save ( char * filename );
};

//-----------------Decorating------------------
class Gas //a circle to show that jet is working, fades as time passes by.
{
      public:
             float fv;          //increasing rate of radius
             float fPx,fPy;     //Position
             float fVx,fVy;     //Velocity
             float fAg;         //Normal direction

             float fR;          //Radius
             float fA;          //Alpha
             int nowTime;       //Times, when T reaches 10, Destroy this object, 单位为DeltaTime 

             Gas ( double px, double py, double vx, double vy, double ag, double v, int t = 0 );
             void Draw ();
             void Update ();
};

class Explosion //a ball to show explosion, fades as time passes by.
{
      public:
             float fv;          //increasing rate of radius
             float fPx,fPy;     //Position
             
             float fR;          //Radius
             float fCR,fCG,fCB; //Color
             float fA;          //Alpha
             int nowTime;       //Times, when T reaches 20, Destroy this object, 单位为DeltaTime 
             
             Explosion ( double px, double py, double v, int t = 0 );
             void Draw ();
             void Update ();
};

//---------------Star Template-----------------
template <class Obj>
void Star::Affect ( Obj * obj )
{
     double dist = sqrt ( (dPx - obj->dPx)*(dPx - obj->dPx)+(dPy - obj->dPy)*(dPy - obj->dPy) ) * LengthUnit;
     obj->dAx += P_G * dM /(dist*dist*dist) * (dPx - obj->dPx) / 2;
     obj->dAy += P_G * dM /(dist*dist*dist) * (dPy - obj->dPy) / 2;

     dist = sqrt ( (dPx - obj->dPx_)*(dPx - obj->dPx_) + (dPy - obj->dPy_)*(dPy - obj->dPy_));
     obj->dAx_ += P_G * dM /(dist*dist*dist) * (dPx - obj->dPx_) / 2;
     obj->dAy_ += P_G * dM /(dist*dist*dist) * (dPy - obj->dPy_) / 2;
}

//-------------BiStar Template-----------------
template <class Obj>
void BiStar::Affect ( Obj * obj )
{
     double dist = sqrt ( (dP1x - obj->dPx)*(dP1x - obj->dPx)+(dP1y - obj->dPy)*(dP1y - obj->dPy) ) * LengthUnit;
     obj->dAx += P_G * dM1 /(dist*dist*dist) * (dP1x - obj->dPx) / 2;
     obj->dAy += P_G * dM1 /(dist*dist*dist) * (dP1y - obj->dPy) / 2;

     dist = sqrt ( (dP2x - obj->dPx)*(dP2x - obj->dPx)+(dP2y - obj->dPy)*(dP2y - obj->dPy) ) * LengthUnit;
     obj->dAx += P_G * dM2 /(dist*dist*dist) * (dP2x - obj->dPx) / 2;
     obj->dAy += P_G * dM2 /(dist*dist*dist) * (dP2y - obj->dPy) / 2;

     dist = sqrt ( (dP1x_ - obj->dPx_)*(dP1x_ - obj->dPx_) + (dP1y_ - obj->dPy_)*(dP1y_ - obj->dPy_));
     obj->dAx_ += P_G * dM1 /(dist*dist*dist) * (dP1x_ - obj->dPx_) / 2;
     obj->dAy_ += P_G * dM1 /(dist*dist*dist) * (dP1y_ - obj->dPy_) / 2;

     dist = sqrt ( (dP2x_ - obj->dPx_)*(dP2x_ - obj->dPx_) + (dP2y_ - obj->dPy_)*(dP2y_ - obj->dPy_));
     obj->dAx_ += P_G * dM2 /(dist*dist*dist) * (dP2x_ - obj->dPx_) / 2;
     obj->dAy_ += P_G * dM2 /(dist*dist*dist) * (dP2y_ - obj->dPy_) / 2;
}

//----------BlackHole Template-----------------
template <class Obj>
void BlackHole::Affect ( Obj * obj )
{
     double dist = sqrt ( (dPx - obj->dPx)*(dPx - obj->dPx)+(dPy - obj->dPy)*(dPy - obj->dPy) ) * LengthUnit;
     obj->dAx += P_G * dM /(dist*dist*dist) * (dPx - obj->dPx) / 2;
     obj->dAy += P_G * dM /(dist*dist*dist) * (dPy - obj->dPy) / 2;

     dist = sqrt ( (dPx - obj->dPx_)*(dPx - obj->dPx_) + (dPy - obj->dPy_)*(dPy - obj->dPy_));
     obj->dAx_ += P_G * dM /(dist*dist*dist) * (dPx - obj->dPx_) / 2;
     obj->dAy_ += P_G * dM /(dist*dist*dist) * (dPy - obj->dPy_) / 2;
}

//-------------Planet Template-----------------
template <class Obj>
void Planet::Affect ( Obj * obj )
{
     double dist = sqrt ( (dPx - obj->dPx)*(dPx - obj->dPx)+(dPy - obj->dPy)*(dPy - obj->dPy) ) * LengthUnit;
     obj->dAx += P_G * dM /(dist*dist*dist) * (dPx - obj->dPx) / 2;
     obj->dAy += P_G * dM /(dist*dist*dist) * (dPy - obj->dPy) / 2;

     dist = sqrt ( (dPx_ - obj->dPx_)*(dPx_ - obj->dPx_) + (dPy_ - obj->dPy_)*(dPy_ - obj->dPy_));
     obj->dAx_ += P_G * dM /(dist*dist*dist) * (dPx_ - obj->dPx_) / 2;
     obj->dAy_ += P_G * dM /(dist*dist*dist) * (dPy_ - obj->dPy_) / 2;
}

#define _SPACEOBJS_
#endif
