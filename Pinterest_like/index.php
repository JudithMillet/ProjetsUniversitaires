<?php
session_start();
require_once('fonctions/bd.php');
require_once('fonctions/user.php');
require_once('fonctions/gestionPhotos.php');
require_once('fonctions/pic.php');

if (empty($_SESSION))//Si la variable de session est vide c'est que personne n'est connecté
    $_SESSION['pseudo']='';
        

$link = getConnection($dbHost, $dbUser, $dbPwd, $dbName); // Connexion à la base de données
$listerCat = rechercheCat($link);

//Si on clique sur le bouton de déconnexion on est déconnecté
if (isset($_POST["deconnexion"]))
{
    disconnect($_SESSION['pseudo'],$link);
    $_SESSION['pseudo']='';			
}

//Si on clique sur le bouton "Ajouter une photo" on va sur la page d'ajout
if (isset($_POST["ajout"]))
{  
	header('location:donnees_pic/ajout.php');				
}

//Si on clique sur "profil" on va sur la page de son profil
if (isset($_POST["profil"]))
{   
	header('location:profil.php');				
}

//SI on clique sur "Gestion" on va sur la page de gestion
if (isset($_POST["gestion"]))
{
	header('location:gestion.php');				
}         
?>

<!doctype html>
<html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Bienvenue sur l'album de BDW</title>
        <link  href="lib/css/bootstrap.css" rel="stylesheet">
   	    <script src="lib/js/jquery-3.3.1.min.js"></script>
   	    <script src="lib/js/bootstrap.min.js"></script>
    </head>
    <body>
        <!-- Entête de la page -->
        <header>
            <div class="container-fluid bg-info" style="justify-content: center; display: flex;">
                <h1 class="mt-sm-3" style=" font-size: 6em;
                            font-family: Georgia, serif; 
                            font-weight: bold;
                            margin:0 auto;">
                    Mini pinterest
                </h1>
                <div class="row">
                    <div class="col-sm-6">
                        <label class="mb-sm-3 mt-sm-4"><strong>Recherchez une categorie:</strong></label>
                        <form class="form-group" id="recherche" method="get" name="cherche">
                            <select class="custom-select mr-sm-2 mb-sm-2" id="select" name="cat" > 
                                <option>all</option>
                                <?php 
                                $res = 'all';
                                    foreach($listerCat as $c):?>
                                    <option><?php echo $c[0];?></option>
                                <?php endforeach; ?>
                            </select>
                            <button type="submit" class="btn btn-outline-light mb-sm-2"><b>Rechercher</b></button>
                        </form>
                    </div>
                    <div class="col-sm-6 mt-sm-2 mb-sm-2">
                        <?php 
                        //Si quelqu'un est connecté on affiche les boutons "ajouter une photo", "profil" et "deconnexion", 
                        if (isconnected($_SESSION['pseudo'], $link))  
                        {
                            if (isAdmin(getUserId($_SESSION['pseudo'],$link)[0][0], $link))
                                $display = '';
                            else $display='none';
                                require_once('donnees_uti/isconnected.php');  
                        }
                        //sinon on affiche les boutons pour s'incrire ou se connecter"
                        else require_once('donnees_uti/notconnected.php');
                        ?>
                    </div>
                </div>
            </div>         
        </header>

        <?php
        if ($_SESSION['pseudo']!='') //Si le pseudo n'est pas nul c'est que quelqu'un est connecté
        {
            echo '<h2 class="mb-sm-4 mt-sm-5"> Bienvenue ' .$_SESSION['pseudo'] . ' </h2>';
            $date = $_SESSION['date'];
            $affiche = afficheDate($date);
            print '<h5>' . $affiche . '</h5>';
        }
        //On affiche les photos de la catégorie passée en methode get
        if (isset($_GET["cat"])){
            $res = $_GET["cat"];
        }    
        ?>
        <h1 class="mb-sm-4 mt-sm-5">Voici les photos
            <?php
            //Si on a selectionné une categorie à afficher
            if($res!="all")
            {
                print ' de la catégorie '.$res.' :';
            }
            else { print ' :';};
            ?>
        </h1>
        <div class="card-columns">
            <?php
            $images = photoParCat($link,$res); // tableau contenant le nomFich des photos à afficher
            $tabFiles = affichePhotos($images,$_SESSION['pseudo'],$link); // Affiche les photos et renvoie un tableau contenant les fichiers affichés
            $tabFilesJson = json_encode($tabFiles); // Convertit le tabFiles (de base en PHP) en un tableau Json
            ?>
        </div> 
    </body>
    <!-- Pied de page -->
    <footer>
   	    <section class="col-sm-auto mt-sm-3 text-center">
            <p>
                <strong>Projet de BDW1 2020<strong><br />
                Créé par Emie Lafourcade et Judith Millet
            </p>
    	</section>
    </footer>
    <!-- script js -->
    <script>
        
        var tabFilesJs = <?php print $tabFilesJson;?> ; // Convertit le tabFilesJson en un tableau Js
        // Lorsqu'on passe le curseur sur une des images, on affiche ou on cache la div d'info
        for(f in tabFilesJs) {
            let card = document.getElementById(`card-${tabFilesJs[f]}`);
            let desc = document.getElementById(`desc-${tabFilesJs[f]}`);

            card.addEventListener("mouseover", () => {
                if(getComputedStyle(desc).display != "none") {
                    desc.style.display = "none";
                } else {
                    desc.style.display = "block";
                }
            })
            card.addEventListener("mouseout", () => {
                if(getComputedStyle(desc).display != "none") {
                    desc.style.display = "none";
                } else {
                    desc.style.display = "block";
                }
            })
        }
    </script>
</html>
