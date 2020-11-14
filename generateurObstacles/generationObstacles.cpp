/**
* \date			14/11/2020
* \version		1.0
* \author		William PENSEC
* \description	Contenu de la classe generationObstacles. On y trouve le code des fonctions ainsi que les appels de fonction pour l'ex�cution du programme.
**/


/**
* \brief Inclusion des fichiers n�cessaire � l'ex�cution du code.
**/
#include "generationObstacles.h"

/**
* \brief Constructeur par d�faut de la classe.
* \detail Ce constructeur ne prend aucun param�tre mais appelle le second constructeur avec 2 noms de fichiers par d�faut.
**/
generationObstacles::generationObstacles(void) {
	string s1 = "map.osm";
	string s2 = "batiments.xml";
	generationObstacles(s1, s2);
}

/**
* \brief Constructeur secondaire de la classe � 2 param�tres.
* \detail Ce constructeur prend 2 param�tres et initialise les attributs de la classe
* \param[1] in : de type string, contient le nom du fichier d'entr�e dans lequel le programme doit lire les informations
* \param[2] out : de type string, contient le nom du fichier de sortie dans lequel le programme �crit le r�sultat
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
generationObstacles::~generationObstacles(void){}

/**
* \brief Fonction principale du programme.
* \detail Cette fonction r�cup�re le fichier le lit et construit le fichier de sortie.
* \return La valeur de retour de la fonction permet de savoir si le programme s'est bien ex�cut� avec EXIT_SUCCESS
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

	// On commence � remplir le fichier de sortie
	_fileOut << "<environment>" << endl;
	_fileOut << "\t<!-- Obstacles -->" << endl;

	// Lire le fichier xml
	vector<char> _buffer((istreambuf_iterator<char>(_fileIn)), istreambuf_iterator<char>());
	_buffer.push_back('\0');

	// Parser le fichier xml
	_doc.parse<0>(&_buffer[0]);
	_rootNode = _doc.first_node("osm");

	// R�cup�rer les informations du fichier xml et les stocker en forme dans le fichier de sortie
	for (xml_node<>* wayNode = _rootNode->first_node("way"); wayNode; wayNode = wayNode->next_sibling()) {
		for (xml_node<>* tagNode = wayNode->first_node("tag"); tagNode; tagNode = tagNode->next_sibling()) {
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
							}
						}
					}
				}
				if (_tabCoord.size() != 0) {
					writeFile();
					_tabCoord.clear();
					initVariablesString();
				}
			}
		}
	}

	// Fermeture du fichier de sortie
	_fileOut << endl << "</environment>" << endl;

	// G�n�ration du fond de map
	genereMap();

	// Fin programme et calcul temps d'ex�cution
	_end = clock();
	cout << "Temps d'execution : " << ((double)_end - _start) / CLOCKS_PER_SEC << " secondes" << endl;
	cout << "Fin programme" << endl;

	// Fermeture des fichiers et sortie du programme
	_fileIn.close();
	_fileOut.close();
	return EXIT_SUCCESS;
}

/**
* \brief Fonction permettant d'�crire le r�sultat dans le fichier de sortie
* \detail Cette fonction est appell�e dans la fonction startProg().
*	Elle r�cup�re les donn�es de latitude et longitude et les inscrit en mettant en forme dans le fichier de sortie.
**/
void generationObstacles::writeFile(void)
{
	_position += _tabCoord[0];
	_position += " 0.1\" ";
	for (auto i = _tabCoord.rbegin(); i != _tabCoord.rend(); ++i) {
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
* \brief Fonction permettant de g�n�rer une URL � partir de donn�es calcul�es pr�c�demment.
* \detail Cette fonction pr�pare l'URL qui sera appell�e apr�s.
*			Elle construit une chaine de caract�re � partir de donn�es d'un tableau.
*			Puis appelle la fonction requete() jusqu'� que la commande se soit bien ex�cut�e.
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
	cout << " Requ�te ex�cut�e !" << endl;
}

/**
* \brief Fonction permettant d'ex�cuter la commande afin de r�cup�rer le fond de map en xml.
* \detail Cette fonction v�rifie qu'il y a une connexion internet.
*			Si oui alors le programme ex�cute la commande g�n�r�e dans la fonction genereMap()
*			sinon elle affiche un message d'erreur et sort du programme entier.
* \return La valeur de retour de la fonction permet de savoir si le programme s'est bien ex�cut� avec EXIT_SUCCESS 
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
