//
//  InstructionEnd.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-06-06.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include "InstructionEnd.h"

using namespace std;

InstructionEnd::InstructionEnd( string command, string parametre, unsigned short line )
:Instruction( "END", parametre, line )
{
}

// END
list<int> InstructionEnd::operation()
{
    list<int> listeInstr;

	/*setInstructionOffset( Instruction::instructionLine, Instruction::instructionOffset++ );
	Instruction::instructionLine++;*/
	setInstructionOffset( getLigne(), Instruction::instructionOffset );
	Instruction::instructionOffset++;
    
    listeInstr.push_back( 4300 );
    return listeInstr;
}
