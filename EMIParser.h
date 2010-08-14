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

/*********************************************************************
 * Autor: Eliseo Ocampos
 * Nombre de Archivo: EMIParser.h
 * Descripcion:
 *    Esta clase es el Analizador Sintactico, y en ella estan defini-
 * -das todas las operaciones que son posibles realizar sobre una 
 * Variable.
 *
 *********************************************************************/

#ifndef __EMIParser_H
#define __EMIParser_H

#include <iostream>
#include <string> 

#include "SymTable.h"


#define MAX_FIL 25
#define MAX_COL 25


using namespace std;

class TokenReader;
class Variable;
class Matriz;

/**
   Esta clase es el 'motor' del proyecto. Cuenta con dos atributos
 * que son: una variable del tipo 'TokenReader' que es 'parseada', y 
 * otra que es del tipo 'SymTable' que es la tabla de simbolos. Tiene 
 * metodos para ir aplicando la siguiente sintaxis expresada en EBNF:
 *<BR> 
 *<BR>	<instr>		=> <def> | <operacion> | <impr> | <cargar> | <help>
 *<BR> 	<impr>		=> '?' <expr>
 *<BR>	<expr>		=> <expr_unaria> {'+' <expr_unaria> }
 *<BR>	<expr_unaria>	=> [<op_unario>] ( <ident> | <matriz> )
 *<BR>	<operacion>	=> <ident> '=' <expr>
 *<BR> 	<help> 		=> '?' ['def'|'print'|'cargar']
 *<BR>	<op_unario>	=> '*' | '^' | '!'
 *<BR>	<cargar>	=> 'cargar' <ident>
 *<BR>	<def>		=> 'def' <ident> ('=' <expr> | '['<nro>','<nro>']' [ '=>' <matriz>] ) 
 *<BR>	<matriz>	=> '[' <fila> {',' <fila>}']'
 *<BR>	<fila>		=> '[' <nro> {',' <nro>} ']'
 *<BR> 	<ident>		=> 'A' | .. | 'Z'
 *<BR> 	<nro>		=> '0' | .. | '9'
 */

class EMIParser
{
	// Constructor
	public:
	   /**
	     Construye un objeto con el comando solicitado, y sobre
	     el realiza el 'parsing' segun la sintaxis propuesta.
	   */
	   EMIParser( string s );

	// Atributos
	public:
	   /** Objeto TokenReader Para "tokenizar" la cadena de entrada. */
	   TokenReader * st;

	   /** Tabla de simbolos. */
	   static SymTable * tabla;

	   /** Atributos auxiliares. */
	   string tok_Aux;
	   char id_Aux;
	   int fila_Aux;
	   int columna_Aux;

	   int mayor_Col;

	   /** Array temporal para guardar constantes de matrices, se 
	    * utiliza en el metodo fila().
	    */
	   double matriz_Aux [MAX_FIL][MAX_COL];

	// Funciones Miembro
	private:
	/**
	 * Procesa la instruccion introducida por el usuario. Deriva al 
	 *    metodo correspondiente dependiendo de si es una instruccion de 
	 * definicion, carga, impresion, u operacion; o lanza una excepcion que es 
	 * atrapada por la funcion main() para imprimir el error de sintaxis en la 
	 * posicion adecuada.
	 * 
	 * @return void
	 *
	 */
	   void instr();

	/**
	 * Se encarga de imprimir una expresion. La excepcion
	 *    ExceptionParser es lanzada si el siguiente caracter introducido
	 * despues del '?' no esta permitido en la sintaxis.
	 * 
	 * @return void
	 * 
	 */
	   void impr();

	/**
	 * Imprime un texto de ayuda, por defecto imprime los 
	 * comandos disponibles, y si se le pasa como argumento
	 * uno de esos comandos presenta una ayuda especifica
	 * sobre el mismo.
	 */
	   void help();

	/** Ayuda especifica sobre el comando 'print'.
	 *  Presenta el siguiente texto:
	 *  :.: print <expresion>
	 *        Donde <expresion> puede ser:
	 *     . constante    : [[1,2,3],[4,5,6],[7,8,9]]
	 *     . identificador: 'A' a la 'Z'
	 *     . operaciones  : A + *D - ^[[1,2],[3,4]]
	 */
	   void help_print();

	/** Ayuda especifica para el comando 'def'.
	 *  Presenta el siguiente texto:
	 *  :.: def <id> <dim> [ => <matriz> ]
	 *      Donde <id>  : caracter de la 'A' a la 'Z'
	 *            <dim> : dimensiones de la matriz.
	 *		      Ej.: [2,2] matriz de 2x2,
	 *		           [5,3] matriz de 5x3 
	 * 	y opcionalmente el signo de asignacion '=>' y
	 *      una matriz constante de la forma:
	 *      [ [<elem>,<elem>... ],[<elem>,<elem>...]...]
	 *      Ej.: def A[2,2] => [[1,0],[0,1]]
	 */
	   void help_def();
	
	/** Ayuda especifica para el comando 'cargar'.
	 * Presenta el siguiente texto:
	 * :.: cargar <id>
	 *     Donde <id> puede ser cualquier identificador
	 *     valido.
	 */
	   void help_cargar();
	
	/**
	 * Es llamado luego de haber encontrado algun token legal de la 
	 * siantaxis y evalua la expresion que corresponda. Lanza 
	 * ExceptionParser si el siguiente caracter es ilegal en la sintaxis.
	 * 
	 * @return Variable *
	 *
	 */
	   Variable * expr();


	/**
	 * Es llamada por la funcion expr(). Es quien enlaza las 
	 *    operaciones con su variables, si se trata de variables definidas con 
	 * anterioridad por el usuario o anonimas; si se tratara de una constante
	 * del tipo Matriz llama al metodo fila().
	 *    Tambien se ha tenido en cuenta que las operaciones deben modificar a 
	 * las variables utiliazadas en las operaciones, para lo cual se utiliza el 
	 * metodo clonar() de la clase SymTable.
	 * 
	 * @return Variable * 
	 *
	 */
	   Variable * expr_unaria();

	/**
	 * Evalua si la expresion es una operacion, y llama
	 *    al metodo expr() para que el haga efectiva la operacion
	 * si esta bien la sintaxis, y luego realiza la modificacion en la
	 * tabla con el resultado de expr(). Para que la operacion
	 * sea exitosa la variable a la que se quiere asignar la operacion
	 * debe haber sido definida con anterioridad por el usuario.
	 * ExceptionParser es lanzada si el siguiente caracter
	 * es ilegal en la sintaxis.
	 * 
	 * @return void
	 * 
	 */
	   void operacion();

	/**
	 * Efectua la carga interactiva con el operador llamando al 
	 * metodo cargaInteracativa() definido en Matriz.
	 * ExceptionParser es lanzada si el siguiente caracter es ilegal en 
	 * la sintaxis.
	 * 
	 * @return void
	 * 
	 */
	   void cargar();

	/**
	 * Es para definir una Variable. 
	 *    Segun la sintaxis, opcionalmente en la linea de definicion se puede 
	 * inicializar la Variable con valores. Si la instruccion es solo una
	 * definicion se agrega a la tabla con los atributos fila y columna 
	 * ingresados por el operador y valores por defecto, en caso de que vaya
	 * acompanhada de una inicializacion debe llamarse al metodo matriz().
	 *    La definicion tambien puede ser basada en una operacion con Variables
	 * anteriormente definidas por el operador, en ese caso debe parsearse
	 * una expr().
	 * ExceptionParser es lanzada si el siguiente caracter es ilegal 
	 * en la sintaxis.
	 * 
	 * @return void
	 * 
	 */
	   void def();

	/**
	 * Parsea una fila, es llamado por el metodo matriz().
	 *    Utiliza un vector (array de una dimension) definido el metodo 
	 * llamador, para almacenar los valores captados para los elementos.
	 *    Si la asgnacion fue exitosa el vector estara cargado con los valores
	 * ingresados por el operador. ExceptionParser es lanzada si 
	 * el siguiente caracter es ilegal en la sintaxis
	 * 
	 * @return void
	 *
	 */
	   void fila( double m[MAX_FIL][MAX_COL] );

	/**
	 * matriz(): Parsea una constante del tipo Matriz. Llama al metodo 
	 *    fila() para parsear una fila.
	 * ExceptionParser es lanzada si el siguiente caracter es ilegal
	 * en la sintaxis.
	 * 
	 * @return Variable *
	 *
	 */
	   Variable * matriz();

	/**
	 * Construye el objeto ExceptionParser con un int
	 * que es un atributo de la variable 'st' (TokenReader) para que la consola 
	 * pueda indicar la posicion de la cadena en donde esta el error de sintaxis.
	 *
	 * @return void
	 * 
	 */
	   void error();

	   void select_operacion( char operador, Variable *, Matriz * );

};
#endif
