<?php

session_start();
require_once('fonctions/bd.php');
require_once('fonctions/user.php');
require_once('fonctions/gestionPhotos.php');
require_once('fonctions/stat.php');

$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);

?>

<!doctype html>
<html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Gestion</title>
        <link  href="lib/css/bootstrap.css" rel="stylesheet">
        <script src="lib/js/jquery-3.3.1.min.js"></script>
        <script src="lib/js/bootstrap.min.js"></script>
    </head>
    <body>
        <div class="mb-sm-4 mt-sm-5"> Connecté en tant que <?php echo $_SESSION['pseudo'] ?> </div>
            <div class="container">
                Il y a <?php echo nbUti($link); ?> utilisateurs
                dont <?php echo nbUtiCo($link); ?> connecté(s) (vous compris).</br>
                </br>
                Il y a <?php echo nbCat($link); ?> catégories. </br>
                <?php echo nbPhotos($link); ?> photos ont été postées.</br>
                Il y a en moyenne <?php echo avgNbPhotoUti ($link) ?> photo(s) postée(s) par utilisateur.</br>
                Il y a en moyenne <?php echo avgNbPhotoCat ($link) ?> photo(s) par catégorie.</br>
                </br>
            </div>
            <h3>Toutes les photos :</h3>
            <div class="card-columns">
                <?php
                    $images = photoParCat($link,'all');//Selectionne toutes les photos
                    $tabFiles = affichePhotos($images,$_SESSION['pseudo'],$link); // Affiche les photos et renvoie un tableau contenant les fichiers affichés
                    $tabFilesJson = json_encode($tabFiles); // Convertit le tabFiles (de base en PHP) en un tableau Json
                    closeConnexion($link);
                ?>
            </div>
            <div class="row" style="display:flex; font-size:20px; justify-content:center;">
                <a href="index.php">Retour à la page principale</a>
            </div>
        </div>
    </body>
    <script>  
        var tabFilesJs = <?php print $tabFilesJson;?> ; // Convertit le tabFilesJson en un tableau Js
        // Lorsqu'on passe le curseur sur une des images, on affiche ou on cache la div
        for(f in tabFilesJs) {
            let card = document.getElementById(`card-${tabFilesJs[f]}`);
            let desc = document.getElementById(`desc-${tabFilesJs[f]}`);
            card.addEventListener("mouseover", () => {
                if(getComputedStyle(desc).display != "none"){
                    desc.style.display = "none";
                } else {
                    desc.style.display = "block";
                }
            })
            card.addEventListener("mouseout", () => {
                if(getComputedStyle(desc).display != "none"){
                    desc.style.display = "none";
                } else {
                    desc.style.display = "block";
                }
            })
        }
    </script>
</html>