<?php

//Renvoie le nombre d'utilisateurs existants
function nbUti ($link)
{
    $req = "SELECT * FROM user";
	$res = executeQuery($link,$req);
    return mysqli_num_rows($res);
}

//Renvoie le nombre d'utilisateurs connectés
function nbUtiCo ($link)
{
    $req = "SELECT * FROM user WHERE etat='connected'";
	$res = executeQuery($link,$req);
    return mysqli_num_rows($res);
}

//Renvoie le nombre de catégories existantes
function nbCat ($link)
{
    $req = "SELECT * FROM Categorie";
	$res = executeQuery($link,$req);
    return mysqli_num_rows($res);
}

//Renvoie le nombre de photos postées (publiques et privées)
function nbPhotos ($link)
{
    $req = "SELECT * FROM Photo";
	$res = executeQuery($link,$req);
    return mysqli_num_rows($res);
}

//Renvoie le nombre de photos postées par l'utilisateur qui a l'id $userId
function nbPhotosParUti ($link, $userId)
{
    $req = "SELECT * FROM Photo WHERE userId LIKE \"$userId\" ";
	$res = executeQuery($link,$req);
    return mysqli_num_rows($res);
}

//Renvoie le nombre de photos postées dans la catégorie qui a l'id $catId
function nbPhotosParCat ($link, $catId)
{
    $req = "SELECT * FROM Photo WHERE catId LIKE \"$catId\" ";
	$res = executeQuery($link,$req);
    return mysqli_num_rows($res);
}

//Renvoie le nombre moyen de photos postées par utilisateur
function avgNbPhotoUti ($link)
{
    $avg = 0;
    $reqMax= "SELECT MAX(userId) FROM user";
    $resMax = executeQuery($link,$reqMax);
    $max = mysqli_fetch_all($resMax);
    for ($id = 0 ; $id <= $max[0][0] ; $id++)
        $avg += nbPhotosParUti($link, $id);

    $avg=$avg/nbUti($link);
    return $avg;
}

//Renvoie le nombre moyen de photos postées par catégorie
function avgNbPhotoCat ($link)
{
    $avg = 0;
    $reqMax= "SELECT MAX(catId) FROM Categorie";
    $resMax = executeQuery($link,$reqMax);
    $max = mysqli_fetch_all($resMax);
    for ($id = 0 ; $id <= $max[0][0] ; $id++)
        $avg += nbPhotosParCat($link, $id);

    $avg=$avg/nbCat($link);
    return $avg;
}

?>