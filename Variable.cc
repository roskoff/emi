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
 * Nombre Archivo: Variable.cc
 * Descripcion:
 *    Contiene la implementacion indicada en el archivo
 * Variable.h. 
 *
 */

#include <iostream>
#include <string>

#include "Variable.h"
#include "Matriz.h"

#include "Exceptions.h"


//
// Constructores
//
/** Este constructor recibe todos los atributos que pueda
    tener una Variable. Son: un identificador, el valor de 
    la fila, el valor de la columna, y el dato en si que es 
    un objeto del tipo Matriz.
    @param id como char
    @param f como int
    @param c como int
    @param dato como Matriz *
*/
Variable::Variable( char identif, int f, int c, Matriz * dato ){
	id = identif;
	value = dato;  //new Matriz( dato );
	fila = f;
	columna = c;		 
}
   
//
// Funciones Miembro
//


/**
 * Obtiene el identificador de la variable.
 * 
 * @return char
 * 
 */		
char Variable::getId() {
	return ( id );
}
		
/**
 * Devuelve el valor de la variable.
 * 
 * @return Matriz *
 * 
 */		   
Matriz * Variable::getValue(){
	return ( value );
}
   
/**
 * Se utiliza para obtener el atributo 
 * fila de la variable.
 * 
 * @return int 
 *
 */				
int Variable::getFila(){
	return ( fila );
}
		
/**
 * Se utiliza para obtener el atributo 
 * columna de la variable.
 * 
 * @return int
 */
int Variable::getColumna(){
	return ( columna );
}
   
/**
 * Modifica el valor de la variable.
 *
 * @param m como Matriz * 
 * @return void
 *
 */		
void Variable::setValue( Matriz * m ){
	value = m;
	fila = m->fila;
	columna = m->columna;   
}
   
   
/**
 * Se implementa como utilidad de 
 * diagnosticos y verificacion.  
 * 
 * @return string
 *
 */
void Variable::imprimir(){
   	
	cout << "ID: " << id << " \nF: " << fila << " C: " << columna << "\n";
	value->imprimir();
  
}


