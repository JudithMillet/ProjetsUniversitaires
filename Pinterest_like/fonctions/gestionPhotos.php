<?php
require_once('bd.php');
require_once('user.php');

// Prend le nom de la catégorie et renvoie un tableau contenant le nom des fichiers de la catégorie
function photoParCat ($link, $nomCat) {
		if ($nomCat == 'all')
			{
				$req = "SELECT photoId FROM Photo;";
				$res = executeQuery($link,$req);
			}
			
		else{
			$req = "SELECT distinct photoId FROM Photo WHERE catId =(SELECT catId FROM Categorie WHERE nomCat LIKE \"$nomCat\");";
			$res = executeQuery($link,$req);
		}
	return mysqli_fetch_all($res);//fetch all renvoie un tableau sous la forme tab[indice du tuple][indice de la colonne PAS LE NOM 0 si il y a une seul colonne de sélectionnée]			
}


// Renvoie un array des photos postées par l'utilisteur dont l'id est passé en parametre
function photoParUti ($link, $userId) {        
    $req = "SELECT distinct photoId FROM Photo WHERE userId =\"$userId\"";
    $res = executeQuery($link,$req);
    return mysqli_fetch_all($res);//fetch all renvoie un tableau sous la forme tab[indice du tuple][indice de la colonne PAS LE NOM 0 si il y a une seul colonne de sélectionnée]			
}

// Renvoie un array des noms des catégories
function rechercheCat($link) {
	$req = "SELECT nomCat FROM Categorie";
	$res = executeQuery($link,$req);
	return mysqli_fetch_all($res);
}

// Renvoie un array des données de l'image en fonction du nom du fichier = $nomIm
function desc($id,$link) {
    $req = "SELECT * FROM Photo WHERE photoId Like \"$id\";";
    $res = executeQuery($link,$req);
	return mysqli_fetch_all($res);
}

// Prend le catId et renvoie un array contenant le nom de la catégorie correpondante
function getCategorie($catid,$link) {
    $req = "SELECT nomCat FROM Categorie WHERE catId Like \"$catid\";";
    $res = executeQuery($link,$req);
    return mysqli_fetch_all($res);
}

// Prends le nom de l'image et renvoie un array contenant l'id de l'image
function getPhotoId($nom,$link) {
    $req = "SELECT photoId FROM Photo WHERE nomFich Like \"$nom\";";
    $res = executeQuery($link,$req);
    return mysqli_fetch_all($res);
}

// Prend l'id de l'image et renvoie un array contenant le nom
function getPhotoNom($id,$link) {
    $req = "SELECT nomFich FROM Photo WHERE photoId Like \"$id\";";
    $res = executeQuery($link,$req);
    return mysqli_fetch_all($res);
}

// Prend l'id de l'image et renvoie un array contenant l'id de l'utilisateur qui l'a posté
function getPhotoUserId($id,$link) {
    $req = "SELECT userId FROM Photo WHERE photoId Like \"$id\";";
    $res = executeQuery($link,$req);
    return mysqli_fetch_all($res);
}

// Prend l'id de l'image et renvoie un array d'"aff" qui indique "oui" quand la photo est en public et "non" quand elle est en privée
function getPhotoaff($id,$link) {
    $req = "SELECT aff FROM Photo WHERE photoId Like \"$id\";";
    $res = executeQuery($link,$req);
    return mysqli_fetch_all($res);
}

// Cette fonction permet d'afficher toutes les photos du dossier ./assets/images/ sur la page web et renvoie un tableau contenant le nom des fichiers affichés
function affichePhotos($im,$pseudo, $link) {
    if ($pseudo=='') $userId=0;
    else $userId = getUserId($pseudo,$link)[0][0];
    $files = Array();
    $it = 0;
    $dossier = "./assets/images/";
    if(is_dir($dossier)) { // si c'est bien un dossier
        if ($dir = opendir($dossier)) { // s'il s'ouvre bien
            while($file = readdir($dir)) { // Tant qu'il y a des fichiers
                if($file[0] != "."){
                    $mime = mime_content_type($dossier.$file); // extension du fichier
                    if(strstr($mime, "image/")) { //Si c'est bien une photo
                        foreach ($im as $i) {
                            $nameFile = stristr($file,'.',true); // nom du fichier file sans l'extension (ex : 'nom.png' devient 'nom')
                            if("DSC-" . $i[0]== $nameFile){
                            if ((getPhotoaff($i[0],$link)[0][0] == 'oui') || ($userId==getPhotoUserId($i[0],$link)[0][0]))
                            {
                                $desc = desc($file,$link); // $desc[0][0] contient la description
                                print '<div id="card-'.$file.'" class="grayscale card" style="padding-right: 0px;
                                padding-left: 0px;">
                                    <a href="description.php?id='.$i[0].'"><img src="'.$dossier."DSC-" . $i[0].stristr($file,'.').'" id="img-'.$file.'" class="card-img-top" alt="'.$nameFile.'" name="'.$file.'" style="height: 500px; object-fit: cover;"></a>
                                    <div class="desc card-body bg-light text-info" id="desc-'.$file.'">
                                        <h5 class="card-title">'.getPhotoNom($i[0],$link)[0][0].'</h5>
                                        <h6 class="card-subtitle text-muted">Cliquez sur l\'image pour plus de détails</h6>';
                                if(isAdmin($userId,$link)) { print '<a href="donnees_pic/modification.php?id='.$i[0].'">Modifier</a>'; }
                                else {
                                    $photoUti = photoParUti($link, $userId);
                                    foreach($photoUti as $p) {
                                        if($p[0]==$i[0]) {
                                            print '<a href="donnees_pic/modification.php?id='.$i[0].'">Modifier</a>';
                                        }
                                    }
                                }
                                print '</div>
                                </div>';
                                $files[$it] = $file;
                                $it++;
                            }
                            }
                        }
                    }
                }
            }
        }
    }
    closedir($dir); // Ferme le dossier
    return $files;
}

?>