<!doctype html>
<html lang="fr">
    <body>
        <form action="index.php" method="post">
        <div class="btn-group-lg btn-group-vertical" role="group" aria-label="Basic example">
            <button type="submit" class="btn btn-light text-info" value="ajouter" name="ajout"> Ajouter une photo </button>

            <button type="submit" class="btn btn-light" value="profil" name="profil"> Mon profil </button>

            <button type="submit" class="btn btn-light text-secondary" value="deconnecter" name="deconnexion"> se déconnecter </button>

            <button type="submit" class="btn btn-light" value="gestion" name="gestion" style = 'display : <?php echo $display ?>;'><strong> Gestion </strong></button>
       </div>
     </form>
        
     </body>
<html>
