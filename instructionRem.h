//
//  instructionRem.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-07.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTIONREM_H
#define INSTRUCTIONREM_H

#include <list>
#include "instruction.h"

using namespace std;

class InstructionRem: public Instruction
{
public:
    InstructionRem( string command, string parametre, unsigned short line );
    virtual list<int> operation();
private:
    //
};

#endif /* INSTRUCTIONREM_H */
