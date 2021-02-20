<?php

/*Cette fonction prend en entrée un pseudo à ajouter à la relation utilisateur et une connexion et 
retourne vrai si le pseudo est disponible (pas d'occurence dans les données existantes), faux sinon*/
function checkAvailability($pseudo, $link)
{
	$req = "SELECT * FROM user WHERE pseudo LIKE \"$pseudo\"";
    $resultat = executeQuery($link,$req);
    return mysqli_num_rows($resultat) == 0;
}

/*Cette fonction prend en entrée un pseudo et un mot de passe et enregistre le nouvel utilisateur dans la relation utilisateur via la connexion*/
function register($pseudo, $hashPwd, $link)
{

$req = "REPLACE INTO user (pseudo , mdp, etat, role)
	VALUES ('". $pseudo ."', '". $hashPwd ."', 'disconnected', 'utilisateur');";
    executeQuery($link,$req);
}

/*Cette fonction prend en entrée un pseudo d'utilisateur et change son état en 'connected' dans la relation 
utilisateur via la connexion*/
function setConnected($pseudo, $link)
{
	$req="UPDATE user SET etat = 'connected' WHERE pseudo LIKE \"$pseudo\"";
	executeUpdate($link,$req);
}

/*Cette fonction prend en entrée un pseudo et mot de passe et renvoie vrai si l'utilisateur existe (au moins un tuple dans le résultat), faux sinon*/
function getUser($pseudo, $hashPwd, $link)
{
	$req = "SELECT * FROM user WHERE pseudo LIKE \"$pseudo\" AND mdp LIKE \"$hashPwd\" ";
	$resultat = executeQuery($link,$req);
    return mysqli_num_rows($resultat) != 0;
	
}

//Renvoie toutes les données de l'utilisateur $pseudo
function getUserData($pseudo, $link)
{
	$req = "SELECT * FROM user WHERE pseudo LIKE \"$pseudo\" ";
	$res = executeQuery($link,$req);
    return mysqli_fetch_all($res);
}

//Renvoie l'id de l'utilisateur ayant pour pseudo $pseudo
function getUserId($pseudo, $link)
{
	$req = "SELECT userId FROM user WHERE pseudo LIKE \"$pseudo\" ";
	$res = executeQuery($link,$req);
    return mysqli_fetch_all($res);
}

/*Cette fonction prend en entrée un pseudo d'utilisateur et change son état en 'disconnected' dans la relation 
utilisateur via la connexion*/
function disconnect($pseudo, $link)
{
	$req="UPDATE user SET etat = \"disconnected\" WHERE pseudo LIKE \"$pseudo\"";
	executeUpdate($link,$req);
}

/*Cette fonction prend en entrée un pseudo et mot de passe et renvoie vrai si l'utilisateur existe (au moins un tuple dans le résultat), faux sinon*/
function isconnected($pseudo, $link)
{
	$req="SELECT * FROM user WHERE pseudo LIKE \"$pseudo\" AND etat LIKE \"connected\"";
	$resultat = executeQuery($link,$req);
  	return mysqli_num_rows($resultat) != 0;
}

/*Cette fonction renvoie le pseudo correspondant au userId*/
function getPseudo($userid,$link)
{
	$req="SELECT pseudo FROM user WHERE userId LIKE \"$userid\"";
	$res = executeQuery($link,$req);
	return mysqli_fetch_all($res);
}

//Change le pseudo de l'utilisateur appelé $current par $new
function setPseudo($current, $new, $link)
{
	$req="UPDATE user SET pseudo=\"$new\" WHERE pseudo LIKE \"$current\"" ; 
	executeQuery($link,$req);
}


//Change le mot de passede l'utilisateur appelé $pseudo par $new
function setMdp($new, $pseudo, $link)
{
	$req="UPDATE user SET mdp=\"$new\" WHERE pseudo LIKE \"$pseudo\"" ; 
	executeQuery($link,$req);
}


//Renvoie true si l'utilisateur est un administrateur
function isAdmin ($userId, $link)
{
    $req = "SELECT * FROM user WHERE userId=$userId AND role LIKE \"administrateur\"";
    $res = executeQuery($link,$req);
    return mysqli_num_rows($res) != 0;
}

// Renvoie "Connecté depuis" ainsi que le temps depuis la connexion
function afficheDate($date)
{
	if(!ctype_digit($date)) // On vérifie que la date est un entier et sinon, on la transforme en timestamp
  		$date = strtotime($date);
	if(date('Ymd', $date) == date('Ymd')) // Si on s'est connecté dans la journée (la date actuelle correspond à la date en parametre)
	{
		$difference = time()-$date; // On fait la différence entre le time actuel et celui en parametre
		if($difference < 60) // Si on est connecté depuis moins de 60 secondes
			return 'Vous êtes connecté depuis ' . $difference . ' secondes.';
		else if($difference < 3600) // Si on est connecté depuis moins d'une heure
			return 'Vous êtes connecté depuis ' . round($difference/60, 0) . ' minutes.';
		else // Si on est connecté depuis plus d'une heure dans la journée on affiche l'heure sous forme HH:MM:SS
			return 'Vous êtes connecté depuis aujourd\'hui à ' . date('H:i:s', $date);
	}
	else
		return 'Vous êtes connecté depuis le ' . date('d/m/Y à H:i:s', $date); // Sinon on renvoie la date et l'heure
}

?>


