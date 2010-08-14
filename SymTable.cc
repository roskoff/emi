/*
 * Copyright (C) EMI Eliseo Ocampos <roskoff@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

/**
 * Autor: Eliseo Ocampos
 * Nombre Archivo: SymTable.cc
 * Descripcion:
 *    Implementacion de todos los metodos que necesitaremos en nuestra
 * "tabla de simbolos".
 *
 */

#include "SymTable.h"
#include "Variable.h"

#include "Exceptions.h"


//
// Constructores
//

/**
 * Se encarga de inicializar el array con MAX_SIZE elementos,
 * todos ellos con valores null por defecto.
 */
SymTable::SymTable(){
	ultimo = 0;
	for( int i = 0; i < MAX_SIZE; i++ )
		tabla[i] = new Variable();
}

//
// Funciones Miembro
//

/**
 * Agrega una Variable a la tabla de simbolos, solo en el caso en que
 *    no se encuentre ya definida.
 * IN : Variable * var
 * OUT: bool
 *
 */	
bool SymTable::add( Variable * var ) throw ( ExceptionIdentificadorDuplicado ){
	int temp;
	try {
	/* Se hace una busqueda en la tabla para ver si ya esta definida,
	 * en el caso de que lookup() encuentre un identificador que tenga 
	 * ese nombre, se lanza 'ExceptionIdentificadorDuplicado'.
	 */         
         
		temp = lookup( var->getId());
		throw ExceptionIdentificadorDuplicado( var->getId());        
	}  
	catch ( ExceptionVariableNoDefinida &e ){
	/* Si se genera esta excepcion, significa que esta variable no se 
	 * encuentra actualmente en la tabla de simbolos, por lo tanto se puede
	 * guardar una nueva con este identificador en la posicion 'ultimo'
	 * del array 'tabla', luego se incrementa 'ultimo' para definir en esa
	 * posicion la proxima variable. Al final se retorna verdadero.
	 */
		tabla[ultimo] = var;
		ultimo++;
		return ( true );
	}   	     
}
   
/**
 * Realiza una busqueda lineal en el array 'tabla', si llegara a 
 *    encontrar alguna Variable, retorna su posicion, de lo contrario
 * lanza la excepcion 'ExceptionVariableNoDefinida'.
 * IN : char identif
 * OUT: int
 *
 */		
int SymTable::lookup( char identif ) throw ( ExceptionVariableNoDefinida ){
	for( int i = 0; i < ultimo; i++)
		if ( tabla[i]->getId() == identif ){
			return ( i ); // Retorna la posicion de la Variable.
		}      
	// No se encontro el elemento. 
	throw ExceptionVariableNoDefinida( identif );
}
   
/**
 * Devuelve la Variable llamada 'identif', si se encuentra en
 *    la tabla de simbolos. De otro modo propaga la excepcion
 * 'ExceptionVariableNoDefinida' lanzada por lookup()
 * 
 * @param identif como char
 * @return Variable *
 *
 */
Variable * SymTable::retrieve ( char identif ) throw ( ExceptionVariableNoDefinida ){
	return ( tabla[ lookup( identif )] );   	
}

/**
 * Se utiliza para modificar el ataributo value con el valor de
 *    la otra variable.
 *
 * @param vieja como Variable *
 * @param nueva como Variable *
 * @return bool
 * 
 */
bool SymTable::modify( Variable * vieja, Variable * nueva ) throw ( ExceptionVariableNoDefinida ){
	// Identificador a recuperar de la tabla de simbolos
	char identificador  = vieja->getId();
         
	// Posicion en la que se encuentra la Variable, si es que esta definida
	int pos = lookup( identificador );
         
	// Elije la variable recuperada de la tabla de simbolos
	// y modifica su 'value' con el 'value' de la Variable 'nueva'
	tabla[ pos ]->setValue ( nueva->getValue());

	return ( true );     
}
   
/**
 * Utilizado en los casos que se quiera realizar la impresion del 
 *    resultado de una operacion sin que dicho resultado sea asignado
 * exlicitamente a una variable, es decir, crea una variable 'anonima'.
 *
 * @param identificador como char
 * @return Variable *
 *
 */		
Variable * SymTable::clonar( char identificador ) throw ( ExceptionVariableNoDefinida ){
	Variable * muestra = new Variable ();
       
	muestra = tabla[ lookup( identificador ) ];
       
	Variable * resultado = new Variable( muestra->getId(), muestra->getFila(), muestra->getColumna(), muestra->getValue());
	
	return ( resultado );
}
