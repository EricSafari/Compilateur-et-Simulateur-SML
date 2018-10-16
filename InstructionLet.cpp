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

unsigned short trouverAddresse2(char * variablePtr)
{
	unsigned short addrVar;

	// 1 - Trouver les addresses des 2 variables
	if( strlen(variablePtr) == 1 )
	{
		if( isalpha(*variablePtr) != 0 )
		{
			if (islower(*variablePtr) != 0)
			{
				// La variable n'a pas encore été utilisée...
				// IL FAUT L'INITIALISER À ZÉRO!!!!!!!!!!
				if (Instruction::getMap().count(*variablePtr) == 0)
				{
					addrVar = Instruction::getAddress();

					Instruction::setMap(*variablePtr, addrVar);
					Instruction::setAddress(addrVar + 1);
				}
				else // La Variable a été utilisée avant
				{
					// Il faut trouver son Adresse dans la Mémoire
					addrVar = Instruction::getMap()[*variablePtr];
				}
			}
			else
			{
				// Variables temporaires
				switch (*variablePtr)
				{
					case 'T':	addrVar = 99; // TEMP1
						break;

					case 'S':	addrVar = 98; // TEMP2
						break;

					case 'A':	addrVar = 97; // PARANTHÈSE
						break;
					default:	cout << "ERREUR: VARIABLE(" << *variablePtr << ") INVALIDE!!!!!!!!!";
						_exit(-7);
						break;
				}
			}
		}
		else if(isdigit(*variablePtr))
		{
			list<int> listeTemp;

			listeTemp = genererNombre(*variablePtr);
			addrVar = 51; // CONSTANTE(S)

			//listeInstruction.insert(listeInstruction.begin(), listeTemp.begin(), listeTemp.end());
		}
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

			//return variable;
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

	return addrVar;
}

list<int> evalOperation(char * variable1Ptr, char operateur, char * variable2Ptr)
{
	list <int> listeInstruction;
	unsigned short	addrVar1,
					addrVar2;
	int instruction;

	// 1 - Trouver les addresses des 2 variables
	addrVar1 = trouverAddresse2(variable1Ptr);
	addrVar2 = trouverAddresse2(variable2Ptr);
	
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
	char * var1Ptr,
		 * var2Ptr,
		 * operPtr;
	//char oper;
	unsigned short	addrVar1,
					addrVar2;
	int instruction;
	list<char *>::iterator it = expression.begin();

	while (it != expression.end())
	{
		if (it != expression.begin())
		{
			*var1Ptr = 'T';
		}
		else
		{
			var1Ptr = *it;
		}

		it++;
		operPtr = *it;

		// L'Itérateur pointe sur l'Opérateur qui suit...
		if( !(*operPtr != '+' || *operPtr != '-' || *operPtr != '*' || *operPtr != '/') || (strlen(operPtr) > 1) )
		{
			cout << "ERREUR: L'OPÉRATION(" << *operPtr << ") N'EST PAS VALIDE!!!!!!!!!";
			_exit(-9);
		}
		
		// Deuxième variable
		it++;
		var2Ptr = *it;

		// Le résultat se trouve dans l'Accumulateur
		list<int> listeTemp = evalOperation( var1Ptr, *operPtr, var2Ptr );
		listeInstruction.insert( listeInstruction.end(), listeTemp.begin(), listeTemp.end() );
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
	list<char *> listInstructionPtr;
	char var1,
		var2,
		oper;
    
    enum Etat_t {	
					INIT, OPERATION, PARANTHESE, OPER_PRIO, EVAL_OPER, EVAL_EXPR,
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
	memset(equation, 0, taille + 1);

	strncpy(equation, getParam().c_str(), taille);
	//equation[taille - 1] = 0;
    
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
			listInstructionPtr.push_back( clePtr );
		}
		else
		{
			int n = strlen(clePtr);
			//char * nb;

			if ( *clePtr == '(' )
            {
                nbParantOuvr++;

				char * paraTemp = new char[2];
				paraTemp[0] = *clePtr;
				paraTemp[1] = 0;
				listInstructionPtr.push_back(paraTemp);

				char * nb = new char[n];
				strncpy(nb, clePtr + 1, n - 1);
				nb[n - 1] = 0;

				//listInstructionPtr.push_back(clePtr); // '('
				listInstructionPtr.push_back(nb);
            }
			else
			{
				if ( *(clePtr + n - 1) == ')' )
				{
					nbParantFerm++;

					char * paraTemp = new char[2];
					paraTemp[0] = *(clePtr + n -1);
					paraTemp[1] = 0;
					listInstructionPtr.push_back(paraTemp);

					char * nb = new char[n];
					strncpy(nb, clePtr, n - 1);
					nb[n - 1] = 0;

					listInstructionPtr.push_back(nb);
					//listInstructionPtr.push_back(clePtr + n - 1);
				}
				else
				{
					char * nb = new char[n + 1];
					strncpy(nb, clePtr, n);
					nb[n] = 0;

					listInstructionPtr.push_back(nb);
				}
			}
		}
	}
    
    // TEST CRUCIAL
    if ( nbParantOuvr != nbParantFerm )
    {
        cout << "Le nombre de paranthèses ouvrantes( " << nbParantOuvr << ") n'est pas égal au nombre de paranthèses fermantes( " << nbParantFerm << " )!!!" << endl;
        _exit(-3);
    }

	Etat_t etat = INIT;

	// L'ITÉRATEUR POINTE SUR LA VARIABLE 1
	list<char *>::iterator listInstructionPtrIter = listInstructionPtr.begin(),
						   itListInstruction1	  = listInstructionPtr.begin(),
						   itListInstruction2,
						   iterParantheseFerm,
						   iterParantheseOuvr;

	list<int> listeInstrSMLTemp;
	
	list<char *> expressionTemp;
	char * var1Ptr,
		 * var2Ptr,
		 * operPtr;

	unsigned short addrVar1 = 100,
				   addrVar2 = 100;

	int instructionTemp = -1;
	char Temp1 = 'T',
		Temp2 = 'S',
		Paranthese = 'A';

	while( etat != FIN )
	{
		switch( etat )
		{
			case INIT:
				// L'ITÉRATEUR POINTE SUR LE 1ER OPÉRATEUR
				listInstructionPtrIter++;

				//oper = *(*listInstructionPtrIter);
				etat = OPERATION;
				break;

			case OPERATION:
				oper = *(*listInstructionPtrIter);

				if( (oper != '(') && (oper != ')') )
				{
					// Isoler Une Expression
					while (expression(oper))
					{
						advance(listInstructionPtrIter, 2);
						oper = *(*listInstructionPtrIter);
					}
					// À la Sortie du WHILE, l'Itérateur pointe sur un '+' ou un '-'

					itListInstruction2 = listInstructionPtrIter;
					expressionTemp.insert(expressionTemp.begin(), itListInstruction1, itListInstruction2);

					// Le 1er Opérateur est un '*' ou un '/'
					if (expressionTemp.size() > 1)
					{
						listeInstrSMLTemp = evalExpression(expressionTemp);
						expressionTemp.clear();

						etat = TEMP;
						break;
					}
					else
					{
						addrVar1 = trouverAddresse2(*expressionTemp.begin());
						instructionTemp = 2000 + addrVar1; // LOAD VAR1

						listeInstrSMLTemp.push_back(instructionTemp);
					}

					// L'Itérateur pointe sur l'Opérateur qui suit
					// le '+' ou le '-', peut-être une PARANTHÈSE...
					advance(listInstructionPtrIter, 2);
					oper = *(*listInstructionPtrIter);

					// L'Opérateur est un '+' ou un '-'
					if (!expression(oper))
					{
						addrVar2 = trouverAddresse2(*expressionTemp.begin());

						if (oper == '+')
						{
							instructionTemp = 3000 + addrVar2; // ADD VAR2
						}
						else
						{
							instructionTemp = 3100 + addrVar2; // SUB VAR2
						}
						listeInstrSMLTemp.push_back(instructionTemp);
						/*var1Ptr = *listInstructionPtr.begin();

						advance(listInstructionPtrIter, -2);
						operPtr = *listInstructionPtrIter;
						listInstructionPtrIter++;
						var2Ptr = *listInstructionPtrIter;
						listInstructionPtrIter++;

						listeInstrSMLTemp = evalOperation(var1Ptr, *operPtr, var2Ptr);*/
					}
					else // encore un '*' ou un '/'
					{
						//// Sauvegarder VAR1
						//listeInstrSMLTemp = evalExpression(expressionTemp);
						//expressionTemp.clear();

						etat = TEMP;
					}

					//listInstructionPtrIter++;
					//itListInstruction1 = listInstructionPtrIter; // itListInstruction1 = itListInstruction2 + 1
				}
				else
				{
					if( oper != ')' )
					{
						paraFerm = true;
						iterParantheseFerm = listInstructionPtrIter;
						listInstructionPtrIter--;
						etat = PARANTHESE;
					}
					//else // (oper != '(')
					//{
					//	//
					//}
				}

				break;

			case PARANTHESE:
				while( (listInstructionPtrIter != listInstructionPtr.begin())  && !paraOuvr )
				{
					if( *(*listInstructionPtrIter) != '(' )
					{
						listInstructionPtrIter--;
					}
					else
					{
						iterParantheseOuvr = listInstructionPtrIter;
						paraOuvr = true;

						//etat = OPER_PRIO;
						break;
					}
				}

				// PEUT_ÊTRE QUE...
				// listInstructionPtrIter = listInstructionPtr.begin()
				if( *(*listInstructionPtrIter)  && !paraOuvr )
				{
					//EXIT...
					cout << "Paranthèse ouvrante manquante!!!!???????" << endl;
					_exit(-5);
				}
				else
				{
					etat = OPER_PRIO;
					paraOuvr = false;
				}

				break;

			case OPER_PRIO:
				if (multDiv)
				{
					expressionTemp.insert( expressionTemp.begin(), ++iterParantheseOuvr, iterParantheseFerm );
					etat = EVAL_EXPR;
				}
				else
				{
					etat = EVAL_OPER;
				}

				break;

			case EVAL_EXPR:
				//while( !finiExpr )
				//{
				//
				//	//etat = TEMP;
				//}
				evalExpression(expressionTemp);

				etat = TEMP;
				break;

			case EVAL_OPER:
				if( !finiOper )
				{
					//evalLesOperations();
					etat = ETAT_FINAL;
				}

				break;

			case ETAT_FINAL:
				if (paraFerm)
				{
					etat = REMPLACER_PARA;
				}
				else
				{
					etat = FIN;
				}

				break;

			case REMPLACER_PARA:
				listInstructionPtr.erase(iterParantheseOuvr, ++iterParantheseFerm);
				listInstructionPtr.push_front( &Paranthese );

				//etat = PARANTHESE1;
				etat = PARANTHESE;

				break;

			case TEMP:
				if( !temp1Util )
				{
					instructionTemp = 2100 + 99; // STORE LE RESULTAT DANS TEMP1
					listeInstrSMLTemp.push_back(instructionTemp);
					temp1Util = true;

					etat = TEMP1;
				}
				else
				{
					instructionTemp = 2100 + 98; // STORE LE RESULTAT DANS TEMP2
					listeInstrSMLTemp.push_back(instructionTemp);

					etat = TEMP2;
				}

				break;

			case TEMP1:
				etat = OPERATION;
				break;

			case TEMP2:
				etat = REMPLACER_TEMP;
				break;

			case REMPLACER_TEMP:
				listInstructionPtr.erase(itListInstruction1, itListInstruction2);
				listInstructionPtr.push_front( &Temp1 );

				etat = TEMP1;
				break;

			default:
				cout << "ÉTAT INCONNU!!!!!!!!!!!!!!!!" << endl;
				_exit(-6);
		}
	}
    
    return listeInstr;
}