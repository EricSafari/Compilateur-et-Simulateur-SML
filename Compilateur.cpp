//
//  main.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-03-22.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>
#include <list>

#include "instruction.h"
#include "instructionRem.h"
#include "instructionEnd.h"
#include "instructionInput.h"
#include "instructionPrint.h"
#include "instructionGoto.h"
#include "instructionIfGoto.h"
#include "instructionLet.h"

#include<cstring>
#include <map>
#include <ctype.h> // ISALPHA, ISNUM, ...

using namespace std;

// Addr = 900;

class InstructionInconnue : public exception
{
public:
	InstructionInconnue(string erreur);
};

class ArgumentInvalide : public exception
{
public:
	ArgumentInvalide(string erreur);
};

int main(int argc, const char * argv[])
{
    string phrase,
           command,
           argument;
    string instruction;
	char * instructionTemp;
         
    string ligneStr;
    unsigned short ligneInstr;
    ifstream fichSrc;
    ofstream fichExec;
    
    char * str;
    
    //Instruction instr;
    //Instruction * instrPtr = new Instruction;
	Instruction * instrPtr;
    list<Instruction *> listeInstr;
    list<int> listeInstrSML;
    list<int> listeInstrSMLtemp;

	// Associer à chaque Instruction sa Ligne
	map<unsigned short, Instruction *> InstructionsPtrMap;
    
    // Fichier HAUT-NIVEAU
    fichSrc.open( "fichierSource.txt", ios::in );
    
    // TEST
    if ( fichSrc.fail() == true )
    {
        cerr << "ERREUR: " << strerror( errno ) << endl << endl;
        cout << "***********   --------------------------------------------------   *************'" << endl;
        cout << "***********   --------------------------------------------------   *************'" << endl;
        cout << "***********   IMPOSSIBLE D'OUVRIR LE FICHIER 'fichierSource.txt'   *************'" << endl;
        cout << "***********   --------------------------------------------------   *************'" << endl;
        cout << "***********   --------------------------------------------------   *************'" << endl;
        
        _exit( -1 );
    }
    
    while ( !fichSrc.eof() )
    {
        //fichSrc >> ligne >> command >> argument;
        getline( fichSrc, instruction );
        ligneStr = instruction.substr( 0, 2 );

		// LE 1RE CHAINE DE CHARACTÈRES EST-ELLE UN NOMBRE
		if (!(isdigit(ligneStr[0]) && isdigit(ligneStr[1]) && ligneStr[2] == ' '))
		{
			// LIGNE	COMMAND		ARGUMENT
			// 10		input		x
			ligneInstr = stoi(ligneStr);

			// *** ENLEVER LA LIGNE ***
			instruction.erase(0, 2 + 1);
			strcpy( instructionTemp, instruction.c_str() );

			// EXTRAIRE LA COMMANDE...
			str = strtok(instructionTemp, " ");
			command.insert(0, str);

			// ****	  ENLEVER LA COMMANDE	****
			instruction.erase(0, sizeof(command) + 1);
			//instruction.erase(0, sizeof(str) + 1);

			// *** Le Reste c'est l'Argument ***
			argument = instruction;

			if (strncmp("rem ", str, 4))
			{
				instrPtr = new InstructionRem( "REM", argument, ligneInstr );
			}
			else if (strncmp("input ", str, 6))
			{
				instrPtr = new InstructionInput( "INPUT", argument, ligneInstr );
			}
			else if (strncmp("let ", str, 4))
			{
				instrPtr = new InstructionLet( "LET", argument, ligneInstr );
			}
			else if (strncmp("print ", str, 6))
			{
				instrPtr = new InstructionPrint( "PRINT", argument, ligneInstr );
			}
			else if (strncmp("goto ", str, 5))
			{
				instrPtr = new InstructionPrint( "GOTO", argument, ligneInstr );
			}
			else if (strncmp("if ", str, 3))
			{
				char * instructionIfGoto;
				strcpy(instructionIfGoto, argument.c_str());
				char var1 = *( strtok(instructionIfGoto, " ") );

				char * operation;
				strcpy( operation, strtok(NULL, " ") );
				char var2 = *( strtok(NULL, " ") );
				char * gotoStr;
				strcpy( gotoStr, strtok(NULL, " ") );
				strcmp(gotoStr, "goto");

				ligneInstr = stoi( strtok(NULL, " ") );

				instrPtr = new InstructionIfGoto( "IF...GOTO", argument, ligneInstr);
			}
			else if (strncmp("end ", str, 4))
			{
				instrPtr = new InstructionEnd( "END", argument, ligneInstr ); // argument est vide
			}
			else
			{
				// ERREUR : Command Invalide
				exit(-1);
			}

			InstructionsPtrMap[ligneInstr] = instrPtr;
			listeInstr.push_back(instrPtr);
		}
    }
    
    // Traitement( REGROUPEMENT DES INSTRUCTIONS/COPIER DANS LE FICHIER )
    // 1 - ........
    list<Instruction *>::iterator listeInstrIter = listeInstr.begin();
    
    while ( listeInstrIter != listeInstr.end() )
    {
		// Instructions SML de Chaque Ligne
		if (((*listeInstrIter)->getCmd() != "GOTO") && ((*listeInstrIter)->getCmd() != "IF...GOTO"))
		{
			listeInstrSMLtemp = (*listeInstrIter)->operation();
		}
		else
		{
			unsigned short addresse = 99;
			listeInstrSMLtemp = (*listeInstrIter)->operation() + addresse;
		}
        
        // INSÉRER LA NOUVELLE LISTE À LA FIN DE LA LISTE listeInstrSML
        listeInstrSML.insert( listeInstrSML.end(), listeInstrSMLtemp.begin(),  listeInstrSMLtemp.end() );
        
        listeInstrIter++;
    }
    
    fichSrc.close();
    
	// 2 - ...
    // Fichier exécutable avec des instructions SML
    // 1007, 1112, 3009, 2133, ...
    fichExec.open("fichierExecutable.txt", ios::out);
    
    list<int>::iterator listeInstrSMLIter = listeInstrSML.begin();
    
    while ( listeInstrSMLIter != listeInstrSML.end() )
    {
        // Écrire les instructions SML dans
        // le fichier exécutable
        fichExec << *listeInstrSMLIter;
        
        // Incrémenter l'Itérateur
        listeInstrSMLIter++;
    }
    
    fichExec.close();
    
    return 0;
}
