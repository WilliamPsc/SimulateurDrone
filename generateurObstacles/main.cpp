/**
* \date			14/11/2020
* \version		1.0
* \author		William PENSEC
* \description Fichier main permettant de tester la classe generationObstacles.
**/

/**
* \brief Inclusion des fichiers nécessaire à l'exécution du code. 
**/
#include "generationObstacles.h"

/**
* \brief Fonction main de la classe generationObstacles
* \detail Cette fonction ne prend aucun paramètre.
*         Elle déclare un objet de type generationObstacles et appelle les fonctions de la classe afin de l'exécuter.
* \param NULL, mais peut être modifié afin de prendre les noms des fichiers d'entrée et sortie
* \return EXIT_SUCCESS en cas de bonne exécution du code sinon EXIT_FAILURE
**/
int main(int argc, char* argv[], char* envp[]) {

	// Vérifications arguments
	if (argc != 1) {
		cout << "Usage : " << argv[0] << endl;
		exit(0);
	}

	// Création de l'objet
	generationObstacles gO("map.osm", "batiments.xml");

	// Appel de la fonction principale
	int val = gO.startProg();

	// Vérification si la fonction s'est bien passée et sortie du programme
	if (val == EXIT_SUCCESS) {
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}