//
//  InstructionLet.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-10.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTIONLET_H
#define INSTRUCTIONLET_H

#include <list>
#include "instruction.h"

class InstructionLet: public Instruction
{
public:
    InstructionLet( string command, string parametre, unsigned short line );
    virtual list<int> operation();
private:
    //
	list<int> genererNombre(char nombre);
	unsigned short trouverAddresse2(char * variablePtr);
	list<int> evalOperation(char * variable1Ptr, char operateur, char * variable2Ptr);
	void evalOperationAccumulateur(char operateur, char * variable2Ptr);
	char * evalOperationsUnit(list<char *> liste);
};

#endif /* INSTRUCTIONLET_H */
