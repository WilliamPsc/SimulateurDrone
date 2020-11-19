/**
* \date			19/11/2020
* \version		1.1
* \author		William PENSEC
* \description	Header de la classe generationObstacles. On y trouve les déclarations des fonctions et des attributs de la classe.
**/


/**
* \brief Inclusion des fichiers nécessaire à l'exécution du code.
**/
// Inclusion des headers génériques C++
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <cstdlib>
// Inclusion des headers de la classe rapidxml afin de parser le fichier d'entrée
#include "rapidxml/rapidxml.hpp"

/**
* \brief Utilisation des namespace afin de ne pas à avoir remettre le nom à chaque fois.
**/
using namespace std;
using namespace rapidxml;

/**
* \brief Déclaration de la classe avec attributs et fonctions propres
**/
#pragma once
class generationObstacles {
public:
	/* Constructeurs */
	generationObstacles(void);
	generationObstacles(string in, string out);

	/* Destructeur */
	virtual ~generationObstacles(void);

	/* Fonctions */
	int startProg();

protected :
	/* Fonctions utilisées par startProg() */
	void writeFile();
	void initVariablesString();
	int requete();
	void genereMap();

private:
	/* Attributs */
	// Nom des fichiers E/S
	string _nomIn;
	string _nomOut;

	// Attributs généraux du programme
	clock_t _start, _end;
	ifstream _fileIn;
	ofstream _fileOut;
	xml_document<> _doc;
	xml_node<>* _rootNode;
	vector<string> _tabCoord;
	double _tabMinMax[2][2];

	// Attributs de la chaîne de caractère
	string _objet;
	string _position;
	string _orientation;
	string _shape;
	string _materiel;
	string _color;
	string _opacity;
	string _final;

	// Attributs pour la requete HTTP
	string _url;
	const char* _command;
};