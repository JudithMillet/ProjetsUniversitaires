/**
 * Auteurs :
 * GINDA Maxime
 * MILLET Judith 11811933
 */


-- Question 6 : création de la nouvelle BD -- 

/*
drop table if exists "olfaction-csv" cascade;

create table "olfaction-csv" (
    "subject" int,
    "age" int,
    "gender" text,
    "smoker" boolean,
    "hunger" int,
    "origin" text,
    cid int,
    "chemical family" text,
    "dilution % (vol/vol)" text,
    "block" text,
    "fatigue" int,
    "order_in_block" int,
    "odorants" text,
    "fp_before_peaks.nb(10secbefore)" int,
    "fp_after_peaks.nb(30secbefore)" int,
    "ar_nb_before(10secbefore)" int,
    "ar_nb_after(30secbefore)" int,
    "st_before_mean(10secbefore)" numeric,
    "st_after_mean(30secbefore)" numeric,
    "hedonic1" int,
    "intensity1" int,
    "relaxation1" int,
    "stress1" int,
    "anxiety1" int,
    "experimental_notes" text,
    "intensity2" numeric,
    "hedonic2" numeric,
    "familiarity2" numeric,
    "edibility2" numeric,
    "disgust" numeric,
    "surprise" numeric,
    "neutral" numeric,
    "pleasure" numeric,
    "joy" numeric
);

-- Charger les données dans la table "olfaction-csv" :

\copy "olfaction-csv"("subject", "age", "gender", "smoker", "hunger", "origin", cid, "chemical family", "dilution % (vol/vol)", "block", "fatigue", "order_in_block", "odorants",
    "fp_before_peaks.nb(10secbefore)", "fp_after_peaks.nb(30secbefore)", "ar_nb_before(10secbefore)", "ar_nb_after(30secbefore)", "st_before_mean(10secbefore)", "st_after_mean(30secbefore)", 
    "hedonic1", "intensity1", "relaxation1", "stress1", "anxiety1", "experimental_notes", "intensity2", "hedonic2", "familiarity2", "edibility2", "disgust", "surprise", "neutral", "pleasure", "joy")
     from 'olfaction.csv' delimiter ','  encoding 'utf-8' csv header
*/


/** réparation des erreurs **/
update "olfaction-csv" set "Dilution % (vol/vol)"=100 where "Dilution % (vol/vol)" like 'pure';
update "olfaction-csv" set "Dilution % (vol/vol)"=100 where "Dilution % (vol/vol)" like '';
ALTER TABLE "olfaction-csv" ALTER COLUMN "Dilution % (vol/vol)" TYPE float4 USING "Dilution % (vol/vol)"::float4;
update "olfaction-csv" set odorants='γ-Decanolactone' where cid=12813;

/** création du nouveau modèle **/
drop table if exists Subject cascade;
create table Subject(
	subject int UNIQUE not null primary key,
	age int,
	gender char(1) check (gender = 'f' or gender = 'm'),
	smoker int check (smoker = 0 or smoker = 1),
	origin varchar(50),
	hunger int check (smoker = 0 or smoker = 1)
);

INSERT INTO subject (subject, age, gender, smoker, origin, hunger) SELECT distinct (subject), age, gender, smoker, origin, hunger FROM "olfaction-csv" order by subject ;


drop table if exists Odorants cascade;
create table Odorants(
	CID int unique not null,
	Odorants varchar(64),
	"Chemical Family" Varchar(1024),
	Dilution varchar(1024),
	primary key(CID)
);

INSERT INTO Odorants (CID, Odorants, "Chemical Family", Dilution) SELECT distinct (CID), odorants, "Chemical Family", "Dilution % (vol/vol)" FROM "olfaction-csv" order by CID;


drop table if exists activite_physiologique cascade;
CREATE TABLE activite_physiologique (
	subject integer,
	cid integer,
	"FP_Before_peaks.nb" integer,
	"AR_nb_before" integer,
	"ST_Before_mean" real,
	"FP_After_peaks.nb" integer,
	"AR_nb_After" integer,
	"ST_After_mean" real,
	fatigue integer check (fatigue >= 0),
	primary key(subject,cid),
	foreign key(subject) references Subject(subject),
	foreign key(cid) references Odorants(cid)
);

INSERT INTO activite_physiologique (subject, cid, "FP_Before_peaks.nb", "AR_nb_before", "ST_Before_mean", "FP_After_peaks.nb", "AR_nb_After", "ST_After_mean")  
select  distinct(subject), cid, "FP_Before_peaks.nb(10secBefore)", "AR_nb_before(10secBefore)", "ST_Before_mean(10secBefore)", "FP_After_peaks.nb(30secBefore)", "AR_nb_after(30secBefore)", "ST_After_mean(30secBefore)" 
FROM "olfaction-csv" order by Subject;


drop table if exists Resultat_Session1 cascade;
CREATE TABLE Resultat_Session1 (
	subject integer,
	cid integer,
	hedonic1 integer check (hedonic1 >= 0 and hedonic1 <= 9),
	intensity1 integer check (intensity1 >= 0 and intensity1 <= 9),
	relaxation1 integer check (relaxation1 >= 0 and relaxation1 <= 9),
	stress1 integer check (stress1 >= 0 and stress1 <= 9 ),
	anxiety1 integer check (anxiety1 >= 0 and anxiety1 <= 9),
	block character varying(2) not null,
	Order_in_Block integer not null,
	primary key (subject, cid),
	foreign key(subject) references Subject(subject),
	foreign key(cid) references Odorants(cid)
);

INSERT INTO Resultat_Session1 (subject, cid, hedonic1, intensity1, relaxation1, stress1, anxiety1, block, Order_in_Block)  
select  distinct(subject), cid, hedonic1, intensity1, relaxation1, stress1, anxiety1, block, Order_in_Block
FROM "olfaction-csv" order by Subject;


drop table if exists Resultat_Session2 cascade;
create table Resultat_Session2 (
   	subject integer,
	cid integer,
    hedonic2 real check (hedonic2 >= 0.0 and hedonic2 <= 1.0 ),
	intensity2 real check (intensity2 >= 0.0 and intensity2 <= 1.0 ),
	familiarity2 real check (familiarity2 >= 0.0 and familiarity2 <= 1.0 ),
	edibility2 real check (edibility2 >= 0.0 and edibility2 <= 1.0 ),
	disgust real check (disgust >= 0.0 and disgust <= 1.0 ),
	surprise real check (surprise >= 0.0 and surprise <= 1.0 ),
	neutral real check (neutral >= 0.0 and neutral <= 1.0 ),
	pleasure real check (pleasure >= 0.0 and pleasure <= 1.0 ),
	joy real check (joy >= 0.0 and joy <= 1.0 ),
	experimental_notes text,
	block character varying(2) not null,
	Order_in_Block integer not null,
	primary key (subject, cid),
	foreign key(subject) references Subject(subject),
	foreign key(cid) references Odorants(cid)
);

INSERT INTO Resultat_Session2 (subject, cid, hedonic2, intensity2, familiarity2, edibility2, disgust, surprise, neutral,pleasure,joy,experimental_notes,block,Order_in_Block)  
select  distinct(subject), cid, hedonic2, intensity2, familiarity2, edibility2, disgust, surprise, neutral,pleasure,joy,experimental_notes,block,Order_in_Block
FROM "olfaction-csv" order by Subject;

-- Question 7 --
select distinct subject from Resultat_Session1 where stress1 = (select max(stress1) from Resultat_Session1);

-- Question 8 --
select distinct s.subject, o.odorants from Subject s join activite_physiologique ap on s.subject=ap.subject join Odorants o on ap.cid=o.cid where (ap."FP_After_peaks.nb" - ap."FP_Before_peaks.nb") = (select max("FP_After_peaks.nb" - "FP_Before_peaks.nb") from activite_physiologique);

-- Question 9 --

-- Pour cette fonction, on crée un type 'stats' qui est composé de 4 variables numériques afin de renvoyer les 4 informations (min, max, avg et stddev). 'stats' est le type de retour qu'on utilise dans cette fonction.
-- EXECUTE permet d'executer des requètes SQL dynamiquement dans la fonction. Le résultat est stocké dans une variable grâce à l'expression INTO. Cela permet de ne stocker qu'une information à la fois, d'où les 4 utilisations de EXECUTE.

/** Création du type de renvoie Stats (utile pour la question 9 & 10) **/
drop type if exists stats cascade;
Create type stats AS (vmin numeric, vmax numeric, moy numeric, ecart numeric);

/** Fonction main_statistics qui prends un odorants et un attribut physiologique **/
create or replace function main_statistics(od text, ap text)
    returns stats
as $BODY$
    declare
   	 qmin numeric := null;
   	 qmax numeric := null;
   	 qavg numeric := null;
   	 qstd numeric := null;
    begin
   	 execute 'select min(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid where o.odorants like ''' || od || '''' into qmin;
   	 execute 'select max(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid where o.odorants like ''' || od || '''' into qmax;
   	 execute 'select avg(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid where o.odorants like ''' || od || '''' into qavg;
   	 execute 'select stddev(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid where o.odorants like ''' || od || '''' into qstd;
   	 return (qmin, qmax, qavg, qstd);
    end
$BODY$
language plpgsql;

select main_statistics('androstenol','FP_After_peaks.nb');

-- Question 10 --
-- Le type stats est réutilisé comme type de retour.
-- Ici, on utilise un id comme incrémenteur de la boucle. Il est égal à chaque valeur de l'attribut ai parametré, d'où 'ai%TYPE' comme type (le type de l'attribut renseigné).

/** Fonction main_statistics_groupe qui prends un odorants, un attribut physiologique et attribut relatif à l'individu **/

create or replace function main_statistics_groupe(od text, ap text, ai text)
    returns setof stats
as $BODY$
    declare
   	 qmin numeric := null;
   	 qmax numeric := null;
   	 qavg numeric := null;
   	 qstd numeric := null;
   	 id ai%TYPE;
    begin
   	 for id in execute 'select distinct ' || ai ||' from Subject order by ' || ai
   	 loop
   		 execute 'select min(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid join Subject s on aps.subject=s.subject where o.odorants like ''' || od || ''' and s.' || ai || '=''' || id || ''' ' into qmin;
   		 execute 'select max(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid join Subject s on aps.subject=s.subject where o.odorants like ''' || od || ''' and s.' || ai || '=''' || id || ''' ' into qmax;
   		 execute 'select avg(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid join Subject s on aps.subject=s.subject where o.odorants like ''' || od || ''' and s.' || ai || '=''' || id || ''' ' into qavg;
   		 execute 'select stddev(aps."' || ap || '") from activite_physiologique aps join Odorants o on aps.cid = o.cid join Subject s on aps.subject=s.subject where o.odorants like ''' || od || ''' and s.' || ai || '=''' || id || ''' ' into qstd;
   		 raise notice 'id est ici : %',id;
   		 return next (qmin,qmax,qavg,qstd);
   	 end loop;
   	 return;
    end
$BODY$
language plpgsql;

select * from main_statistics_groupe('androstenol','FP_After_peaks.nb','smoker');


/** Création de la table où se trouveront les données discretisées **/
drop table if exists Discretisation cascade;

CREATE table Discretisation (
	subject integer,
	cid integer,
	hedonic1 varchar,
	intensity1 varchar,
	relaxation1 varchar,
	stress1 varchar,
	anxiety1 varchar,
	foreign key(subject) references Subject(subject),
	foreign key(cid) references Odorants(cid)
);


insert into Discretisation (subject, cid) (select distinct subject,cid from Resultat_Session1 order by subject);


-- a --

create or replace procedure discretise_echelle()
as $BODY$
declare
	 id int;
	 value int;
	 cidp int;
 begin
	 /** BOUCLE FOR pour chaque subject **/
	 for id in select distinct subject from Discretisation 
	 loop
		 /** BOUCLE FOR pour chaque CID **/
		 for cidp in select cid from Discretisation where subject = id
		 loop  	 
   			 /** BOUCLE POUR anxiety1 **/
   			 for value in select anxiety1 from Resultat_Session1 where subject = id and cid = cidp
   			 loop
       			 UPDATE Discretisation set anxiety1 = (
       				 case
           				 when value <= 3 then 'peu'
           				 when value >= 7 then 'fort'
           			 else 'moyen'
       				 end ) WHERE subject=id and cid=cidp;
   			 end loop;
 
   			 /** BOUCLE POUR Stress1 **/
   			 for value in select stress1 from Resultat_Session1 where subject = id and cid = cidp
   			 loop
       			 UPDATE Discretisation set stress1 = (
       				 case
           				 when value <= 3 then 'peu'
           				 when value >= 7 then 'fort'
           			 else 'moyen'
       			 end ) WHERE subject=id and cid = cidp;
   			 end loop;

   			 /** BOUCLE POUR intensity1 **/
   			 for value in select intensity1 from Resultat_Session1 where subject = id and cid = cidp
   			 loop
       			 UPDATE Discretisation set intensity1 = (
       				 case
           				 when value <= 3 then 'peu'
           				 when value >= 7 then 'fort'
           			 else 'moyen'
       			 end ) WHERE subject=id and cid = cidp;
   			 end loop;
 
   			 /** BOUCLE POUR relaxation1 **/
   			 for value in select relaxation1 from Resultat_Session1 where subject = id and cid = cidp
   			 loop
       			 UPDATE Discretisation set relaxation1 = (
       				 case
           				 when value <= 3 then 'peu'
           				 when value >= 7 then 'fort'
           			 else 'moyen'
       			 end ) WHERE subject=id and cid = cidp;
   			 end loop;
 
   			 /** BOUCLE POUR hedonic1 **/
   			 for value in select hedonic1 from Resultat_Session1 where subject = id and cid = cidp
   			 loop
       			 UPDATE Discretisation set hedonic1 = (
       				 case
           				 when value <= 3 then 'peu'
           				 when value >= 7 then 'fort'
           			 else 'moyen'
       			 end ) WHERE subject=id and cid = cidp;
   			 end loop;
		 end loop;
	 end loop;
end
$BODY$
language plpgsql;


-- b --

create or replace procedure discretisations_percentile()
as $BODY$
declare 
	id int;
	value int;
	cidp int;
	anxiety_perc_tier1 numeric;
	stress1_perc_tier1 numeric;
	intensity1_perc_tier1 numeric;
	relaxation1_perc_tier1 numeric;
	hedonic1_perc_tier1 numeric;
	anxiety_perc_tier3 numeric;
	stress1_perc_tier3 numeric;
	intensity1_perc_tier3 numeric;
	relaxation1_perc_tier3 numeric;
	hedonic1_perc_tier3 numeric;
begin
	/** CALCULE DES PERCENTILES ***/
	anxiety_perc_tier1 = PERCENTILE_CONT ( 0.33 )  WITHIN GROUP ( ORDER BY anxiety1 ) from resultat_session1;
	anxiety_perc_tier3 = PERCENTILE_CONT ( 0.99 )  WITHIN GROUP ( ORDER BY anxiety1 ) from resultat_session1;
	stress1_perc_tier1 = PERCENTILE_CONT ( 0.33 )  WITHIN GROUP ( ORDER BY stress1 ) from resultat_session1;
	stress1_perc_tier3 = PERCENTILE_CONT ( 0.99 )  WITHIN GROUP ( ORDER BY stress1 ) from resultat_session1;
	intensity1_perc_tier1 = PERCENTILE_CONT ( 0.33 )  WITHIN GROUP ( ORDER BY intensity1 ) from resultat_session1;
	intensity1_perc_tier3 = PERCENTILE_CONT ( 0.99 )  WITHIN GROUP ( ORDER BY intensity1 ) from resultat_session1;
	relaxation1_perc_tier1 = PERCENTILE_CONT ( 0.33 )  WITHIN GROUP ( ORDER BY relaxation1 ) from resultat_session1;
	relaxation1_perc_tier3 = PERCENTILE_CONT ( 0.99 )  WITHIN GROUP ( ORDER BY relaxation1 ) from resultat_session1;
	hedonic1_perc_tier1 = PERCENTILE_CONT ( 0.33 )  WITHIN GROUP ( ORDER BY hedonic1 ) from resultat_session1;
	hedonic1_perc_tier3 = PERCENTILE_CONT ( 0.99 )  WITHIN GROUP ( ORDER BY hedonic1 ) from resultat_session1;
	
	/** BOUCLE FOR pour chaque subject **/
	for id in select distinct subject from subject
	loop 
	
		/** BOUCLE FOR pour chaque CID **/
		for cidp in select CID from resultat_session1 where subject = id 
		loop 
			
			/** BOUCLE POUR ANXIETY1 **/
			for value in select anxiety1 from resultat_session1 where subject = id and cid = cidp
			loop
				
				UPDATE discretisation set anxiety1 = (
				case 
					when value <= anxiety_perc_tier1 then 'peu'
					when value >= anxiety_perc_tier3 then 'fort'
					else 'moyen'
				end ) WHERE subject=id and cid = cidp;
			end loop;
		
			/** BOUCLE POUR Stress1 **/
			for value in select stress1 from resultat_session1 where subject = id and cid = cidp
			loop
				UPDATE discretisation set stress1 = (
				case 
					when value <= stress1_perc_tier1 then 'peu'
					when value >= stress1_perc_tier3 then 'fort'
					else 'moyen'
				end ) WHERE subject=id and cid = cidp;
			end loop;
		
		
			/** BOUCLE POUR intensity1 **/
			for value in select intensity1 from resultat_session1 where subject = id and cid = cidp
			loop
				UPDATE discretisation set intensity1 = (
				case 
					when value <= intensity1_perc_tier1 then 'peu'
					when value >= intensity1_perc_tier3 then 'fort'
					else 'moyen'
				end ) WHERE subject=id and cid = cidp;
			end loop;
		
			/** BOUCLE POUR relaxation1 **/
			for value in select relaxation1 from resultat_session1 where subject = id and cid = cidp
			loop
				UPDATE discretisation set relaxation1 = (
				case 
					when value <= relaxation1_perc_tier1 then 'peu'
					when value >= relaxation1_perc_tier3 then 'fort'
					else 'moyen'
				end ) WHERE subject=id and cid = cidp;
			end loop;
		
			/** BOUCLE POUR hedonic1 **/
			for value in select hedonic1 from resultat_session1 where subject = id and cid = cidp
			loop
				UPDATE discretisation set hedonic1 = (
				case 
					when value <= hedonic1_perc_tier1 then 'peu'
					when value >= hedonic1_perc_tier3 then 'fort'
					else 'moyen'
				end ) WHERE subject=id and cid = cidp;
			end loop;

		end loop;
	end loop;
end
$BODY$
language plpgsql;


-- c --

-- On crée 3 tables pour les 3 clusters
drop table if exists cluster1;
drop table if exists cluster2;
drop table if exists cluster3;
drop table if exists Discretisation_Clusters;

create table cluster1 (
	nb integer
);
create table cluster2(
	nb integer
);

create table cluster3(
	nb integer
);

-- On crée une table pour la discrétisation des clusters qui est une copie de Resultat_Session1
create table Discretisation_Clusters as table Resultat_Session1;

-- Cette fonction va permettre d'attribuer 'peu', 'moyen' ou 'fort' en fonction de la valeur, et des 3 moyennes des clusters
create or replace function discretise_par_val(val integer,clust1 numeric,clust2 numeric,clust3 numeric)
returns text
as $$
	begin
		-- On vérifie que la valeur est entre 0 et 9
	    if (val>=0) and (val<10) then
	        clust1 = abs(clust1-val); -- les clust prennent comme valeur |clust - val|
	        clust2 = abs(clust2-val);
	        clust3 = abs(clust3-val);
	       -- On attribue un mot à val en fonction du cluster
	        if (clust1<=clust2) and (clust1<=clust3) then
	            return 'peu';
	        elsif (clust2<=clust1) and (clust2<=clust3) then
	            return 'moyen';
	        elsif (clust3<=clust1) and (clust3<=clust2) then
	            return 'fort';
	        else
	            return 'Mauvaise valeur';
	        end if;
	    else
	        return 'Mauvaise valeur';
	   	end if;
	end
$$ 
language plpgsql;


create or replace function discretise_val_par_attribut(attribut text)
    returns void
as $BODY$
	declare
	    diverge boolean;
	   	maxtour numeric;
	    nbval integer;
    	moyclust1 numeric;
    	moyclust2 numeric;
   		moyclust3 numeric;
    	transimoyclust1 numeric;
    	transimoyclust2 numeric;
   		transimoyclust3 numeric;
   	   	i int;
   		val int;

	begin
		diverge = true;
	    maxtour = 0;
	   -- On initialise les moyennes des clusters "aléatoirement" pour le premier calcul, soit 3, 6 et 9
		moyclust1 := 3;
		moyclust2 := 6;
		moyclust3 := 9;
		
		-- nbval correspond au nombre de valeur dans un attribut
	    execute 'select count (' || attribut || ') from Resultat_Session1' into nbval;
	   -- On répète les calculs tant qu'on n'a pas de convergence, en mettant un nombre afin d'éviter d'avoir trop de calculs
	    while diverge and maxtour<8 loop
	    	for i in 0..nbval-1
	        	loop
	            	execute 'select ' || attribut || ' from (select row_number() over(order by ' || attribut || ') as rownumber, ' || attribut || ' from Resultat_Session1) as row where rownumber = ' || i || '+1' into val;
	                if (val>0) then
	   		            transimoyclust1 = abs(moyclust1-val);
	                    transimoyclust2 = abs(moyclust2-val);
	                    transimoyclust3 = abs(moyclust3-val);
	                    if (transimoyclust1<=transimoyclust2) and (transimoyclust1<=transimoyclust3) then
	                        insert into cluster1(nb) values (val);
	                    elsif (transimoyclust2<=transimoyclust1) and (transimoyclust2 <= transimoyclust3) then
	                       	insert into cluster2(nb) values (val);
	                    elsif (transimoyclust3<=transimoyclust1) and (transimoyclust3<=transimoyclust2) then
	                        insert into cluster3(nb) values (val);
	                    end if;
	                end if;
	            end loop;
	       	transimoyclust1 = (select avg(nb) from cluster1);
	       	transimoyclust2 = (select avg(nb) from cluster2);
	        transimoyclust1 = (select avg(nb) from cluster3);
	    	   -- Si les moyennes de clusters sont égales aux moyennes des clusters après calcul, alors il y a convergence donc on va sortir du while
	        if moyclust1=transimoyclust1 and moyclust2=transimoyclust2 and moyclust3=transimoyclust3 then
	            diverge = false;
            end if;
            moyclust1 = transimoyclust1;
            moyclust2 = transimoyclust2;
	        moyclust3 = transimoyclust3;
	        delete from cluster1 where 0=0;
            delete from cluster2 where 0=0;
            delete from cluster3 where 0=0;
            maxtour = maxtour + 1;
        end loop;
       -- On modifie alors la table de discretisation en mettant dans chaque valeur d'attribut le mot corrrespondant
		execute 'alter table Discretisation_Clusters alter column ' || attribut || ' type text using discretise_par_val(Discretisation_Clusters.' || attribut || ', ' || moyclust1 || ', ' || moyclust2 || ', ' || moyclust3 || ')';
	end
$BODY$
language plpgsql;


create or replace procedure discretise_clustering()
as $BODY$
	declare
	    chaque_val text;
	begin
		-- Pour chaque valeur des colonnes 'hedonic1','intensity1','relaxation1','stress1','anxiety1' on les discrétise
	    foreach chaque_val in array array['hedonic1','intensity1','relaxation1','stress1','anxiety1']
	    loop
	        execute discretise_val_par_attribut(chaque_val);
	    end loop;
	end
$BODY$ 
language plpgsql;

-- On peut maintenant supprimer les 3 tables des clusters
drop table cluster1;
drop table cluster2;
drop table cluster3;


/** Appels à 3 différentes discrétisations, décommenter la méthode choisie **/
--call discretise_echelle();
--call discretisations_percentile();
--call discretise_clustering();


--select * from discretisation;

