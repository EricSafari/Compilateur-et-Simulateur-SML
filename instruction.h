//
//  instruction.h
//  CompilateurSML
//
//  Created by Eric Safari on 2017-05-07.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <list>
#include <string>
#include <map>
using namespace std;

class Instruction
{
public:
    Instruction( string command, string parametre, unsigned short line );
    Instruction(void);
    ~Instruction(void);
    
    string getCmd(void);
    void setCmd( string command );
    
    string getParam(void);
    void setParam( string parametre );
    
    unsigned short getLigne(void);
    void setLigne(unsigned short line);
    
    virtual list<int> operation() = 0;
    
    static unsigned int getAddress(void);
    static void setAddress(unsigned int addr);
    
    static map<char, unsigned short> getMap(void);
    static void setMap(char cle, unsigned short addresse);

	static map<unsigned short, unsigned short> getMapOffset();
	//void setMapOffset( map<unsigned short, unsigned short> MapOffset );
	static void setInstructionOffset( unsigned short instructionLine, unsigned short Offset );

	/*static list<Instruction *> getInstructionsList();
	static void AddToInstructionsList(Instruction *);*/
	static map<unsigned short, Instruction *> InstructionsPtrMap;

protected:
	// MEMBRE STATIQUE
	static unsigned short instructionOffset;
	static unsigned short instructionLine;
	static map<unsigned short, unsigned short> InstructionsOffsetMap;
	static list<unsigned short> lignesOffsetsTemp;
private:
    string cmd;
    string param;
    unsigned short ligne;
    
    // MEMBRES STATIQUES
    static unsigned short addresse;
    static map<char, unsigned short> mapVariables;
};

#endif /* INSTRUCTION_H */
