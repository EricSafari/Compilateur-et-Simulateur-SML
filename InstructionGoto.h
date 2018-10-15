//
//  InstructionGoto.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-10.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTIONGOTO_H
#define INSTRUCTIONGOTO_H

#include "instruction.h"

class InstructionGoto: public Instruction
{
public:
    InstructionGoto( string command, string parametre, unsigned short line );
    virtual list<int> operation();
	/*map<unsigned short, unsigned short> getMapOffset();
	void setMapOffset( map<unsigned short, unsigned short> MapOffset );*/
private:
    //
	//map<unsigned short, unsigned short> InstructionsOffset;
};

#endif /* INSTRUCTIONGOTO_H */
