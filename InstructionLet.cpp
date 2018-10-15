//
//  InstructionLet.cpp
//  CompilateurSML
//
//  Created by Eric Safari on 2017-06-08.
//  Copyright © 2017 Eric Safari. All rights reserved.
//

#include <iostream>
#include "InstructionLet.h"
#include <cstring>
#include <vector>

InstructionLet::InstructionLet( string command, string parametre, unsigned short line )
:Instruction( "LET", parametre, line )
{
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
    
    // 1 - Il faut trouver une adresse pour la Variable Dépendante
    // -------------------------------------------------------
    //    cle = *getParam().c_str();
    equation = (char *)getParam().c_str();
    
    // Extraction de la Var. dép.
    cleVarDep = *strtok( equation, " " );
    vecteurInstruction.push_back( cleVarDep );
    
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
    
    // 2 - Sauvegarder les éléments de l'Instruction
    //     sans les Espaces Blancs
    while ( equation != NULL)
    {
        clePtr = strtok( NULL, " " );
        
        if( sizeof( clePtr ) == 1 )
        {
            vecteurInstruction.push_back( *clePtr );
        }
        else
        {
            if ( *clePtr == '(' )
            {
                nbParantOuvr++;
            }
            else if ( *(clePtr+1) == ')' )
            {
                nbParantFerm++;
            }
            
            vecteurInstruction.push_back( *clePtr );
            vecteurInstruction.push_back( *( clePtr + 1 ) );
        }
        cle = *clePtr;
    }
    
    // TEST CRUCIAL
    if ( nbParantOuvr != nbParantFerm )
    {
        cout << "Le nombre de paranthèses ouvrantes( " << nbParantOuvr << ") n'est pas égal au nombre de paranthèses fermantes( " << nbParantFerm << " )!!!" << endl;
        _exit(-3);
    }
    
    // 3 - Parcourir les éléments de l'Équation et
    //     commencer par les Opérations Prioritaires
    vector<char>::iterator itInstruction,
                           itInstruction1,
                           itInstruction2;
    itInstruction = vecteurInstruction.begin();
    
    while( itInstruction != vecteurInstruction.end() )
    {
        //while ( ( *itInstruction != ')' ) /*&& (itInstruction != vecteurInstruction.end())*/ )
        if( (*itInstruction != ')') && (nbParantFerm > 0) )
        {
            itInstruction++;
        }
        else // *itInstruction = ')'
        {
            if( nbParantFerm > 0 )
            {
                // Dernière paranthèse
                itInstruction2 = itInstruction;
                
                do
                {
                    itInstruction--;
                }
                while ( (*itInstruction != '(') && (*itInstruction != ')') && (itInstruction != (vecteurInstruction.begin()) - 1) );
                
                // Instruction1 pointe sur vecteurInstruction.begin() OU AVANT
                itInstruction1 = itInstruction;
            }
            else
            {
                itInstruction1 = vecteurInstruction.begin() - 1;
                itInstruction2 = vecteurInstruction.end();
            }
            // Les SOUS-STRING ne sont pas nécessaires
            vector<char> sousInstr( itInstruction1 + 1, itInstruction2 - 1);
            vector<char>::iterator itSousInstr;
            
            // Maintenant il faut trouver l'Opérateur(*,/,+,-)
            char cle1,
                 cle2;
            
            itSousInstr = sousInstr.begin();
            while ( itSousInstr != sousInstr.end() )
            {
                // TROUVER L'OPÉRATION À EFFECTUER, ENSUITE
                // L'ADRESSE de VAR1 et VAR2, ENSUITE LOAD et MULT
                
                // TROUVER L'OPÉRATEUR(*,/,+,-)
                if ( (*itSousInstr == '*') || (*itSousInstr == '/') ) // IL FAUT TENIR COMPTE DES PARANTHÈSES
                {
                    // ADDR VAR2
                    cle2 = *( itSousInstr + 1 );
                    addrVarInd = Instruction::getMap()[ cle2 ];
                    
                    if( *itSousInstr == '*' )
                    {
                        // MULT VAR1, VAR2
                        instruction = 3300 + addrVarInd;
                    }
                    else
                    {
                        // DIVD VAR1, VAR2
                        instruction = 3200 + addrVarInd;
                    }
                    // Ajouter l'Instruction dans la Liste
                    listeInstr.push_back( instruction );
                    //break; // Sortie du While une fois rendu à la fin
                    itSousInstr++;
                }
                else if ( (*itSousInstr == '+') || (*itSousInstr == '-') )
                {
                    // ADDR VAR2
                    cle2 = *( itSousInstr + 1 );
                    addrVarInd = Instruction::getMap()[ cle2 ];
                    
                    if( *itSousInstr == '+' )
                    {
                        // ADD VAR1, VAR2
                        instruction = 3000 + addrVarInd;
                    }
                    else
                    {
                        // SUB VAR1, VAR2
                        instruction = 3100 + addrVarInd;
                    }
                    // Ajouter l'Instruction dans la Liste
                    listeInstr.push_back( instruction );
                    //break; // Sortie du While une fois rendu à la fin
                    itSousInstr++;
                }
                else // VARIABLES et CONSTANTES
                {
                    // ADDR VAR1
                    cle1 = *itSousInstr;
                    addrVarInd = Instruction::getMap()[ cle1 ];
                    
                    // LOAD VAR1 => ( ACC = VAR1 )
                    instruction = 2000 + addrVarInd;
                    // Ajouter l'Instruction dans la Liste
                    listeInstr.push_back( instruction );
                }
                
                itSousInstr++;
            }
            // STORE DS LA VAR DÉP
            instruction = 2100 + addrVarDep;
            listeInstr.push_back( instruction );
            
            // Chercher les paranthèses suivantes
            // Commencer après la PARANTHÈSE FERMANTE précédente
            itInstruction = itInstruction2 + 1;
            
            // Enlever la Paranthèse pour la remplacer
            // par La Variable Dépendante
            vecteurInstruction.erase( itInstruction1, itInstruction2 );
            vecteurInstruction.insert( itInstruction, cleVarDep ); // VAUT MIEUX UNE LISTE PLUTOT QU'UN VECTEUR
        }
    }
    
    return listeInstr;
}