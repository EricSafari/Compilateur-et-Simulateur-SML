//
//  InstructionInput.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-10.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTIONINPUT_H
#define INSTRUCTIONINPUT_H

#include <list>
#include "instruction.h"

class InstructionInput: public Instruction
{
public:
    InstructionInput( string command, string parametre, unsigned short line );
    virtual list<int> operation();
private:
    //
};


#endif /* INSTRUCTIONINPUT_H */
