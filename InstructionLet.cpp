//
//  InstructionLet.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-06-08.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include <iostream>
#include <string>
#include "InstructionLet.h"
#include <cstring>
#include <vector>
#include <cctype>

InstructionLet::InstructionLet( string command, string parametre, unsigned short line )
:Instruction( "LET", parametre, line )
{
}

// MEM[50] = 1
list <int> genererNombre(char nombre)
{
	list<int> instructionsSML;

	instructionsSML.push_back(2050); // LOAD MEM[50]   -----> ACC = 1
	//instructionsSML.push_back(2151); // STORE MEM[51]  -----> MEM[51] = 1

	int N = nombre - 48;

	for (size_t i = 0; i < N - 1; i++)
	{
		instructionsSML.push_back(3150); // ADD MEM[50]  -----> ACC++
	}

	instructionsSML.push_back(2151); // STORE MEM[51]  -----> MEM[51] = nombre

	return instructionsSML;
}

bool expression( char operateur )
{
	if ( (operateur == '*') || (operateur == '/') )
	{
		return true;
	}
	
	if ( (operateur == '+') || (operateur == '-') )
	{
		return false;
	}
	else
	{
		cout << "ERREUR : OPÉRATEUR(" << operateur << ") INCONNU" << endl;
		_exit(-7);
	}
}

// Il faudra peut-être initialiser les variables
// nouvellement utilisées...
unsigned short trouverAddresse( char variable )
{
	unsigned short addrVar = 666;

	if( isalpha(variable) != 0 )
	{
		if(islower(variable) != 0)
		{
			// La variable n'a pas encore été utilisée...
			// IL FAUT L'INITIALISER À ZÉRO!!!!!!!!!!
			if (Instruction::getMap().count(variable) == 0)
			{
				addrVar = Instruction::getAddress();

				Instruction::setMap(variable, addrVar);
				Instruction::setAddress(addrVar + 1);
			}
			else // La Variable a été utilisée avant
			{
				// Il faut trouver son Adresse dans la Mémoire
				addrVar = Instruction::getMap()[variable];
			}
		}
		else
		{
			// Variables temporaires
			switch (variable)
			{
			case 'T':	addrVar = 99; // TEMP1
				break;

			case 'S':	addrVar = 98; // TEMP2
				break;

			case 'A':	addrVar = 97; // PARANTHÈSE
				break;
			default:	cout << "ERREUR: VARIABLE(" << variable << ") INVALIDE!!!!!!!!!";
				_exit(-7);
				break;
			}
		}
	}
	else
	{
		cout << "ERREUR: LA VARIABLE(" << variable << ") N'EST PAS UN CHARACTÈRE!!!!!!!!!";
		_exit(-8);
	}

	return addrVar;
}

int f(char * variablePtr)
{
	unsigned short addrVar;

	// 1 - Trouver les addresses des 2 variables
	if( strlen(variablePtr) == 1 )
	{
		if( (isalpha(*variablePtr) != 0) && (islower(*variablePtr) != 0) )
		{
			addrVar = trouverAddresse( *variablePtr );
			//........ À Completer
		}
		else if(isdigit(*variablePtr))
		{
			list<int> listeTemp;

			listeTemp = genererNombre(*variablePtr);
			addrVar = 51; // CONSTANTE(S)

			//listeInstruction.insert(listeInstruction.begin(), listeTemp.begin(), listeTemp.end());
		}

		return *variablePtr;
	}
	else
	{
		// On essaie de Convertir la Chaine de Caractère
		// en un nombre entier.  Ensuite...
		try
		{
			list<int> listeTemp;
			int variable = stoi( variablePtr );

			listeTemp = genererNombre(*variablePtr);
			addrVar = 51; // CONSTANTE(S)

			return variable;
		}
		catch( invalid_argument& ArgInval )
		{
			cout << "Argument invalide: " << ArgInval.what();
			throw;
		}
		catch( out_of_range& ofr )
		{
			cout << "Nombre non représentable sur 32-bit : " << ofr.what();
			throw;
		}
	}
}

list<int> evalOperation(char * variable1Ptr, char operateur, char * variable2Ptr)
{
	list <int> listeInstruction;
	unsigned short	addrVar1,
					addrVar2;
	int instruction;

	// 1 - Trouver les addresses des 2 variables
	addrVar1 = trouverAddresse( *variable1Ptr );
	addrVar2 = trouverAddresse( *variable1Ptr);
	
	// 2 - LOAD VAR1 & VAR2
	instruction = 2000 + addrVar1;
	listeInstruction.push_back(instruction);

	if (operateur == '+')
	{
		// OPÉRATION( ACC = ACC + MEM[addrVar2])
		instruction = 3000 + addrVar2;

	}
	else if (operateur == '-')// operateur = '-'
	{
		// OPÉRATION( ACC = ACC - MEM[addrVar2])
		instruction = 3100 + addrVar2;
	}
	else if (operateur == '*')
	{
		// OPÉRATION( ACC = ACC * MEM[addrVar2])
		instruction = 3300 + addrVar2;

	}
	else // operateur = '/'
	{
		// OPÉRATION( ACC = ACC / MEM[addrVar2])
		instruction = 3200 + addrVar2;
	}

	listeInstruction.push_back(instruction);
	return listeInstruction;
}

list<int> evalExpression( list<char *> expression )
{
	list <int> listeInstruction;
	char var1,
		 var2,
		 oper;
	unsigned short	addrVar1,
					addrVar2;
	int instruction;
	list<char *>::iterator it = expression.begin();

	while (it != expression.end())
	{
		if (it != expression.begin())
		{
			var1 = 'T';
		}
		else
		{
			var1 = *(*it);
			//it++;

		}

		it++;
		oper = *(*it);

		if( !(oper != '+' || oper != '-' || oper != '*' || oper != '/') || (strlen(*it) > 1) )
		{
			cout << "ERREUR: L'OPÉRATION(" << oper << ") N'EST PAS VALIDE!!!!!!!!!";
			_exit(-9);
		}
		
		it++;
		var2 = *(*it);

		// Le résultat se trouve dans l'Accumulateur
		list<int> listeTemp = evalOperation(var1, oper, var2);	
		listeInstruction.insert( listeInstruction.end(), listeTemp.begin(), listeTemp.end() );
		
		//// L'Itérateur pointe sur l'Opérateur
		//// qui suit...
		//it++;
	}

	return listeInstruction;
}

// Let x = u - y * z - 2 * (y - z - 3) / 4 - 1/u - u / y / z
list<int> InstructionLet::operation(void)
{
    list<int> listeInstr;
    unsigned short addrVarDep,
                   addrVarInd;
    unsigned short nbParantOuvr = 0,
                   nbParantFerm = 0;
    short instruction;
    char * equation;
    char * clePtr;
    char cle,
         cleVarDep;
    vector<char> vecteurInstruction;
	vector<char *> vecteurInstructionPtr;
	char var1,
		var2,
		oper;
    
    enum Etat_t {	
					INIT, OPERATION, PARANTHESE1, PARANTHESE2, OPER_PRIO, EVAL_OPER, EVAL_EXPR,
					ETAT_FINAL, REMPLACER_PARA, TEMP, TEMP1, TEMP2, REMPLACER_TEMP, FIN
				};

	bool paraFerm = false,
		paraOuvr = false,
		multDiv = false,
		finiOper = false,
		finiExpr = false,
		temp1Util = false;

	// 1 - Il faut trouver une adresse pour la Variable Dépendante
    // -------------------------------------------------------
    //    cle = *getParam().c_str();
	unsigned short taille = getParam().size();
	equation = new char[taille + 1];


	strncpy(equation, getParam().c_str(), taille);
    
    // Extraction de la Var. dép.
    cleVarDep = *strtok( equation, " " );
    //vecteurInstruction.push_back( cleVarDep );
    
    if( Instruction::getMap().count(cleVarDep) == 0 )
    {
        addrVarDep = Instruction::getAddress();
        
        Instruction::setMap( cleVarDep, addrVarDep );
        Instruction::setAddress( addrVarDep + 1 );
    }
    else // La Variable a été utilisée avant
    {
        // Il faut trouver son Adresse dans la Mémoire
        addrVarDep = Instruction::getMap()[ cleVarDep ];
    }
    // -------------------------------------------------------
    
    /************************************/
    // 2 - ÉLIMINER LE SIGNE '=' ...
    equation = strtok( NULL, " " );
    if( (sizeof( equation ) != 1) || (*equation != '=') )
    {
        //.....................................
        cout << "ERREUR!!!!!!!!!!!!!!!!!!!!" << endl;
        //return listeInstr;
        _exit( -4 );
    }
    /************************************/

	// 3 - Sauvegarder les éléments de l'Instruction
    //     sans les Espaces Blancs
	while (equation != NULL)
	{
		clePtr = strtok(NULL, " ");

		if (sizeof(clePtr) == 1)
		{
			cle = *clePtr;

			if (isdigit(cle))
			{
				list<int> liste = genererNombre(cle);
				listeInstr.insert( listeInstr.end(), liste.begin(), liste.end() );
			}
			else if (isalpha(cle))
			{
				//
			}
			else if ( (cle == '+') || (cle == '-') || (cle == '*') || (cle == '/') )
			{
				//
			}
			else
			{
				//
				cout << "ERREUR : OPÉRATION INCONNUE(" << cle << ")" << endl;
			}

			//vecteurInstruction.push_back(cle);
			vecteurInstructionPtr.push_back( clePtr );
		}
		else
		{
			int n = strlen(clePtr);
			//char * nb;

			if ( *clePtr == '(' )
            {
                nbParantOuvr++;
				//clePtr++;

				char * nb = new char[n];
				strncpy(nb, clePtr + 1, n - 1);
				nb[n] = 0;

				vecteurInstructionPtr.push_back(nb);
            }
			else
			{
				if ( *(clePtr + n - 1) == ')' )
				{
					nbParantFerm++;
					//clePtr--;

					char * nb = new char[n];
					strncpy(nb, clePtr, n - 1);
					nb[n] = 0;

				vecteurInstructionPtr.push_back(nb);
				}
				else
				{
					char * nb = new char[n + 1];
					strncpy(nb, clePtr, n);
					nb[n] = 0;

				vecteurInstructionPtr.push_back(nb);
				}
				vecteurInstruction.push_back( *clePtr );
			}
            
            /*vecteurInstruction.push_back( *clePtr );
            vecteurInstruction.push_back( *( clePtr + 1 ) );*/
		}
	}
    
    // TEST CRUCIAL
    if ( nbParantOuvr != nbParantFerm )
    {
        cout << "Le nombre de paranthèses ouvrantes( " << nbParantOuvr << ") n'est pas égal au nombre de paranthèses fermantes( " << nbParantFerm << " )!!!" << endl;
        _exit(-3);
    }

	Etat_t etat = INIT;

	switch (etat)
	{
	case INIT:
		etat = OPERATION;

	case OPERATION:
		if (expression(var2))
		{
			listeInstr = evalExpression(var1, oper, var2);
			etat = TEMP1;
		}
		else
		{
			// LOAD
		}

	case PARANTHESE1:
		if (paraFerm)
		{
			etat = PARANTHESE2;
		}
		else
		{
			etat = OPER_PRIO;
		}

	case PARANTHESE2:
		if (paraOuvr)
		{
			etat = PARANTHESE2;
		}
		else
		{
			//etat = OPER_PRIO;
			//EXIT...
			cout << "Paranthèse ouvrante manquante!!!!???????" << endl;
			_exit(-5);
		}

	case OPER_PRIO:
		if (multDiv)
		{
			etat = EVAL_EXPR;
		}
		else
		{
			etat = EVAL_OPER;
		}

	case EVAL_EXPR:
		if (finiExpr)
		{
			etat = TEMP;
		}

	case EVAL_OPER:
		if (finiOper)
		{
			etat = ETAT_FINAL;
		}

	case ETAT_FINAL:
		if (paraFerm)
		{
			etat = REMPLACER_PARA;
		}
		else
		{
			etat = FIN;
		}

	case REMPLACER_PARA:
		etat = PARANTHESE1;

	case TEMP:
		if (!temp1Util)
		{
			etat = TEMP1;
		}
		else
		{
			etat = TEMP2;
		}
		
	case TEMP1:
	case TEMP2:
	case REMPLACER_TEMP:
	default:
		cout << "ÉTAT INCONNU!!!!!!!!!!!!!!!!" << endl;
		_exit(-6);
	}
    
    return listeInstr;
}