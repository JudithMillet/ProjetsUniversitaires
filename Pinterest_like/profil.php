<?php
session_start();
require_once('fonctions/bd.php');
require_once('fonctions/user.php');
require_once('fonctions/gestionPhotos.php');

$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName); // Connexion à la base de données

$data=getUserData($_SESSION['pseudo'], $link); // Contenu de User où pseudo est le pseudo de la personne connectée
// renvoyé sous la forme d'un tableau à 2 dimensions
$nom=$data[0][1];
$role=$data[0][4];

//Si on clique sur le bouton "changer de pseudo" on va sur la page attendue
if (isset($_POST["chgmntPseudo"]))
    header('location:donnees_uti/chgmntPseudo.php');	
    
//Si on clique sur le bouton "changer de mot de passe" on va sur la page attendue
if (isset($_POST["chgmntMdp"]))
    header('location:donnees_uti/chgmntMdp.php');

//Si on clique sur le bouton "se deconnecter" on est déconnecté et on retourne à l'accueil
if (isset($_POST["deconnexion"]))
{
    disconnect($_SESSION['pseudo'],$link);
    $_SESSION['pseudo']='';		
    header('location:index.php');			
}
// closeConnexion($link);
?>



<!doctype html>
<html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Profil</title>
        <link  href="lib/css/bootstrap.css" rel="stylesheet">
        <script src="lib/js/jquery-3.3.1.min.js"></script>
        <script src="lib/js/bootstrap.min.js"></script>
    </head>
    <body>
        <div class="container">
            <h1 style="text-align: center; margin:40px;">Mon profil :</h1>
            <div class="row" style="display:flex; justify-content:center;">
                <div class="col-sm-5" style="display:flex; font-size:20px; align-items: center;">
                    <div>
                        <?php echo  "Vous êtes un " . $role ."</br>" . "Votre pseudo est " . $nom; ?>
                    </div>
                </div>
            </div>

            <form action="profil.php" method="post">
                <div class="form-group">
                    <button type="submit" class="btn btn-info" value="chgmntPseudo" name="chgmntPseudo"> Changer de pseudo </button>
                </div>
                <div class="form-group">
                    <button type="submit" class="btn btn-info" value="chgmntMdp" name="chgmntMdp"> Changer de Mot de passe </button>
                </div>
                <div class="form-group">
                    <button type="submit" class="btn btn-info" value="deconnecter" name="deconnexion"> se déconnecter </button>
                </div>
            </form>
            <h3>Vos photos :</h3>
            <div class="card-columns">
                <?php
                    $images = photoParUti($link,getUserId($_SESSION['pseudo'],$link)[0][0]);//Selectionne les photos postées par l'utilisateur
                    $tabFiles = affichePhotos($images, $_SESSION['pseudo'],$link);  // Affiche les photos et renvoie un tableau contenant les fichiers affichés
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
