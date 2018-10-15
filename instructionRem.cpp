//
//  instructionRem.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-07.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include "instructionRem.h"

using namespace std;

InstructionRem::InstructionRem( string command, string parametre, unsigned short line )
:Instruction( "REM", parametre, line )
{
}

list<int> InstructionRem::operation(void)
{
    list<int> listeVide;
    
	// Il faut tenir compte de l'instruction
	// REM à cause de l'instruction GOTO...
	// À CONFIRMER...
	setInstructionOffset( getLigne(), Instruction::instructionOffset );
	Instruction::instructionOffset++;

    return listeVide;
}