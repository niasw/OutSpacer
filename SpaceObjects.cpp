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
 *  Name: SpaceObjects.cpp
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震)
 *  Description: 小课题Object实现文件 
 */
#include "SpaceObjects.h"
#include "GameProcedure.h"

//-----------------------Constants--------------------------
const double DeltaTime = double (FreshTime) * TimeUnit; //虚拟世界里的时间微元 

//-----------------------Double Random----------------------
int srand_d::seed=time(0);

srand_d::srand_d()
{
        srand(seed);
        x=double(::rand())/2147483648.;
        x+=double(::rand())/4611686018427387904.;
        seed=::rand();
}

double srand_d::rand()
{
       double temp=x;
       srand(seed);
       x=double(::rand())/2147483648.;
       x+=double(::rand())/4611686018427387904.;
       seed=::rand();
       return temp;
}

double srand_d::normdev()
{
       double normdev;
       double v1, v2, fac, rsq;
       static int iset = 0;
       static double gset;
       if ( iset == 0 )
       {
            do
            {
                 v1 = 2. * rand () - 1.;
                 v2 = 2. * rand () - 1.;
                 rsq = v1 * v1 + v2 * v2;
            }
            while ( rsq >= 1. || rsq == 0. );
            fac = sqrt ( -2. * log (rsq) / rsq );
            gset = v1 * fac;
            iset = 1;
            return v2 * fac;
       }
       else
       {
           iset = 0;
           return gset;
       }
}

//-----------------------Class Star-------------------------
void Star::Draw()
{
     glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_Star    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_Star    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_Star   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission_Star   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_Star );

     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( StarColor );
     glutSolidSphere ( dR, 30, 30 );
     glPopMatrix ();

     glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_Obj    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_Obj    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_Obj   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission_Obj   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_Obj );
}

//-----------------------Class BiStar-------------------------
BiStar::BiStar (double m1, double m2, double pcx, double pcy,
                double r1, double r2, double ao, double ag,
                double  d, double  e, int direction, int period):
                dM1 (m1), dM2 (m2), dPCx(pcx), dPCy(pcy), dR1 (r1), dR2 (r2),
                dAo (ao), dD  ( d), dE  ( e), T (period),
                direc(direction), dAg (ag)
{
                nowTime = int ( double (T) / 2. / M_PI * (dAg - dE * sin (dAg * M_PI / 180.) ) );
                omega = 2 * M_PI / double (T);
                double tmp1 = dM1 + dM2;//mass
                dA1 = dD * dM2 / tmp1;
                dA2 = - dD * dM1 / tmp1;
                tmp1 = sqrt ( 1. - dE * dE );//根号(1-e^2) 
                dB1 = direc * dA1 * tmp1;
                dB2 = direc * dA2 * tmp1;
                
                double sin_Ag = sin (dAg * M_PI / 180.), cos_Ag = cos (dAg * M_PI / 180.),
                       sin_Ao = sin (dAo * M_PI / 180.), cos_Ao = cos (dAo * M_PI / 180.);
                dP1x = dPCx + ( cos_Ag + dE ) * dA1 * cos_Ao - dB1 * sin_Ag * sin_Ao;
                dP1y = dPCy + dA1 * cos_Ag * sin_Ao + dB1 * sin_Ag * cos_Ao;
                dP2x = dPCx + ( cos_Ag + dE ) * dA2 * cos_Ao - dB2 * sin_Ag * sin_Ao;
                dP2y = dPCy + dA2 * cos_Ag * sin_Ao + dB2 * sin_Ag * cos_Ao;

                dAg_ = omega / ( 1 - dE * cos (dAg * M_PI / 180.) ) + dAg;

                sin_Ag = sin (dAg_ * M_PI / 180.); cos_Ag = cos (dAg_ * M_PI / 180.);
                dP1x_ = dPCx + ( cos_Ag + dE ) * dA1 * cos_Ao - dB1 * sin_Ag * sin_Ao;
                dP1y_ = dPCy + dA1 * cos_Ag * sin_Ao + dB1 * sin_Ag * cos_Ao;
                dP2x_ = dPCx + ( cos_Ag + dE ) * dA2 * cos_Ao - dB2 * sin_Ag * sin_Ao;
                dP2y_ = dPCy + dA2 * cos_Ag * sin_Ao + dB2 * sin_Ag * cos_Ao;
}

void BiStar::Draw()
{
     glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_Star    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_Star    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_Star   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission_Star   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_Star );

     glPushMatrix ();
     glTranslated ( dP1x, 0, dP1y );
     glColor3d ( StarColor );
     glutSolidSphere ( dR1, 30, 30 );
     glPopMatrix ();
     glPushMatrix ();
     glTranslated ( dP2x, 0, dP2y );
     glColor3d ( StarColor );
     glutSolidSphere ( dR2, 30, 30 );
     glPopMatrix ();

     glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_Obj    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_Obj    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_Obj   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission_Obj   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_Obj );
}

void BiStar::Update()
{
     double sin_Ag = sin (dAg * M_PI / 180.), cos_Ag = cos (dAg * M_PI / 180.),
            sin_Ao = sin (dAo * M_PI / 180.), cos_Ao = cos (dAo * M_PI / 180.);

     if ( T == nowTime )
     {
        dAg = dAo;
        nowTime = 0;
     }
     else if ( T == nowTime * 2 )
     {
          dAg = dAo + 180.;
     }
     else dAg += ( omega / ( 1 - dE * cos (dAg * M_PI / 180.) ) + omega / ( 1 - dE * cos ( dAg_ * M_PI / 180. ) ) ) / 2 * 180 / M_PI;

     dP1x = dPCx + ( cos_Ag + dE ) * dA1 * cos_Ao - dB1 * sin_Ag * sin_Ao;
     dP1y = dPCy + dA1 * cos_Ag * sin_Ao + dB1 * sin_Ag * cos_Ao;
     dP2x = dPCx + ( cos_Ag + dE ) * dA2 * cos_Ao - dB2 * sin_Ag * sin_Ao;
     dP2y = dPCy + dA2 * cos_Ag * sin_Ao + dB2 * sin_Ag * cos_Ao;

     dAg_ = omega / ( 1 - dE * cos (dAg * M_PI / 180.) ) + dAg;

     sin_Ag = sin (dAg_ * M_PI / 180.); cos_Ag = cos (dAg_ * M_PI / 180.);
     dP1x_ = dPCx + ( cos_Ag + dE ) * dA1 * cos_Ao - dB1 * sin_Ag * sin_Ao;
     dP1y_ = dPCy + dA1 * cos_Ag * sin_Ao + dB1 * sin_Ag * cos_Ao;
     dP2x_ = dPCx + ( cos_Ag + dE ) * dA2 * cos_Ao - dB2 * sin_Ag * sin_Ao;
     dP2y_ = dPCy + dA2 * cos_Ag * sin_Ao + dB2 * sin_Ag * cos_Ao;
     
     ++ nowTime;
}

//-----------------------Class BlackHole-------------------------
void BlackHole::Draw()
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( BlackHoleColor );
     glutSolidSphere ( dR, 30, 30 );
     glPopMatrix ();
}

//-----------------------Class Planet-------------------------
Planet::Planet (double m, double px, double py, double vx, double vy, double r):
               dM(m), dPx(px), dPy(py), dVx(vx), dVy(vy), dR(r)
{
     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     
     dAx_ = dAx = dAy_ = dAy = 0;
}

void Planet::Draw()
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( PlanetColor );
     glutSolidSphere ( dR, 30, 30 );
     glPopMatrix ();
}

void Planet::Update()
{
     double dVx_ = dVx, dVy_ = dVy;
     dVx_ += dAx * FreshTime * TimeUnit;
     dVy_ += dAy * FreshTime * TimeUnit;

     dPx += ( dVx + dVx_ ) / 2 * FreshTime * TimeUnit;
     dPy += ( dVy + dVy_ ) / 2 * FreshTime * TimeUnit;

     dVx += ( dAx + dAx_ ) / 2 * FreshTime * TimeUnit;
     dVy += ( dAy + dAy_ ) / 2 * FreshTime * TimeUnit;

     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     
     dAx_ = dAx = dAy_ = dAy = 0;
     if ( sqrt ( dPx_ * dPx_ + dPy_ * dPy_ ) >= MapRadius - dR )
     {
          dVx = - ((dPx*dPx-dPy*dPy)*dVx + dPx*dPy*dVy*2) / (dPx*dPx+dPy*dPy);
          dVy =   ((dPx*dPx-dPy*dPy)*dVy - dPx*dPy*dVx*2) / (dPx*dPx+dPy*dPy);
          dPx_ = dVx * FreshTime * TimeUnit + dPx;
          dPy_ = dVy * FreshTime * TimeUnit + dPy;
     }
     
     for (Node<SpaceShip>::cur=Node<SpaceShip>::beg;Node<SpaceShip>::cur!=0;){   //NUMBER ONE
         double delpx=Node<SpaceShip>::cur->obj->dPx-dPx,delpy=Node<SpaceShip>::cur->obj->dPy-dPy,delvx=Node<SpaceShip>::cur->obj->dVx-dVx,delvy=Node<SpaceShip>::cur->obj->dVy-dVy;
         double csa=delpx/sqrt(delpx*delpx+delpy*delpy),sia=delpy/sqrt(delpx*delpx+delpy*delpy);
         double SM=Node<SpaceShip>::cur->obj->dMB+Node<SpaceShip>::cur->obj->dMb+Node<SpaceShip>::cur->obj->dMw+Node<SpaceShip>::cur->obj->dMf+Node<SpaceShip>::cur->obj->dMe;
         if (sqrt(delpx*delpx+delpy*delpy)<=Node<SpaceShip>::cur->obj->dR+dR){
         Node<SpaceShip>::cur->obj->dVx-=(delvx*csa+delvy*sia)*2000*dM*csa/
         (1000*dM+SM);
         Node<SpaceShip>::cur->obj->dVy-=(delvx*csa+delvy*sia)*2000*dM*sia/(1000*dM+SM);
         dVx+=(delvx*csa+delvy*sia)*2*SM*csa/(1000*dM+SM);
         dVy+=(delvx*csa+delvy*sia)*2*SM*sia/(1000*dM+SM);
         }
         Node<SpaceShip>::cur=Node<SpaceShip>::cur->next;}
         
     for (Node<Star>::cur=Node<Star>::beg;Node<Star>::cur!=0;){        //NUMBER TWO
         double delpx=Node<Star>::cur->obj->dPx-dPx,delpy=Node<Star>::cur->obj->dPy-dPy,delvx=-dVx,delvy=-dVy;
         double csa=delpx/sqrt(delpx*delpx+delpy*delpy),sia=delpy/sqrt(delpx*delpx+delpy*delpy);
         double SM=1000*Node<Star>::cur->obj->dM;
         if (sqrt(delpx*delpx+delpy*delpy)<=Node<Star>::cur->obj->dR+dR){
         dVx+=(delvx*csa+delvy*sia)*2*SM*csa/(dM+SM);
         dVy+=(delvx*csa+delvy*sia)*2*SM*sia/(dM+SM);
         }
         Node<Star>::cur=Node<Star>::cur->next;}
}

//-----------------------Class SpaceShip-------------------------
SpaceShip::SpaceShip (double px, double py, double vx, double vy, double mf, double me,
                      int hp, int jeti,
                      double ag, double wg, double agw, double agc):
                      dMB(ShipBmass),dMb(Shipbmass),dMw(Shipwmass),dJw(ShipJw),
                      dR(ShipRadius),dLb(ShipLb),dHb(ShipHb),dPx(px),dPy(py),dAg(ag),
                      dAgw(agw),dVx(vx),dVy(vy),dWg(wg),dAgc(agc),
                      dMf(mf),dMe(me),JetI(jeti),HP(hp)
{
         dJb = ( ( ShipRadius + ShipLb ) * ShipRadius + ShipLb * ShipLb / 3 ) * Shipbmass;
         dPx_ = dVx * FreshTime * TimeUnit + dPx;
         dPy_ = dVy * FreshTime * TimeUnit + dPy;
         dAg_ = dAga * FreshTime * TimeUnit + dAg;

         dAx_ = dAx = dAy_ = dAy = dAga = dAga_ = 0;
}

void SpaceShip::UseJet( int ID )
{
  if ( dMf > GasMass * JetI )
  {
     dMf -= GasMass * JetI;
     double delV = Sp_JetSpeed / ( 1 + ( dMB + dMb + dMw + dMf + dMe ) / GasMass * JetI );
     switch (control_mode)
     {
            case 0://mode1
            {
                 double Sin_Agw=sin(dAgw * M_PI / 180.),Cos_Agw=cos(dAgw * M_PI / 180.);
                 switch (ID)
                 {
                        case 0://Front
                             dVx -= delV * Sin_Agw * GasAmplifier;
                             dVy -= delV * Cos_Agw * GasAmplifier;
                             new Node<Gas>( new Gas( dPx + dR * Sin_Agw , dPy + dR * Cos_Agw ,
                                            dVx + ( Sp_JetSpeed - delV ) * Sin_Agw , dVy + ( Sp_JetSpeed - delV ) * Cos_Agw ,
                                            dAgw - 180 , JetI * Sp_GasV ) );
                             break;
                        case 1://Left
                             dVx -= delV * Cos_Agw * GasAmplifier;
                             dVy += delV * Sin_Agw * GasAmplifier;
                             new Node<Gas>( new Gas( dPx + dR * Cos_Agw , dPy - dR * Sin_Agw ,
                                            dVx + ( Sp_JetSpeed - delV ) * Cos_Agw , dVy - ( Sp_JetSpeed - delV ) * Sin_Agw ,
                                            dAgw - 90 , JetI * Sp_GasV ) );
                             break;
                        case 2://Back
                             dVx += delV * Sin_Agw * GasAmplifier;
                             dVy += delV * Cos_Agw * GasAmplifier;
                             new Node<Gas>( new Gas( dPx - dR * Sin_Agw , dPy - dR * Cos_Agw ,
                                                dVx - ( Sp_JetSpeed - delV ) * Sin_Agw , dVy - ( Sp_JetSpeed - delV ) * Cos_Agw ,
                                                dAgw , JetI * Sp_GasV ) );
                             break;
                        case 3://Right
                             dVx += delV * Cos_Agw * GasAmplifier;
                             dVy -= delV * Sin_Agw * GasAmplifier;
                             new Node<Gas>( new Gas( dPx - dR * Cos_Agw , dPy + dR * Sin_Agw ,
                                            dVx - ( Sp_JetSpeed - delV ) * Cos_Agw , dVy + ( Sp_JetSpeed - delV ) * Sin_Agw ,
                                            dAgw + 90 , JetI * Sp_GasV ) );
                             break;
                 }
                 break;
            }
            case 1://mode2
            {
                 double Sin_Agc=sin(dAgc * M_PI / 180.),Cos_Agc=cos(dAgc * M_PI / 180.);
                 switch (ID)
                 {
                        case 0://Front
                             dVx -= delV * Sin_Agc * GasAmplifier;
                             dVy -= delV * Cos_Agc * GasAmplifier;
                             new Node<Gas>( new Gas( dPx + dR * Sin_Agc , dPy + dR * Cos_Agc ,
                                            dVx + ( Sp_JetSpeed - delV ) * Sin_Agc , dVy + ( Sp_JetSpeed - delV ) * Cos_Agc ,
                                            dAgc - 180 , JetI * Sp_GasV ) );
                             break;
                        case 2://Back
                             dVx += delV * Sin_Agc * GasAmplifier;
                             dVy += delV * Cos_Agc * GasAmplifier;
                             new Node<Gas>( new Gas( dPx - dR * Sin_Agc , dPy - dR * Cos_Agc ,
                                            dVx - ( Sp_JetSpeed - delV ) * Sin_Agc , dVy - ( Sp_JetSpeed - delV ) * Cos_Agc ,
                                            dAgc , JetI * Sp_GasV ) );
                             break;
                 }
                 break;
            }
            case 2://mode3
                 //todo
                 break;
     }
  }
}

void SpaceShip::FireBullet()
{
   if ( dMe > BulletMass )
   {
     dMe -= BulletMass;
     double Sin_Agw=sin(dAgw * M_PI / 180.),Cos_Agw=cos(dAgw * M_PI / 180.);
     double delV = Sp_BulletSpeed / ( 1 + ( dMB + dMb + dMw + dMf + dMe ) / BulletMass );
     dVx += delV * Sin_Agw * BulletAmplifier;
     dVy += delV * Cos_Agw * BulletAmplifier;
     new Node<Bullet>( new Bullet( dPx - 1.01 * ( dR + BulletRadius ) * Sin_Agw , dPy - 1.01 * ( dR + BulletRadius ) * Cos_Agw ,
             dVx - ( Sp_BulletSpeed - delV ) * Sin_Agw , dVy - ( Sp_BulletSpeed - delV ) * Cos_Agw ) );
   }
}

void SpaceShip::EnhanceJet()
{
     if (JetI < Sp_JetImax) ++ JetI;
}

void SpaceShip::ReduceJet()
{
     if (JetI > Sp_JetImin) -- JetI;
}

void SpaceShip::SpinCtrl1()
{
     dAgc += 3;
}

void SpaceShip::SpinCtrl2()
{
     dAgc -= 3;
}

void SpaceShip::SpinWeapon1()
{
     dAgw += 2;
     dAg -= dJw / dJb * 2;
     dAg_ -= dJw / dJb * 2;
}

void SpaceShip::SpinWeapon2()
{
     dAgw -= 2;
     dAg += dJw / dJb * 2;
     dAg_ += dJw / dJb * 2;
}

void SpaceShip::Draw()
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( ShipColor );
     glutSolidSphere ( dR, 30, 30 );
     glRotated ( dAg, 0, 1, 0 );
     glColor3d ( ShipbColor );
     glBegin ( GL_QUADS );
     glVertex3d ( - dR , dHb , 0 );
     glVertex3d ( - dR - dLb , dHb , 0 );
     glVertex3d ( - dR - dLb , - dHb , 0 );
     glVertex3d ( - dR , - dHb , 0 );
     glEnd ();
     glBegin ( GL_QUADS );
     glVertex3d ( dR , dHb , 0 );
     glVertex3d ( dR + dLb , dHb , 0 );
     glVertex3d ( dR + dLb , - dHb , 0 );
     glVertex3d ( dR , - dHb , 0 );
     glEnd ();
     if ( control_mode == 1 )//mode2
     {
          glRotated ( 180 + dAgc - dAg, 0, 1, 0 );
          glColor4f ( 1, 0, 0, 0.2 );
          QuadTmp = gluNewQuadric ();
          gluCylinder ( QuadTmp, 0.03, 0.03, 3, 10, 10 );
          gluDeleteQuadric ( QuadTmp );
     }
     glPopMatrix ();
}

void SpaceShip::Update()
{
     double dVx_ = dVx, dVy_ = dVy, dWg_ = dWg;
     dVx_ += dAx * FreshTime * TimeUnit;
     dVy_ += dAy * FreshTime * TimeUnit;
     dWg_ += dAga * FreshTime * TimeUnit;

     dPx += ( dVx + dVx_ ) / 2 * FreshTime * TimeUnit;
     dPy += ( dVy + dVy_ ) / 2 * FreshTime * TimeUnit;
     dAg += ( dWg + dWg_ ) / 2 * FreshTime * TimeUnit;

     dVx += ( dAx + dAx_ ) / 2 * FreshTime * TimeUnit;
     dVy += ( dAy + dAy_ ) / 2 * FreshTime * TimeUnit;
     dWg += ( dAga + dAga_ ) / 2 * FreshTime * TimeUnit;

     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     dAg_ = dAga * FreshTime * TimeUnit + dAg;
     
     dAx_ = dAx = dAy_ = dAy = dAga = dAga_ = 0;
     if ( sqrt ( dPx_ * dPx_ + dPy_ * dPy_ ) >= MapRadius - dR )
     {
          double surplus = sqrt ( dPx * dPx + dPy * dPy ) - MapRadius + dR;
          dPx -= surplus * dPx / ( MapRadius - dR ) * 2;
          dPy -= surplus * dPy / ( MapRadius - dR ) * 2;

          dVx = - ((dPx*dPx-dPy*dPy)*dVx + dPx*dPy*dVy*2) / (dPx*dPx+dPy*dPy);
          dVy =   ((dPx*dPx-dPy*dPy)*dVy - dPx*dPy*dVx*2) / (dPx*dPx+dPy*dPy);

          dPx_ = dVx * FreshTime * TimeUnit + dPx;
          dPy_ = dVy * FreshTime * TimeUnit + dPy;
     }

     if ( dAg <= -90 ) dAg += 360;
     else if ( dAg > 270 ) dAg -= 360;
     if ( dAgw <= -90 ) dAgw += 360;
     else if ( dAgw > 270 ) dAgw -= 360;
     
     for (Node<Star>::cur=Node<Star>::beg;Node<Star>::cur!=0;){             //NUMBER THREE
         double delpx=Node<Star>::cur->obj->dPx-dPx,delpy=Node<Star>::cur->obj->dPy-dPy,delvx=-dVx,delvy=-dVy;
         double csa=delpx/sqrt(delpx*delpx+delpy*delpy),sia=delpy/sqrt(delpx*delpx+delpy*delpy);
         double SM=1000*Node<Star>::cur->obj->dM;
         if (sqrt(delpx*delpx+delpy*delpy)<=Node<Star>::cur->obj->dR+dR){
         dVx+=(delvx*csa+delvy*sia)*2*SM*csa/(dMB+dMb+dMw+dMf+dMe+SM);
         dVy+=(delvx*csa+delvy*sia)*2*SM*sia/(dMB+dMb+dMw+dMf+dMe+SM);
         }
         Node<Star>::cur=Node<Star>::cur->next;}
}

//-----------------------Class Fort-------------------------
Fort::Fort (double ao, double r, int fq, int attachcate, void *attach, int aimcate, void *aim, int nowtime):
            dAo(ao), dR(r), Fq(fq), AttachCate(attachcate), Attach(attach), AimCate(aimcate), Aim(aim), nowTime(nowtime)
{
     switch (AttachCate&0xf)
     {
//            case CATE_Star:
//                 break;
            case CATE_BiStar:
                 switch (AttachCate&0xf0)
                 {
                        case 0x20:
                             dAo += ( ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg * M_PI / 180.) ) +
                                      ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg_ * M_PI / 180. ) ) ) * 90 / M_PI;
                        case 0x00:
                             dPx = ((BiStar*)Attach)->dP1x - ((BiStar*)Attach)->dR1 * sin(dAo * M_PI / 180.);
                             dPy = ((BiStar*)Attach)->dP1y - ((BiStar*)Attach)->dR1 * cos(dAo * M_PI / 180.);
                             break;
                        case 0x30:
                             dAo += ( ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg * M_PI / 180.) ) +
                                     ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg_ * M_PI / 180. ) ) ) * 90 / M_PI;
                        case 0x10:
                             dPx = ((BiStar*)Attach)->dP2x - ((BiStar*)Attach)->dR2 * sin(dAo * M_PI / 180.);
                             dPy = ((BiStar*)Attach)->dP2y - ((BiStar*)Attach)->dR2 * cos(dAo * M_PI / 180.);
                             break;
                 }
                 break;
            case CATE_Planet:
                 dPx = ((Planet*)Attach)->dPx - ((Planet*)Attach)->dR * sin(dAo * M_PI / 180.);
                 dPy = ((Planet*)Attach)->dPy - ((Planet*)Attach)->dR * cos(dAo * M_PI / 180.);
                 break;
     }
     
     double px = dPx, py = dPy;
     switch (AimCate)
     {
            case CATE_SpaceShip:
                 px -= ((SpaceShip*)Aim)->dPx;
                 py -= ((SpaceShip*)Aim)->dPy;
                 break;
            case CATE_Fort:
                 px -= ((Fort*)Aim)->dPx;
                 py -= ((Fort*)Aim)->dPy;
                 break;
     }
     dAg = atan2 ( px , py ) * 180. / M_PI;
}

void Fort::Draw()//to improve
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( FortColor );
     glutSolidSphere ( dR, 30, 30 );
     glRotated ( dAg + 180, 0, 1, 0 );
     glColor4d ( FortwColor , 0.8 );
     gluCylinder ( QuadTmp, 0.03, 0.03, 2 * dR, 10, 10 );
     glPopMatrix ();
}

void Fort::Update()
{
     switch (AttachCate&0xf)
     {
//            case CATE_Star:
//                 break;
            case CATE_BiStar:
                 switch (AttachCate&0xf0)
                 {
                        case 0x20:
                             dAo += ( ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg * M_PI / 180.) ) +
                                      ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg_ * M_PI / 180. ) ) ) / 2;
                        case 0x00:
                             dPx = ((BiStar*)Attach)->dP1x - ((BiStar*)Attach)->dR1 * sin(dAo * M_PI / 180.);
                             dPy = ((BiStar*)Attach)->dP1y - ((BiStar*)Attach)->dR1 * cos(dAo * M_PI / 180.);
                             dPx_ = ((BiStar*)Attach)->dP1x_ - ((BiStar*)Attach)->dR1 * sin(dAo * M_PI / 180.);
                             dPy_ = ((BiStar*)Attach)->dP1y_ - ((BiStar*)Attach)->dR1 * cos(dAo * M_PI / 180.);
                             break;
                        case 0x30:
                             dAo += ( ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg * M_PI / 180.) ) +
                                      ((BiStar*)Attach)->omega / ( 1 - ((BiStar*)Attach)->dE * cos ( ((BiStar*)Attach)->dAg_ * M_PI / 180. ) ) ) / 2;
                        case 0x10:
                             dPx = ((BiStar*)Attach)->dP2x - ((BiStar*)Attach)->dR2 * sin(dAo * M_PI / 180.);
                             dPy = ((BiStar*)Attach)->dP2y - ((BiStar*)Attach)->dR2 * cos(dAo * M_PI / 180.);
                             dPx_ = ((BiStar*)Attach)->dP2x_ - ((BiStar*)Attach)->dR2 * sin(dAo * M_PI / 180.);
                             dPy_ = ((BiStar*)Attach)->dP2y_ - ((BiStar*)Attach)->dR2 * cos(dAo * M_PI / 180.);
                             break;
                 }
                 break;
            case CATE_Planet:
                 dPx = ((Planet*)Attach)->dPx - ((Planet*)Attach)->dR * sin(dAo * M_PI / 180.);
                 dPy = ((Planet*)Attach)->dPy - ((Planet*)Attach)->dR * cos(dAo * M_PI / 180.);
                 dPx_ = ((Planet*)Attach)->dPx_ - ((Planet*)Attach)->dR * sin(dAo * M_PI / 180.);
                 dPy_ = ((Planet*)Attach)->dPy_ - ((Planet*)Attach)->dR * cos(dAo * M_PI / 180.);
                 break;
     }
     
     double px = dPx, py = dPy;
     switch (AimCate)
     {
            case CATE_SpaceShip:
                 px -= ((SpaceShip*)Aim)->dPx;
                 py -= ((SpaceShip*)Aim)->dPy;
                 break;
            case CATE_Fort:
                 px -= ((Fort*)Aim)->dPx;
                 py -= ((Fort*)Aim)->dPy;
                 break;
     }
     dAg = atan2 ( px , py ) * 180. / M_PI;
     ++ nowTime;
     if( nowTime > int ( 1 / TimeUnit / FreshTime / Fq ) )
     {
         Fire();
         nowTime = 0;
     }
}

void Fort::Fire()
{
     static srand_d srand;
     SpaceShip *ship = (SpaceShip*) Aim;
     double FireAg = dAg + srand.normdev() * Ft_FireAngleVar;
     double tmp = rand() / 2147483648.;
     if ( tmp < 0.5 )
        new Node<Bullet>( new Bullet( dPx - 1.01 * ( dR + BulletRadius ) * sin( FireAg * M_PI / 180 ), dPy - 1.01 * ( dR + BulletRadius ) * cos( FireAg * M_PI / 180 ), - Sp_BulletSpeed * sin( FireAg * M_PI / 180 ), - Sp_BulletSpeed * cos( FireAg * M_PI / 180 ) ) );
     else
         new Node<Missile>( new Missile ( dPx - 1.01 * ( dR + MissRadius ) * sin( FireAg * M_PI / 180 ), dPy - 1.01 * ( dR + MissRadius ) * cos( FireAg * M_PI / 180 ), - Sp_MissInitSpeed * sin( FireAg * M_PI / 180 ), 
                                          - Sp_MissInitSpeed * cos( FireAg * M_PI / 180 ), AimCate, Aim ) );
}

//-----------------------Class Missile-------------------------
Missile::Missile (double px, double py, double vx, double vy, int aimcate, void *aim):
                  dMB(MissMass), dMf(MissFuel), dPx(px), dPy(py), dVx(vx), dVy(vy), AimCate(aimcate), Aim(aim), dR(MissRadius)
{
          switch (AimCate)
          {
                 case CATE_SpaceShip:
                      px -= ((SpaceShip*)Aim)->dPx;
                      py -= ((SpaceShip*)Aim)->dPy;
                      break;
                 case CATE_Fort:
                      px -= ((Fort*)Aim)->dPx;
                      py -= ((Fort*)Aim)->dPy;
                      break;
          }
          dAg = atan2 ( px , py ) * 180. / M_PI;
          if ( px < 0 ) dAg += 180;
          Sin_Ag = px / sqrt ( px * px + py * py );
          Cos_Ag = py / sqrt ( px * px + py * py );
          dPx_ = dVx * FreshTime * TimeUnit + dPx;
          dPy_ = dVy * FreshTime * TimeUnit + dPy;
          dAx_ = dAx = dAy_ = dAy = 0;
}

void Missile::Draw()
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( MissColor );
     glutSolidSphere ( dR, 30, 30 );
     glPopMatrix ();
}

void Missile::Update()
{
     if ( dMf > GasMass )
     {
        dMf -= GasMass;
        double delV = MissJetSpeed / ( 1 + ( dMB + dMf ) / GasMass );
        dVx -= delV * Sin_Ag;
        dVy -= delV * Cos_Ag;

        new Node<Gas>( new Gas( dPx + 1.1 * dR * Sin_Ag , dPy + 1.1 * dR * Cos_Ag ,
                           dVx + ( MissJetSpeed - delV ) * Sin_Ag , dVy + ( MissJetSpeed - delV ) * Cos_Ag ,
                           dAg - 180 , MissGasV ) );

        double px = dPx, py = dPy;
        switch (AimCate)
        {
               case CATE_SpaceShip:
                    px -= ((SpaceShip*)Aim)->dPx;
                    py -= ((SpaceShip*)Aim)->dPy;
                    break;
               case CATE_Fort:
                    px -= ((Fort*)Aim)->dPx;
                    py -= ((Fort*)Aim)->dPy;
                    break;
        }
        dAg = atan2 ( px , py ) * 180. / M_PI;
        if ( px < 0 ) dAg += 180;
        Sin_Ag = px / sqrt ( px * px + py * py );
        Cos_Ag = py / sqrt ( px * px + py * py );
     }
     double dVx_ = dVx, dVy_ = dVy;
     dVx_ += dAx * FreshTime * TimeUnit;
     dVy_ += dAy * FreshTime * TimeUnit;

     dPx += ( dVx + dVx_ ) / 2 * FreshTime * TimeUnit;
     dPy += ( dVy + dVy_ ) / 2 * FreshTime * TimeUnit;

     dVx += ( dAx + dAx_ ) / 2 * FreshTime * TimeUnit;
     dVy += ( dAy + dAy_ ) / 2 * FreshTime * TimeUnit;

     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     
     dAx_ = dAx = dAy_ = dAy = 0;
     if ( sqrt ( dPx_ * dPx_ + dPy_ * dPy_ ) >= MapRadius - dR )
     {
          double surplus = sqrt ( dPx * dPx + dPy * dPy ) - MapRadius + dR;
          dPx -= surplus * dPx / ( MapRadius - dR ) * 2;
          dPy -= surplus * dPy / ( MapRadius - dR ) * 2;

          dVx = - ((dPx*dPx-dPy*dPy)*dVx + dPx*dPy*dVy*2) / (dPx*dPx+dPy*dPy);
          dVy =   ((dPx*dPx-dPy*dPy)*dVy - dPx*dPy*dVx*2) / (dPx*dPx+dPy*dPy);

          dPx_ = dVx * FreshTime * TimeUnit + dPx;
          dPy_ = dVy * FreshTime * TimeUnit + dPy;
     }
}

int Missile::Damage ( SpaceShip * obj )
{
    if ( sqrt( (dPx_ - obj->dPx) * (dPx_ - obj->dPx) + (dPy_ - obj->dPy) * (dPy_ - obj->dPy) ) 
              <= dR + obj->dR )
    {
              obj->HP -= int( (dMB + dMf) );
              new Node<Explosion>( new Explosion( dPx_, dPy_, MissExpV ) );
              return 1;
    }
    return 0;
}

int Missile::Damage ( Fort * obj )
{
    if ( sqrt( (dPx_ - obj->dPx) * (dPx_ - obj->dPx) + (dPy_ - obj->dPy) * (dPy_ - obj->dPy) ) 
              <= dR + obj->dR )
    {
              obj->Fq -= int( dMB + dMf );
              new Node<Explosion>( new Explosion( dPx_, dPy_, MissExpV ) );
              return 1;
    }
    return 0;
}

int Missile::Damage ( Node<Missile> * objnode ) //Damage( Node<Missile> * missnode ) 需要在返回后做删除工作 
{
    if ( sqrt( (dPx_ - objnode->obj->dPx_) * (dPx_ - objnode->obj->dPx_) +
               (dPy_ - objnode->obj->dPy_) * (dPy_ - objnode->obj->dPy_) )
               <= dR + objnode->obj->dR )
    {
         new Node<Explosion>( new Explosion( dPx_, dPy_, MissExpV ) );
         new Node<Explosion>( new Explosion( objnode->obj->dPx_, objnode->obj->dPy_, MissExpV ) );
         return 1;
    }
    else return 0;
}

int Missile::Crash( Star* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dR )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, MissExpV ) );
           return 1;
      }
      else return 0;
}

int Missile::Crash( BlackHole* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dR )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, MissExpV ) );
           return 1;
      }
      else return 0;
}
int Missile::Crash ( Planet * obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dR )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, MissExpV ) );
           return 1;
      }
      else return 0;
}

int Missile::Crash( BiStar* obj )
{
    if ( sqrt( (dPx_ - obj->dP1x_) * (dPx_ - obj->dP1x_) +
               (dPy_ - obj->dP1y_) * (dPy_ - obj->dP1y_) ) <= dR + obj->dR1 )
    {
         new Node<Explosion>( new Explosion( dPx_, dPy_, MissExpV ) );
         return 1;
    }
    if ( sqrt( (dPx_ - obj->dP2x_) * (dPx_ - obj->dP2x_) +
               (dPy_ - obj->dP2y_) * (dPy_ - obj->dP2y_) ) <= dR + obj->dR2 )
    {
         new Node<Explosion>( new Explosion( dPx, dPy, MissExpV ) );
         return 1;
    }
    return 0;
}

//-----------------------Class Bullet-------------------------
Bullet::Bullet (double px, double py, double vx, double vy):
               dMB(BulletMass), dPx(px), dPy(py), dVx(vx), dVy(vy), dR(BulletRadius)
{
     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     
     dAx_ = dAx = dAy_ = dAy = 0;
}


void Bullet::Draw ()
{
     glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_Star    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_Star    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_Star   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission_Star   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_Star );

     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( BulletColor );
     glutSolidSphere ( dR, 30, 30 );
     glPopMatrix ();

     glMaterialfv ( GL_FRONT_AND_BACK, GL_AMBIENT,   mat_ambient_Obj    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_DIFFUSE,   mat_diffuse_Obj    );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SPECULAR,  mat_specular_Obj   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_EMISSION,  mat_emission_Obj   );
     glMaterialfv ( GL_FRONT_AND_BACK, GL_SHININESS, high_shininess_Obj );
}

void Bullet::Update()
{
     double dVx_ = dVx, dVy_ = dVy;
     dVx_ += dAx * FreshTime * TimeUnit;
     dVy_ += dAy * FreshTime * TimeUnit;

     dPx += ( dVx + dVx_ ) / 2 * FreshTime * TimeUnit;
     dPy += ( dVy + dVy_ ) / 2 * FreshTime * TimeUnit;

     dVx += ( dAx + dAx_ ) / 2 * FreshTime * TimeUnit;
     dVy += ( dAy + dAy_ ) / 2 * FreshTime * TimeUnit;

     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     
     dAx_ = dAx = dAy_ = dAy = 0;
}

int Bullet::Damage ( SpaceShip * obj )
{
    if ( sqrt( (dPx_ - obj->dPx) * (dPx_ - obj->dPx) + (dPy_ - obj->dPy) * (dPy_ - obj->dPy) ) 
              < dR + obj->dR )
    {
              obj->HP -= int( dMB );
              new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
              return 1;
    }
    return 0;
}

int Bullet::Damage ( Fort * obj )
{
    if ( sqrt( (dPx_ - obj->dPx) * (dPx_ - obj->dPx) + (dPy_ - obj->dPy) * (dPy_ - obj->dPy) ) 
              <= dR + obj->dR )
    {
              obj->Fq -= int( dMB );
              new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
              return 1;
    }
    return 0;
}

int Bullet::Damage ( Node<Missile> * objnode )
{
    if ( sqrt( (dPx_ - objnode->obj->dPx_) * (dPx_ - objnode->obj->dPx_) +
               (dPy_ - objnode->obj->dPy_) * (dPy_ - objnode->obj->dPy_) )
               <= dR + objnode->obj->dR )
    {
         new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
         new Node<Explosion>( new Explosion( objnode->obj->dPx_, objnode->obj->dPy_, MissExpV ) );
         delete objnode;
         return 1;
    }
    else return 0;
}

int Bullet::Crash( Star* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dR )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
           return 1;
      }
      else return 0;
}

int Bullet::Crash( BlackHole* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dR )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
           return 1;
      }
      else return 0;
}

int Bullet::Crash ( Planet * obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dR )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
           return 1;
      }
      else return 0;
}

int Bullet::Crash( BiStar* obj )
{
    if ( sqrt( (dPx_ - obj->dP1x_) * (dPx_ - obj->dP1x_) +
               (dPy_ - obj->dP1y_) * (dPy_ - obj->dP1y_) ) <= dR + obj->dR1 )
    {
         new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
         return 1;
    }
    if ( sqrt( (dPx_ - obj->dP2x_) * (dPx_ - obj->dP2x_) +
               (dPy_ - obj->dP2y_) * (dPy_ - obj->dP2y_) ) <= dR + obj->dR2 )
    {
         new Node<Explosion>( new Explosion( dPx, dPy, BulletExpV ) );
         return 1;
    }
    return 0;
}

int Bullet::Crash()
{
      if ( sqrt ( dPx_ * dPx_ + dPy_ * dPy_ ) > MapRadius - dR )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, BulletExpV ) );
           return 1;
      }
      else return 0;
}

//-----------------------Class FuelBox-------------------------
void FuelBox::Draw()
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( FuelBoxColor );
     glutSolidCube ( dL );
     glPopMatrix ();
}

void FuelBox::Update()
{
     double dVx_ = dVx, dVy_ = dVy;
     dVx_ += dAx * FreshTime * TimeUnit;
     dVy_ += dAy * FreshTime * TimeUnit;

     dPx += ( dVx + dVx_ ) / 2 * FreshTime * TimeUnit;
     dPy += ( dVy + dVy_ ) / 2 * FreshTime * TimeUnit;

     dVx += ( dAx + dAx_ ) / 2 * FreshTime * TimeUnit;
     dVy += ( dAy + dAy_ ) / 2 * FreshTime * TimeUnit;

     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     
     dAx_ = dAx = dAy_ = dAy = 0;
     if ( sqrt ( dPx_ * dPx_ + dPy_ * dPy_ ) >= MapRadius - dL / 2 )
     {
          double surplus = sqrt ( dPx * dPx + dPy * dPy ) - MapRadius + dL / 2;
          dPx -= surplus * dPx / ( MapRadius - dL / 2 ) * 2;
          dPy -= surplus * dPy / ( MapRadius - dL / 2 ) * 2;

          dVx = - ((dPx*dPx-dPy*dPy)*dVx + dPx*dPy*dVy*2) / (dPx*dPx+dPy*dPy);
          dVy =   ((dPx*dPx-dPy*dPy)*dVy - dPx*dPy*dVx*2) / (dPx*dPx+dPy*dPy);

          dPx_ = dVx * FreshTime * TimeUnit + dPx;
          dPy_ = dVy * FreshTime * TimeUnit + dPy;
     }
}

int FuelBox::Fill ( SpaceShip * obj )
{
    if ( sqrt( (dPx_ - obj->dPx_) * (dPx_ - obj->dPx_) + (dPy_ - obj->dPy_) * (dPy_ - obj->dPy_) ) 
              <= dL / 2 + obj->dR )
    {
              obj->dMf += int( dMf );
              return 1;
    }
    return 0;
}

int FuelBox::Crash( Star* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dL / 2 )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, FuelBoxExpV ) );
           return 1;
      }
      else return 0;
}

int FuelBox::Crash( BlackHole* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dL / 2 )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, FuelBoxExpV ) );
           return 1;
      }
      else return 0;
}
int FuelBox::Crash ( Planet * obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dL / 2 )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, FuelBoxExpV ) );
           return 1;
      }
      else return 0;
}

int FuelBox::Crash( BiStar* obj )
{
    if ( sqrt( (dPx_ - obj->dP1x_) * (dPx_ - obj->dP1x_) +
               (dPy_ - obj->dP1y_) * (dPy_ - obj->dP1y_) ) <= dL / 2 + obj->dR1 )
    {
         new Node<Explosion>( new Explosion( dPx_, dPy_, FuelBoxExpV ) );
         return 1;
    }
    if ( sqrt( (dPx_ - obj->dP2x_) * (dPx_ - obj->dP2x_) +
               (dPy_ - obj->dP2y_) * (dPy_ - obj->dP2y_) ) <= dL / 2 + obj->dR2 )
    {
         new Node<Explosion>( new Explosion( dPx, dPy, FuelBoxExpV ) );
         return 1;
    }
    return 0;
}

//-----------------------Class ExplosiveBox-------------------------
void ExplosiveBox::Draw()
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( ExplBoxColor );
     glutSolidCube ( dL );
     glPopMatrix ();
}

void ExplosiveBox::Update()
{
     double dVx_ = dVx, dVy_ = dVy;
     dVx_ += dAx * FreshTime * TimeUnit;
     dVy_ += dAy * FreshTime * TimeUnit;

     dPx += ( dVx + dVx_ ) / 2 * FreshTime * TimeUnit;
     dPy += ( dVy + dVy_ ) / 2 * FreshTime * TimeUnit;

     dVx += ( dAx + dAx_ ) / 2 * FreshTime * TimeUnit;
     dVy += ( dAy + dAy_ ) / 2 * FreshTime * TimeUnit;

     dPx_ = dVx * FreshTime * TimeUnit + dPx;
     dPy_ = dVy * FreshTime * TimeUnit + dPy;
     
     dAx_ = dAx = dAy_ = dAy = 0;
     if ( sqrt ( dPx_ * dPx_ + dPy_ * dPy_ ) >= MapRadius - dL / 2 )
     {
          double surplus = sqrt ( dPx * dPx + dPy * dPy ) - MapRadius + dL / 2;
          dPx -= surplus * dPx / ( MapRadius - dL / 2 ) * 2;
          dPy -= surplus * dPy / ( MapRadius - dL / 2 ) * 2;

          dVx = - ((dPx*dPx-dPy*dPy)*dVx + dPx*dPy*dVy*2) / (dPx*dPx+dPy*dPy);
          dVy =   ((dPx*dPx-dPy*dPy)*dVy - dPx*dPy*dVx*2) / (dPx*dPx+dPy*dPy);

          dPx_ = dVx * FreshTime * TimeUnit + dPx;
          dPy_ = dVy * FreshTime * TimeUnit + dPy;
     }
}

int ExplosiveBox::Fill ( SpaceShip * obj )
{
    if ( sqrt( (dPx_ - obj->dPx_) * (dPx_ - obj->dPx_) + (dPy_ - obj->dPy_) * (dPy_ - obj->dPy_) ) 
              <= dL / 2 + obj->dR )
    {
              obj->dMe += int( dMe );
              return 1;
    }
    return 0;
}

int ExplosiveBox::Crash( Star* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dL / 2 )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, ExplBoxExpV ) );
           return 1;
      }
      else return 0;
}

int ExplosiveBox::Crash( BlackHole* obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dL / 2 )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, ExplBoxExpV ) );
           return 1;
      }
      else return 0;
}
int ExplosiveBox::Crash ( Planet * obj )
{
      if ( sqrt ( (obj->dPx - dPx_) * (obj->dPx - dPx_) +
                  (obj->dPy - dPy_) * (obj->dPy - dPy_) ) <= obj->dR + dL / 2 )
      {
           new Node<Explosion>( new Explosion( dPx_, dPy_, ExplBoxExpV ) );
           return 1;
      }
      else return 0;
}

int ExplosiveBox::Crash( BiStar* obj )
{
    if ( sqrt( (dPx_ - obj->dP1x_) * (dPx_ - obj->dP1x_) +
               (dPy_ - obj->dP1y_) * (dPy_ - obj->dP1y_) ) <= dL / 2 + obj->dR1 )
    {
         new Node<Explosion>( new Explosion( dPx_, dPy_, ExplBoxExpV ) );
         return 1;
    }
    if ( sqrt( (dPx_ - obj->dP2x_) * (dPx_ - obj->dP2x_) +
               (dPy_ - obj->dP2y_) * (dPy_ - obj->dP2y_) ) <= dL / 2 + obj->dR2 )
    {
         new Node<Explosion>( new Explosion( dPx, dPy, ExplBoxExpV ) );
         return 1;
    }
    return 0;
}

//-----------------------Class Station-------------------------
void Station::Draw ()
{
     glPushMatrix ();
     glTranslated ( dPx, 0, dPy );
     glColor3d ( StationColor );
     glutSolidSphere ( dR, 30, 30 );
     glPopMatrix ();
}

void Station::Refresh ( SpaceShip * ship )
{
     if ( (dPx - ship->dPx)*(dPx - ship->dPx) + (dPy - ship->dPy)*(dPy - ship->dPy) < (dR + ship->dR)*(dR + ship->dR) )
     {
          ship->HP = Sp_HPmax;
          ship->dMf = Sp_FuelMax;
          ship->dMe = Sp_ExplMax;
     }
}

void Station::SaveRecord (int NO)
{
     switch ( gamestate )
     {
            case CASE_GAMEtime:
                 Save ( SAV_SAVING1 );
                 break;
            case CASE_GAMEfight:
                 Save ( SAV_SAVING2 );
                 break;
            case CASE_GAMEfree:
                 Save ( SAV_SAVING3 );
                 break;
     }
}

//-----------------------Class Gas-------------------------
Gas::Gas ( double px, double py, double vx, double vy, double ag, double v, int t ):
         fPx(px), fPy(py), fVx(vx), fVy(vy), fR(0.01), fv(v), fA(1), fAg(ag), nowTime(t) {}

void Gas::Draw ()
{
     glPushMatrix ();
     glTranslatef ( fPx, 0, fPy );
     glRotatef ( fAg, 0, 1, 0 );
     glColor4f ( GasColor, fA );
     glDepthMask ( GL_FALSE );
     glBlendFunc ( GL_ZERO, GL_SRC_COLOR );
     glutSolidSphere ( fR, 30, 30 );
     glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
     glutSolidSphere ( fR, 30, 30 );
     glDepthMask ( GL_TRUE );
     glPopMatrix ();
}

void Gas::Update()
{
     nowTime ++;
     fA -= 0.1;                     //Alpha decreases to 0
     fR += fv;                       //the radius of the fire ball increases
     fPx += fVx * FreshTime * TimeUnit; 
     fPy += fVy * FreshTime * TimeUnit;
}

//-----------------------Class Explosion-------------------------
Explosion::Explosion ( double px, double py, double v, int t ):
                     fPx(px), fPy(py), fR(0.01), fv(v), fA(1), fCR(1), fCG(1), fCB(0), nowTime(t) {}

void Explosion::Draw ()
{
     glPushMatrix ();
     glTranslatef ( fPx, 0, fPy );
     glColor4f ( fCR, fCG, fCB, fA );
     glDepthMask ( GL_FALSE );
     glBlendFunc ( GL_ZERO, GL_SRC_COLOR );
     glutSolidSphere ( fR, 30, 30 );
     glBlendFunc ( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
     glutSolidSphere ( fR, 30, 30 );
     glDepthMask ( GL_TRUE );
     glPopMatrix ();
}

void Explosion::Update()
{
     nowTime ++;
     switch (nowTime/30)          //colour changing from (1,1,0) to (1,0,0) to (0.5,0.25,2,25)
     {
            case 0: fCG -= 1.0/30;
                    break;
            case 1: fCR -= 0.5/30;
                    fCG += 0.25/30;
                    fCB += 0.25/30;
                    break;
     }
     fA -= 0.05;                  //Alpha decreases to 0
     fR += fv;                    //the radius of the fire ball increases
}
