//
//  InstructionIfGoto.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-06-06.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include "InstructionGoto.h"
#include "InstructionIfGoto.h"
#include <iostream>

using namespace std;

InstructionIfGoto::InstructionIfGoto( char a, char b, char * condition, unsigned short addrGoto, unsigned short line )
:InstructionGoto("IF...GOTO", to_string(addrGoto), line)
{
	setVar1(a);
	setVar1(b);

	setOperateur(condition);
}

char InstructionIfGoto::getVar1()
{
	return var1;
}

void InstructionIfGoto::setVar1(char variable1)
{
	var1 = variable1;
}

char InstructionIfGoto::getVar2()
{
	return var2;
}

void InstructionIfGoto::setVar2(char variable2)
{
	var2 = variable2;
}

char * InstructionIfGoto::getOperateur()
{
	return operateur;
}

void InstructionIfGoto::setOperateur(char * compateur)
{
	strcpy(operateur, compateur);
}

list<int> InstructionIfGoto::operation(void)
{
    list<int> listeInstr;
    unsigned short addrVar1,
                   addrVar2;

	//unsigned short instruction;
	unsigned short addrGoto;
    string addrGotoStr;
    size_t posGoto;
    char cle1,
         cle2;
	char * condition,
		 * instructionX;
    
	strcpy(instructionX, getOperateur() );

	cle1 = getVar1();
	cle2 = getVar2();
    
    // Vérifier si les 2 variables à comparer EXISTENT
    if( (Instruction::getMap().count(cle1) > 0) && (Instruction::getMap().count(cle2) > 0) )
    {
        // Il faut trouver les Adresses dans la Mémoire
        addrVar1 = Instruction::getMap()[ cle1 ];
        addrVar2 = Instruction::getMap()[ cle2 ];
    }
    else // La ou les 2 Variables N'EXISTENT PAS
    {
        cout << "ERREUR: Tentative d'imprimer une ou plusieurs variable ((" << cle1 << "ET/OU" << cle2 << ")) qui N'EXISTENT PAS!!!" << endl;
        _exit(-2);
    }
    
	unsigned short Ligne = getLigne();
	int instruction = 0;

	// Les lignes ont été utilisés comme CLÉ dans la MAP
	// map<unsigned short, Instruction *> InstructionsPtrMap(main.cpp)
	if( Instruction::InstructionsPtrMap.count(Ligne) > 0 )
	{
		//unsigned short addrTemp;

		if( Instruction::InstructionsOffsetMap.count(Ligne) > 0 )
		{
			addrGoto = Instruction::InstructionsOffsetMap[Ligne];
		}
		else
		{
			// VALEUR TEMPORAIRE...
			// L'Instruction se trouve plus loin!!!
			addrGoto = 99;
			// Sauvegarde de la Ligne
			Instruction::lignesOffsetsTemp.push_back(Ligne);
		}

		//instruction = 4000 + (int)addrTemp;
	}
	else
	{
		// À voir plus tard...
		//throw ...
	}
	
    // 3 - VÉRIFIER SI ACC = 0 (VAR1 = VAR2) OU ...
	if (strcmp(condition, "<") == 0)
	{
		// À faire...
		// 1 - LOAD VAR1 (ACC = MEM[addrVar1])
		instruction = 2100 + addrVar1;
		listeInstr.push_back( instruction );
    
		// 2 - SUB ACC, MEM[addrVar2] (ACC = ACC - MEM[addrVar2])
		instruction = 3100 + addrVar2;
		listeInstr.push_back( instruction );

		// BRANCH NÉG( VAR1 < VAR2 )
		instruction = 4100 + (int)addrGoto;
	}
	else if (strcmp(condition, ">") == 0)
	{
		// 1 - LOAD VAR1 (ACC = MEM[addrVar2])
		instruction = 2100 + addrVar2;
		listeInstr.push_back( instruction );
    
		// 2 - SUB ACC, MEM[addrVar1] (ACC = ACC - MEM[addrVar1])
		instruction = 3100 + addrVar1;
		listeInstr.push_back( instruction );

		// BRANCH NÉG( VAR1 > VAR2 )
		instruction = 4100 + (int)addrGoto;
	}
	else if (strcmp(condition, "==") == 0)
	{
		// 1 - LOAD VAR1 (ACC = MEM[addrVar1])
		instruction = 2100 + addrVar1;
		listeInstr.push_back( instruction );
    
		// 2 - SUB ACC, MEM[addrVar2] (ACC = ACC - MEM[addrVar2])
		instruction = 3100 + addrVar2;
		listeInstr.push_back( instruction );

		// BRANCH ZÉRO( VAR1 = VAR2 )
		instruction = 4200 + (int)addrGoto;
	}
	else if (strcmp(condition, "<=") == 0)
	{
		// 1 - LOAD VAR1 (ACC = MEM[addrVar1])
		instruction = 2100 + addrVar1;
		listeInstr.push_back( instruction );
    
		// 2 - SUB ACC, MEM[addrVar2] (ACC = ACC - MEM[addrVar2])
		instruction = 3100 + addrVar2;
		listeInstr.push_back( instruction );

		// BRANCH ZÉRO( VAR1 = VAR2 )
		instruction = 4200 + (int)addrGoto;
		// BRANCH NÉG( VAR1 < VAR2 )
		instruction = 4100 + (int)addrGoto;
	}
	else if (strcmp(condition, ">=") == 0)
	{
		// 1 - LOAD VAR1 (ACC = MEM[addrVar2])
		instruction = 2100 + addrVar2;
		listeInstr.push_back( instruction );
    
		// 2 - SUB ACC, MEM[addrVar1] (ACC = ACC - MEM[addrVar1])
		instruction = 3100 + addrVar1;
		listeInstr.push_back( instruction );

		// BRANCH ZÉRO( VAR1 = VAR2 )
		instruction = 4200 + (int)addrGoto;
		// BRANCH NÉG( VAR1 > VAR2 )
		instruction = 4100 + (int)addrGoto;
	}
	else
	{
		cout << "ERREUR : Condition inconnue!!!!!!!!!!!!!!!" << endl;
		exit(-3);
	}

	listeInstr.push_back( instruction );

    Instruction::instructionOffset += 3;
    return listeInstr;
}
