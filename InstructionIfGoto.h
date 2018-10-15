//
//  InstructionIfGoto.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-10.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTIONIFGOTO_H
#define INSTRUCTIONIFGOTO_H

#include <list>
//#include "instruction.h"
#include "InstructionGoto.h"

class InstructionIfGoto: public InstructionGoto
{
public:
    InstructionIfGoto( char a, char b, char * condition, unsigned short addrGoto, unsigned short line );
	~InstructionIfGoto();
	virtual list<int> operation();

	char getVar1();
	void setVar1(char variable1);
	char getVar2();
	void setVar2(char variable2);

	char * getOperateur();
	void setOperateur(char * compateur);
private:
    // Attributs spécifiques
	char var1;
	char var2;

	char * operateur;
};

#endif /* INSTRUCTIONIFGOTO_H */
