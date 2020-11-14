/**
* \date			14/11/2020
* \version		1.0
* \author		William PENSEC
* \description Fichier main permettant de tester la classe generationObstacles.
**/

/**
* \brief Inclusion des fichiers n�cessaire � l'ex�cution du code. 
**/
#include "generationObstacles.h"

/**
* \brief Fonction main de la classe generationObstacles
* \detail Cette fonction ne prend aucun param�tre.
*         Elle d�clare un objet de type generationObstacles et appelle les fonctions de la classe afin de l'ex�cuter.
* \param NULL, mais peut �tre modifi� afin de prendre les noms des fichiers d'entr�e et sortie
* \return EXIT_SUCCESS en cas de bonne ex�cution du code sinon EXIT_FAILURE
**/
int main(int argc, char* argv[], char* envp[]) {

	// V�rifications arguments
	if (argc != 1) {
		cout << "Usage : " << argv[0] << endl;
		exit(0);
	}

	// Cr�ation de l'objet
	generationObstacles gO("map.osm", "batiments.xml");

	// Appel de la fonction principale
	int val = gO.startProg();

	// V�rification si la fonction s'est bien pass�e et sortie du programme
	if (val == EXIT_SUCCESS) {
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}