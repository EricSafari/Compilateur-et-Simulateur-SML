//
//  InstructionInput.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-06-05.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include "InstructionInput.h"

InstructionInput::InstructionInput( string command, string parametre, unsigned short line )
:Instruction( "INPUT", parametre, line )
{
}

// Input X
list<int> InstructionInput::operation(void)
{
    list<int> listeInstr;
    unsigned short addrTemp;
    int instruction;
    char cle;
    
    //    unsigned int addrTemp = Instruction::getAddress();
    cle = *getParam().c_str();
    
	// Vérifier si la CLÉ X est dans la MAP
    if( Instruction::getMap().count(cle) == 0 )
    {
		// La CLÉ X n'est pas dans la MAP,
        // Il faut assigner une Adresse dans la Mémoire...
		// à partir de 50...
        addrTemp = Instruction::getAddress();
        
        Instruction::setMap( cle, addrTemp );
        Instruction::setAddress( addrTemp + 1 );
    }
    else // La Variable a été utilisée avant
    {
		// La CLÉ X est dans la MAP,
        // Il faut trouver son Adresse dans la Mémoire...
		// à partir de 50...
        addrTemp = Instruction::getMap()[ cle ];
    }
    
	setInstructionOffset( getLigne(), Instruction::instructionOffset );
	Instruction::instructionOffset++;

	// INPUT ==> instruction = 10|ADDR
    instruction = 1000 + (int)addrTemp;
    listeInstr.push_back( instruction );
    return listeInstr;
}