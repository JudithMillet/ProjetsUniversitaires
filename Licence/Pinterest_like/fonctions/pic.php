<?php

//Renvoie vrai si la catégorie passée en parametre n'existe pas, faux sinon
function catNotExist($cat, $link)
{
	$req="SELECT * FROM Categorie WHERE nomCat LIKE \"$cat\"";
	$resultat = executeQuery($link,$req);
    return mysqli_num_rows($resultat) == 0;
}

//Ajoute la catégorie $cat à la bd
function addCat($cat, $link)
{	
	$req = "INSERT INTO Categorie (nomCat)
	VALUES ('".$cat."')";
	executeQuery($link,$req);
}

//Ajoute les données d'une photo dans la bd
function addPic ($nom, $cat, $desc, $aff, $pseudo, $link)
{
   	$req = "INSERT INTO Photo (nomFich, description, catId, userId, aff)
   	VALUES ('". $nom ."', '".$desc."', (SELECT catId FROM Categorie WHERE nomCat LIKE \"$cat\"), (SELECT userId FROM user WHERE pseudo LIKE \"$pseudo\"), '". $aff ."');";
    executeUpdate($link,$req);
}

// Fonction pour modifier une ligne une ligne (nom de la photo, description, catégorie) en fonction de l'id de la photo
function modifPic($nomIm,$desc,$cat,$aff,$photoId,$link) {
    $req = "UPDATE Photo SET nomFich=\"$nomIm\",description=\"$desc\",catId=(SELECT catId FROM Categorie WHERE nomCat LIKE \"$cat\"),aff=\"$aff\" WHERE photoId LIKE \"$photoId\";"; //TODO faire en fonction de userId
	executeUpdate($link,$req);
}

// Fonction pour supprimer une image en fonction de son nom de fichier
function deletePic($idIm,$link) {
    $req = "DELETE FROM Photo WHERE photoId LIKE \"$idIm\"";
	executeUpdate($link,$req);
}

?>
