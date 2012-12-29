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
 *  Name: SpecialAffect.h
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震)
 *  Description: 小课题Outspacer主程序中Affect特化函数定义 
 */
//---------------Affect Template-------------
template <>
void Star::Affect ( SpaceShip * obj )
{
     double forcex, forcey, forcex_, forcey_;
     double dist = sqrt ( (dPx - obj->dPx)*(dPx - obj->dPx)+(dPy - obj->dPy)*(dPy - obj->dPy) ) * LengthUnit;

     //calculate the gravity on the Ball
     double mass = obj->dMB + obj->dMf + obj->dMw +obj->dMe;//此时mass不包括broad 
     forcex = P_G * dM * mass/(dist*dist*dist) * (dPx - obj->dPx) ;
     forcey = P_G * dM * mass/(dist*dist*dist) * (dPy - obj->dPy) ;

     dist = sqrt ( (dPx - obj->dPx_)*(dPx - obj->dPx_)+(dPy - obj->dPy_)*(dPy - obj->dPy_) ) * LengthUnit;
     forcex_ = P_G * dM * mass/(dist*dist*dist) * (dPx - obj->dPx_) / 2;
     forcey_ = P_G * dM * mass/(dist*dist*dist) * (dPy - obj->dPy_) / 2;
     
     mass += obj->dMb;//此时mass成为总质量 

     //calculate the gravity on the board
     double forceXb, forceYb;
     double areadens = obj->dMb / ( obj->dLb * obj->dHb * 4 ); //Area Density of the board

     double Sin_Ag = sin ( obj->dAg * M_PI / 180. ), Cos_Ag = cos ( obj->dAg * M_PI / 180. );
     double h = std::abs( (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag );
     double X = (dPx - obj->dPx) * Cos_Ag + (dPy - obj->dPy) * Sin_Ag;
     double Y = (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag ;
     double H2_Plus_A2 = obj->dHb * obj->dHb + h * h;
     
     forceXb = log ( (sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) - obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) + obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) + obj->dHb ) )
               * P_G * dM * areadens;
     forceYb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb+X ) * ( obj->dR+obj->dLb+X ) ) )-
                 atan2( obj->dHb * ( obj->dR+X )          , h*sqrt( H2_Plus_A2 + ( obj->dR+X ) * ( obj->dR+X ) ) )+
                 atan2( obj->dHb * ( obj->dR+obj->dLb-X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb-X ) * ( obj->dR+obj->dLb-X)))-
                 atan2( obj->dHb * ( obj->dR-X )          , h*sqrt( H2_Plus_A2 + ( obj->dR-X ) * ( obj->dR-X ) ) ) )
               * 2 * P_G * dM * areadens;
     forcex += forceXb * Cos_Ag + forceYb * Sin_Ag;
     forcey += forceYb * Cos_Ag - forceXb * Sin_Ag;
     
     obj->dAga += ( forceXb * ( (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag ) -
                    forceYb * ( (dPx - obj->dPx) * Cos_Ag - (dPy - obj->dPy) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx  += forcex / mass;
     obj->dAy  += forcey / mass;

     //校正
     
     //改变需用到的变量 
     Sin_Ag = sin ( obj->dAg_ * M_PI / 180. ); Cos_Ag = cos ( obj->dAg_ * M_PI / 180. );
     h = std::abs( (dPy - obj->dPy_) * Cos_Ag - (dPx - obj->dPx_) * Sin_Ag );
     X = (dPx - obj->dPx_) * Cos_Ag + (dPy - obj->dPy_) * Sin_Ag;
     Y = (dPy - obj->dPy_) * Cos_Ag - (dPx - obj->dPx_) * Sin_Ag;
     H2_Plus_A2 = obj->dHb * obj->dHb + h * h;

     forceXb = log ( (sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) - obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) + obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) + obj->dHb ) )
               * P_G * dM * areadens;
     forceYb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb+X ) * ( obj->dR+obj->dLb+X ) ) )-
                 atan2( obj->dHb * ( obj->dR+X )          , h*sqrt( H2_Plus_A2 + ( obj->dR+X ) * ( obj->dR+X ) ) )+
                 atan2( obj->dHb * ( obj->dR+obj->dLb-X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb-X ) * ( obj->dR+obj->dLb-X)))-
                 atan2( obj->dHb * ( obj->dR-X )          , h*sqrt( H2_Plus_A2 + ( obj->dR-X ) * ( obj->dR-X ) ) ) )
               * 2 * P_G * dM * areadens;
     forcex_ += forceXb * Cos_Ag + forceYb * Sin_Ag;
     forcey_ += forceYb * Cos_Ag - forceXb * Sin_Ag;
     
     obj->dAga_ += ( forceXb * ( (dPy - obj->dPy_) * Cos_Ag - (dPx - obj->dPx_) * Sin_Ag ) -
                     forceYb * ( (dPx - obj->dPx_) * Cos_Ag - (dPy - obj->dPy_) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx_ += forcex_ / mass;
     obj->dAy_ += forcey_ / mass; 
}

template <>
void BiStar::Affect ( SpaceShip * obj )
{
     double force1x, force1y, force1x_, force1y_,force2x, force2y, force2x_, force2y_;
     double dist1 = sqrt ( (dP1x - obj->dPx)*(dP1x - obj->dPx)+(dP1y - obj->dPy)*(dP1y - obj->dPy) ) * LengthUnit,
            dist2 = sqrt ( (dP2x - obj->dPx)*(dP2x - obj->dPx)+(dP2y - obj->dPy)*(dP2y - obj->dPy) ) * LengthUnit;

     //calculate the gravity on the Ball
     double mass = obj->dMB + obj->dMf + obj->dMw +obj->dMe;//此时mass不包括broad 
     force1x = P_G * dM1 * mass/(dist1*dist1*dist1) * (dP1x - obj->dPx) ;
     force1y = P_G * dM1 * mass/(dist1*dist1*dist1) * (dP1y - obj->dPy) ;
     force2x = P_G * dM2 * mass/(dist2*dist2*dist2) * (dP2x - obj->dPx) ;
     force2y = P_G * dM2 * mass/(dist2*dist2*dist2) * (dP2y - obj->dPy) ;

     dist1 = sqrt ( (dP1x_ - obj->dPx_)*(dP1x_ - obj->dPx_)+(dP1y_ - obj->dPy_)*(dP1y_ - obj->dPy_) ) * LengthUnit;
     dist2 = sqrt ( (dP2x_ - obj->dPx_)*(dP2x_ - obj->dPx_)+(dP2y_ - obj->dPy_)*(dP2y_ - obj->dPy_) ) * LengthUnit;
     force1x_ = P_G * dM1 * mass/(dist1*dist1*dist1) * (dP1x_ - obj->dPx_) / 2;
     force1y_ = P_G * dM1 * mass/(dist1*dist1*dist1) * (dP1y_ - obj->dPy_) / 2;
     force2x_ = P_G * dM1 * mass/(dist2*dist2*dist2) * (dP2x_ - obj->dPx_) / 2;
     force2y_ = P_G * dM1 * mass/(dist2*dist2*dist2) * (dP2y_ - obj->dPy_) / 2;
     
     mass += obj->dMb;//此时mass成为总质量 

     //calculate the gravity on the board
     double force1Xb, force1Yb,force2Xb, force2Yb;
     double areadens = obj->dMb / ( obj->dLb * obj->dHb * 4 ); //Area Density of the board

     double Sin_Ag = sin ( obj->dAg * M_PI / 180. ), Cos_Ag = cos ( obj->dAg * M_PI / 180. );
     double h1 = std::abs( (dP1y - obj->dPy) * Cos_Ag - (dP1x - obj->dPx) * Sin_Ag ),
            h2 = std::abs( (dP2y - obj->dPy) * Cos_Ag - (dP2x - obj->dPx) * Sin_Ag );
     double X1 = (dP1x - obj->dPx) * Cos_Ag + (dP1y - obj->dPy) * Sin_Ag,
            X2 = (dP2x - obj->dPx) * Cos_Ag + (dP2y - obj->dPy) * Sin_Ag;
     double Y1 = (dP1y - obj->dPy) * Cos_Ag - (dP1x - obj->dPx) * Sin_Ag,
            Y2 = (dP2y - obj->dPy) * Cos_Ag - (dP2x - obj->dPx) * Sin_Ag;
     double H2_Plus_A2_1 = obj->dHb * obj->dHb + h1 * h1,
            H2_Plus_A2_2 = obj->dHb * obj->dHb + h2 * h2;
     
     force1Xb = log ( (sqrt( H2_Plus_A2_1 + (obj->dR+X1) * (obj->dR+X1)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb+X1)*(obj->dR+obj->dLb+X1)) - obj->dHb )
                     *(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb-X1)*(obj->dR+obj->dLb-X1)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_1 + (obj->dR-X1) * (obj->dR-X1)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR+X1) * (obj->dR+X1)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb+X1)*(obj->dR+obj->dLb+X1)) + obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb-X1)*(obj->dR+obj->dLb-X1)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR-X1) * (obj->dR-X1)) + obj->dHb ) )
                * P_G * dM1 * areadens;
     force1Yb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X1 ) , h1*sqrt( H2_Plus_A2_1 + ( obj->dR+obj->dLb+X1 ) * ( obj->dR+obj->dLb+X1 ) ) )-
                  atan2( obj->dHb * ( obj->dR+X1 )          , h1*sqrt( H2_Plus_A2_1 + ( obj->dR+X1 ) * ( obj->dR+X1 ) ) )+
                  atan2( obj->dHb * ( obj->dR+obj->dLb-X1 ) , h1*sqrt( H2_Plus_A2_1 + ( obj->dR+obj->dLb-X1 ) * ( obj->dR+obj->dLb-X1)))-
                  atan2( obj->dHb * ( obj->dR-X1 )          , h1*sqrt( H2_Plus_A2_1 + ( obj->dR-X1 ) * ( obj->dR-X1 ) ) ) )
                * 2 * P_G * dM1 * areadens;
     force2Xb = log ( (sqrt( H2_Plus_A2_2 + (obj->dR+X2) * (obj->dR+X2)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb+X2)*(obj->dR+obj->dLb+X2)) - obj->dHb )
                     *(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb-X2)*(obj->dR+obj->dLb-X2)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_2 + (obj->dR-X2) * (obj->dR-X2)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR+X2) * (obj->dR+X2)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb+X2)*(obj->dR+obj->dLb+X2)) + obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb-X2)*(obj->dR+obj->dLb-X2)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR-X2) * (obj->dR-X2)) + obj->dHb ) )
                * P_G * dM2 * areadens;
     force2Yb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X2 ) , h2*sqrt( H2_Plus_A2_2 + ( obj->dR+obj->dLb+X2 ) * ( obj->dR+obj->dLb+X2 ) ) )-
                  atan2( obj->dHb * ( obj->dR+X2 )          , h2*sqrt( H2_Plus_A2_2 + ( obj->dR+X2 ) * ( obj->dR+X2 ) ) )+
                  atan2( obj->dHb * ( obj->dR+obj->dLb-X2 ) , h2*sqrt( H2_Plus_A2_2 + ( obj->dR+obj->dLb-X2 ) * ( obj->dR+obj->dLb-X2)))-
                  atan2( obj->dHb * ( obj->dR-X2 )          , h2*sqrt( H2_Plus_A2_2 + ( obj->dR-X2 ) * ( obj->dR-X2 ) ) ) )
                * 2 * P_G * dM2 * areadens;

     force1x += force1Xb * Cos_Ag + force1Yb * Sin_Ag;
     force1y += force1Yb * Cos_Ag - force1Xb * Sin_Ag;
     force2x += force2Xb * Cos_Ag + force2Yb * Sin_Ag;
     force2y += force2Yb * Cos_Ag - force2Xb * Sin_Ag;
     
     obj->dAga += ( force1Xb * ( (dP1y - obj->dPy) * Cos_Ag - (dP1x - obj->dPx) * Sin_Ag ) -
                    force1Yb * ( (dP1x - obj->dPx) * Cos_Ag - (dP1y - obj->dPy) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx  += force1x / mass;
     obj->dAy  += force1y / mass;
     
     obj->dAga += ( force2Xb * ( (dP2y - obj->dPy) * Cos_Ag - (dP2x - obj->dPx) * Sin_Ag ) -
                    force2Yb * ( (dP2x - obj->dPx) * Cos_Ag - (dP2y - obj->dPy) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx  += force2x / mass;
     obj->dAy  += force2y / mass;

     //校正
     
     //改变需用到的变量 
     Sin_Ag = sin ( obj->dAg_ * M_PI / 180. ); Cos_Ag = cos ( obj->dAg_ * M_PI / 180. );
     h1 = std::abs( (dP1y_ - obj->dPy_) * Cos_Ag - (dP1x_ - obj->dPx_) * Sin_Ag );
     h2 = std::abs( (dP2y_ - obj->dPy_) * Cos_Ag - (dP2x_ - obj->dPx_) * Sin_Ag );
     X1 = (dP1x_ - obj->dPx_) * Cos_Ag + (dP1y_ - obj->dPy_) * Sin_Ag;
     X2 = (dP2x_ - obj->dPx_) * Cos_Ag + (dP2y_ - obj->dPy_) * Sin_Ag;
     Y1 = (dP1y_ - obj->dPy_) * Cos_Ag - (dP1x_ - obj->dPx_) * Sin_Ag;
     Y2 = (dP2y_ - obj->dPy_) * Cos_Ag - (dP2x_ - obj->dPx_) * Sin_Ag;
     H2_Plus_A2_1 = obj->dHb * obj->dHb + h1 * h1;
     H2_Plus_A2_2 = obj->dHb * obj->dHb + h2 * h2;

     force1Xb = log ( (sqrt( H2_Plus_A2_1 + (obj->dR+X1) * (obj->dR+X1)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb+X1)*(obj->dR+obj->dLb+X1)) - obj->dHb )
                     *(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb-X1)*(obj->dR+obj->dLb-X1)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_1 + (obj->dR-X1) * (obj->dR-X1)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR+X1) * (obj->dR+X1)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb+X1)*(obj->dR+obj->dLb+X1)) + obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR+obj->dLb-X1)*(obj->dR+obj->dLb-X1)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_1 + (obj->dR-X1) * (obj->dR-X1)) + obj->dHb ) )
                * P_G * dM1 * areadens;
     force1Yb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X1 ) , h1*sqrt( H2_Plus_A2_1 + ( obj->dR+obj->dLb+X1 ) * ( obj->dR+obj->dLb+X1 ) ) )-
                  atan2( obj->dHb * ( obj->dR+X1 )          , h1*sqrt( H2_Plus_A2_1 + ( obj->dR+X1 ) * ( obj->dR+X1 ) ) )+
                  atan2( obj->dHb * ( obj->dR+obj->dLb-X1 ) , h1*sqrt( H2_Plus_A2_1 + ( obj->dR+obj->dLb-X1 ) * ( obj->dR+obj->dLb-X1)))-
                  atan2( obj->dHb * ( obj->dR-X1 )          , h1*sqrt( H2_Plus_A2_1 + ( obj->dR-X1 ) * ( obj->dR-X1 ) ) ) )
                * 2 * P_G * dM1 * areadens;
     force2Xb = log ( (sqrt( H2_Plus_A2_2 + (obj->dR+X2) * (obj->dR+X2)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb+X2)*(obj->dR+obj->dLb+X2)) - obj->dHb )
                     *(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb-X2)*(obj->dR+obj->dLb-X2)) + obj->dHb )
                     *(sqrt( H2_Plus_A2_2 + (obj->dR-X2) * (obj->dR-X2)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR+X2) * (obj->dR+X2)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb+X2)*(obj->dR+obj->dLb+X2)) + obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR+obj->dLb-X2)*(obj->dR+obj->dLb-X2)) - obj->dHb )
                     /(sqrt( H2_Plus_A2_2 + (obj->dR-X2) * (obj->dR-X2)) + obj->dHb ) )
                * P_G * dM2 * areadens;
     force2Yb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X2 ) , h2*sqrt( H2_Plus_A2_2 + ( obj->dR+obj->dLb+X2 ) * ( obj->dR+obj->dLb+X2 ) ) )-
                  atan2( obj->dHb * ( obj->dR+X2 )          , h2*sqrt( H2_Plus_A2_2 + ( obj->dR+X2 ) * ( obj->dR+X2 ) ) )+
                  atan2( obj->dHb * ( obj->dR+obj->dLb-X2 ) , h2*sqrt( H2_Plus_A2_2 + ( obj->dR+obj->dLb-X2 ) * ( obj->dR+obj->dLb-X2)))-
                  atan2( obj->dHb * ( obj->dR-X2 )          , h2*sqrt( H2_Plus_A2_2 + ( obj->dR-X2 ) * ( obj->dR-X2 ) ) ) )
                * 2 * P_G * dM2 * areadens;

     force1x_ += force1Xb * Cos_Ag + force1Yb * Sin_Ag;
     force1y_ += force1Yb * Cos_Ag - force1Xb * Sin_Ag;
     force2x_ += force2Xb * Cos_Ag + force2Yb * Sin_Ag;
     force2y_ += force2Yb * Cos_Ag - force2Xb * Sin_Ag;
     
     obj->dAga_ += ( force1Xb * ( (dP1y_ - obj->dPy_) * Cos_Ag - (dP1x_ - obj->dPx_) * Sin_Ag ) -
                     force1Yb * ( (dP1x_ - obj->dPx_) * Cos_Ag - (dP1y_ - obj->dPy_) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx_ += force1x_ / mass;
     obj->dAy_ += force1y_ / mass; 

     obj->dAga_ += ( force2Xb * ( (dP2y_ - obj->dPy_) * Cos_Ag - (dP2x_ - obj->dPx_) * Sin_Ag ) -
                     force2Yb * ( (dP2x_ - obj->dPx_) * Cos_Ag - (dP2y_ - obj->dPy_) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx_ += force2x_ / mass;
     obj->dAy_ += force2y_ / mass; 
}

template <>
void BlackHole::Affect ( SpaceShip * obj )
{
     double forcex, forcey, forcex_, forcey_;
     double dist = sqrt ( (dPx - obj->dPx)*(dPx - obj->dPx)+(dPy - obj->dPy)*(dPy - obj->dPy) ) * LengthUnit;

     //calculate the gravity on the Ball
     double mass = obj->dMB + obj->dMf + obj->dMw +obj->dMe;//此时mass不包括broad 
     forcex = P_G * dM * mass/(dist*dist*dist) * (dPx - obj->dPx) ;
     forcey = P_G * dM * mass/(dist*dist*dist) * (dPy - obj->dPy) ;

     dist = sqrt ( (dPx - obj->dPx_)*(dPx - obj->dPx_)+(dPy - obj->dPy_)*(dPy - obj->dPy_) ) * LengthUnit;
     forcex_ = P_G * dM * mass/(dist*dist*dist) * (dPx - obj->dPx_) / 2;
     forcey_ = P_G * dM * mass/(dist*dist*dist) * (dPy - obj->dPy_) / 2;
     
     mass += obj->dMb;//此时mass成为总质量 

     //calculate the gravity on the board
     double forceXb, forceYb;
     double areadens = obj->dMb / ( obj->dLb * obj->dHb * 4 ); //Area Density of the board

     double Sin_Ag = sin ( obj->dAg * M_PI / 180. ), Cos_Ag = cos ( obj->dAg * M_PI / 180. );
     double h = std::abs( (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag );
     double X = (dPx - obj->dPx) * Cos_Ag + (dPy - obj->dPy) * Sin_Ag;
     double Y = (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag ;
     double H2_Plus_A2 = obj->dHb * obj->dHb + h * h;
     
     forceXb = log ( (sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) - obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) + obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) + obj->dHb ) )
               * P_G * dM * areadens;
     forceYb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb+X ) * ( obj->dR+obj->dLb+X ) ) )-
                 atan2( obj->dHb * ( obj->dR+X )          , h*sqrt( H2_Plus_A2 + ( obj->dR+X ) * ( obj->dR+X ) ) )+
                 atan2( obj->dHb * ( obj->dR+obj->dLb-X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb-X ) * ( obj->dR+obj->dLb-X)))-
                 atan2( obj->dHb * ( obj->dR-X )          , h*sqrt( H2_Plus_A2 + ( obj->dR-X ) * ( obj->dR-X ) ) ) )
               * 2 * P_G * dM * areadens;
     forcex += forceXb * Cos_Ag + forceYb * Sin_Ag;
     forcey += forceYb * Cos_Ag - forceXb * Sin_Ag;
     
     obj->dAga += ( forceXb * ( (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag ) -
                    forceYb * ( (dPx - obj->dPx) * Cos_Ag - (dPy - obj->dPy) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx  += forcex / mass;
     obj->dAy  += forcey / mass;

     //校正
     
     //改变需用到的变量 
     Sin_Ag = sin ( obj->dAg_ * M_PI / 180. ); Cos_Ag = cos ( obj->dAg_ * M_PI / 180. );
     h = std::abs( (dPy - obj->dPy_) * Cos_Ag - (dPx - obj->dPx_) * Sin_Ag );
     X = (dPx - obj->dPx_) * Cos_Ag + (dPy - obj->dPy_) * Sin_Ag;
     Y = (dPy - obj->dPy_) * Cos_Ag - (dPx - obj->dPx_) * Sin_Ag;
     H2_Plus_A2 = obj->dHb * obj->dHb + h * h;

     forceXb = log ( (sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) - obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) + obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) + obj->dHb ) )
               * P_G * dM * areadens;
     forceYb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb+X ) * ( obj->dR+obj->dLb+X ) ) )-
                 atan2( obj->dHb * ( obj->dR+X )          , h*sqrt( H2_Plus_A2 + ( obj->dR+X ) * ( obj->dR+X ) ) )+
                 atan2( obj->dHb * ( obj->dR+obj->dLb-X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb-X ) * ( obj->dR+obj->dLb-X)))-
                 atan2( obj->dHb * ( obj->dR-X )          , h*sqrt( H2_Plus_A2 + ( obj->dR-X ) * ( obj->dR-X ) ) ) )
               * 2 * P_G * dM * areadens;
     forcex_ += forceXb * Cos_Ag + forceYb * Sin_Ag;
     forcey_ += forceYb * Cos_Ag - forceXb * Sin_Ag;
     
     obj->dAga_ += ( forceXb * ( (dPy - obj->dPy_) * Cos_Ag - (dPx - obj->dPx_) * Sin_Ag ) -
                     forceYb * ( (dPx - obj->dPx_) * Cos_Ag - (dPy - obj->dPy_) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx_ += forcex_ / mass;
     obj->dAy_ += forcey_ / mass; 
}

template <>
void Planet::Affect ( SpaceShip * obj )
{
     double forcex, forcey, forcex_, forcey_;
     double dist = sqrt ( (dPx - obj->dPx)*(dPx - obj->dPx)+(dPy - obj->dPy)*(dPy - obj->dPy) ) * LengthUnit;

     //calculate the gravity on the Ball
     double mass = obj->dMB + obj->dMf + obj->dMw +obj->dMe;//此时mass不包括broad 
     forcex = P_G * dM * mass/(dist*dist*dist) * (dPx - obj->dPx) ;
     forcey = P_G * dM * mass/(dist*dist*dist) * (dPy - obj->dPy) ;

     dist = sqrt ( (dPx_ - obj->dPx_)*(dPx_ - obj->dPx_)+(dPy_ - obj->dPy_)*(dPy_ - obj->dPy_) ) * LengthUnit;
     forcex_ = P_G * dM * mass/(dist*dist*dist) * (dPx_ - obj->dPx_) / 2;
     forcey_ = P_G * dM * mass/(dist*dist*dist) * (dPy_ - obj->dPy_) / 2;
     
     mass += obj->dMb;//此时mass成为总质量 

     //calculate the gravity on the board
     double forceXb, forceYb;
     double areadens = obj->dMb / ( obj->dLb * obj->dHb * 4 ); //Area Density of the board

     double Sin_Ag = sin ( obj->dAg * M_PI / 180. ), Cos_Ag = cos ( obj->dAg * M_PI / 180. );
     double h = std::abs( (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag );
     double X = (dPx - obj->dPx) * Cos_Ag + (dPy - obj->dPy) * Sin_Ag;
     double Y = (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag ;
     double H2_Plus_A2 = obj->dHb * obj->dHb + h * h;
     
     forceXb = log ( (sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) - obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) + obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) + obj->dHb ) )
               * P_G * dM * areadens;
     forceYb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb+X ) * ( obj->dR+obj->dLb+X ) ) )-
                 atan2( obj->dHb * ( obj->dR+X )          , h*sqrt( H2_Plus_A2 + ( obj->dR+X ) * ( obj->dR+X ) ) )+
                 atan2( obj->dHb * ( obj->dR+obj->dLb-X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb-X ) * ( obj->dR+obj->dLb-X)))-
                 atan2( obj->dHb * ( obj->dR-X )          , h*sqrt( H2_Plus_A2 + ( obj->dR-X ) * ( obj->dR-X ) ) ) )
               * 2 * P_G * dM * areadens;
     forcex += forceXb * Cos_Ag + forceYb * Sin_Ag;
     forcey += forceYb * Cos_Ag - forceXb * Sin_Ag;
     
     obj->dAga += ( forceXb * ( (dPy - obj->dPy) * Cos_Ag - (dPx - obj->dPx) * Sin_Ag ) -
                    forceYb * ( (dPx - obj->dPx) * Cos_Ag - (dPy - obj->dPy) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx  += forcex / mass;
     obj->dAy  += forcey / mass;

     //校正
     
     //改变需用到的变量 
     Sin_Ag = sin ( obj->dAg_ * M_PI / 180. ); Cos_Ag = cos ( obj->dAg_ * M_PI / 180. );
     h = std::abs( (dPy_ - obj->dPy_) * Cos_Ag - (dPx_ - obj->dPx_) * Sin_Ag );
     X = (dPx_ - obj->dPx_) * Cos_Ag + (dPy_ - obj->dPy_) * Sin_Ag;
     Y = (dPy_ - obj->dPy_) * Cos_Ag - (dPx_ - obj->dPx_) * Sin_Ag;
     H2_Plus_A2 = obj->dHb * obj->dHb + h * h;

     forceXb = log ( (sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) - obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) + obj->dHb )
                    *(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+X) * (obj->dR+X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb+X)*(obj->dR+obj->dLb+X)) + obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR+obj->dLb-X)*(obj->dR+obj->dLb-X)) - obj->dHb )
                    /(sqrt( H2_Plus_A2 + (obj->dR-X) * (obj->dR-X)) + obj->dHb ) )
               * P_G * dM * areadens;
     forceYb = ( atan2( obj->dHb * ( obj->dR+obj->dLb+X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb+X ) * ( obj->dR+obj->dLb+X ) ) )-
                 atan2( obj->dHb * ( obj->dR+X )          , h*sqrt( H2_Plus_A2 + ( obj->dR+X ) * ( obj->dR+X ) ) )+
                 atan2( obj->dHb * ( obj->dR+obj->dLb-X ) , h*sqrt( H2_Plus_A2 + ( obj->dR+obj->dLb-X ) * ( obj->dR+obj->dLb-X)))-
                 atan2( obj->dHb * ( obj->dR-X )          , h*sqrt( H2_Plus_A2 + ( obj->dR-X ) * ( obj->dR-X ) ) ) )
               * 2 * P_G * dM * areadens;
     forcex_ += forceXb * Cos_Ag + forceYb * Sin_Ag;
     forcey_ += forceYb * Cos_Ag - forceXb * Sin_Ag;

     obj->dAga_ += ( forceXb * ( (dPy_ - obj->dPy_) * Cos_Ag - (dPx_ - obj->dPx_) * Sin_Ag ) -
                     forceYb * ( (dPx_ - obj->dPx_) * Cos_Ag - (dPy_ - obj->dPy_) * Sin_Ag ) )
                  /( obj->dJb + obj->dJw );

     obj->dAx_ += forcex_ / mass;
     obj->dAy_ += forcey_ / mass; 
}
