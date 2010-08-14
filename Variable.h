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
 * Nombre Archivo: Variable.h
 */

#ifndef __VARIABLE_H
#define __VARIABLE_H


#include <string>

using namespace std;

class Matriz;

/**
 *    Contiene todos los atributos y metodos necesarios
 * para manipular una matriz como si fuera una variable.
 *    Nos sirve para crear la tabla de simbolos.
 *
 */
class Variable
{
	// Constructores
	public:
	/** Este constructor recibe todos los atributos que pueda
	    tener una Variable. Son: un identificador, el valor de 
	    la fila, el valor de la columna, y el dato en si que es 
	    un objeto del tipo Matriz.
	    @param id como char
	    @param f como int
	    @param c como int
	    @param dato como Matriz *
	 */
	   Variable( char id, int f, int c, Matriz * dato );

	   Variable(){};

	// Atributos
	private:
	   char id; 		/**< El identificador. */
	   Matriz * value; 	/**< El valor que representa. */
	   int fila;		/**< El valor de la fila. */
	   int columna;		/**< El valor de la columna. */

	// Funciones Miembro
	public:
	/**
	 * Obtiene el identificador de la variable.
	 * 
	 * @return char
	 * 
	 */
	   char getId();

	/**
	 * Devuelve el valor de la variable.
	 * 
	 * @return Matriz *
	 * 	
	 */
	   Matriz * getValue();

	/**
	 * Se utiliza para obtener el atributo 
	 * fila de la variable.
	 * 
	 * @return int 
	 *
	 */
	   int getFila();

	/**
	 * Se utiliza para obtener el atributo 
	 * columna de la variable.
	 * 
	 * @return int
	 */
	   int getColumna();

	/**	
	 * Modifica el valor de la variable.
	 *
	 * @param m como Matriz * 
	 * @return void
	 *
	 */
	   void setValue( Matriz * m );

	/**
	 * Se implementa como utilidad de 
	 * diagnosticos y verificacion.  
	 * 
	 * @return void 
	 *
	 */
	   void imprimir();

};

#endif

