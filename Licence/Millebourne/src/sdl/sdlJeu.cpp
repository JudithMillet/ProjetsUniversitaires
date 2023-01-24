#include "sdlJeu.h"

const int dimCartex = 50;
const int dimCartey = 68;

const int dimy = 720;
const int dimx = 1280;

void position(SDL_Rect &pos, int X, int Y, int W, int H)
{
    pos.x = X;
    pos.y = Y;
    pos.w = W;
    pos.h = H;
}

std::string presentation(SDL_Renderer *renderer, SDL_Texture *fond, TTF_Font *police, SDL_Color couleurNoire)
{
    //C'est le loop flag
    bool quit = true;

    SDL_Surface *Qui = NULL;
    SDL_Texture *textQui;
    SDL_Rect positionQui;

    Qui = TTF_RenderText_Solid(police, "Bonjour, inscrivez votre nom svp", couleurNoire);

    textQui = SDL_CreateTextureFromSurface(renderer, Qui);
    position(positionQui, 250, 300, 800, 30);

    SDL_Surface *Enter = NULL;
    SDL_Texture *textEnter;
    SDL_Rect positionEnter;

    Enter = TTF_RenderText_Solid(police, "(Appuyer sur Entree pour valider)", couleurNoire);

    textEnter = SDL_CreateTextureFromSurface(renderer, Enter);
    position(positionEnter, 280, 340, 700, 30);

    SDL_RenderCopy(renderer, fond, NULL, NULL);
    SDL_RenderCopy(renderer, textQui, NULL, &positionQui);
    SDL_RenderCopy(renderer, textEnter, NULL, &positionEnter);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    SDL_FreeSurface(Qui);
    SDL_FreeSurface(Enter);

    SDL_Rect positionNom;
    //On fait un nouvel Evenement pour le nom
    SDL_Event e;

    std::string text = "";
    SDL_StartTextInput();

    while (quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                quit = false;
            else if (e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN)
            {
                SDL_Surface *nom = NULL;
                SDL_Texture *textNom;

                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
                {
                    text = text.substr(0, text.length() - 1);
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && text.length() > 0)
                {
                    return text;
                    quit = false;
                }
                else if (e.type == SDL_TEXTINPUT)
                {
                    text += e.text.text;
                }
                nom = TTF_RenderText_Solid(police, text.c_str(), couleurNoire);
                textNom = SDL_CreateTextureFromSurface(renderer, nom);
                position(positionNom, 515, 400, 200, 40);

                SDL_RenderCopy(renderer, fond, NULL, NULL);
                SDL_RenderCopy(renderer, textQui, NULL, &positionQui);
                SDL_RenderCopy(renderer, textEnter, NULL, &positionEnter);
                SDL_RenderCopy(renderer, textNom, NULL, &positionNom);
                SDL_RenderPresent(renderer);
                SDL_RenderClear(renderer);

                SDL_DestroyTexture(textNom);
                SDL_FreeSurface(nom);
            }
        }
    }

    //SDL_RenderClear(renderer);
    SDL_StopTextInput();

    SDL_DestroyTexture(textQui);
    SDL_DestroyTexture(textEnter);
    return text;
}

int combienJoueur(SDL_Renderer *renderer, SDL_Texture *fond, TTF_Font *police, SDL_Color couleurNoire)
{
    bool quit = true;

    SDL_Surface *combien = NULL;
    SDL_Texture *textCombien;
    SDL_Rect positionCombien;

    combien = TTF_RenderText_Solid(police, "Ecrivez le nombre de joueur svp, entre 2 et 4", couleurNoire);

    textCombien = SDL_CreateTextureFromSurface(renderer, combien);
    position(positionCombien, 230, 300, 850, 30);

    SDL_Surface *Enter = NULL;
    SDL_Texture *textEnter;
    SDL_Rect positionEnter;

    Enter = TTF_RenderText_Solid(police, "(Appuyer sur Entree pour valider)", couleurNoire);

    textEnter = SDL_CreateTextureFromSurface(renderer, Enter);
    position(positionEnter, 280, 340, 700, 30);

    SDL_RenderCopy(renderer, fond, NULL, NULL);
    SDL_RenderCopy(renderer, textCombien, NULL, &positionCombien);
    SDL_RenderCopy(renderer, textEnter, NULL, &positionEnter);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    SDL_FreeSurface(combien);
    SDL_FreeSurface(Enter);

    SDL_Rect positionNombre;
    //On itilise un nouvel évènement
    SDL_Event e;

    std::string text = "";
    SDL_StartTextInput();

    while (quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
                quit = false;
            else if (e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN)
            {
                SDL_Surface *nombre = NULL;
                SDL_Texture *textNombre;

                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
                {
                    text = text.substr(0, text.length() - 1);
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && text.length() > 0)
                {
                    if ((std::stoi(text) <= 4) && std::stoi(text) >= 2)
                    {
                        return std::stoi(text);
                        quit = false;
                    }
                }
                else if (e.type == SDL_TEXTINPUT)
                {
                    text += e.text.text;
                }
                nombre = TTF_RenderText_Solid(police, text.c_str(), couleurNoire);
                textNombre = SDL_CreateTextureFromSurface(renderer, nombre);
                position(positionNombre, 610, 400, 50, 40);

                SDL_RenderCopy(renderer, fond, NULL, NULL);
                SDL_RenderCopy(renderer, textCombien, NULL, &positionCombien);
                SDL_RenderCopy(renderer, textEnter, NULL, &positionEnter);
                SDL_RenderCopy(renderer, textNombre, NULL, &positionNombre);
                SDL_RenderPresent(renderer);
                SDL_RenderClear(renderer);

                SDL_DestroyTexture(textNombre);
                SDL_FreeSurface(nombre);
            }
        }
    }

    SDL_StopTextInput();

    SDL_DestroyTexture(textCombien);
    SDL_DestroyTexture(textEnter);
    return std::stoi(text);
}

void lesRegles(SDL_Renderer *renderer, SDL_Texture *fond, int tab[], TTF_Font *police, SDL_Color couleurNoire)
{
    bool quit = true;

    SDL_Surface *quel = NULL;
    SDL_Texture *textQuel;
    SDL_Rect positionQuel;

    quel = TTF_RenderText_Solid(police, "Voulez-vous utiliser les regles par defaut ? [o/n]", couleurNoire);

    textQuel = SDL_CreateTextureFromSurface(renderer, quel);
    position(positionQuel, 230, 300, 850, 30);

    SDL_RenderCopy(renderer, fond, NULL, NULL);
    SDL_RenderCopy(renderer, textQuel, NULL, &positionQuel);
    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    SDL_FreeSurface(quel);
    SDL_DestroyTexture(textQuel);

    SDL_Event e;

    while (quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            switch (e.type)
            {
            case SDL_QUIT:
                quit = false;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_n)
                {
                    SDL_Surface *defausse = NULL;
                    SDL_Texture *textDefausse;
                    SDL_Rect positionDefausse;

                    defausse = TTF_RenderText_Solid(police, "Voulez-vous piocher dans la defausse ? [o/n]", couleurNoire);

                    textDefausse = SDL_CreateTextureFromSurface(renderer, defausse);
                    position(positionDefausse, 230, 300, 850, 30);

                    SDL_RenderCopy(renderer, fond, NULL, NULL);
                    SDL_RenderCopy(renderer, textDefausse, NULL, &positionDefausse);
                    SDL_RenderPresent(renderer);
                    SDL_RenderClear(renderer);

                    SDL_FreeSurface(defausse);
                    SDL_DestroyTexture(textDefausse);

                    bool quit2 = true;

                    SDL_Event ev;

                    while (quit2)
                    {
                        while (SDL_PollEvent(&ev) != 0)
                        {
                            switch (ev.type)
                            {
                            case SDL_QUIT:
                                quit2 = false;
                                break;
                            case SDL_KEYDOWN:
                                if (ev.key.keysym.sym == SDLK_o)
                                {
                                    tab[0] = 1;
                                    quit2 = false;
                                }
                                else if (ev.key.keysym.sym == SDLK_n)
                                {
                                    quit2 = false;
                                }
                                break;
                            }
                        }
                    }
                    SDL_Surface *feu = NULL;
                    SDL_Texture *textFeu;
                    SDL_Rect positionFeu;

                    feu = TTF_RenderText_Solid(police, "Ne pas poser un feu vert apres une carte parade ? [o/n]", couleurNoire);

                    textFeu = SDL_CreateTextureFromSurface(renderer, feu);
                    position(positionFeu, 230, 300, 850, 30);

                    SDL_RenderCopy(renderer, fond, NULL, NULL);
                    SDL_RenderCopy(renderer, textFeu, NULL, &positionFeu);
                    SDL_RenderPresent(renderer);
                    SDL_RenderClear(renderer);

                    SDL_FreeSurface(feu);
                    SDL_DestroyTexture(textFeu);

                    quit2 = true;

                    while (quit2)
                    {
                        while (SDL_PollEvent(&ev) != 0)
                        {
                            switch (ev.type)
                            {
                            case SDL_QUIT:
                                quit2 = false;
                                break;
                            case SDL_KEYDOWN:
                                if (ev.key.keysym.sym == SDLK_o)
                                {
                                    tab[1] = 1;
                                    quit = false;
                                    quit2 = false;
                                }
                                else if (ev.key.keysym.sym == SDLK_n)
                                {
                                    quit = false;
                                    quit2 = false;
                                }
                                break;
                            }
                        }
                    }
                }
                else if (e.key.keysym.sym == SDLK_o)
                {
                    quit = false;
                }
                break;
            }
        }
    }
}

void sdlInit(SDL_Renderer *renderer, SDL_Texture *fond, int lesCartes[], TTF_Font *police, SDL_Color couleurNoire)
{
    std::string c;
    //int tab[19] = {1, 1, 1, 1, 5, 4, 3, 3, 3, 14, 6, 6, 6, 6, 10, 10, 10, 12, 4};

    SDL_Event events; //Ceci est une union de type SDL_Event
    bool valide = true;
    bool valide2 = true;

    while (valide)
    {
        while (SDL_PollEvent(&events) != 0)
        {
            switch (events.type)
            {
            case SDL_QUIT:
                valide = false;
                break;
            default:
                for (int i = 1; i <= 19; i++)
                {
                    SDL_Surface *CarteRecup = NULL;
                    SDL_Texture *textCarteRecup;
                    CarteRecup = TTF_RenderText_Solid(police, ("Combien de " + Carte(i).getNom() + " voulez-vous ?").c_str(), couleurNoire);

                    SDL_Rect positionCarteRecup;
                    position(positionCarteRecup, 280, 315, 700, 20);
                    textCarteRecup = SDL_CreateTextureFromSurface(renderer, CarteRecup);

                    SDL_Surface *Enter = NULL;
                    SDL_Texture *textEnter;
                    SDL_Rect positionEnter;

                    Enter = TTF_RenderText_Solid(police, "(Appuyer sur Entree pour valider)", couleurNoire);

                    textEnter = SDL_CreateTextureFromSurface(renderer, Enter);
                    position(positionEnter, 280, 340, 700, 30);

                    SDL_RenderCopy(renderer, fond, NULL, NULL);
                    SDL_RenderCopy(renderer, textCarteRecup, NULL, &positionCarteRecup);
                    SDL_RenderCopy(renderer, textEnter, NULL, &positionEnter);

                    SDL_RenderPresent(renderer);
                    SDL_RenderClear(renderer);

                    SDL_Event e;

                    SDL_Rect positionNombre;
                    std::string text = "";
                    SDL_StartTextInput();

                    valide2 = true;

                    while (valide2)
                    {
                        while (SDL_PollEvent(&e) != 0)
                        {
                            if (e.type == SDL_QUIT)
                                valide2 = false;
                            else if (e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN)
                            {
                                SDL_Surface *nombre = NULL;
                                SDL_Texture *textNombre;

                                if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_BACKSPACE && text.length() > 0)
                                {
                                    text = text.substr(0, text.length() - 1);
                                }
                                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN && text.length() > 0)
                                {
                                    if ((std::stoi(text) <= 25) && std::stoi(text) >= 0)
                                    {
                                        lesCartes[i - 1] = std::stoi(text);
                                        valide2 = false;
                                    }
                                }
                                else if (e.type == SDL_TEXTINPUT)
                                {
                                    text += e.text.text;
                                }
                                nombre = TTF_RenderText_Solid(police, text.c_str(), couleurNoire);
                                textNombre = SDL_CreateTextureFromSurface(renderer, nombre);
                                position(positionNombre, 600, 400, 30, 30);

                                SDL_RenderCopy(renderer, fond, NULL, NULL);
                                SDL_RenderCopy(renderer, textCarteRecup, NULL, &positionCarteRecup);
                                SDL_RenderCopy(renderer, textEnter, NULL, &positionEnter);
                                SDL_RenderCopy(renderer, textNombre, NULL, &positionNombre);
                                SDL_RenderPresent(renderer);
                                SDL_RenderClear(renderer);

                                SDL_DestroyTexture(textNombre);
                                SDL_FreeSurface(nombre);
                            }
                        }
                    }

                    SDL_StopTextInput();

                    SDL_DestroyTexture(textEnter);
                    SDL_FreeSurface(Enter);

                    SDL_FreeSurface(CarteRecup);
                    SDL_DestroyTexture(textCarteRecup);
                }
                valide = false;
                //break;
            }
        }
    }
}

void sdlBoucle(SDL_Renderer *renderer, SDL_Texture *fond, Jeu *jeu, TTF_Font *police, SDL_Color couleurNoire)
{
    //std::cout << "Tu viens de rentrer dans sdlBoucle" << std::endl;
    std::vector<int> classement;
    bool ok = true;
    bool stop = false;
    bool piocher = false;
    int carte, cible, ret;
    int posCarteMainx = 350 + dimCartex + 10;
    int posCarteMainy = 630;

    SDL_Event event;
    SDL_PollEvent(&event);

    do
    {
        //std::cout << "tu es dans la boucle principal" << std::endl;
        if (jeu->getAQuiLeTour() == 0)
        {
            if ((jeu->getTableauRegle().at(0) == 1) && (jeu->getPD().tailleDefausse() != 0))
            {
                sdlAffichage(renderer, fond, jeu, -1, police, couleurNoire);
                piocher = false;

                do
                {

                    //std::cout << "choisir un endroit ou piocher" << std::endl;
                    SDL_WaitEvent(&event);
                    // std::cout << "piocher est à "<<piocher << std::endl;

                    if (event.type == SDL_QUIT)
                    {
                        //std::cout << "Tu es dans le quit de piocher" << std::endl;
                        ok = false;
                        ret = 0;
                        stop = true;
                        piocher = true;
                        sdlAffichageFin(renderer, fond, jeu, police, couleurNoire, stop);
                    }

                    if ((event.type == SDL_MOUSEBUTTONUP) && (event.button.y <= dimy / 2 - dimCartey / 2 + dimCartey) && (event.button.y > dimy / 2 - dimCartey / 2))
                    {
                        if ((event.button.x >= dimx / 2 - dimCartex - 20) && (event.button.x < (dimx / 2 - dimCartex - 20) + dimCartex))
                        {
                            //std::cout << "Je pioche dans la pioche " << std::endl;
                            jeu->piocherUneCarte(0, false);
                            piocher = true;
                        }

                        if ((event.button.x >= (dimx / 2 - dimCartex - 20) + 30) && (event.button.x >= (dimx / 2 - dimCartex - 20) + dimCartex + 30))
                        {
                            //std::cout << "Je pioche dans la defausse " << std::endl;
                            jeu->piocherUneCarte(0, true);
                            piocher = true;
                        }
                    }

                } while (piocher == false);
            }

            else
            {
                //std::cout << "Je pioche la carte " << jeu->getPD().getPioche().top().getNomDisplay() << std::endl;
                ret = jeu->piocherUneCarte(0, false);
            }

            sdlAffichage(renderer, fond, jeu, -1, police, couleurNoire);
            if (!stop)
                do
                {

                    carte = -1;
                    if (!stop)
                        do //Choisir la carte à jouer
                        {
                            SDL_WaitEvent(&event);

                            if (event.type == SDL_QUIT)
                            {
                                //std::cout << "Tu es dans le quit de carte" << std::endl;
                                ok = false;
                                ret = 0;
                                stop = true;
                                carte = 500;
                                sdlAffichageFin(renderer, fond, jeu, police, couleurNoire, stop);
                            }

                            if ((event.type == SDL_MOUSEBUTTONUP) && (event.button.y >= posCarteMainy) && (event.button.y <= posCarteMainy + dimCartey))
                            {
                                if ((event.button.x >= posCarteMainx) && (event.button.x <= posCarteMainx + dimCartex))
                                {
                                    sdlAffichage(renderer, fond, jeu, 0, police, couleurNoire);
                                    carte = 0;
                                }

                                if ((event.button.x >= posCarteMainx + dimCartex + 10) && (event.button.x <= posCarteMainx + 2 * dimCartex + 10))
                                {
                                    sdlAffichage(renderer, fond, jeu, 1, police, couleurNoire);
                                    carte = 1;
                                }

                                if ((event.button.x >= posCarteMainx + 2 * dimCartex + 2 * 10) && (event.button.x <= posCarteMainx + 3 * dimCartex + 2 * 10))
                                {
                                    sdlAffichage(renderer, fond, jeu, 2, police, couleurNoire);
                                    carte = 2;
                                }

                                if ((event.button.x >= posCarteMainx + 3 * dimCartex + 3 * 10) && (event.button.x <= posCarteMainx + 4 * dimCartex + 3 * 10))
                                {
                                    sdlAffichage(renderer, fond, jeu, 3, police, couleurNoire);
                                    carte = 3;
                                }

                                if ((event.button.x >= posCarteMainx + 4 * dimCartex + 4 * 10) && (event.button.x <= posCarteMainx + 5 * dimCartex + 4 * 10))
                                {
                                    sdlAffichage(renderer, fond, jeu, 4, police, couleurNoire);
                                    carte = 4;
                                }

                                if ((event.button.x >= posCarteMainx + 5 * dimCartex + 5 * 10) && (event.button.x <= posCarteMainx + 6 * dimCartex + 5 * 10))
                                {
                                    sdlAffichage(renderer, fond, jeu, 5, police, couleurNoire);
                                    carte = 5;
                                }

                                if ((event.button.x >= posCarteMainx + 6 * dimCartex + 6 * 10) && (event.button.x <= posCarteMainx + 7 * dimCartex + 6 * 10))
                                {
                                    sdlAffichage(renderer, fond, jeu, 6, police, couleurNoire);
                                    carte = 6;
                                }
                            }

                            // else if (event.type=SDL_QUIT)
                            //     ok = ok;

                        } while (carte == -1);

                    cible = -2;
                    if (!stop)
                        do // choisir l'adversaire à qui poser une carte
                        {
                            SDL_WaitEvent(&event);

                            if (event.type == SDL_QUIT)
                            {
                                //std::cout << "Tu es dans le quit de cible" << std::endl;
                                ok = false;
                                ret = 0;
                                stop = true;
                                cible = 80;
                                sdlAffichageFin(renderer, fond, jeu, police, couleurNoire, stop);
                            }

                            if (event.type == SDL_MOUSEBUTTONUP)
                            {
                                if ((event.button.y >= 540) && (event.button.x >= 340) && (event.button.x <= 900) && (event.button.y < 960))
                                {
                                    cible = 0;
                                }

                                if ((event.button.y < 180) && (event.button.x >= 340) && (event.button.x <= 900) && (event.button.y < 960))
                                {
                                    cible = 1;
                                }

                                if ((event.button.y >= 180) && (event.button.y < 540) && (event.button.x >= 1000) && (jeu->getnbrJoueurs() >= 3))
                                {
                                    cible = 2;
                                }

                                if ((event.button.y >= 180) && (event.button.y < 540) && (event.button.x >= 0) && (event.button.x < 290) && (jeu->getnbrJoueurs() == 4))
                                {
                                    cible = 3;
                                }

                                if ((event.button.y > 600) && (event.button.x > 1000))
                                    cible = -1;
                            }

                        } while (cible == -2);

                    if (!stop)
                    {
                        ret = jeu->poserUneCarte(carte, jeu->getAQuiLeTour(), cible);
                    }
                } while (ret != 0);

            sdlAffichage(renderer, fond, jeu, -1, police, couleurNoire);
        }
        else
        {
            jeu->jouerBot(jeu->getAQuiLeTour());
        }

        classement = jeu->gagnant(false);
        if (!classement.empty())
        {
            sdlAffichageFin(renderer, fond, jeu, police, couleurNoire, false);
            ok = !ok;
        }
    } while (ok);
}

void sdlAffichageFin(SDL_Renderer *renderer, SDL_Texture *fond, Jeu *jeu, TTF_Font *police, SDL_Color couleurNoire, bool b)
{
    SDL_Surface *gagnant = NULL;
    SDL_Texture *textGagnant;
    gagnant = TTF_RenderText_Solid(police, jeu->jeuAffichageClassement(b).c_str(), couleurNoire);
    textGagnant = SDL_CreateTextureFromSurface(renderer, gagnant);

    SDL_Rect positionGagnant;
    position(positionGagnant, 280, 350, 650, 40);

    SDL_RenderCopy(renderer, fond, NULL, NULL);
    SDL_RenderCopy(renderer, textGagnant, NULL, &positionGagnant);

    SDL_RenderPresent(renderer);
    SDL_RenderClear(renderer);

    SDL_Delay(3000);

    SDL_FreeSurface(gagnant);
    SDL_DestroyTexture(textGagnant);
}

void sdlAffichage(SDL_Renderer *renderer, SDL_Texture *fond, Jeu *jouer, int emplacement, TTF_Font *police, SDL_Color couleurNoire)

{

    SDL_Surface *carte_dos = IMG_Load("./data/carteDos.jpg");
    SDL_Texture *dosTexture;
    dosTexture = SDL_CreateTextureFromSurface(renderer, carte_dos);

    SDL_Rect posPioche;
    SDL_Rect posDefausseCarte;
    SDL_Rect positionCarteMain;
    SDL_Rect positionCarteAttaque;
    SDL_Rect positionCarteVitesse;
    SDL_Rect positionCarteBorne;
    SDL_Rect positionCarteBotte;
    SDL_Rect posScore;

    position(posPioche, (dimx / 2 - dimCartex - 20), (dimy / 2 - dimCartey / 2), dimCartex, dimCartey);

    posDefausseCarte = posPioche;
    posDefausseCarte.x += posDefausseCarte.w + 30;

    SDL_Rect posDefausse;
    position(posDefausse, 900, 720, 380, 380);

    positionCarteAttaque.w = dimCartex;
    positionCarteAttaque.h = dimCartey;

    positionCarteBorne.w = dimCartex;
    positionCarteBorne.h = dimCartey;

    positionCarteVitesse.w = dimCartex;
    positionCarteVitesse.h = dimCartey;

    positionCarteBotte.w = dimCartex;
    positionCarteBotte.h = dimCartey;

    SDL_Surface *hitboxDefausse = NULL;
    SDL_Texture *textHitboxDefausse = NULL;
    SDL_Rect positionHitboxDefausse;
    
    hitboxDefausse = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0);
    SDL_FillRect(hitboxDefausse, NULL, SDL_MapRGB(hitboxDefausse->format, 255, 255, 255));
    
    textHitboxDefausse = SDL_CreateTextureFromSurface(renderer, hitboxDefausse);
    position(positionHitboxDefausse, 1000, 600, 500, 400);
    
    SDL_Surface *nomDefausse = NULL;
    SDL_Texture *textNomDefausse;
    SDL_Rect positionNomDefausse;
    
    nomDefausse = TTF_RenderText_Solid(police, "Defausse", couleurNoire);
    textNomDefausse = SDL_CreateTextureFromSurface(renderer, nomDefausse);
    position(positionNomDefausse, 1075, 650, 100, 20);
    
    SDL_RenderCopy(renderer, fond, NULL, NULL);
    SDL_RenderCopy(renderer, textHitboxDefausse, NULL, &positionHitboxDefausse);
    SDL_RenderCopy(renderer, textNomDefausse, NULL, &positionNomDefausse);
    
    SDL_FreeSurface(hitboxDefausse);
    SDL_DestroyTexture(textHitboxDefausse);
    SDL_FreeSurface(nomDefausse);
    SDL_DestroyTexture(textNomDefausse);

    if (jouer->getPD().taillePioche() != 0)
        SDL_RenderCopy(renderer, dosTexture, NULL, &posPioche);

    if (jouer->getPD().tailleDefausse() != 0)
    {
        std::string chemin = "./data/" + std::to_string(jouer->getPD().getDefausse().top().getValeur()) + ".jpg";
        SDL_Surface *carte = IMG_Load(chemin.c_str());
        SDL_Texture *carteTexture;
        carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

        SDL_RenderCopy(renderer, carteTexture, NULL, &posDefausseCarte);
        SDL_FreeSurface(carte);
        SDL_DestroyTexture(carteTexture);
    }

    for (int i = 0; i < jouer->getnbrJoueurs(); i++)
    {
        if (i == 0)
        {

            position(positionCarteMain, 360, 630, dimCartex, dimCartey);

            positionCarteAttaque = positionCarteMain;
            positionCarteAttaque.y = positionCarteMain.y - dimCartey - 30;
            positionCarteAttaque.x += positionCarteAttaque.w + 10;

            positionCarteVitesse = positionCarteAttaque;
            positionCarteVitesse.x += positionCarteVitesse.w + 10;

            positionCarteBorne = positionCarteAttaque;
            positionCarteBorne.x += 2 * positionCarteBorne.w + 2 * 10;

            positionCarteBotte = positionCarteAttaque;
            positionCarteBotte.x -= (dimCartex + 10);
            positionCarteBotte.y -= dimCartey + 10;

            SDL_Surface *hitbox = NULL;
            SDL_Texture *textHitbox = NULL;
            SDL_Rect positionHitbox;
            
            hitbox = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0x000000ff);
            SDL_FillRect(hitbox, NULL, SDL_MapRGB(hitbox->format, 0, 255, 0));
            
            textHitbox = SDL_CreateTextureFromSurface(renderer, hitbox);
            position(positionHitbox, 340, 540, 560, 440);
            
            SDL_RenderCopy(renderer, textHitbox, NULL, &positionHitbox);
            
            SDL_FreeSurface(hitbox);
            SDL_DestroyTexture(textHitbox);
            
            SDL_Surface *score = NULL;
            SDL_Texture *scoreTexture;
            position(posScore, 280, 670, 100, 20);
            score = TTF_RenderText_Solid(police, ("Score: " + std::to_string(jouer->getJoueurs().at(i).getScore())).c_str(), couleurNoire);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
            SDL_RenderCopy(renderer, scoreTexture, NULL, &posScore);
            SDL_FreeSurface(score);
            SDL_DestroyTexture(scoreTexture);

            for (int j = 0; j < 7; j++)
            {
                if (jouer->getJoueurs().at(i).getCarteEnMain().at(j).getValeur() != 0)
                {
                    if (j == emplacement)
                    {
                        SDL_Surface *bleu = IMG_Load("./data/bleu.png");
                        SDL_Texture *textBleu = SDL_CreateTextureFromSurface(renderer, bleu);
                        SDL_Rect positionBleu;
                        position(positionBleu, 360, 623, dimCartex + 15, dimCartey + 15);

                        positionCarteMain.x += dimCartex + 10;
                        positionBleu.x = positionCarteMain.x - 8;

                        std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCarteEnMain().at(j).getValeur()) + ".jpg";
                        SDL_Surface *carte = IMG_Load(chemin.c_str());
                        SDL_Texture *carteTexture;
                        carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                        SDL_RenderCopy(renderer, textBleu, NULL, &positionBleu);
                        SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteMain);

                        SDL_FreeSurface(bleu);
                        SDL_DestroyTexture(textBleu);
                        SDL_FreeSurface(carte);
                        SDL_DestroyTexture(carteTexture);
                    }
                    //std::cout << "J'affiche la carte " << j << " devant toi" << std::endl;
                    else
                    {
                        positionCarteMain.x += dimCartex + 10;

                        std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCarteEnMain().at(j).getValeur()) + ".jpg";
                        SDL_Surface *carte = IMG_Load(chemin.c_str());
                        SDL_Texture *carteTexture;
                        carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                        SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteMain);
                        SDL_FreeSurface(carte);
                        SDL_DestroyTexture(carteTexture);
                    }
                }
            }

            for (unsigned int j = 1; j <= jouer->getJoueurs().at(i).getCartePoseeBotte().size(); j++)
            {
                positionCarteBotte.x += dimCartex + 10;

                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBotte().at(j - 1).getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBotte);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteAttaque);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteVitesse);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBorne);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }
        }
        //break;

        if (i == 1)
        //case 1:
        {

            position(positionCarteMain, 360, -40, dimCartex, dimCartey);

            positionCarteBotte = positionCarteMain;
            positionCarteBotte.y += dimCartey + 30;

            positionCarteAttaque = positionCarteBotte;
            positionCarteAttaque.y = positionCarteBotte.y + dimCartey + 10;
            positionCarteAttaque.x = positionCarteAttaque.w + positionCarteMain.x + 10;

            positionCarteVitesse = positionCarteAttaque;
            positionCarteVitesse.x += positionCarteVitesse.w + 10;

            positionCarteBorne = positionCarteAttaque;
            positionCarteBorne.x += 2 * positionCarteBorne.w + 2 * 10;

            SDL_Surface *hitbox = NULL;
            SDL_Texture *textHitbox = NULL;
            SDL_Rect positionHitbox;
            
            hitbox = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0x000000ff);
            SDL_FillRect(hitbox, NULL, SDL_MapRGB(hitbox->format, 255, 0, 0));
            
            textHitbox = SDL_CreateTextureFromSurface(renderer, hitbox);
            position(positionHitbox, 340, 0, 560, 180);
            
            SDL_RenderCopy(renderer, textHitbox, NULL, &positionHitbox);
            
            SDL_FreeSurface(hitbox);
            SDL_DestroyTexture(textHitbox);
            
            SDL_Surface *score = NULL;
            SDL_Texture *scoreTexture;
            position(posScore, 280, 7, 100, 20);
            score = TTF_RenderText_Solid(police, ("Score: " + std::to_string(jouer->getJoueurs().at(i).getScore())).c_str(), couleurNoire);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
            SDL_RenderCopy(renderer, scoreTexture, NULL, &posScore);
            SDL_FreeSurface(score);
            SDL_DestroyTexture(scoreTexture);

            for (int j = 0; j < 6; j++)
            {
                if (jouer->getJoueurs().at(i).getCarteEnMain().at(j).getValeur() != -1)
                {
                    positionCarteMain.x += dimCartex + 10;
                    SDL_RenderCopy(renderer, dosTexture, NULL, &positionCarteMain);
                }
            }

            for (unsigned int j = 1; j < +jouer->getJoueurs().at(i).getCartePoseeBotte().size(); j++)
            {
                positionCarteBotte.x += dimCartex + 10;

                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBotte().at(j - 1).getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBotte);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteAttaque);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteVitesse);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBorne);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }
        }

        if (i == 2)
        //case 1:
        {

            position(positionCarteMain, (dimx - 30), 130, dimCartey, dimCartex);

            positionCarteAttaque.x = dimx - positionCarteAttaque.w - 250;
            positionCarteAttaque.y = 320;

            positionCarteVitesse = positionCarteAttaque;
            positionCarteBorne = positionCarteAttaque;

            positionCarteVitesse.x += positionCarteVitesse.w + 10;
            positionCarteBorne.x += 2 * positionCarteBorne.w + 2 * 10;

            positionCarteBotte = positionCarteAttaque;
            positionCarteBotte.x -= (dimCartex + 10);
            positionCarteBotte.y -= dimCartey + 10;

            SDL_Surface *hitbox = NULL;
            SDL_Texture *textHitbox = NULL;
            SDL_Rect positionHitbox;
            
            hitbox = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0x000000ff);
            SDL_FillRect(hitbox, NULL, SDL_MapRGB(hitbox->format, 255, 0, 0));
            
            textHitbox = SDL_CreateTextureFromSurface(renderer, hitbox);
            position(positionHitbox, 1000, 180, 580, 360);
            
            SDL_RenderCopy(renderer, textHitbox, NULL, &positionHitbox);
            
            SDL_FreeSurface(hitbox);
            SDL_DestroyTexture(textHitbox);
            
            SDL_Surface *score = NULL;
            SDL_Texture *scoreTexture;
            position(posScore, dimx - 105, 550, 100, 20);
            score = TTF_RenderText_Solid(police, ("Score: " + std::to_string(jouer->getJoueurs().at(i).getScore())).c_str(), couleurNoire);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
            SDL_RenderCopy(renderer, scoreTexture, NULL, &posScore);
            SDL_FreeSurface(score);
            SDL_DestroyTexture(scoreTexture);

            for (int j = 0; j < 6; j++)
            {
                if (jouer->getJoueurs().at(i).getCarteEnMain().at(j).getValeur() != -1)
                {
                    positionCarteMain.y += positionCarteMain.h + 10;
                    SDL_RenderCopy(renderer, dosTexture, NULL, &positionCarteMain);
                }
            }
            for (unsigned int j = 1; j <= jouer->getJoueurs().at(i).getCartePoseeBotte().size(); j++)
            {
                positionCarteBotte.x += dimCartex + 10;

                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBotte().at(j - 1).getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBotte);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }
            if (jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteAttaque);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteVitesse);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBorne);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }
        }

        if (i == 3)
        //case 1:
        {

            position(positionCarteMain, -30, 130, dimCartey, dimCartex);

            positionCarteAttaque.x = positionCarteAttaque.w;
            positionCarteAttaque.y = 320;

            positionCarteVitesse = positionCarteAttaque;
            positionCarteBorne = positionCarteAttaque;

            positionCarteVitesse.x += positionCarteVitesse.w + 10;
            positionCarteBorne.x += 2 * positionCarteBorne.w + 2 * 10;

            positionCarteBotte = positionCarteAttaque;
            positionCarteBotte.x -= (dimCartex + 10);
            positionCarteBotte.y -= dimCartey + 10;

            SDL_Surface *hitbox = NULL;
            SDL_Texture *textHitbox = NULL;
            SDL_Rect positionHitbox;
            
            hitbox = SDL_CreateRGBSurface(0, 100, 100, 32, 0, 0, 0, 0x000000ff);
            SDL_FillRect(hitbox, NULL, SDL_MapRGB(hitbox->format, 255, 0, 0));
            
            textHitbox = SDL_CreateTextureFromSurface(renderer, hitbox);
            position(positionHitbox, 0, 180, 290, 360);
            
            SDL_RenderCopy(renderer, textHitbox, NULL, &positionHitbox);
            
            SDL_FreeSurface(hitbox);
            SDL_DestroyTexture(textHitbox);
            
            SDL_Surface *score = NULL;
            SDL_Texture *scoreTexture;
            position(posScore, 5, 550, 100, 20);
            score = TTF_RenderText_Solid(police, ("Score: " + std::to_string(jouer->getJoueurs().at(i).getScore())).c_str(), couleurNoire);
            scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
            SDL_RenderCopy(renderer, scoreTexture, NULL, &posScore);
            SDL_FreeSurface(score);
            SDL_DestroyTexture(scoreTexture);

            for (int j = 0; j < 6; j++)
            {
                if (jouer->getJoueurs().at(i).getCarteEnMain().at(j).getValeur() != -1)
                {
                    positionCarteMain.y += positionCarteMain.h + 10;
                    SDL_RenderCopy(renderer, dosTexture, NULL, &positionCarteMain);
                }
            }

            for (unsigned int j = 1; j <= jouer->getJoueurs().at(i).getCartePoseeBotte().size(); j++)
            {
                positionCarteBotte.x += dimCartex + 10;

                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBotte().at(j - 1).getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBotte);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }
            if (jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeBataille().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteAttaque);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeLimitationVitesse().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteVitesse);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }

            if (jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur() != -1)
            {
                std::string chemin = "./data/" + std::to_string(jouer->getJoueurs().at(i).getCartePoseeDistance().back().getValeur()) + ".jpg";
                SDL_Surface *carte = IMG_Load(chemin.c_str());
                SDL_Texture *carteTexture;
                carteTexture = SDL_CreateTextureFromSurface(renderer, carte);

                SDL_RenderCopy(renderer, carteTexture, NULL, &positionCarteBorne);
                SDL_FreeSurface(carte);
                SDL_DestroyTexture(carteTexture);
            }
        }
    }

    SDL_RenderPresent(renderer);

    //printf("SDL_Init failed: %s\n", SDL_GetError());

    SDL_RenderClear(renderer);
    SDL_FreeSurface(carte_dos);
    SDL_DestroyTexture(dosTexture);
}
