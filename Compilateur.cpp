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
    list<int> listeIstrSMLtemp;
    
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
			instruction.erase(0, 2 + 1);

			instructionTemp = (char *)instruction.c_str();
			//command = (char *)instruction.c_str();
			str = strtok(instructionTemp, " ");
			command.insert(0, str);

			if (strncmp("rem ", str, 4))
			{
				//
				InstructionRem( "REM", command, ligneInstr );
			}
			else if (strncmp("input ", str, 6))
			{
				InstructionInput( "INPUT", command, ligneInstr );
			}
			else if (strncmp("let ", str, 4))
			{
				InstructionLet( "LET", command, ligneInstr );
			}
			else if (strncmp("print ", str, 6))
			{
				InstructionPrint( "PRINT", command, ligneInstr );
			}
			else if (strncmp("goto ", str, 5))
			{
				InstructionPrint( "GOTO", command, ligneInstr );
			}
			else if (strncmp("if...goto ", str, 10))
			{
				InstructionIfGoto( "IF...GOTO", command, ligneInstr);
			}
			else if (strncmp("end ", str, 4))
			{
				InstructionEnd( "END", command, ligneInstr );
			}
			else
			{
				// ERREUR : Command Invalide
				exit(-1);
			}

			//command.insert(0, str);
			//instruction.erase( 3, sizeof(str) + 1 );
			instruction.erase(0, sizeof(str) + 1);
			argument = instruction;
			/*
			instrPtr->setLigne(ligneInstr);
			instrPtr->setCmd(command);
			instrPtr->setParam(argument);
			*/
			listeInstr.push_back(instrPtr);
		}
    }
    
    /*instrPtr->setLigne( 50 );
    instrPtr->setCmd( "rem" );
    instrPtr->setParam( "this is a remark" );
    
    listeInstr.push_back( instrPtr );
    
    instrPtr->setLigne( 30 );
    instrPtr->setCmd( "input" );
    instrPtr->setParam( "x" );
    
    listeInstr.push_back( instrPtr );
    
    instrPtr->setLigne( 35 );
    instrPtr->setCmd( "input" );
    instrPtr->setParam( "y" );
    
    listeInstr.push_back( instrPtr );
    
    instrPtr->setLigne( 40 );
    instrPtr->setCmd( "input" );
    instrPtr->setParam( "j" );
    
    listeInstr.push_back( instrPtr );
    
    instrPtr->setLigne( 45 );
    instrPtr->setCmd( "input" );
    instrPtr->setParam( "k" );
    
    listeInstr.push_back( instrPtr );
    
    instrPtr->setLigne( 10 );
    instrPtr->setCmd( "input" );
    instrPtr->setParam( "x" );
    
    listeInstr.push_back( instrPtr );
    
    instrPtr->setLigne( 80 );
    instrPtr->setCmd( "let" );
    instrPtr->setParam( "u = x * (j - k) + y" );
    
    listeInstr.push_back( instrPtr );
    
    instrPtr->setLigne( 10 );
    instrPtr->setCmd( "print" );
    instrPtr->setParam( "w" );
    
    listeInstr.push_back( instrPtr );*/
    
    // Traitement
    // 1 - ........
    list<Instruction *>::iterator listeInstrIter = listeInstr.begin();
    
    while ( listeInstrIter != listeInstr.end() )
    {
        listeIstrSMLtemp = (*listeInstrIter)->operation();
        // INSÉRER LA NOUVELLE LISTE À LA FIN DE LA LISTE listeInstrSML
        listeInstrSML.insert( listeInstrSML.end(), listeIstrSMLtemp.begin(),  listeIstrSMLtemp.end() );
        
        listeInstrIter++;
    }
    
    fichSrc.close();
    
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
