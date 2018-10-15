//
//  InstructionEnd.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-10.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTIONEND_H
#define INSTRUCTIONEND_H

#include <list>
#include "instruction.h"

class InstructionEnd: public Instruction
{
public:
    InstructionEnd( string command, string parametre, unsigned short line );
    virtual list<int> operation();
private:
    //
};

#endif /* INSTRUCTIONEND_H */
