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
 * Nombre Archivo: SymTable.h
 */

#ifndef __SYMTABLE_H
#define __SYMTABLE_H

#include "Exceptions.h"

#define MAX_SIZE 30

using namespace std;

class Variable;

/**
 *    Esta clase define una "tabla de simbolos",
 * con funciones miembro necesarios para manipular
 * objetos del tipo Variable. Escencialmente, es
 * un array que se controla y modifica durante la 
 * ejecucion, segun vaya siendo necesario, es decir,
 * de acuerdo a las operaciones que se vayan realizando.
 */
class SymTable
{

	// Constructores
	public:
	/**
	 * Se encarga de inicializar el array con MAX_SIZE elementos,
	 * todos ellos con valores null por defecto.
	 */
	   SymTable();

	// Atributos
	private:
	/** Array de una dimension destinado a almacenar Variables definidas. */
	   Variable * tabla [ MAX_SIZE ];

	public:
	/** Posicion de la ultima variable definida. */
	   int ultimo;

	// Funciones Miembro
	public:
	/**
	 * Agrega una Variable a la tabla de simbolos, solo en el caso en que
	 *    no se encuentre ya definida.
	 *
	 * @param var como variable *
	 * @return bool
	 *
	 */
	   bool add( Variable * var ) throw ( ExceptionIdentificadorDuplicado );

	/**
	 * Realiza una busqueda lineal en el array 'tabla', si llegara a 
	 *    encontrar alguna Variable, retorna su posicion, de lo contrario
	 * lanza la excepcion 'ExceptionVariableNoDefinida'.
	 *
	 * @param identif como char
	 * @return int
	 *
	 */	
	   int lookup( char identif ) throw ( ExceptionVariableNoDefinida );

	/**
	 * Devuelve la Variable llamada 'identif', si se encuentra en
	 *    la tabla de simbolos. De otro modo propaga la excepcion
	 * 'ExceptionVariableNoDefinida' lanzada por lookup()
	 * 
	 * @param identif como char
	 * @return Variable *
	 *
	 */
	   Variable * retrieve ( char identif ) throw ( ExceptionVariableNoDefinida );

	/**
	 * Se utiliza para modificar el ataributo value con el valor de
	 *    la otra variable.
	 *
	 * @param vieja como Variable *
	 * @param nueva como Variable *
	 * @return bool
	 * 
	 */
	   bool modify( Variable * vieja, Variable * nueva ) throw ( ExceptionVariableNoDefinida );

	/**
	 * Utilizado en los casos que se quiera realizar la impresion del 
	 *    resultado de una operacion sin que dicho resultado sea asignado
	 * exlicitamente a una variable, es decir, crea una variable 'anonima'.
	 *
	 * @param identificador como char
	 * @return Variable *
	 *
	 */
	   Variable * clonar( char identificador ) throw ( ExceptionVariableNoDefinida );


};

#endif
