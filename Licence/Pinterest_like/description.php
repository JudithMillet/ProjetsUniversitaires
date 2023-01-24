<?php
session_start();
require_once('fonctions/gestionPhotos.php');
require_once('fonctions/bd.php');
require_once('fonctions/user.php');

$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);
if (isset($_GET["id"])) { // on verifie que $_GET["id"] (id passé dans l'url) n'est pas vide
    $id = $_GET["id"];
    
    $desc = desc($id,$link); // Contenu de Photo lorsque photoId = $id
    // renvoyé sous forme d'un tableau à 2 dimensions
    $nomFich = $desc[0][1];
    $description = $desc[0][2];
    $catid = $desc[0][3];
    $userid = $desc[0][4];
    $affichage = $desc[0][5];

    $pseudo = getPseudo($userid,$link); // Renvoie le pseudo correspondant à l'id passé en parametre
    $categorie = getCategorie($catid,$link); // Renvoie la catégorie correspondante à l'id passé en parametre
    closeConnexion($link); // Ferme la connexion à la base de données

}
else {
    header('Location:index.php'); // Sinon on retourne sur la page d'accueil
}
?>

<!doctype html>
<html lang="fr">
    <head>
        <meta charset="utf-8">
        <title>Description</title>
        <link  href="lib/css/bootstrap.css" rel="stylesheet">
        <script src="lib/js/jquery-3.3.1.min.js"></script>
        <script src="lib/js/bootstrap.min.js"></script>
    </head>
    <body>
        <?php
            if ($_SESSION['pseudo']!='') 
                print '<div class="mb-sm-4 mt-sm-5"> Vous êtes connecté en tant que ' .$_SESSION['pseudo'] . ' </div>';
        ?>
        <div class="container">
            <h1 style="text-align: center; margin:40px;">Les détails sur cette photo :</h1>
            <div class="row" style="display:flex; justify-content:center;">
                <div class="col-sm-3" style="display=inline-block;">
                    <?php
                    $dossier = "./assets/images/";
                    if(is_dir($dossier)) { // si c'est bien un dossier
                    if ($dir = opendir($dossier)) { // s'il s'ouvre bien
                            while($file = readdir($dir)) { // Tant qu'il y a des fichiers
                                $nameFile = stristr($file,'.',true); // nom du fichier file sans l'extension (ex : 'nom.png' devient 'nom')
                                if("DSC-" . $id== $nameFile) print '<img src="./assets/images/DSC-'.$id.stristr($file,'.').'" id="img-'.$file.'" class="img-fluid" alt="'.$nameFile.'">';
                            }
                        }
                    }
                    ?>
                </div>
                <div class="col-sm-5" style="display:flex; font-size:20px; align-items: center;">
                    <table class="table table-bordered">
                        <tr>
                            <th>Nom</th>
                            <td><?php echo $nomFich; ?></td>
                        </tr>
                        <tr>
                            <th>Description</th>
                            <td><?php echo $description; ?></td>
                        </tr>
                        <tr>
                            <th>Catégorie</th>
                            <td><?php print '<a href="index.php?cat='.$categorie[0][0].'">'.$categorie[0][0]; ?></a></td>
                        </tr>
                        <tr>
                            <th>Postée par</th>
                            <td><?php if ($pseudo[0][0]==$_SESSION['pseudo']) 
                                        {
                                            echo 'vous ';
                                            if ($affichage=='non') echo '(photo privée)';
                                        }
                                else echo $pseudo[0][0]; ?></td>
                        </tr>
                    </table>
                </div>
            </div>
            <div class="row" style="display:flex; font-size:20px; justify-content:center;">
                <a href="index.php">Retour à la page principale</a>
            </div>
        </div>
    </body>
</html>