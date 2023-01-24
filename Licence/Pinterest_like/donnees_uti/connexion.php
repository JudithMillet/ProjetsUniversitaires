<?php

session_start();

require_once('../fonctions/bd.php');
require_once('../fonctions/user.php');

// Fuseau horaire
date_default_timezone_set('Europe/Paris');

$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);
if (isset($_POST["valider"]))
{
	if (getUser($_POST['user_name'], hash("md5", $_POST["user_pwd"]), $link)) // On verifie que l'utilisateur existe (son pseudo et mdp existe)
	{
		setConnected($_POST['user_name'], $link); //On passe l'utilisteur à connecté
		$_SESSION['pseudo']=$_POST["user_name"];
		$_SESSION['date']=time();
		header('location:../index.php');
	}
	else echo '<p style="color : red;">Pseudo inconnu ou mdp incorrect</p>';
}
closeConnexion($link); // Ferme la connexion à la base de données
	

?>

<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>Premi&egrave;re inscription</title>
		<link  href="../lib/css/bootstrap.css" rel="stylesheet">
		<script src="../lib/js/jquery-3.3.1.min.js"></script>
		<script src="../lib/js/bootstrap.min.js"></script>
	</head>
	<body>
		<div class="container">
			</br>
			<h1 style="text-align: center;">Connectez-vous au miniPinterest</h1>
			</br>
			<form action="connexion.php" method="post">
					<div class="form-group">
						<label class="control-label col-sm-2" for="name">Nom :</label>
						<div class="col-sm-12">
							<input type="text" class="form-control" id="name" name="user_name">
						</div>
					</div>
					<div class="form-group">
						<label class="control-label col-sm-2" for="pwd">MDP :</label>
						<div class="col-sm-12">
							<input type="password" class="form-control" id="pwd" name="user_pwd"></textarea>
						</div>
					</div>    		
					<div class="button form-checked col-sm-offset-3 col-sm-10">
						<button type="submit" class="btn btn-primary" name="valider" >Se connecter</button>
					</div>
				</form>
				</br>
				<div class="form-group col-sm-offset-3 col-sm-10"> 
					<a href="inscription.php">S'incrire</a>
				</div>
				<div class="form-group col-sm-offset-3 col-sm-10"> 
					<a href="../index.php">Retour</a>
				</div>
		</div>
	</body>

</html>
