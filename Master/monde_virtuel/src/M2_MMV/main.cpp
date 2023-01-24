 #include "utils.h"
#include "wavefront.h"
 #include "texture.h"
 
 #include "orbiter.h"
 #include "draw.h"        
 #include "app_camera.h"
 #include "terrain.h"
 
 
 // utilitaire. creation d'un repere.
 Mesh make_grid( const int n= 10 )
 {
     Mesh grid= Mesh(GL_LINES);
     
     // axes XYZ
     grid.color(Red());
     grid.vertex(Point(0, .1, 0));
     grid.vertex(Point(1, .1, 0));
     
     grid.color(Green());
     grid.vertex(Point(0, .1, 0));
     grid.vertex(Point(0, 1, 0));
     
     grid.color(Blue());
     grid.vertex(Point(0, .1, 0));
     grid.vertex(Point(0, .1, 1));
     
     glLineWidth(2);
     
     return grid;
 }

/// COMMANDES ///


void usage(int argc, char** argv)
{
    // Name
    std::cout<<"TP MMV"<<std::endl;
    std::cout<<"        "<<argv[0]<<" - Création de cartes d'analyse et de terrains.";
    std::cout<<std::endl<<std::endl;
    // Synopsis
    std::cout<<"SYNOPSIS"<<std::endl;
    std::cout<<"        "<<argv[0]<<" [-data ../data/file]"
            <<" [--help|-h|-?] "
            <<"[-smooth S] [-erosion E] [-road R]"
            <<std::endl<<std::endl;
    // Options
    std::cout<<"        --help, -h, -?"<<std::endl
            <<"                Affichage de l'aide."
            <<std::endl<<std::endl;
    std::cout<<"        -data FILE"
            <<std::endl
            <<"                Chemin défini vers un terrain (terrain_exemple_512.png par defaut)."
            <<std::endl<<std::endl;
    std::cout<<"        -smooth S"
            <<std::endl
            <<"                Lissage du terrain S fois (pas de lissage par defaut)."
            <<std::endl<<std::endl;
    std::cout<<"        -erosion E"
            <<std::endl
            <<"                erosion du terrain avec E itérations (pas d'erosion par defaut)."
            <<std::endl<<std::endl;
    std::cout<<"        -road"
            <<std::endl
            <<"                Affichage d'une route selon la pente (=1) ou selon l'environnement (=2), "
            <<std::endl
            <<"                ou les 2 combinés (>=3). "
                                
            <<std::endl<<std::endl;
    exit(EXIT_FAILURE);
}

void erreur_commande(int argc, char** argv, const char* msg)
{
    std::cout<<"ERROR: "<<msg<<std::endl;
    usage(argc, argv);
}

void commande(int argc, char** argv, Utils_commande& utils)
{
    utils.filename = "data/terrain/terrain_exemple_512.png";
    utils.smooth = 0;
    utils.erosion = 0;
    utils.road = 0;

    bool help = false;

    std::string arg;

    for (int i=1; i<argc; ++i)
    {
        arg=std::string(argv[i]);
        if (arg==std::string("-h") || arg==std::string("--help") || arg==std::string("-?"))
            help=true; 

        else if(arg=="-data")
        {
            if (argc-1-i<1)
                erreur_commande(argc, argv, "Ajouter un chemin vers un fichier après l'option -data.");
            
            utils.filename = argv[++i];
        }
        else if(arg=="-smooth")
        {
            if (argc-1-i<1)
                erreur_commande(argc, argv, "Ajouter un nombre après l'option -smooth.");
            
            utils.smooth = std::stod(std::string(argv[++i]));
        }
        else if(arg=="-erosion")
        {
            if (argc - 1 - i < 1)
                erreur_commande(argc, argv, "Ajouter un nombre d'itération après l'option -erosion.");

            utils.erosion = std::stod(std::string(argv[++i]));
        }
        else if(arg=="-road")
        {
            if (argc-1-i<1)
                erreur_commande(argc, argv, "Ajouter 1,2 ou 3 après l'option -road.");

            utils.road = std::stod(std::string(argv[++i]));
        }
    }
    if (help) usage(argc, argv);
}
 
 
 class TP : public AppCamera
 {
 public:
     // constructeur : donner les dimensions de l'image, et eventuellement la version d'openGL.
     TP(Utils_commande _utils) : AppCamera(1024, 640) {
        utils = _utils;
     }
     
     // creation des objets de l'application
    int init( )
    {
        srand(time(NULL));
        // decrire un repere / grille 
        m_repere= make_grid(10);
           
        // etat openGL par defaut
        glClearColor(0.2f, 0.2f, 0.2f, 1.f);        // couleur par defaut de la fenetre
         
        glClearDepth(1.f);                          // profondeur par defaut
        glDepthFunc(GL_LESS);                       // ztest, conserver l'intersection la plus proche de la camera
        glEnable(GL_DEPTH_TEST);                    // activer le ztest
 
        // chargement du terrain selon une image en niveaux de gris
        terrain.loadScalarField(utils.filename);
        
        // Application d'un lissage
        if (utils.smooth) {
            for (uint i = 0; i < utils.smooth; i++)
                terrain.smooth();
        }
        
        // Application d'un erosion
        if (utils.erosion) {
            for (uint i = 0; i < utils.erosion; i++) {
                ScalarField erosion = terrain.erosionMap();
                terrain += erosion * 0.01;
                write_image(erosion.draw(), "data/terrain/resultats/erosion.png");
            }
        }
        
        // Création de l'image de texture
        Image image(terrain.nx, terrain.ny,Color(0.5,0.5,0.5));
        for (int i = 0; i < terrain.nx; i++) {
            for (int j = 0;  j < terrain.ny;  j++)
            {
                float depth = terrain.height(i, j) * (10.f / (float)terrain.nx);
                if (depth > 0.8)
                {
                    image(i, j) = Color(1, 1, 1);
                }
                else if (depth > 0.4) {
                    image(i, j) = Color(0.5, 0.5, 0.5);
                }
                else if (depth > 0.15) {
                    image(i, j) = Color(0.0, 0.5, 0.0);
                }
                else
                {
                    image(i, j) = Color(0.0, 0.0, 0.5);
                }
            }
        }

        // Génération d'une route
        if (utils.road) {
          vertex_t source = terrain.index(rand() % (terrain.nx / 2 + 1),
                                          rand() % (terrain.ny / 2 + 1));
          vertex_t end =
              terrain.index(rand() % (terrain.nx / 2 + 1) + terrain.nx / 2,
                            rand() % (terrain.ny / 2 + 1) + terrain.ny / 2);

          std::list<vertex_t> path = terrain.createRoad(source, end, utils.road);
          terrain.drawPath(image, path);
        }

        write_image(image, "data/terrain/resultats/texture.png");

        m_texture = read_texture(0, "data/terrain/resultats/texture.png");

        // Création du mesh et des differentes cartes d'analyse
        m_terrain = terrain.createMesh();
        ScalarField drain = terrain.drainMap();
        ScalarField slope = terrain.slopeMap();

        ScalarField res = drain * slope;
        image = drain.draw();

        Image image2 = slope.draw();
        Image image3 = terrain.laplacianMap().draw();
        write_image(image, "data/terrain/resultats/drainMap.png");
        write_image(image2, "data/terrain/resultats/slope.png");
        write_image(image3, "data/terrain/resultats/laplacian.png");

         

        return 0;   // pas d'erreur, sinon renvoyer -1
    }
     
    // destruction des objets de l'application
    int quit( )
    {
        m_repere.release();
        m_terrain.release();
        return 0;   // pas d'erreur
    }
     
    // dessiner une nouvelle image
    int render( )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
     // dessine le repere, place au centre du monde, pour le point de vue de la camera 
        draw(m_repere, /* model */ Identity(), camera());

        if (terrain.nx > 1000) {
            draw(m_terrain, /* model */Scale(0.001) * Translation(-terrain.nx / 2, 0, -terrain.ny / 2), camera(), m_texture);
        }
        else {
            draw(m_terrain, /* model */Scale(0.01) * Translation(-terrain.nx / 2, 0, -terrain.ny / 2), camera(), m_texture);
        }
        return 1;
    }
 
protected:
    Utils_commande utils;
    Mesh m_repere;
    Mesh m_terrain;
    HeightField terrain;
    GLuint m_texture;
};

int main( int argc, char **argv )
{
    Utils_commande utils;
    commande(argc, argv, utils);
    TP tp(utils);
    tp.run();
     
    return 0;
}