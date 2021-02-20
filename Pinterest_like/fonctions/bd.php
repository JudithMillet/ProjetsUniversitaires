<?php

$dbHost = "localhost";
$dbUser = "p1802635";
$dbPwd = "f95d96";
$dbName = "p1802635";
/*Cette fonction prend en entrée l'identifiant de la machine hôte de la base de données, les identifiants (login, mot de passe) d'un utilisateur autorisé 
sur la base de données contenant les tables pour le chat et renvoie une connexion active sur cette base de donnée. Sinon, un message d'erreur est affiché.*/
function getConnection($dbHost, $dbUser, $dbPwd, $dbName)
{
	$link = mysqli_connect($dbHost, $dbUser, $dbPwd, $dbName);
	if (mysqli_connect_errno()) 
	{
		printf("Echec connexion : %s\n", mysqli_connect_error());
		exit();
	}
	return $link;
}

/*Cette fonction prend en entrée une connexion vers la base de données du chat ainsi 
qu'une requête SQL SELECT et renvoie les résultats de la requête. Si le résultat est faux, un message d'erreur est affiché*/
function executeQuery($link, $query)
{
	$verif = mysqli_query($link, $query);
	if ($verif == FALSE )
	{
		printf("Echec récup requète %s");
	}
	
	else return $verif;
}

/*Cette fonction prend en entrée une connexion vers la base de données du chat ainsi 
qu'une requête SQL INSERT/UPDATE/DELETE et ne renvoie rien si la mise à jour a fonctionné, sinon un 
message d'erreur est affiché.*/
function executeUpdate($link, $query)
{
	$verif = mysqli_query($link, $query);
	if ($verif == FALSE )
	{
		printf("Echec MAJ %s");
	}
}

/*Cette fonction ferme la connexion active $link passée en entrée*/
function closeConnexion($link)
{
	mysqli_close($link);
}
?>
