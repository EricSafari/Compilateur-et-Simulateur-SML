//
//  InstructionPrint.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-06-08.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include <iostream>
#include "InstructionPrint.h"

using namespace std;

InstructionPrint::InstructionPrint( string command, string parametre, unsigned short line )
:Instruction( "PRINT", parametre, line )
{
}

// Print Y
list<int> InstructionPrint::operation(void)
{
    list<int> listeInstr;
    unsigned short addrVar;
    int instruction;
    char cle;
    
    //    unsigned int addrTemp = Instruction::getAddress();
    cle = *getParam().c_str(); // cle = Y
    
	// Vérifier si la CLÉ Y est dans la MAP
    if( Instruction::getMap().count(cle) > 0 )
    {
        // Il faut trouver son Adresse dans la Mémoire
        addrVar = Instruction::getMap()[ cle ];
    }
    else // La Variable a été utilisée avant
    {
        cout << "ERREUR: Tentative d'imprimer une variable ((" << cle << ")) qui n'existe pas!!!" << endl;
        _exit(-2);
    }

	setInstructionOffset( getLigne(), Instruction::instructionOffset );
    Instruction::instructionOffset++;

    instruction = 1100 + (int)addrVar;
    listeInstr.push_back( instruction );
	
    return listeInstr;
}