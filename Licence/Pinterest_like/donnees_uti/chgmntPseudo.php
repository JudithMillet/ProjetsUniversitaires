<?php
session_start();
require_once('../fonctions/bd.php');
require_once('../fonctions/user.php');


$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);
			
if (isset($_POST["valider"]))
{
	if (checkAvailability($_POST["user_name"], $link)) // On verifie que le pseudo n'existe pas déja
	{
		setPseudo($_SESSION['pseudo'], $_POST["user_name"], $link ); //On change le pseudo de l'utilisateur puis on retourne sur le profil
		$_SESSION['pseudo']=$_POST["user_name"]; 
		header('location:../profil.php');
	}
	else echo '<p style="color : red;">Pseudo déjà utilisé!</p>';
}

closeConnexion($link); // Ferme la connexion à la base de données
?>


<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>Nouveau Pseudo</title>
		<link  href="../lib/css/bootstrap.css" rel="stylesheet">
		<script src="../lib/js/jquery-3.3.1.min.js"></script>
		<script src="../lib/js/bootstrap.min.js"></script>
	</head>
	<body>

		<div class="mb-sm-4 mt-sm-5"> Connecté en tant que <?php echo $_SESSION['pseudo'] ?> </div>
		<div class="container">
			</br>
			<h1 style="text-align: center;">Nouveau Pseudo</h1>
			</br>
			<form action="chgmntPseudo.php" method="post">
				<div class="form-group">
					<label class="control-label col-sm-2" for="name">Nom :</label>
					<div class="col-sm-12">
					<input type="text" class="form-control" id="name" name="user_name">
					</div>
				</div>
				<div class="button form-checked col-sm-offset-3 col-sm-10">
					<button type="submit" class="btn btn-primary" name="valider" >Valider</button>
				</div>
				</br>
				<div class="form-group col-sm-offset-3 col-sm-10"> 
						<a href="../profil.php">Retour</a></div>
				</div>
			</form>	

		</div>

	</body>
</html>


