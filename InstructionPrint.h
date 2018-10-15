//
//  InstructionPrint.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-10.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTIONPRINT_H
#define INSTRUCTIONPRINT_H

#include <list>
#include "instruction.h"

class InstructionPrint: public Instruction
{
public:
    InstructionPrint( string command, string parametre, unsigned short line );
    virtual list<int> operation();
private:
    //
};

#endif /* INSTRUCTIONPRINT_H */
