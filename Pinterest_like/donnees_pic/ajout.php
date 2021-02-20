<?php

session_start();
require_once('../fonctions/bd.php');
require_once('../fonctions/gestionPhotos.php');
require_once('../fonctions/pic.php');

$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);

if (isset($_POST["ajouter"]))//Si on clique sur le bouton "ajouter"
	{
		if ($_POST["description"] != "")//Si la description n'est pas vide
		{
			if ($_POST["CatName"] != "") //Si la catégorie n'est pas vide
			{
				if (isset($_POST["prive"])) //Si la checkbox "privé" est cochée, on n'affichera pas la photo aux autres
				{
					$aff = "non";
				}
				else $aff = "oui"; //Sinon, elle est en public

				$dossier = '../assets/images/';//Le dossier dans lequel on veut envoyer le fichier
				$taille_maxi = 100000;//La taille maxi acceptée
				$taille = filesize($_FILES['myFile']['tmp_name']); // On recupere la taille du fichier
				$extensions = array('.png', '.gif', '.jpeg', '.jpg');// Un tableau regroupant lees extensions acceptées
				$extension = strrchr($_FILES['myFile']['name'], '.');// L'extension du fichier

				if(!in_array($extension, $extensions)) //Si l'extension n'est pas dans le tableau
				{
					$erreur = 'Vous devez uploader un fichier de type png, gif, jpg, ou jpeg';
				}
				if($taille>$taille_maxi) //Si la taille de l'image est trop grande
				{
					$erreur = 'Le fichier est trop gros...';
				}
				if(!isset($erreur)) //S'il n'y a pas d'erreur, on upload
				{	 
					if (catNotExist($_POST["CatName"], $link)) //Si la catégorie n'exite pas 
					{
						addCat($_POST["CatName"], $link); //On en crée une nouvelle
					}

					if (isset($_POST["prive"]))//Si la checkbox est cochée 
					{
						$aff = "non"; //La photo est privée
					}
					else $aff = "oui";//Si la checkBox n'est pas cochée la photo est publique
					
					addPic($_POST["PicName"], $_POST["CatName"], $_POST["description"],$aff, $_SESSION['pseudo'], $link); // On ajoute la data de l'image dans la bd
					

					$fichier = "DSC-".getPhotoId($_POST["PicName"],$link)[0][0].$extension;
					if(move_uploaded_file($_FILES['myFile']['tmp_name'], $dossier . $fichier)) //Si la fonction renvoie TRUE, c'est que ça a fonctionné
					{
						header('location:../description.php?id='.getPhotoId($_POST["PicName"],$link)[0][0]); //On va sur la description de l'image
					}
					else{
			  	echo 'Echec de l\'upload !';
			  	deletePic(getPhotoId($_POST["PicName"],$link)[0][0], $link); //Si ça n'a pas fonctionné on retire la photo de la bd
				}
			}
			else
			{
				echo $erreur;
			}
		}
		else  echo '<p style="color : red;">Choississez ou créez une categorie</p>';
	}
	else  echo '<p style="color : red;">La description ne doit pas être vide</p>';
}

closeConnexion($link);
?>

<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>Ajout Image</title>
		<link rel="stylesheet" href="style.css">
		<link  href="../lib/css/bootstrap.css" rel="stylesheet">
		<script src="../lib/js/jquery-3.3.1.min.js"></script>
		<script src="../lib/js/bootstrap.min.js"></script>
	</head>
	<body>
		<div class="container">
			<div class="alert  alert-success" id="photoAjoutee" role="alert">
				<strong>Une photo a été ajoutée</strong>
			</div>
			<div class="mb-sm-4 mt-sm-5"> Connecté en tant que <?php echo $_SESSION['pseudo'] ?> </div>
			<h1 class="mb-sm-4" style="text-align: center;">Ajoutez une nouvelle photo</h1>
			<div class="col-auto">
			<span> Tous les champs sont obligatoires </span>
				<form action="ajout.php" method="post" enctype="multipart/form-data">
					<div class="form-group">
						<label for="myFile" class="control-label"><strong>Quelle photo souhaitez-vous ajouter ?</strong></label>
						<input type="file"  class="form-control-file" id="myFile" name="myFile">
					</div>
					<div class="form-group">
						<label for="picname" class="control-label">Nom de l'image :</label>
						<input type = "text"  class="form-control" name = "PicName" id = "picname">
					</div>
					<div class="form-group">
						<label for="name" class="control-label">Nom de la catégorie :</label>
						<input type = "text"  class="form-control" name = "CatName" id = "catname">
					</div>
					<div class="form-group">
						<label for="name" class="control-label">Description de l'image :</label>
						<textarea name = "description"  class="form-control" id = "desc"></textarea>
					</div>
					<div class="form-group">
						<label for="affichage">Mettre la photo en privé :</label>
						<div class="form-check">
							<input type="checkbox" class="form-check-input mr-sm-1" name="prive" value="prive">
							<label for="prive" class="form-check-label ml-sm-2">Photo privée</label>
						</div>
					</div>
					<div class="button">
						<button type="submit" class="btn btn-primary" name ="ajouter" onclick="message()" >Ajouter</button>
					</div>
				</form>
				<div class="mt-sm-2"> 
					<a href="../index.php">Retour</a>
				</div>
			</div>
        </div>
	</body>
    	
    	
    <script>
    	function message() {
			$("#photoAjoutee").fadeIn();
		 	setTimeout(fade, 3000);
		}

		function fade() {
      		$("#photoAjoutee").fadeOut();
      	}
              
        window.onload = function(){ //Message de reussite caché au debut
            $("#photoAjoutee").hide();
        };
    </script>
</html>
