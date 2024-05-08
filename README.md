
#Paris trees

-L'objectif de ce programme est de calculer un minimum spanning tree d'un ensemble de points donnés.
Pour cela, j'ai procédé en plusieurs étapes :
	-Récupérer les données intéressantes d'un .CSV
	-Triangulation de Delaunay avec l'algorithme de Bowyer-Watson
	-Algorithme de Prim sur cette triangulation
	-Sauvegarde du minimum spanning tree

-Diverses options sont disponibles : 
				     -o chemin de sauvegarde du fichier binaire représentant la base de données
				     -i chemin du .CSV
				     -d chemin de sauvegarde du fichier binaire représentant la triangulation de Delaunay
				     -f chemin de sauvegarde du minimum spanning tree
				     -l chemin du fichier à charger pour reprendre le calcul ou afficher le minimum spanning tree
				     

-Format des fichiers binaires dans file.h

-Triangulation de Delaunay
	-Ajout d'un super triangle englobant tous les points à trianguler
	-Calcul de son cercle circonscrit
	-Pour chaque point à trianguler 
	 | chercher à quel(s) cercle(s) circonscrit il appartient
	 | chercher l'enveloppe délimitée par ces triangles (enlever les arêtes en communs)
	 | ajouter les triangles ainsi recalculés & calculer leur cercle circonscrit
	 Fin Pour
	-Supprimer les triangles ayant un sommet en commum avec le super triangle
    
    Remarque : L'algorithme fonctionne sauf pour quelques points, cela est dû au fait que le super triangle devrait s'étendre à l'infini
    Problème expliqué ici : https://stackoverflow.com/questions/30741459/bowyer-watson-algorithm-how-to-fill-holes-left-by-removing-triangles-with-sup 
    
-Algorithme de Prim
	-Pour chaque point i
	| Ajouter l'arête de poids minimum ayant un sommet dans l'ensemble déja relié et pour autre sommet i à l'arbre
	| Ajouter i au sommet déja reliés
	Fin Pour
	
-Partie graphique
	-clic droit pour zoomer
	-r pour reset
	
Avec cette méthode et mon ordinateur, le calcul du minimum spanning tree avec tous les points se fait en environ 35 minutes.

Remarque : Problème avec Valgrind au niveau de l'interface graphique (semble venir de tps.h)
