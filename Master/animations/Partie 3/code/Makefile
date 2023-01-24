#
# Pour installer GLFW et GLEW, tapez la commande suivante (ou équivalente suivant votre gestionnaire de paquet) :
# apt install libglfw3-dev libglew-dev
# 
# Pour compiler les librairies IMGUI et BOX2D, tapez la commande suivante :
# make install
#
# Ensuite, pour compiler le code du TP, tapez (à chaque modification de code) :
# make
#
# Une fois la compilation réussie, pour executer le programme, tapez :
# ./TP_CONTROL_BIPED.out
#
# Pour nettoyer le projet (fichiers objets du TP, executable), tapez :
# make clean
#
# Pour supprimer aussi les librairies, tapez :
# make superclean
#

.DEFAULT_GOAL := main

IMGUI_CPP_DIR = Box2D/src/imgui
IMGUI_O_DIR = Box2D/obj/Release/imgui
IMGUI_CPP = $(wildcard $(IMGUI_CPP_DIR)/*.cpp)
IMGUI_O = $(subst $(IMGUI_CPP_DIR), $(IMGUI_O_DIR), $(patsubst %.cpp, %.o, $(IMGUI_CPP)))
IMGUI_AR = Box2D/lib/linux/libIMGUI_d.a

BOX2D_CPP_DIR = Box2D/src/Box2D
BOX2D_O_DIR = Box2D/obj/Release/Box2D
BOX2D_CPP = $(shell find $(BOX2D_CPP_DIR) -iname *.cpp)
BOX2D_O = $(subst $(BOX2D_CPP_DIR), $(BOX2D_O_DIR), $(patsubst %.cpp, %.o, $(BOX2D_CPP)))
BOX2D_AR = Box2D/lib/linux/libBox2D_d.a

MAIN_CPP_DIR = src
MAIN_O_DIR = obj/Debug
MAIN_CPP = $(shell find src -iname *.cpp)
MAIN_O = $(subst $(MAIN_CPP_DIR), $(MAIN_O_DIR), $(patsubst %.cpp, %.o, $(MAIN_CPP)))

MAIN_LIBS = -LBox2D/lib/linux -lBox2D_d -lGLEW -lglfw -lIMGUI_d -lGL -lGLU

$(IMGUI_O_DIR):
	mkdir -p $@

$(IMGUI_O_DIR)/%.o: $(IMGUI_CPP_DIR)/%.cpp
	g++ -Wall -O2 -std=c++11 -DGLEW_STATIC -IBox2D/src -c $< -o $@

$(BOX2D_O_DIR):
	mkdir -p $@
	mkdir -p $(subst $(BOX2D_CPP_DIR), $(BOX2D_O_DIR), $(shell find $(BOX2D_CPP_DIR) -type d))

$(BOX2D_O_DIR)/%.o: $(BOX2D_CPP_DIR)/%.cpp
	g++ -Wall -O2 -std=c++11 -IBox2D/src -c $< -o $@

compile_imgui: $(IMGUI_O_DIR) $(IMGUI_O)
	rm -f $(IMGUI_AR) && ar -rs $(IMGUI_AR) $(IMGUI_O)

compile_box2d: $(BOX2D_O_DIR) $(BOX2D_O)
	rm -f $(BOX2D_AR) && ar -rs $(BOX2D_AR) $(BOX2D_O)

$(MAIN_O_DIR):
	mkdir -p $@
	mkdir -p $@/Framework

$(MAIN_O_DIR)/%.o: $(MAIN_CPP_DIR)/%.cpp $(MAIN_O_DIR)
	g++ -g -Wall -std=c++11 -DGLEW_STATIC -IBox2D/src -Isrc -c $< -o $@

main: $(MAIN_O)
	g++ $^ $(MAIN_LIBS) -o ./TP_CONTROL_BIPED.out

install: compile_imgui compile_box2d
	@echo "Compliation de IMGUI et BOX2D terminée."

clean:
	rm -rf obj/ *.out imgui.ini

superclean: clean
	rm -rf Box2D/obj/
