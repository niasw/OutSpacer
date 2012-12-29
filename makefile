OutSpacer: Outspacer.o GameProcedure.o MenuPaintings.o SpaceObjects.o
	g++ -o OutSpacer `pkg-config --cflags glu` Outspacer.o GameProcedure.o MenuPaintings.o SpaceObjects.o `pkg-config --libs glu` -lglut
Outspacer.o: Outspacer.cpp MenuItems.h SpaceObjects.h SpecialAffect.h MenuPaintings.h GameProcedure.h
	g++ -Wall -o Outspacer.o -c Outspacer.cpp
GameProcedure.o: GameProcedure.cpp GameProcedure.h MenuItems.h SpaceObjects.h SpecialAffect.h MenuPaintings.h
	g++ -Wall -o GameProcedure.o -c GameProcedure.cpp
MenuPaintings.o: MenuPaintings.cpp MenuItems.h SpaceObjects.h SpecialAffect.h MenuPaintings.h GameProcedure.h
	g++ -Wall -o MenuPaintings.o -c MenuPaintings.cpp
SpaceObjects.o: SpaceObjects.cpp MenuItems.h SpaceObjects.h SpecialAffect.h MenuPaintings.h GameProcedure.h
	g++ -Wall -o SpaceObjects.o -c SpaceObjects.cpp
clean:
	rm *.o
run:
	./OutSpacer
