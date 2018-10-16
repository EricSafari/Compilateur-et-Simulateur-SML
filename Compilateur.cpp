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

int main(int argc, const char * argv[])
{
    string phrase,
           command,
           argument;
    string instruction;
	char * instructionTemp;
         /* command,
		 * argument;*/
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

	// Associer chaque Instruction à un Offset
	unsigned short Offset = 0;
	map<unsigned short, unsigned short> InstructionsOffset;
    
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
			ligneInstr = stoi(ligneStr);

			/*if (ligneInstr < 100)
			{
			}*/
			//str = strtok( instruction, " " );
			// *** ENLEVER LA LIGNE ***
			instruction.erase(0, 2 + 1);

			instructionTemp = (char *)instruction.c_str();
			// EXTRAIRE LA COMMANDE
			str = strtok(instructionTemp, " ");
			command.insert(0, str);

			// ****	  ENLEVER LA COMMANDE	****
			instruction.erase(0, sizeof(str) + 1);

			// *** Le Reste c'est l'Argument ***
			argument = instruction;

			if (strncmp("rem ", str, 4))
			{
				instrPtr = new InstructionRem( "REM", argument, ligneInstr );
			}
			else if (strncmp("input ", str, 6))
			{
				InstructionsOffset[ligneInstr] = Offset;
				instrPtr = new InstructionInput( "INPUT", argument, ligneInstr );
				Offset++;
			}
			else if (strncmp("let ", str, 4))
			{
				InstructionsOffset[ligneInstr] = Offset;
				instrPtr = new InstructionLet( "LET", argument, ligneInstr );
				Offset++;
			}
			else if (strncmp("print ", str, 6))
			{
				InstructionsOffset[ligneInstr] = Offset;
				instrPtr = new InstructionPrint( "PRINT", argument, ligneInstr );
				Offset++;
			}
			else if (strncmp("goto ", str, 5))
			{
				InstructionsOffset[ligneInstr] = Offset;
				instrPtr = new InstructionPrint( "GOTO", argument, ligneInstr );
				Offset++;
			}
			else if (strncmp("if...goto ", str, 10))
			{
				InstructionsOffset[ligneInstr] = Offset;
				instrPtr = new InstructionIfGoto( "IF...GOTO", argument, ligneInstr);
				Offset++;
			}
			else if (strncmp("end ", str, 4))
			{
				InstructionsOffset[ligneInstr] = Offset;
				instrPtr = new InstructionEnd( "END", argument, ligneInstr );
				Offset++;
			}
			else
			{
				// ERREUR : Command Invalide
				exit(-1);
			}

			listeInstr.push_back(instrPtr);
		}
    }
    
    // Traitement( REGROUPEMENT DES INSTRUCTIONS/COPIER DANS LE FICHIER )
    // 1 - ........
    list<Instruction *>::iterator listeInstrIter = listeInstr.begin();
    
    while ( listeInstrIter != listeInstr.end() )
    {
		// Instructions SML de Chaque Ligne
		listeInstrSMLtemp = (*listeInstrIter)->operation();
        
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
