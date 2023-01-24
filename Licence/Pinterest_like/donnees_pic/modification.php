<?php

session_start();
require_once('../fonctions/bd.php');
require_once('../fonctions/user.php');
require_once('../fonctions/gestionPhotos.php');
require_once('../fonctions/pic.php');

$link = getConnection ($dbHost, $dbUser, $dbPwd, $dbName);
if (isset($_GET["id"])) { // verifie que $_GET["id"] (id de la photo passé dans l'url) n'est pas vide
    $id = $_GET["id"];
	$desc = desc($id,$link);

    $nomFich = $desc[0][1];
    $description = $desc[0][2];
	$catid = $desc[0][3];
	$userid = $desc[0][4];
	$aff = $desc[0][5];

    $pseudo = getPseudo($userid,$link);
	$categorie = getCategorie($catid,$link);

	// SI on appuie sur le bouton "supprimer", on supprime la photo de la bd et de assest/images et on retourne sur la page d'accueil
	if(isset($_POST["supprimer"])) {
		deletePic($id,$link);
		foreach (glob("../assets/images/DSC-".$id.".*") as $filename) {
			unlink($filename);
		}
		header('location:../index.php');
	}

	// Avec 'required' les champs doivent être tous remplis pour être validés mais on vérifie quand même
	if (isset($_POST["modifier"]))
	{
		if ($_POST["PicName"] != "") // Si le nom de fichier n'est pas vide
		{
			if ($_POST["description"] != "") // Si la description n'est pas vide
			{
				if ($_POST["CatName"] != "") // Si la categorie n'est pas vide
				{
					if (isset($_POST["prive"]))//Si la checkbox est cochée 
					{
						$aff = "non"; //La photo est privée
					}
					else $aff = "oui";//Si la checkBox n'est pas cochée la photo est publique
					
					if (catNotExist($_POST["CatName"], $link)) // Si la catégorie n'existe pas ...
					{
						addCat($_POST["CatName"], $link); // ... on la crée
					}
					modifPic($_POST["PicName"], $_POST["description"], $_POST["CatName"], $aff, $id, $link);// On modifie les données dans la bd
					header('location:../description.php?id='.getPhotoId($_POST["PicName"],$link)[0][0]); // On est redirigé vers la description de la page de description photo
				}
				else  echo '<p style="color : red;">Remplissez bien la catégorie</p>';
			}
			else  echo '<p style="color : red;">Remplissez bien la description</p>';
		}
		else  echo '<p style="color : red;">Remplissez bien le nom de votre photo</p>';
	}

}
else {
    header('Location: ../index.php'); // Sinon on retourne sur la page d'accueil
}

closeConnexion($link); // Ferme la connexion à la base de données

?>

<!doctype html>
<html lang="fr">
	<head>
		<meta charset="utf-8">
		<title>Modifier les détails</title>
		<link  href="../lib/css/bootstrap.css" rel="stylesheet">
		<script src="../lib/js/jquery-3.3.1.min.js"></script>
		<script src="../lib/js/bootstrap.min.js"></script>
	</head>
	<body>

		<!-- Modal -->
		<div class="modal fade" id="modalSupprimer" tabindex="-1" role="dialog" aria-labelledby="Supprime" aria-hidden="true">
			<div class="modal-dialog modal-dialog-centered" role="document">
				<div class="modal-content">
					<div class="modal-header">
						<h5 class="modal-title" id="Supprime">Etes-vous sûr de vouloir supprimer ?</h5>
						<button type="button" class="close" data-dismiss="modal" aria-label="Close">
							<span aria-hidden="true">&times;</span>
						</button>
					</div>
					<div class="modal-footer">
						<button type="button" class="btn btn-secondary" id="confirmNon" data-dismiss="modal">Non</button>
						<button type="submit" form="formPhoto" name="supprimer" class="btn btn-primary" id="confirmOui" >Oui</button>
					</div>
				</div>
			</div>
		</div>

       	<div class="container">
		   <div class="mb-sm-4 mt-sm-5"> Connecté en tant que <?php echo $_SESSION['pseudo'] ?> </div>
			<h1 class="mb-sm-3">Modifiez votre photo nommée <strong><?php echo $nomFich; ?></strong></h1>
			<form action="modification.php?id=<?php echo $id; ?>" id="formPhoto" method="post" enctype="multipart/form-data">
			   	<div class="form-group form-inline">
			   		<?php
					   	$dossier = "../assets/images/";
						if(is_dir($dossier)) { // si c'est bien un dossier
						  	if ($dir = opendir($dossier)) { // s'il s'ouvre bien
							   	while($file = readdir($dir)) { // Tant qu'il y a des fichiers
									$nameFile = stristr($file,'.',true); // nom du fichier file sans l'extension (ex : 'nom.png' devient 'nom')
									if("DSC-" . $id== $nameFile) print '<img src="../assets/images/DSC-'.$id.stristr($file,'.').'" class="col-sm-2 img-thumbnail" alt="'.$nameFile.'" name="'.$file.'">';
								}
							}
						}
											   
					?>
        	    </div>
        	    <div class="form-group">
        	    	<label for="picname">Nom de l'image :</label>
        	    	<input type = "text" class="form-control" name = "PicName" id = "picname" value ="<?php echo $nomFich; ?>" required>
        		</div>
            	<div class="form-group">
    	    		<label for="CatName">Nom de la catégorie :</label>
    	    		<input type = "text" class="form-control" name = "CatName" id = "catname" value ="<?php echo $categorie[0][0]; ?>" required>
        	    </div>
        		<div class="form-group">
            		<label for="description">Description de l'image :</label>
    	    		<textarea name = "description" class="form-control" id = "desc" required><?php echo $description; ?></textarea>
    	    	</div>
				<div class="form-group">
					<label for="affichage">Mettre la photo en privé :</label>
					<div class="form-check">
						<?php if($aff == "non") {
							print '<input type="checkbox" class="form-check-input mr-sm-1" name="prive" value="prive" checked>
							<label for="prive" class="form-check-label ml-sm-2">Photo privée</label>';
						}
						else {
							print '<input type="checkbox" class="form-check-input" name="prive" value="prive">
							<label for="prive" class="form-check-label ml-sm-2">Photo privée</label>';
						}
  						?>
					</div>
    	    	</div>
        		<div class="button">
            		<input type="submit" class="btn btn-primary" name="modifier" value="Modifier">
					<button type="button" class="btn confirmSuppModal" data-toggle="modal" data-target="#modalSupprimer">Supprimer</button>
            	</div>
    	
        	</form>
        	<div class="mt-sm-2"> 
            	<a href="../index.php">Retour</a>
			</div>
    	</div>
    </body>
</html>
