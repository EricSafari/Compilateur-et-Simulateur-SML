//
//  InstructionGoto.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-06-06.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include "InstructionGoto.h"

using namespace std;

InstructionGoto::InstructionGoto( string command, string parametre, unsigned short line )
:Instruction( command = "GOTO", parametre, line )
{
}

list<int> InstructionGoto::operation(void)
{
    list<int> listeInstr;
    
	unsigned short addrGoto;
    string addrGotoStr;

	addrGotoStr = getParam();
	int instruction = 0;
	unsigned short instrOffset;

	addrGoto = static_cast<unsigned short>( stoi(addrGotoStr) );
	// Les lignes ont été utilisés comme CLÉ dans la MAP
	// map<unsigned short, Instruction *> InstructionsPtrMap(main.cpp)
	if( Instruction::InstructionsPtrMap.count(addrGoto) > 0 )
	{
		if( Instruction::InstructionsOffsetMap.count(addrGoto) > 0 )
		{
			instrOffset = Instruction::InstructionsOffsetMap[addrGoto];
		}
		else
		{
			// VALEUR TEMPORAIRE...
			// L'Instruction se trouve plus loin!!!
			instrOffset = 99;
			// Sauvegarde de la Ligne
			Instruction::listeLignesOffsetsTemp.push_back(addrGoto);
		}

		// BRANCH...
		instruction = 4000 + (int)instrOffset;
	}
	else
	{
		// À voir plus tard...
		//throw ...
	}
    
	setInstructionOffset( getLigne(), Instruction::instructionOffset );
	Instruction::instructionOffset++;

	listeInstr.push_back( instruction );
    return listeInstr;
}
