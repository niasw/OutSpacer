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
 *  Name: GameProcedure.cpp
 *  Author: niasw(ËïËŒ°×) JzZhang(ÕÅŸžÖÐ) FrankXu(ÐìÕð)
 *  Description: Ð¡¿ÎÌâOutspacerÓÎÏ·¹ý³ÌÖÐžšÖúº¯ÊýÊµÏÖ 
 */
#include "GameProcedure.h"
int SearchOrder( int cate, void* obj );

int LoadMap ( char* filename )
{
     std::ifstream filein ( filename , std::ios::binary );
     if (filein.good())
     {
        filein.seekg ( 0x06, std::ios::beg );
        filein.read ( (char*)&Level , 4 );
        filein.seekg ( 0x12, std::ios::beg );
        filein.read ( (char*)&gamestate , 2 );
        if (gamestate==0)
        {
           filein.close();
           return 1;
        }
        filein.read ( (char*)&MapRadius , 8 );
     
        char *tmp1 = new char[88];
        filein.read ( tmp1 , 48 );
        new Node<SpaceShip>(new SpaceShip(*((double*)tmp1),*((double*)tmp1+1),*((double*)tmp1+2),
                                 *((double*)tmp1+3),*((double*)tmp1+4),*((double*)tmp1+5) ));
        mainship = Node<SpaceShip>::beg->obj;
        filein.seekg ( 0x0a, std::ios::beg );
        int tmp2;
        filein.read ( (char*)&tmp2 , 4 );
        filein.seekg ( tmp2, std::ios::beg );
        filein.read ( (char*)&tmp2 , 4 );
        int cate = 0;
        int attachorder, aimorder;
        for (int tmp3 = 0; tmp3 < tmp2; ++ tmp3)
        {
            filein.read ( (char*)&cate, 4 );
            void *aimnode, *attachnode;
            switch (cate)
            {
                   case CATE_Star:
                        filein.read ( tmp1, 88 );
                        new Node<Star>(new Star(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24))));
                        break;
                   case CATE_BiStar:
                        filein.read ( tmp1, 88 );
                        new Node<BiStar>(new BiStar(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),
                                                  *((double*)(tmp1+32)),*((double*)(tmp1+40)),*((double*)(tmp1+48)),*((double*)(tmp1+56)),
                                                  *((double*)(tmp1+64)),*((double*)(tmp1+72)),*((int*)(tmp1+80)),*((int*)(tmp1+84))));
                        break;
                   case CATE_BlackHole:
                        filein.read ( tmp1, 88 );
                        new Node<BlackHole>(new BlackHole(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24))));
                        break;
                   case CATE_Planet:
                        filein.read ( tmp1, 88 );
                        new Node<Planet>(new Planet(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),*((double*)(tmp1+32)),*((double*)(tmp1+40))));
                        break;
                   case CATE_SpaceShip://to improve
//                        filein.read ( tmp1, 88 );
//                        new Node<SpaceShip>(new SpaceShip(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),
//                                                  *((double*)(tmp1+32)),*((double*)(tmp1+40)),*((int*)(tmp1+48)),*((int*)(tmp1+52)),
//                                                  *((double*)(tmp1+56)),*((double*)(tmp1+64)),*((double*)(tmp1+72)),*((int*)(tmp1+80))));
                        break;
                   case CATE_Fort:
                        filein.read ( tmp1, 88 );
                        attachorder = *((int*)(tmp1+0x18));
                        aimorder = *((int*)(tmp1+0x20));
                        switch ((*((int*)(tmp1+0x14))) & 0xf)//AttachCate
                        {
                               case CATE_Star:
                               {
                                    Node<Star>* node;
                                    node = Node<Star>::beg;
                                    for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                        node = node->next;
                                    attachnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_BiStar:
                               {
                                    Node<BiStar>* node;
                                    node = Node<BiStar>::beg;
                                    for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                        node = node->next;
                                    attachnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_Planet:
                               {
                                    Node<Planet>* node;
                                    node = Node<Planet>::beg;
                                    for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                        node = node->next;
                                    attachnode = (void*) (node->obj);
                                    break;
                               }
                        }
                        switch ((*((int*)(tmp1+0x1c))) & 0xf)//AimCate
                        {
                               case CATE_SpaceShip:
                               {
                                    Node<SpaceShip>* node;
                                    node = Node<SpaceShip>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_Fort:
                               {
                                    Node<Fort>* node;
                                    node = Node<Fort>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                        }
                        new Node<Fort>(new Fort(*((double*)tmp1),*((double*)(tmp1+0x8)),*((int*)(tmp1+0x10)),*((int*)(tmp1+0x14)), attachnode, *((int*)(tmp1+0x1c)), aimnode, *((int*)(tmp1+0x24))));
                        break;
                   case CATE_Missile:
                        filein.read ( tmp1, 88 );
                        aimorder = *((int*)(tmp1+0x24));
                        switch ((*((int*)(tmp1+0x20))) & 0xf)//AimCate
                        {
                               case CATE_SpaceShip:
                               {
                                    Node<SpaceShip>* node;
                                    node = Node<SpaceShip>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_Fort:
                               {
                                    Node<Fort>* node;
                                    node = Node<Fort>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                        }
                        new Node<Missile>(new Missile(*((double*)tmp1),*((double*)(tmp1+0x8)),*((double*)(tmp1+0x10)),*((double*)(tmp1+0x18)),*((int*)(tmp1+0x20)), aimnode));
                        break;
                   case CATE_Bullet:
                        filein.read ( tmp1, 88 );
                        new Node<Bullet>(new Bullet(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)) ));
                        break;
                   case CATE_FuelBox:
                        filein.read ( tmp1, 88 );
                        new Node<FuelBox>(new FuelBox(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)) ));
                        break;
                   case CATE_ExplosiveBox:
                        filein.read ( tmp1, 88 );
                        new Node<ExplosiveBox>(new ExplosiveBox(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)) ));
                        break;
                   case CATE_Station:
                        filein.read ( tmp1, 88 );
                        new Node<Station>(new Station(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)) ));
                        break;
                   case CATE_Gas:
                        filein.read ( tmp1, 88 );
                        new Node<Gas>(new Gas(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),*((double*)(tmp1+32)),*((double*)(tmp1+40)),*((int*)(tmp1+48)) ));
                        break;
                   case CATE_Explosion:
                        filein.read ( tmp1, 88 );
                        new Node<Explosion>(new Explosion(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((int*)(tmp1+24)) ));
                        break;
            }
        }
        filein.seekg ( 0x74, std::ios::beg );
        switch (gamestate)
        {
               case CASE_GAMEtime:
                    filein.read ( (char*)&DstPx , 8 );
                    filein.read ( (char*)&DstPy , 8 );
                    break;
               case CASE_GAMEfight:
               {
                    filein.read ( tmp1 , 32 );
                    attachorder = *((int*)(tmp1+0x18));
                    aimorder = *((int*)(tmp1+0x20));
                    void* attachnode;
                    switch ((*((int*)(tmp1+0x14))) & 0xf)//AttachCate
                    {
                            case CATE_Star:
                            {
                                 Node<Star>* node;
                                 node = Node<Star>::beg;
                                 for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                     node = node->next;
                                 attachnode = (void*) (node->obj);
                                 break;
                            }
                            case CATE_BiStar:
                            {
                                 Node<BiStar>* node;
                                 node = Node<BiStar>::beg;
                                 for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                     node = node->next;
                                 attachnode = (void*) (node->obj);
                                 break;
                            }
                            case CATE_Planet:
                            {
                                 Node<Planet>* node;
                                 node = Node<Planet>::beg;
                                 for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                     node = node->next;
                                 attachnode = (void*) (node->obj);
                                 break;
                            }
                    }
                    Node<Fort>::cur = 0; //insert as the head
                    Node<Fort>::ins(new Fort(*((double*)tmp1),*((double*)(tmp1+8)),*((int*)(tmp1+16)),
                                        *((int*)(tmp1+20)),attachnode,CATE_SpaceShip, mainship,*((int*)(tmp1+28))));
                    bossfort = Node<Fort>::beg->obj;
                    break;
               }
               case CASE_GAMEfree:
                    break;
        }
        filein.close();
        return 0;
     }
     else return 1;
}

int LoadSav ( char* filename )
{
     std::ifstream filein ( filename , std::ios::binary );
     if (filein.good())
     {
        filein.seekg ( 0x06, std::ios::beg );
        filein.read ( (char*)&Level , 4 );
        filein.seekg ( 0x12, std::ios::beg );
        filein.read ( (char*)&gamestate , 2 );
        if (gamestate==0)
        {
           filein.close();
           return 1;
        }
        filein.read ( (char*)&MapRadius , 8 );
     
        char *tmp1 = new char[88];
        filein.read ( tmp1 , 88 );
        new Node<SpaceShip>(new SpaceShip(*((double*)tmp1),*((double*)(tmp1+0x8)),*((double*)(tmp1+0x10)),
                                 *((double*)(tmp1+0x18)),*((double*)(tmp1+0x20)),*((double*)(tmp1+0x28)),
                                 *((int*)(tmp1+0x30)),*((int*)(tmp1+0x34)),*((double*)(tmp1+0x38)),
                                 *((double*)(tmp1+0x40)),*((double*)(tmp1+0x48)),*((double*)(tmp1+0x50)) ));
        mainship = Node<SpaceShip>::beg->obj;
        filein.seekg ( 0x0a, std::ios::beg );
        int tmp2;
        filein.read ( (char*)&tmp2 , 4 );
        filein.seekg ( tmp2, std::ios::beg );
        filein.read ( (char*)&tmp2 , 4 );
        int cate = 0;
        int attachorder, aimorder;
        for (int tmp3 = 0; tmp3 < tmp2; ++ tmp3)
        {
            filein.read ( (char*)&cate, 4 );
            void *aimnode, *attachnode;
            switch (cate)
            {
                   case CATE_Star:
                        filein.read ( tmp1, 88 );
                        new Node<Star>(new Star(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24))));
                        break;
                   case CATE_BiStar:
                        filein.read ( tmp1, 88 );
                        new Node<BiStar>(new BiStar(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),
                                                  *((double*)(tmp1+32)),*((double*)(tmp1+40)),*((double*)(tmp1+48)),*((double*)(tmp1+56)),
                                                  *((double*)(tmp1+64)),*((double*)(tmp1+72)),*((int*)(tmp1+80)),*((int*)(tmp1+84))));
                        break;
                   case CATE_BlackHole:
                        filein.read ( tmp1, 88 );
                        new Node<BlackHole>(new BlackHole(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24))));
                        break;
                   case CATE_Planet:
                        filein.read ( tmp1, 88 );
                        new Node<Planet>(new Planet(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),*((double*)(tmp1+32)),*((double*)(tmp1+40))));
                        break;
                   case CATE_SpaceShip://to improve
//                        filein.read ( tmp1, 88 );
//                        new Node<SpaceShip>(new SpaceShip(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),
//                                                  *((double*)(tmp1+32)),*((double*)(tmp1+40)),*((int*)(tmp1+48)),*((int*)(tmp1+52)),
//                                                  *((double*)(tmp1+56)),*((double*)(tmp1+64)),*((double*)(tmp1+72)),*((int*)(tmp1+80))));
                        break;
                   case CATE_Fort:
                        filein.read ( tmp1, 88 );
                        attachorder = *((int*)(tmp1+0x18));
                        aimorder = *((int*)(tmp1+0x20));
                        switch ((*((int*)(tmp1+0x14))) & 0xf)//AttachCate
                        {
                               case CATE_Star:
                               {
                                    Node<Star>* node;
                                    node = Node<Star>::beg;
                                    for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                        node = node->next;
                                    attachnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_BiStar:
                               {
                                    Node<BiStar>* node;
                                    node = Node<BiStar>::beg;
                                    for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                        node = node->next;
                                    attachnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_Planet:
                               {
                                    Node<Planet>* node;
                                    node = Node<Planet>::beg;
                                    for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                        node = node->next;
                                    attachnode = (void*) (node->obj);
                                    break;
                               }
                        }
                        switch ((*((int*)(tmp1+0x1c))) & 0xf)//AimCate
                        {
                               case CATE_SpaceShip:
                               {
                                    Node<SpaceShip>* node;
                                    node = Node<SpaceShip>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_Fort:
                               {
                                    Node<Fort>* node;
                                    node = Node<Fort>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                        }
                        new Node<Fort>(new Fort(*((double*)tmp1),*((double*)(tmp1+0x8)),*((int*)(tmp1+0x10)),*((int*)(tmp1+0x14)), attachnode, *((int*)(tmp1+0x1c)), aimnode, *((int*)(tmp1+0x24))));
                        break;
                   case CATE_Missile:
                        filein.read ( tmp1, 88 );
                        aimorder = *((int*)(tmp1+0x24));
                        switch ((*((int*)(tmp1+0x20))) & 0xf)//AimCate
                        {
                               case CATE_SpaceShip:
                               {
                                    Node<SpaceShip>* node;
                                    node = Node<SpaceShip>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                               case CATE_Fort:
                               {
                                    Node<Fort>* node;
                                    node = Node<Fort>::beg;
                                    for ( int tmp4 = 1; tmp4 < aimorder; ++ tmp4 )
                                        node = node->next;
                                    aimnode = (void*) (node->obj);
                                    break;
                               }
                        }
                        new Node<Missile>(new Missile(*((double*)tmp1),*((double*)(tmp1+0x8)),*((double*)(tmp1+0x10)),*((double*)(tmp1+0x18)),*((int*)(tmp1+0x20)), aimnode));
                        break;
                   case CATE_Bullet:
                        filein.read ( tmp1, 88 );
                        new Node<Bullet>(new Bullet(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)) ));
                        break;
                   case CATE_FuelBox:
                        filein.read ( tmp1, 88 );
                        new Node<FuelBox>(new FuelBox(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)) ));
                        break;
                   case CATE_ExplosiveBox:
                        filein.read ( tmp1, 88 );
                        new Node<ExplosiveBox>(new ExplosiveBox(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)) ));
                        break;
                   case CATE_Station:
                        filein.read ( tmp1, 88 );
                        new Node<Station>(new Station(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)) ));
                        break;
                   case CATE_Gas:
                        filein.read ( tmp1, 88 );
                        new Node<Gas>(new Gas(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((double*)(tmp1+24)),*((double*)(tmp1+32)),*((double*)(tmp1+40)),*((int*)(tmp1+48)) ));
                        break;
                   case CATE_Explosion:
                        filein.read ( tmp1, 88 );
                        new Node<Explosion>(new Explosion(*((double*)tmp1),*((double*)(tmp1+8)),*((double*)(tmp1+16)),*((int*)(tmp1+24)) ));
                        break;
            }
        }
        filein.seekg ( 0x74, std::ios::beg );
        switch (gamestate)
        {
               case CASE_GAMEtime:
                    filein.read ( (char*)&DstPx , 8 );
                    filein.read ( (char*)&DstPy , 8 );
                    filein.read ( (char*)&Time , 4 );
                    break;
               case CASE_GAMEfight:
               {
                    filein.read ( tmp1 , 32 );
                    attachorder = *((int*)(tmp1+0x18));
                    aimorder = *((int*)(tmp1+0x20));
                    void* attachnode;
                    switch ((*((int*)(tmp1+0x14))) & 0xf)//AttachCate
                    {
                            case CATE_Star:
                            {
                                 Node<Star>* node;
                                 node = Node<Star>::beg;
                                 for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                     node = node->next;
                                 attachnode = (void*) (node->obj);
                                 break;
                            }
                            case CATE_BiStar:
                            {
                                 Node<BiStar>* node;
                                 node = Node<BiStar>::beg;
                                 for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                     node = node->next;
                                 attachnode = (void*) (node->obj);
                                 break;
                            }
                            case CATE_Planet:
                            {
                                 Node<Planet>* node;
                                 node = Node<Planet>::beg;
                                 for ( int tmp4 = 1; tmp4 < attachorder; ++ tmp4 )
                                     node = node->next;
                                 attachnode = (void*) (node->obj);
                                 break;
                            }
                    }
                    Node<Fort>::cur = 0; //insert as the head
                    Node<Fort>::ins(new Fort(*((double*)tmp1),*((double*)(tmp1+8)),*((int*)(tmp1+16)),
                                        *((int*)(tmp1+20)),attachnode,CATE_SpaceShip, mainship,*((int*)(tmp1+28))));
                    bossfort = Node<Fort>::beg->obj;
                    break;
               }
               case CASE_GAMEfree:
                    break;
        }
        filein.close();
        return 0;
     }
     else return 1;
}

void Save ( char * filename )
{
     std::ofstream fileout( filename, std::ios::binary );
     fileout.seekp( 0, std::ios::beg );
     fileout.write( "OS", 2 );                                        //ÎÄŒþÀàÐÍ±êÊ¶ 
     int size;
     if (gamestate==CASE_GAMEfight)
     {
        size = Node<Star>::num + Node<BiStar>::num + Node<BlackHole>::num + Node<Planet>::num 
             + Node<SpaceShip>::num + Node<Fort>::num + Node<Bullet>::num + Node<Missile>::num
             + Node<FuelBox>::num + Node<ExplosiveBox>::num + Node<Station>::num 
             + Node<Gas>::num + Node<Explosion>::num - 2;
     }
     else
     {
        size = Node<Star>::num + Node<BiStar>::num + Node<BlackHole>::num + Node<Planet>::num 
             + Node<SpaceShip>::num + Node<Fort>::num + Node<Bullet>::num + Node<Missile>::num
             + Node<FuelBox>::num + Node<ExplosiveBox>::num + Node<Station>::num 
             + Node<Gas>::num + Node<Explosion>::num - 1;
     }
     fileout.seekp( 0x94, std::ios::beg );
     fileout.write( (char*)&size, 4 );                                //ÀàžöÊý 
     size = 0x98 + size * 92;
     fileout.seekp( 2, std::ios::beg );
     fileout.write( (char*)&size, 4 );                                //ÎÄŒþŽóÐ¡ 
     fileout.write( (char*)&Level, 4 );                               //Œ¶±ð 
     int tmp = 0x94;
     fileout.write( (char*)&tmp, 4 );                                 //ÊýŸÝÆðÊŒÎ»ÖÃ
     tmp = 0x86;
     fileout.write( (char*)&tmp, 4 );                                 //ÐÅÏ¢Í·ŽóÐ¡
     fileout.write( (char*)&gamestate, 2);                            //Ä£Êœ
     fileout.write( (char*)&MapRadius, 8);                            //µØÍŒ°ëŸ¶
     //ÐŽmainshipÊýŸÝ 
     fileout.write( (char*)&(mainship->dPx), 8);
     fileout.write( (char*)&(mainship->dPy), 8);
     fileout.write( (char*)&(mainship->dVx), 8);
     fileout.write( (char*)&(mainship->dVy), 8);
     fileout.write( (char*)&(mainship->dMf), 8);
     fileout.write( (char*)&(mainship->dMe), 8);
     fileout.write( (char*)&(mainship->HP), 4);
     fileout.write( (char*)&(mainship->JetI), 4);
     fileout.write( (char*)&(mainship->dAg), 8);
     fileout.write( (char*)&(mainship->dWg), 8);
     fileout.write( (char*)&(mainship->dAgw), 8);
     fileout.write( (char*)&(mainship->dAgc), 8);
     
     switch ( gamestate )
     {
            case CASE_GAMEtime:
                 fileout.seekp( 0x74, std::ios::beg );           //write the position of the destination
                 fileout.write( (char*)&DstPx, 8 );
                 fileout.write( (char*)&DstPy, 8 );
                 fileout.write( (char*)&Time, 4 );
                 break;
            case CASE_GAMEfight:
                 fileout.seekp( 0x74, std::ios::beg );
                 fileout.write( (char*)&(bossfort->dAo), 8);
                 fileout.write( (char*)&(bossfort->dR), 8);
                 fileout.write( (char*)&(bossfort->Fq), 4);
                 fileout.write( (char*)&(bossfort->AttachCate), 4);
                 tmp = SearchOrder ( bossfort->AttachCate, bossfort->Attach );
                 fileout.write( (char*)&tmp, 4 );
                 fileout.write( (char*)&(bossfort->nowTime), 4 );
                 break;
            case CASE_GAMEfree:
                 break;
     }
     int sdTmp;
     fileout.seekp ( 0x98, std::ios::beg );
     Node<Star>::cur = Node<Star>::beg;
     while ( Node<Star>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Star;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Star>::cur->obj->dM, 8 );
           fileout.write( (char*)&Node<Star>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<Star>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<Star>::cur->obj->dR, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Star>::cur = Node<Star>::cur->next;
     }
     Node<BiStar>::cur = Node<BiStar>::beg;
     while ( Node<BiStar>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_BiStar;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dM1, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dM2, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dPCx, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dPCy, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dR1, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dR2, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dAo, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dAg, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dD, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->dE, 8 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->direc, 4 );
           fileout.write( (char*)&Node<BiStar>::cur->obj->T, 4 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<BiStar>::cur = Node<BiStar>::cur->next;
     }
     
     Node<BlackHole>::cur = Node<BlackHole>::beg;
     while ( Node<BlackHole>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_BlackHole;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<BlackHole>::cur->obj->dM, 8 );
           fileout.write( (char*)&Node<BlackHole>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<BlackHole>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<BlackHole>::cur->obj->dR, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<BlackHole>::cur = Node<BlackHole>::cur->next;
     }
     
     Node<Planet>::cur = Node<Planet>::beg;
     while ( Node<Planet>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Planet;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Planet>::cur->obj->dM, 8 );
           fileout.write( (char*)&Node<Planet>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<Planet>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<Planet>::cur->obj->dVx, 8 );
           fileout.write( (char*)&Node<Planet>::cur->obj->dVy, 8 );
           fileout.write( (char*)&Node<Planet>::cur->obj->dR, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Planet>::cur = Node<Planet>::cur->next;
     }
     
     Node<SpaceShip>::cur = Node<SpaceShip>::beg->next;
     while ( Node<SpaceShip>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_SpaceShip;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dVx, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dVy, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dR, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dMf, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dMe, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->HP, 4 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->JetI, 4 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dAg, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dWg, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dAgw, 8 );
           fileout.write( (char*)&Node<SpaceShip>::cur->obj->dAgc, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<SpaceShip>::cur = Node<SpaceShip>::cur->next;
     }
     
     Node<Fort>::cur = Node<Fort>::beg;
     if ( gamestate == CASE_GAMEfight ) Node<Fort>::cur = Node<Fort>::cur->next;
     while ( Node<Fort>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Fort;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Fort>::cur->obj->dAo, 8 );
           fileout.write( (char*)&Node<Fort>::cur->obj->dR, 8 );
           fileout.write( (char*)&Node<Fort>::cur->obj->Fq, 4 );
           fileout.write( (char*)&Node<Fort>::cur->obj->AttachCate, 4 );
           tmp = SearchOrder ( Node<Fort>::cur->obj->AttachCate, Node<Fort>::cur->obj->Attach );
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Fort>::cur->obj->AimCate, 4 );
           tmp = SearchOrder ( Node<Fort>::cur->obj->AimCate, Node<Fort>::cur->obj->Aim );
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&(Node<Fort>::cur->obj->nowTime), 4 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Fort>::cur = Node<Fort>::cur->next;
     }
     
     Node<Missile>::cur = Node<Missile>::beg;
     while ( Node<Missile>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Missile;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Missile>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<Missile>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<Missile>::cur->obj->dVx, 8 );
           fileout.write( (char*)&Node<Missile>::cur->obj->dVy, 8 );
           fileout.write( (char*)&Node<Missile>::cur->obj->AimCate, 4 );
           tmp = SearchOrder ( Node<Missile>::cur->obj->AimCate, Node<Missile>::cur->obj->Aim );
           fileout.write( (char*)&tmp, 4 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Missile>::cur = Node<Missile>::cur->next;
     }
     
     Node<Bullet>::cur = Node<Bullet>::beg;
     while ( Node<Bullet>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Bullet;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Bullet>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<Bullet>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<Bullet>::cur->obj->dVx, 8 );
           fileout.write( (char*)&Node<Bullet>::cur->obj->dVy, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Bullet>::cur = Node<Bullet>::cur->next;
     }
     
     Node<FuelBox>::cur = Node<FuelBox>::beg;
     while ( Node<FuelBox>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_FuelBox;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<FuelBox>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<FuelBox>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<FuelBox>::cur->obj->dVx, 8 );
           fileout.write( (char*)&Node<FuelBox>::cur->obj->dVy, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<FuelBox>::cur = Node<FuelBox>::cur->next;
     }
     
     Node<ExplosiveBox>::cur = Node<ExplosiveBox>::beg;
     while ( Node<ExplosiveBox>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_ExplosiveBox;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<ExplosiveBox>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<ExplosiveBox>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<ExplosiveBox>::cur->obj->dVx, 8 );
           fileout.write( (char*)&Node<ExplosiveBox>::cur->obj->dVy, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<ExplosiveBox>::cur = Node<ExplosiveBox>::cur->next;
     }
     
     Node<Station>::cur = Node<Station>::beg;
     while ( Node<Station>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Station;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Station>::cur->obj->dPx, 8 );
           fileout.write( (char*)&Node<Station>::cur->obj->dPy, 8 );
           fileout.write( (char*)&Node<Station>::cur->obj->dR, 8 );
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Station>::cur = Node<Station>::cur->next;
     }

     Node<Gas>::cur = Node<Gas>::beg;
     while ( Node<Gas>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Gas;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Gas>::cur->obj->fPx, 8 );
           fileout.write( (char*)&Node<Gas>::cur->obj->fPy, 8 );
           fileout.write( (char*)&Node<Gas>::cur->obj->fVx, 8 );
           fileout.write( (char*)&Node<Gas>::cur->obj->fVy, 8 );
           fileout.write( (char*)&Node<Gas>::cur->obj->fAg, 8 );
           fileout.write( (char*)&Node<Gas>::cur->obj->fv, 8 );
           fileout.write( (char*)&Node<Gas>::cur->obj->nowTime, 4);
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Gas>::cur = Node<Gas>::cur->next;
     }
     
     Node<Explosion>::cur = Node<Explosion>::beg;
     while ( Node<Explosion>::cur )
     {
           sdTmp = fileout.tellp();
           tmp = CATE_Explosion;
           fileout.write( (char*)&tmp, 4 );
           fileout.write( (char*)&Node<Explosion>::cur->obj->fPx, 8 );
           fileout.write( (char*)&Node<Explosion>::cur->obj->fPy, 8 );
           fileout.write( (char*)&Node<Explosion>::cur->obj->fv, 8 );
           fileout.write( (char*)&Node<Explosion>::cur->obj->nowTime, 4);
           fileout.seekp( sdTmp + 92, std::ios::beg );
           Node<Explosion>::cur = Node<Explosion>::cur->next;
     }

     fileout.seekp( sdTmp + 91, std::ios::beg );
     tmp = 0;
     fileout.write( (char*)&tmp, 1 );
}

void ProceedGame ()
{
     glDisable(GL_TEXTURE_2D);
     double ar = double(glutGet(GLUT_SCREEN_WIDTH)) / double(glutGet(GLUT_SCREEN_HEIGHT));
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     gluPerspective(60,ar,VIEW_GAME_FRONT,VIEW_GAME_BACK);
     glMatrixMode(GL_MODELVIEW);
     glLoadIdentity();
     glLightfv ( GL_LIGHT0, GL_POSITION, light_position_menu1 );
     glTranslated(0,0,- SHOW_SHIP_DISTANCE);
     glRotated(1,0,0, slope_angle);
     glRotated(0,1,0, - mainship->dAgw);
     glTranslated( - mainship->dPx, 0, - mainship->dPy );
}

void DeleteAllObjs ()
{
     for ( ;Node<Star>::beg != 0; )
     {
         delete Node<Star>::beg;
     }
     for ( ;Node<BiStar>::beg != 0; )
     {
         delete Node<BiStar>::beg;
     }
     for ( ;Node<BlackHole>::beg != 0; )
     {
         delete Node<BlackHole>::beg;
     }
     for ( ;Node<Planet>::beg != 0; )
     {
         delete Node<Planet>::beg;
     }
     mainship = 0;
     for ( ;Node<SpaceShip>::beg != 0; )
     {
         delete Node<SpaceShip>::beg;
     }
     bossfort = 0;
     for ( ;Node<Fort>::beg != 0; )
     {
         delete Node<Fort>::beg;
     }
     for ( ;Node<Missile>::beg != 0; )
     {
         delete Node<Missile>::beg;
     }
     for ( ;Node<Bullet>::beg != 0; )
     {
         delete Node<Bullet>::beg;
     }
     for ( ;Node<FuelBox>::beg != 0; )
     {
         delete Node<FuelBox>::beg;
     }
     for ( ;Node<ExplosiveBox>::beg != 0; )
     {
         delete Node<ExplosiveBox>::beg;
     }
     for ( ;Node<Station>::beg != 0; )
     {
         delete Node<Station>::beg;
     }
     for ( ;Node<Gas>::beg != 0; )
     {
         delete Node<Gas>::beg;
     }
     for ( ;Node<Explosion>::beg != 0; )
     {
         delete Node<Explosion>::beg;
     }
}

int SearchOrder( int cate, void* obj )
{
    int numTmp = 0;
    switch (cate)
    {
           case CATE_Star:
           {
                Star* NewObj = (Star*)obj;
                Node<Star>::cur = Node<Star>::beg;
                while ( Node<Star>::cur )
                {
                      if ( Node<Star>::cur->obj == NewObj )
                         {
                             numTmp ++;
                             break;
                         }
                      Node<Star>::cur = Node<Star>::cur->next;
                }
                break;
           }
           case CATE_BiStar:
           {
                BiStar* NewObj = (BiStar*)obj;
                Node<BiStar>::cur = Node<BiStar>::beg;
                while ( Node<BiStar>::cur )
                {
                      if ( Node<BiStar>::cur->obj == NewObj )
                         {
                             numTmp ++;
                             break;
                         }
                      Node<BiStar>::cur = Node<BiStar>::cur->next;
                }
                break;
           }
           case CATE_Planet:
           {
                Planet* NewObj = (Planet*)obj;
                Node<Planet>::cur = Node<Planet>::beg;
                while ( Node<Planet>::cur )
                {
                      if ( Node<Planet>::cur->obj == NewObj )
                         {
                             numTmp ++;
                             break;
                         }
                      Node<Planet>::cur = Node<Planet>::cur->next;
                }
                break;
           }
           case CATE_SpaceShip:
           {
                SpaceShip* NewObj = (SpaceShip*)obj;
                Node<SpaceShip>::cur = Node<SpaceShip>::beg;
                while ( Node<SpaceShip>::cur )
                {
                      if ( Node<SpaceShip>::cur->obj == NewObj )
                         {
                             numTmp ++;
                             break;
                         }
                      Node<SpaceShip>::cur = Node<SpaceShip>::cur->next;
                }
                break;
           }
           case CATE_Fort:
           {
                Fort* NewObj = (Fort*)obj;
                Node<Fort>::cur = Node<Fort>::beg;
                while ( Node<Fort>::cur )
                {
                      if ( Node<Fort>::cur->obj == NewObj )
                         {
                             numTmp ++;
                             break;
                         }
                      Node<Fort>::cur = Node<Fort>::cur->next;
                }
                break;
           }
    }
    return numTmp;
}
