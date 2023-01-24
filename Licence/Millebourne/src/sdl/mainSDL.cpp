#include "sdlJeu.h"
#include <fstream>
#include <ostream>
 
int main(int argc, char *argv[]) {
    if(TTF_Init()==-1) 
        {
            printf("TTF_Init: %s\n", TTF_GetError());
            exit(2);
        }

    srand(unsigned(time(NULL)));

    TTF_Font *police = NULL;
    police = TTF_OpenFont("./data/police_bitmap.ttf", 65);
    SDL_Color couleurNoire = {0, 0, 0, 0xFF};



    SDL_Init(SDL_INIT_VIDEO);
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stdout, "Échec de l'initialisation de la SDL (%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    SDL_Window *mainWindow = SDL_CreateWindow("MilleBourne", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_RESIZABLE);
    if (mainWindow == NULL)
    {
        fprintf(stdout, "Échec de la création de la fenêtre (%s)\n", SDL_GetError());
        return EXIT_FAILURE;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Surface *fond = IMG_Load("./data/tapis.jpg");
    SDL_Texture *textFond = SDL_CreateTextureFromSurface(renderer, fond);;
    bool continuer = true;
    SDL_FreeSurface(fond);

    SDL_RenderCopy(renderer, textFond, NULL, NULL);
    std::string nomJoueur = presentation(renderer, textFond, police, couleurNoire);

    int nbrJoueur = combienJoueur(renderer, textFond, police, couleurNoire);

    std::string leNom = "Bienvenue sur le jeu MilleBourne " + nomJoueur;

    SDL_Surface *bienvenue = NULL;
    SDL_Texture *textBienvenue;
    SDL_Rect positionBienvenue;

    bienvenue = TTF_RenderText_Solid(police, leNom.c_str(), couleurNoire);
    textBienvenue = SDL_CreateTextureFromSurface(renderer, bienvenue);
    position(positionBienvenue, 270, 220, 750, 60);

    SDL_FreeSurface(bienvenue);

    SDL_Surface *optionP = NULL;
    SDL_Texture *textOptionP;
    SDL_Rect positionOptionP;

    optionP = TTF_RenderText_Solid(police, "- Appuyer sur p pour personnaliser le paquet", couleurNoire);
    textOptionP = SDL_CreateTextureFromSurface(renderer, optionP);
    position(positionOptionP, 280, 330, 700, 30);
    SDL_FreeSurface(optionP);
    
    SDL_Surface *optionE = NULL;
    SDL_Texture *textOptionE;
    SDL_Rect positionOptionE;

    optionE = TTF_RenderText_Solid(police, "- Appuyer sur e pour quitter", couleurNoire);
    textOptionE = SDL_CreateTextureFromSurface(renderer, optionE);
    position(positionOptionE, 280, 430, 450, 30);
    SDL_FreeSurface(optionE);
    
    SDL_Surface *optionN = NULL;
    SDL_Texture *textOptionN;
    SDL_Rect positionOptionN;

    optionN = TTF_RenderText_Solid(police, "- Appuyer sur n pour lancer une nouvelle partie", couleurNoire);
    textOptionN = SDL_CreateTextureFromSurface(renderer, optionN);
    position(positionOptionN, 280, 380, 700, 30);
    SDL_FreeSurface(optionN);
    
    int tab[NBREGLES] = {0, 0, 0};
    lesRegles(renderer, textFond, tab, police, couleurNoire);
    Jeu *jouer = nullptr;

    SDL_Event event;

    int lesCartes[19] = {1, 1, 1, 1, 5, 4, 3, 3, 3, 14, 6, 6, 6, 6, 10, 10, 10, 12, 4};

    while (continuer)
    {
        //SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            switch (event.type)
            {
            case SDL_QUIT:
                continuer = false;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == SDLK_p)
                {
                    sdlInit(renderer, textFond, lesCartes, police, couleurNoire);
                }
                if (event.key.keysym.sym == SDLK_e)
                {
                    continuer = false;
                }
                if (event.key.keysym.sym == SDLK_n)
                {
                    delete jouer;
                    
                    Jeu *jouer = new Jeu(nbrJoueur, nomJoueur, tab);
                    jouer->initialiserLaPartie(lesCartes);
                    
                    sdlBoucle(renderer, textFond, jouer, police, couleurNoire);
                }
            break;
            }

            SDL_RenderCopy(renderer, textFond, NULL, NULL);
            SDL_RenderCopy(renderer, textBienvenue, NULL, &positionBienvenue);
            SDL_RenderCopy(renderer, textOptionP, NULL, &positionOptionP);
            SDL_RenderCopy(renderer, textOptionE, NULL, &positionOptionE);
            SDL_RenderCopy(renderer, textOptionN, NULL, &positionOptionN);
            SDL_RenderPresent(renderer);
            SDL_RenderClear(renderer);
        }
    }

    SDL_DestroyTexture(textFond);
    SDL_DestroyTexture(textBienvenue);
    SDL_DestroyTexture(textOptionP);
    SDL_DestroyTexture(textOptionE);
    SDL_DestroyTexture(textOptionN);

    SDL_DestroyWindow(mainWindow);
    TTF_CloseFont(police);
    TTF_Quit();
    SDL_Quit();
    delete jouer;
    return 0;
}
