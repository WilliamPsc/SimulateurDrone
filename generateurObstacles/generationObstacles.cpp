/**
* \date			19/11/2020
* \version		1.1
* \author		William PENSEC
* \description	Contenu de la classe generationObstacles. On y trouve le code des fonctions ainsi que les appels de fonction pour l'exécution du programme.
**/


/**
* \brief Inclusion des fichiers nécessaire à l'exécution du code.
**/
#include "generationObstacles.h"

/**
* \brief Variable globale pour afficher des messages afin de debuger le programme
* \description Si DEBUG = 1 alors le debug est en place sinon si il est à 0 alors le debug est desactivé
**/
#define DEBUG 0

/**
* \brief Constructeur par défaut de la classe.
* \detail Ce constructeur ne prend aucun paramètre mais appelle le second constructeur avec 2 noms de fichiers par défaut.
**/
generationObstacles::generationObstacles(void) {
	string s1 = "map.osm";
	string s2 = "batiments.xml";
	generationObstacles(s1, s2);
}

/**
* \brief Constructeur secondaire de la classe à 2 paramètres.
* \detail Ce constructeur prend 2 paramètres et initialise les attributs de la classe
* \param[1] in : de type string, contient le nom du fichier d'entrée dans lequel le programme doit lire les informations
* \param[2] out : de type string, contient le nom du fichier de sortie dans lequel le programme écrit le résultat
**/
generationObstacles::generationObstacles(string in, string out) {
	_nomIn = in;
	_nomOut = out;

	_start = 0;
	_end = 0;

	_fileIn.open(_nomIn, ios::in);
	_fileOut.open(_nomOut, ios::out | ios::trunc);

	_rootNode = NULL;
	_url = "";
	_command = NULL;

	initVariablesString();
}

/**
* \brief Destructeur de la classe.
**/
generationObstacles::~generationObstacles(void) {}

/**
* \brief Fonction principale du programme.
* \detail Cette fonction récupère le fichier le lit et construit le fichier de sortie.
* \return La valeur de retour de la fonction permet de savoir si le programme s'est bien exécuté avec EXIT_SUCCESS
**/
int generationObstacles::startProg(void)
{
	// Variables locales
	/*
		_tabMinMax[0][0] = minLon = 180.0;
		_tabMinMax[0][1] = minLat = 90.0;
		_tabMinMax[1][0] = maxLon = -180.0;
		_tabMinMax[1][1] = maxLat = -90.0;
	*/
	_tabMinMax[0][0] = 180.0;
	_tabMinMax[0][1] = 90.0;
	_tabMinMax[1][0] = -180.0;
	_tabMinMax[1][1] = -90.0;

	// Lancement du programme
	_start = clock();
	cout << "Debut programme" << endl;

	// On commence à remplir le fichier de sortie
	_fileOut << "<environment>" << endl;
	_fileOut << "\t<!-- Obstacles -->" << endl;

	// Lire le fichier xml
	vector<char> _buffer((istreambuf_iterator<char>(_fileIn)), istreambuf_iterator<char>());
	_buffer.push_back('\0');

	// Parser le fichier xml
	_doc.parse<0>(&_buffer[0]);
	_rootNode = _doc.first_node("osm");

	int sortie = 0;

	// Récupérer les informations du fichier xml et les stocker en forme dans le fichier de sortie
	for (xml_node<>* wayNode = _rootNode->first_node("way"); wayNode && !sortie; wayNode = wayNode->next_sibling()) {
		for (xml_node<>* tagNode = wayNode->first_node("tag"); tagNode && !sortie; tagNode = tagNode->next_sibling()) {
			if (strcmp(tagNode->first_attribute("k")->value(), "building") == 0) {
				for (xml_node<>* ndNode = wayNode->first_node("nd"); ndNode; ndNode = ndNode->next_sibling()) {
					if (strcmp(ndNode->name(), "nd") == 0) {
						for (xml_node<>* nodeNode = _rootNode->first_node("node"); nodeNode; nodeNode = nodeNode->next_sibling()) {
							if (strcmp(nodeNode->first_attribute("id")->value(), ndNode->first_attribute("ref")->value()) == 0) {
								string s1 = nodeNode->first_attribute("lat")->value();
								string s2 = nodeNode->first_attribute("lon")->value();
								string coord = s1 + " " + s2;
								_tabCoord.push_back(coord);

								if (_tabMinMax[0][0] > stod(s2)) _tabMinMax[0][0] = stod(s2);
								if (_tabMinMax[0][1] > stod(s1))_tabMinMax[0][1] = stod(s1);
								if (_tabMinMax[1][0] < stod(s2)) _tabMinMax[1][0] = stod(s2);
								if (_tabMinMax[1][1] < stod(s1)) _tabMinMax[1][1] = stod(s1);

								if (DEBUG) {
									cout << "Noeud : " << ndNode->first_attribute("ref")->value() << " | Latitude : " << nodeNode->first_attribute("lat")->value() << " | Longitude : " << nodeNode->first_attribute("lon")->value() << endl;
								}
							}
						}
					}
				}
				if (_tabCoord.size() != 0) {
					writeFile();
					_tabCoord.clear();
					_tabCoord.resize(0);
					initVariablesString();
				}
				if (DEBUG) {
					sortie = DEBUG;
					break;
				}
			}
		}
	}

	// Fermeture du fichier de sortie
	_fileOut << endl << "</environment>" << endl;

	// Génération du fond de map
	genereMap();

	// Fin programme et calcul temps d'exécution
	_end = clock();
	cout << "Temps d'execution : " << ((double)_end - _start) / CLOCKS_PER_SEC << " secondes" << endl;
	cout << "Fin programme" << endl;

	// Fermeture des fichiers et sortie du programme
	_fileIn.close();
	_fileOut.close();
	return EXIT_SUCCESS;
}

/**
* \brief Fonction permettant d'écrire le résultat dans le fichier de sortie
* \detail Cette fonction est appellée dans la fonction startProg().
*	Elle récupère les données de latitude et longitude et les inscrit en mettant en forme dans le fichier de sortie.
**/
void generationObstacles::writeFile(void)
{
	string delimiter = " ";
	string pos1, pos2;
	size_t position = 0;
	double tabLatMinLonMin[2];
	tabLatMinLonMin[0] = 90; // Latitude Min
	tabLatMinLonMin[1] = 180; // Longitude Min

	// On cherche la latitude et longitude minimale du batiment afin de le placer
	for (int i = 0; i < _tabCoord.size(); i++) {
		position = _tabCoord[i].find(delimiter);
		pos1 = _tabCoord[i].substr(position + delimiter.length(), _tabCoord[i].size());
		
		position = _tabCoord[i].find(delimiter);
		pos2 = _tabCoord[i].substr(0, position);

		if (tabLatMinLonMin[0] > stod(pos1)) tabLatMinLonMin[0] = stod(pos1);
		if (tabLatMinLonMin[1] > stod(pos2)) tabLatMinLonMin[1] = stod(pos2);
	}

	// On construit la chaine de caractère pour la position minimale
	_position += to_string(tabLatMinLonMin[1]);
	_position += " ";
	_position += to_string(tabLatMinLonMin[0]);
	_position += " 0\" ";

	if (DEBUG) {
		cout << "Valeur des positions : " << tabLatMinLonMin[1] << " " << tabLatMinLonMin[0] << endl;
		cout << "Position : " << _position << endl;
	}

	// On construit la chaine de caractère pour la forme de l'objet
	for (auto i = _tabCoord.rbegin(); i != _tabCoord.rend() - 1; ++i) {
		_shape += " ";
		_shape += *i;
	}
	_shape += "\" ";
	_final = _objet + _position + _orientation + _shape + _materiel + _color + _opacity;

	_fileOut << _final << endl;
}

/**
* \brief Fonction d'initialisation des variables afin de construire les lignes du fichier xml.
**/
void generationObstacles::initVariablesString(void)
{
	_objet = "\t<object ";
	_position = "position=\"min ";
	_orientation = "orientation=\"0 0 0\" ";
	_shape = "shape=\"prism 10";
	_materiel = "material=\"brick\" ";
	_color = "fill-color=\"255 100 50\" ";
	_opacity = "opacity=\"0.9\"/>";
	_final = "";
}

/**
* \brief Fonction permettant de générer une URL à partir de données calculées précédemment.
* \detail Cette fonction prépare l'URL qui sera appellée après.
*			Elle construit une chaine de caractère à partir de données d'un tableau.
*			Puis appelle la fonction requete() jusqu'à que la commande se soit bien exécutée.
**/
void generationObstacles::genereMap()
{
	int res = -10;
	_url = "curl -s -o fondMap.xml ";
	_url += "https://www.openstreetmap.org/api/0.6/map?bbox=";
	string pt1 = to_string(_tabMinMax[0][0]) + "%2C";
	pt1 += to_string(_tabMinMax[0][1]) + "%2C";
	string pt2 = to_string(_tabMinMax[1][0]) + "%2C";
	pt2 += to_string(_tabMinMax[1][1]);

	_url += pt1 + pt2;
	_command = _url.c_str();

	do {
		res = requete();
	} while (res != 0);
	cout << "Requete executee !" << endl;
}

/**
* \brief Fonction permettant d'exécuter la commande afin de récupérer le fond de map en xml.
* \detail Cette fonction vérifie qu'il y a une connexion internet.
*			Si oui alors le programme exécute la commande générée dans la fonction genereMap()
*			sinon elle affiche un message d'erreur et sort du programme entier.
* \return La valeur de retour de la fonction permet de savoir si le programme s'est bien exécuté avec EXIT_SUCCESS
*			ou sinon EXIT_FAILURE s'il n'y a pas de connexion internet
**/
int generationObstacles::requete()
{
	int val = system("ping -n 1 www.google.fr > nul");
	if (val == 0) {
		return system(_command);
	}
	else {
		cout << "Erreur, pas de connexion" << endl;
		exit(EXIT_FAILURE);
	}
}
