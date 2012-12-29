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
 *  Name: GameAttributes.h
 *  Author: niasw(孙思白) JzZhang(张靖中) FrankXu(徐震)
 *  Description: 小课题游戏参数头文件 
 */

#ifndef _PHYCONST_
#define P_G 1e-1
#define TimeUnit .001
#define LengthUnit 0.5
#define MassUnit 1
#define _PHYCONST_
#endif

#ifndef _ATTRIBUTES_
#define FreshTime 30
#define MapEdgeH 0.5
#define MapEdgeColor 0,1,0
#define DstRadius 1
#define DstColor 0,1,0
#define ShipJetFactor1 1 //Acceleration = ShipJetFactor1 * JetI
#define ShipJetFactor2 1 //Acceleration = ShipJetFactor2 * GasRelativeVelocity^2 
#define ShipBmass 100
#define Shipbmass 50
#define Shipwmass 50
#define ShipJw    30
#define ShipColor 0.5,0.5,0.5
#define ShipbColor 1,0,1
#define ShipRadius 0.5
#define ShipLb    0.5
#define ShipHb    0.5
#define Sp_FuelMax 1000
#define Sp_ExplMax 1000
#define Sp_HPmax 1000
#define Sp_JetImax 5
#define Sp_JetImin 1
#define Sp_BulletSpeed 10
#define Sp_MissInitSpeed 1
#define Sp_JetSpeed 15
#define Sp_GasV 0.01
#define Sp_ExplosionV 0.2
#define StarColor 1,1,0
#define BlackHoleColor 0,0,0
#define PlanetColor 0,0,1
#define FortColor 1,0,0
#define FortwColor 0.5,0.5,0.5
#define Ft_ExplosionV 0.2
#define Ft_FireAngleVar 2
#define MissColor 1,0.5,0
#define MissMass 5
#define MissRadius 0.3
#define MissFuel 100
#define MissJetSpeed 20
#define MissGasV 0.05
#define MissExpV 0.1
#define BulletColor 0.25,0.25,0
#define BulletMass 1
#define BulletRadius 0.08
#define BulletExpV 0.05
#define BulletAmplifier 5
#define FuelBoxColor 0,0.5,1
#define FuelBoxMass 2
#define FuelBoxLength 0.5
#define FuelBoxExpV 0.05
#define ExplBoxColor 1,0.5,0
#define ExplBoxMass 2
#define ExplBoxLength 0.5
#define ExplBoxExpV 0.1
#define StationColor 0,1,0
#define GasColor 0.5,0.75,1
#define GasMass 1
#define GasAmplifier 50

#define VIEW_GAME_FRONT 1
#define VIEW_GAME_FRONT2 2 // 2 * VIEW_FRONT
#define VIEW_GAME_BACK 30
#define VIEW_GAME_BACK2 60 // 2 * VIEW_BACK
#define SHOW_SHIP_DISTANCE 3.5
#define _ATTRIBUTES_
#endif

#ifndef _SPACEOBJECTS_
#define CATE_Star 1
#define CATE_BiStar 2
#define CATE_BlackHole 3
#define CATE_Planet 4
#define CATE_SpaceShip 5
#define CATE_Fort 6
#define CATE_Missile 7
#define CATE_Bullet 8
#define CATE_FuelBox 9
#define CATE_ExplosiveBox 10
#define CATE_Station 11

#define CATE_Gas -1
#define CATE_Explosion -2
#define _SPACEOBJECTS_
#endif

#ifndef _MAPFILE_
#define MAP_TIME1    "Map/Time1.map"
#define MAP_TIME2    "Map/Time2.map"
#define MAP_TIME3    "Map/Time3.map"
#define MAP_TIMEDIY  "Map/TimeDIY.map"
#define MAP_FIGHT1   "Map/Fight1.map"
#define MAP_FIGHT2   "Map/Fight2.map"
#define MAP_FIGHT3   "Map/Fight3.map"
#define MAP_FIGHTDIY "Map/FightDIY.map"
#define MAP_FREE1    "Map/Free1.map"
#define MAP_FREE2    "Map/Free2.map"
#define MAP_FREE3    "Map/Free3.map"
#define MAP_FREEDIY  "Map/FreeDIY.map"
#define _MAPFILE_
#endif

#ifndef _SAVFILE_
#define SAV_LASTONE  "Sav/LastOne.sav"
#define SAV_SAVING1  "Sav/Saving1.sav"
#define SAV_SAVING2  "Sav/Saving2.sav"
#define SAV_SAVING3  "Sav/Saving3.sav"
#define REC_TIME     "Sav/TimeRecord.txt"
#define _SAVFILE_
#endif
