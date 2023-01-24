<?php
session_start();
require_once('../fonctions/bd.php');
require_once('../fonctions/user.php');


$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);
			
if (isset($_POST["valider"]))
{
    if (getUser($_SESSION['pseudo'], hash("md5", $_POST["user_pwd1"]), $link)) // On verifie que l'utilisateur existe (son pseudo et mdp existe)
    {
        if ($_POST["user_pwd1"] != $_POST["user_pwd2"]) // On verifie que l'ancien  mdp et le nouveau ne sont pas les mêmes
		{
            //On modifie le mot de passe de l'utilisateur dans la bd puis on retourne sur son profil
            setMDP(hash("md5", $_POST["user_pwd2"]),$_SESSION['pseudo'],$link);
            header('location:../profil.php');
        }
        else echo '<p style="color : red;"> Le nouveau mot de passe doit être différent de l\'ancien </p>';
    }
    else echo '<p style="color : red;"> Mot de passe actuel incorrect</p>';
}

closeConnexion($link); // Ferme la connexion à la base de données
?>


<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>Nouveau MDP</title>
		<link  href="../lib/css/bootstrap.css" rel="stylesheet">
		<script src="../lib/js/jquery-3.3.1.min.js"></script>
		<script src="../lib/js/bootstrap.min.js"></script>
	</head>
	<body>
		<div class="mb-sm-4 mt-sm-5"> Connecté en tant que <?php echo $_SESSION['pseudo'] ?> </div>
		<div class="container">
			</br>
			<h1 style="text-align: center;">Nouveau mot de passe</h1>
			</br>
			<form action="chgmntMdp.php" method="post">
			<div class="form-group">
					<label class="control-label col-sm-2" for="pwd1">MDP actuel :</label>
					<div class="col-sm-12">
						<input type="password" class="form-control" id="pwd1" name="user_pwd1"></textarea>
					</div>
				</div>
				<div class="form-group">
					<label class="control-label col-sm-2" for="pwd2">Nouveau mot de passe :</label>
					<div class="col-sm-12">
					<input type="password" class="form-control" id="name" name="user_pwd2">
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