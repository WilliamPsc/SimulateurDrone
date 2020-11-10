/**
* @date			05/11/2020
* @version		2.0
* @author		William PENSEC | M2 LSE
* @description	Génération d'obstacles aléatoires pour une simulation de drônes. Les obstacles ne doivent pas se superposer.
*
**/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_iterators.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"

using namespace std;
using namespace rapidxml;

const char* nomIn = "map.osm";
const char* fichierTemp = "tmp.xml";
const char* nomOut = "batiments.xml";

int main(int argc, char* argv[], char* envp[])
{
	// Vérifications arguments
	if (argc != 1) {
		cout << "Usage : " << argv[0] << endl;
		exit(0);
	}

	// Variables
	string ligne;
	int i = 1;
	ofstream fileTemp;
	ofstream fileOut;
	ifstream fileIn;
	xml_document<> doc;
	xml_node<>* rootNode;
	bool premiereValeur = true;

	// Ouverture du fichier de lecture
	fileIn.open(nomIn, ios::in);

	// Ouverture des fichiers d'écritures
	fileTemp.open(fichierTemp, ios::out | ios::trunc);
	/*fileOut.open(nomOut, ios::out | ios::trunc);
	fileOut << "<environment>" << endl;
	fileOut << "\t<!-- Obstacles -->" << endl;*/

	// Lire le fichier xml
	vector<char> buffer((istreambuf_iterator<char>(fileIn)), istreambuf_iterator<char>());
	buffer.push_back('\0');


	// Récupérer le fichier en le parsant
	doc.parse<0>(&buffer[0]);
	rootNode = doc.first_node("osm");


	// Récupérer les informations
	for (xml_node<>* wayNode = rootNode->first_node("way"); wayNode != 0; wayNode = wayNode->next_sibling()) {
		for (xml_node<>* tagNode = wayNode->first_node("tag"); tagNode != 0; tagNode = tagNode->next_sibling()) {
			if (strcmp(tagNode->first_attribute("k")->value(), "building") == 0) {
				/*for (xml_node<>* ndNode = wayNode->first_node("nd"); ndNode != 0; ndNode = ndNode->next_sibling()) {
					for (xml_node<>* nodeNode = rootNode->first_node("node"); nodeNode != 0 ; nodeNode = nodeNode->next_sibling()) {
						if (strcmp(nodeNode->first_attribute("id")->value(), ndNode->first_attribute("ref")->value()) == 0) {
							fileTemp << "Latitude : " << nodeNode->first_attribute("lat")->value() << " | Longitude : " << nodeNode->first_attribute("lon")->value() << endl;
						}
					}
				}*/
				fileTemp << wayNode->name() << endl;
				//fileTemp << endl;
			}
		}
	}


	// Fermer la balise avant de finir le fichier
	//fileOut << "</environment>" << endl;

	// Fermeture du fichier et sortie du programme
	fileIn.close();
	/*fileOut.close();*/
	fileTemp.close();
	return EXIT_SUCCESS;
}