#############################################################################
# Makefile pour le projet de LIFAP4
#############################################################################
####### Compiler, tools and options

CXX           = g++
CXXFLAGS      = -pipe -g  -Wall -W  #-O2
LINK          = g++
LFLAGS        = 
INCPATH       = -I. -Isrc -Iextern/ncurses/include -I/usr/include/SDL2
CHK_DIR_EXISTS= test -d
MKDIR         = mkdir -p

ifeq ($(OS),Windows_NT) #.exe pour windows ? et \ vs /
MKDIR         = mkdir
DEL_FILE      = del /fsq
LIBS          =  -Lextern/ncurses/lib -lncursesw
LIBS_SDL      = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
else
DEL_FILE      = rm -rf
DEL_DIR       = rmdir
LIBS          = -Lextern/ncurses/lib -lncursesw
LIBS_SDL      = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

endif
####### Output directory

OBJECTS_DIR   = obj/

####### Files

SOURCES       = src/console/mainConsole.cpp \
		mainTestRegression.cpp \
		src/console/txtJeu.cpp \
		src/core/Carte.cpp \
		src/reseau/Client.cpp \
		src/core/Jeu.cpp \
		src/core/Joueur.cpp \
		src/core/PiocheDefausse.cpp \
		src/reseau/Serveur.cpp 
OBJECTSTESTREG = obj/mainTestRegression.o 
OBJECTCONSOLE = obj/mainConsole.o
OBJECTSDL = obj/mainSDL.o

OBJCOMP       = obj/txtJeu.o \
		obj/Carte.o \
		obj/Client.o \
		obj/Jeu.o \
		obj/Joueur.o \
		obj/PiocheDefausse.o \
		obj/Serveur.o

OBJCOMPSDL       = obj/sdlJeu.o \
		obj/Carte.o \
		obj/Client.o \
		obj/Jeu.o \
		obj/Joueur.o \
		obj/PiocheDefausse.o \
		obj/Serveur.o

DESTDIR       = bin/
TARGETCONSOLE = $(DESTDIR)/milleEtUneBorneConsole
TARGETTESTREGRESSION = $(DESTDIR)/testRegression
TARGETSDL = $(DESTDIR)/milleEtUneBorneSDL

first: all
####### Build rules

milleEtUneBorneConsole: mkdir   $(OBJECTCONSOLE)  $(OBJCOMP)
	#@test -d bin/ || mkdir -p bin/
	$(LINK) $(LFLAGS) -o $(TARGETCONSOLE) $(OBJECTCONSOLE) $(OBJCOMP) $(LIBS)

milleEtUneBorneTestRegression: mkdir  $(OBJECTSTESTREG)  $(OBJCOMP)
	#@test -d bin/ || mkdir -p bin/
	$(LINK) $(LFLAGS) -o $(TARGETTESTREGRESSION) $(OBJECTSTESTREG) $(OBJCOMP) $(LIBS)

milleEtUneBorneSDL: mkdir   $(OBJECTSDL)  $(OBJCOMPSDL)
	#@test -d bin/ || mkdir -p bin/
	$(LINK) $(LFLAGS) -o $(TARGETSDL) $(OBJECTSDL) $(OBJCOMPSDL) $(LIBS_SDL) 

all: milleEtUneBorneConsole milleEtUneBorneTestRegression milleEtUneBorneSDL


clean:  
	-$(DEL_FILE) $(OBJCOMP) $(OBJECTCONSOLE) $(TARGETTESTREGRESSION) $(OBJECTSDL) $(OBJCOMPSDL)

distclean: clean 
	-$(DEL_FILE) $(TARGETCONSOLE) $(TARGETCONSOLE) $(OBJECTSDL)

mkdir:
	$(CHK_DIR_EXISTS) $(OBJECTS_DIR) || $(MKDIR) $(OBJECTS_DIR)
	$(CHK_DIR_EXISTS) $(DESTDIR) || $(MKDIR) $(DESTDIR)
 

####### Compile

obj/mainSDL.o: src/sdl/mainSDL.cpp \
		src/sdl/sdlJeu.h \
		src/core/Jeu.h \
		src/core/PiocheDefausse.h \
		src/core/Carte.h \
		src/core/Joueur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/mainSDL.o src/sdl/mainSDL.cpp

obj/sdlJeu.o: src/sdl/sdlJeu.cpp \
		src/sdl/sdlJeu.h \
		src/core/Jeu.h \
		src/core/PiocheDefausse.h \
		src/core/Carte.h \
		src/core/Joueur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/sdlJeu.o src/sdl/sdlJeu.cpp

obj/mainConsole.o: src/console/mainConsole.cpp extern/ncurses/include/ncurses.h \
		extern/ncurses/include/ncurses_dll.h \
		extern/ncurses/include/unctrl.h \
		extern/ncurses/include/curses.h \
		src/console/txtJeu.h \
		src/core/Jeu.h \
		src/core/PiocheDefausse.h \
		src/core/Carte.h \
		src/core/Joueur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/mainConsole.o src/console/mainConsole.cpp

obj/mainTestRegression.o: src/mainTestRegression.cpp extern/ncurses/include/ncurses.h \
		extern/ncurses/include/ncurses_dll.h \
		extern/ncurses/include/unctrl.h \
		extern/ncurses/include/curses.h \
		src/console/txtJeu.h \
		src/core/Jeu.h \
		src/core/PiocheDefausse.h \
		src/core/Carte.h \
		src/core/Joueur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/mainTestRegression.o src/mainTestRegression.cpp

obj/txtJeu.o: src/console/txtJeu.cpp extern/ncurses/include/ncurses.h \
		extern/ncurses/include/ncurses_dll.h \
		extern/ncurses/include/unctrl.h \
		extern/ncurses/include/curses.h \
		src/console/txtJeu.h \
		src/core/Jeu.h \
		src/core/PiocheDefausse.h \
		src/core/Carte.h \
		src/core/Joueur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/txtJeu.o src/console/txtJeu.cpp

obj/Carte.o: src/core/Carte.cpp src/core/Carte.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/Carte.o src/core/Carte.cpp

obj/Client.o: src/reseau/Client.cpp src/reseau/Client.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/Client.o src/reseau/Client.cpp

obj/Jeu.o: src/core/Jeu.cpp src/core/Jeu.h \
		src/core/PiocheDefausse.h \
		src/core/Carte.h \
		src/core/Joueur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/Jeu.o src/core/Jeu.cpp

obj/Joueur.o: src/core/Joueur.cpp src/core/Joueur.h \
		src/core/Carte.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/Joueur.o src/core/Joueur.cpp

obj/PiocheDefausse.o: src/core/PiocheDefausse.cpp src/core/PiocheDefausse.h \
		src/core/Carte.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/PiocheDefausse.o src/core/PiocheDefausse.cpp

obj/Serveur.o: src/reseau/Serveur.cpp src/reseau/Serveur.h
	$(CXX) -c $(CXXFLAGS) $(INCPATH) -o obj/Serveur.o src/reseau/Serveur.cpp

