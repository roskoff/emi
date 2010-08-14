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

/****************************************************************
 * Autor: Eliseo Ocampos
 * Nombre Archivo: Exceptions.h
 * Descripcion: 
 *   En este archivo se guardan todas las cabeceras de las clases
 * referentes a Excepciones.
 *
 ****************************************************************/
#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H


#include <iostream>


using namespace std;


/**
    Esta clase define una excepcion que es lanzada cuando
  se desea declarar un identificador que ya existe en la 
  tabla de simbolos.
    Se capturan objetos de esta clase en aquellos casos
  como el de 'EMIParser::def()', que tiene que asegurarse
  que no se dupliquen identificadores en la tabla
 
 */
class ExceptionIdentificadorDuplicado: public exception {
	public:
	   /** Construye con 'char id', para poder indicar
               cual fue el identificador que se duplico.
	       @param i como char
            */
	   ExceptionIdentificadorDuplicado( char i ): exception(), id( i ){};
	public:
	   /** Almacena el identificador duplicado. */
	   char id;
};

/** 
     Esta clase define una excepcion que es lanzada cuando
  se intenta realizar una operacion sobre matrices que 
  requiera que tengan el mismo orden.
     Simplemente se controlan las dimensiones de las 
  matrices que se tienen como operandos, y en caso de 
  incongruencia se lanza un objeto de esta clase, que se
  manipulara con el correspondiente manejador de excepciones.
 
 */
class ExceptionMatrizDistintoOrden: public exception {
	public:
	   /** Constructor por defecto. */
	   ExceptionMatrizDistintoOrden(): exception() {};
};

/** 
     Esta clase define una execepcion que es lanzada cuando
  se intenta realizar una operacion sobre una Matriz cuya condicion
  necesaria es que sea cuadrada.
     Operaciones como la adjunta de una matriz requieren de
  esta condicion, que de no cumplirse, se notifica como  error
  La operacion inversa, tambien requiere que sea cuadrada. 
 
 */
class ExceptionNoEsMatrizCuadrada: public exception {
	public:
	   /** Constructor por defecto. */
	   ExceptionNoEsMatrizCuadrada(): exception() {};
};

/** 
     Esta clase define una excepcion que es lanzada, cuando se 
  intenta realizar una operacion que necesita matrices regulares.
  Por 'regular' se entiende que la determinante de la matriz 
  tiene un valor distinto a cero.
 
 */
class ExceptionNoEsMatrizRegular: public exception {
	public:
	   /** Constructor por defecto. */
	   ExceptionNoEsMatrizRegular(): exception() {};
};

/** 
     Clase de excepciones creadas en base a los posibles errores
  de sintaxis que puedan surgir durante el parsing.
 
 */
class ExceptionParser: public exception {
	public:
	   /** Construye con un 'int pos', que se refiere a la
               posicion en la que se produjo el error de sintaxis. 
	       @param pos como int
	    */
	   ExceptionParser( int pos ): exception(), posicion( pos ){};
	public:
	   /** Indica la posicion en el comando del caracter erroneo. */
	   int posicion;

};
	
/** 
     Esta clase define una excepcion que es lanzada cuando se 
  intenta realizar alguna operacion sobre una variable que aun 
  no se ha definido. Esto significa que debio ser utilizada la
  instruccion 'def' para definirla y que se pueda 
  utilizar.
 
 */
class ExceptionVariableNoDefinida: public exception {
	public:
	   /** Construye con 'char id', para poder indicar
               cual fue el identificador que se duplico.
	       @param i como char
            */
	   ExceptionVariableNoDefinida( char i ): exception(), id( i ){};
	public:
	   /** Almacena el identificador de la variable no definida. */
	   char id;
};

/** 
     Esta clase se define para propagar un error que haya ocurrido.
  Esto incluye las demas excepciones, sin contar con ExceptionParser,
  de esta forma se evita que el paser continue "haciendo" su
  trabajo.
 
 */
class ExceptionError: public exception {
	public:
	   /** Constructor por defecto. */
	   ExceptionError(): exception(){};
};
#endif
