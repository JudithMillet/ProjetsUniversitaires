<?php
session_start();
require_once('../fonctions/bd.php');
require_once('../fonctions/user.php');


$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);
			
if (isset($_POST["valider"]))
{
	if ($_POST["user_pwd1"] == $_POST["user_pwd2"]) // On verifie que le mdp et sa confirmation sont les mêmes
	{
		if (checkAvailability($_POST["user_name"], $link)) // On verifie que le pseudo n'existe pas déja
		{
			register($_POST["user_name"], hash("md5", $_POST["user_pwd1"]), $link); // On enregistre le nouvel utilisateur dans la bd
			setConnected($_POST['user_name'], $link); // On passe l'utilisateur à connecté
			$_SESSION['pseudo']=$_POST["user_name"]; 
			header('location:../index.php');
		}
		else echo '<p style="color : red;">Pseudo déjà utilisé!</p>';
	}
	else echo '<p style="color : red;">Erreur sur le mot de passe!</p>';
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
			<h1 style="text-align: center;">Inscrivez-vous au miniPinterest</h1>
			</br>
			<form action="inscription.php" method="post">
				<div class="form-group">
					<label class="control-label col-sm-2" for="name">Nom :</label>
					<div class="col-sm-12">
					<input type="text" class="form-control" id="name" name="user_name">
					</div>
				</div>
				<div class="form-group">
					<label class="control-label col-sm-2" for="pwd1">MDP :</label>
					<div class="col-sm-12">
						<input type="password" class="form-control" id="pwd1" name="user_pwd1"></textarea>
					</div>
				</div>
				<div class="form-group">
					<label class="control-label col-sm-2" for="pwd2">Confirmation MDP :</label>
					<div class="col-sm-12">
						<input type="password" class="form-control" id="pwd2" name="user_pwd2"></textarea>
					</div>
				</div>


				<!-- <div>
					<input type="checkbox" id="admin" name="admin">
					<label for="admin"> profil administrateur</label><br>
				</div> -->
				
				
				<div class="button form-checked col-sm-offset-3 col-sm-10">
					<button type="submit" class="btn btn-primary" name="valider" >S'incrire</button>
				</div>
				</br>
				<div class="form-group col-sm-offset-3 col-sm-10">
					<div> <a href="connexion.php">Deja inscrit</a> </div>
				</div>
				<div class="form-group col-sm-offset-3 col-sm-10"> 
						<a href="../index.php">Retour</a></div>
				</div>
			</form>	

		</div>

	</body>
</html>


