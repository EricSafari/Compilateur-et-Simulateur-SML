//
//  instruction.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-07.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include "instruction.h"

//unsigned int Instruction::addresse = 0;
// ***********	INITIALISATION ATTRIBUTS STATIQUES	***********
// C'est à partir de cette Adresse que sera stocké les variables
unsigned short Instruction::addresse = 50;
unsigned short Instruction::instructionOffset = 0;
unsigned short Instruction::instructionLine   = 1;

map<unsigned short, unsigned short> Instruction::InstructionsOffsetMap = map<unsigned short, unsigned short>();
list<unsigned short> Instruction::lignesOffsetsTemp = list<unsigned short>();

map<unsigned short, Instruction *> Instruction::InstructionsPtrMap = map<unsigned short, Instruction *>();

// MAP qui associe une VARIABLE à son ADDRESSE dans la MÉMOIRE
map< char, unsigned short > Instruction::mapVariables;

Instruction::Instruction( string command, string parametre, unsigned short line )
{
    setCmd( command );
    setParam( parametre );
    setLigne( line );

	// POINTEUR VERS LA CLASSE COURANTE
	//instructions.push_back(this);
	//Instruction::mapInstructions[line] = this;
}

Instruction::Instruction()
:Instruction("", "", 0)
{
    //Instruction::mapVariables = map< char, unsigned short >();
}

Instruction::~Instruction()
{
}

string Instruction::getCmd()
{
    return cmd;
}

void Instruction::setCmd( string command )
{
    cmd = command;
}

string Instruction::getParam(void)
{
    return param;
}

void Instruction::setParam( string parametre )
{
    param = parametre;
}

unsigned short Instruction::getLigne(void)
{
    return ligne;
}

void Instruction::setLigne( unsigned short line )
{
    ligne = line;
}

unsigned int Instruction::getAddress(void)
{
    return addresse;
}
void Instruction::setAddress(unsigned int addr)
{
    addresse = addr;
}

map<char, unsigned short> Instruction::getMap(void)
{
    return Instruction::mapVariables;
}
void Instruction::setMap(char cle, unsigned short addr)
{
    Instruction::mapVariables[ cle ] = addr;
}

map<unsigned short, unsigned short> Instruction::getMapOffset()
{
	return Instruction::InstructionsOffsetMap;
}

//void Instruction::setMapOffset( map<unsigned short, unsigned short> Map )
//{
//	InstructionsOffsetMap = Map;
//}

void Instruction::setInstructionOffset(unsigned short instructionLine, unsigned short Offset)
{
	Instruction::InstructionsOffsetMap[instructionLine] = Offset;
}

//list<Instruction *> Instruction::getInstructionsList()
//{
//	return Instruction::instructions;
//}
//
//void Instruction::AddToInstructionsList(Instruction * uneInstruction)
//{
//	Instruction::instructions.push_back(uneInstruction);
//}