// genObstaclesDrone.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <Windows.h>

#define POSITIONX 1000
#define POSITIONY 1000

#define ORIENTATIONX 0
#define ORIENTATIONY 0
#define ORIENTATIONZ 0

#define SHAPEX 100
#define SHAPEY 100
#define SHAPEZ 50


using namespace std;
const char* nomFichier = "batiments.xml";

int main(int argc, char* argv[], char* envp[])
{
	if (argc != 2) {
		cout << "Usage : " << argv[0] << " <nombre d'obstacles>\n";
		exit(0);
	}
	else {
		cout << "Usage : " << argv[0] << " " << argv[1] << "\n";
	}

	int nbObstacles = atoi(argv[1]);
	string objet = "\t<object ";
	string position = "position=\"min ";
	string orientation = "orientation=\"";
	string shape = "shape=\"cuboid ";
	string materiel = "material=\"brick\" ";
	string color = "fill-color=\"255 100 50\" ";
	string opacity = "opacity=\"0.9\" ";
	string texture = "texture=\"brick.jpg\"/> ";
	string commentaire = "\t<!-- 10m de long / 50m de large / 10m de haut-->";
	string final = "";


	cout << "Génération de : " << nbObstacles << " obstacles\n";

	ofstream file;
	file.open(nomFichier, ios::out | ios::trunc);
	file << "<environment>" << endl;
	file << "\t<!-- Obstacles -->" << endl;

	// Initialisation du rand()
	srand(time(0));

	for (int i = 0; i < nbObstacles; i++) {
		int x, y, z;
		// position
		x = rand() % POSITIONX;
		y = rand() % POSITIONY;

		position += to_string(x) + " ";
		position += to_string(y);
		position += " 0\" ";

		// orientation
		if (ORIENTATIONX != 0 && ORIENTATIONY != 0 && ORIENTATIONZ != 0) {
			x = rand() % ORIENTATIONX;
			y = rand() % ORIENTATIONY;
			z = rand() % ORIENTATIONZ;
		}
		else {
			x = 0;
			y = 0;
			z = 0;
		}

		orientation += to_string(x) + " ";
		orientation += to_string(y) + " ";
		orientation += to_string(z) + "\" ";

		// shape
		x = rand() % SHAPEX + 10;
		y = rand() % SHAPEY + 10;
		z = rand() % SHAPEZ + 10;

		shape += to_string(x) + " ";
		shape += to_string(y) + " ";
		shape += to_string(z) + "\" ";

		// commentaire
		commentaire = "\t<!-- ";
		commentaire += to_string(x) + "m de long / ";
		commentaire += to_string(y) + "m de large / ";
		commentaire += to_string(z) + "m de haut";
		commentaire += "-->";

		// assemblage
		final = objet + position + orientation + shape + materiel + color + opacity + texture + commentaire;

		// écriture dans fichier
		file << final << endl;

		// réinitialisation des variables
		objet = "\t<object ";
		position = "position=\"min ";
		orientation = "orientation=\"";
		shape = "shape=\"cuboid ";
		materiel = "material=\"brick\" ";
		color = "fill-color=\"255 100 50\" ";
		opacity = "opacity=\"0.9\" ";
		texture = "texture=\"brick.jpg\"/> ";
		commentaire = "\t<!-- 10m de long / 50m de large / 10m de haut-->";
		final = "";
		//Sleep(1000);
	}


	//file << "\n" << endl;
	file << "</environment>" << endl;

	file.close();
	return EXIT_SUCCESS;
}