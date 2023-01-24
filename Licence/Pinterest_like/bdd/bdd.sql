SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";

--
-- Structure de la table `Categorie`
--


CREATE TABLE IF NOT EXISTS `Categorie`
(
	`catId` INT NOT NULL AUTO_INCREMENT,
    `nomCat` VARCHAR(250) NOT NULL,
   	PRIMARY KEY(`catId`)
)
ENGINE=InnoDB DEFAULT CHARSET=utf8;


--
-- Structure de la table `utilisateur`
--

CREATE TABLE IF NOT EXISTS `user` (
  `userId` INT NOT NULL AUTO_INCREMENT,
  `pseudo` varchar(255) NOT NULL,
  `mdp` varchar(255) NOT NULL,
  `etat` varchar(255) NOT NULL,
  `role` varchar(255) NOT NULL,
  PRIMARY KEY (`userId`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Structure de la table `Photo` 
--

CREATE TABLE IF NOT EXISTS `Photo`
(
	`photoId` INT NOT NULL AUTO_INCREMENT,
    `nomFich` VARCHAR(250) NOT NULL,
    `description` VARCHAR(250) NOT NULL,
    `catId` INT NOT NULL,
    `userId` INT NOT NULL,
    `aff` VARCHAR(250) NOT NULL,
    PRIMARY KEY(`photoId`),
    FOREIGN KEY (`catId`) References Categorie (`catId`),
    FOREIGN KEY (`userId`) References user (`userId`)
)
ENGINE=InnoDB DEFAULT CHARSET=utf8;
