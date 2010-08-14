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
 * Nombre Archivo: TokenReader.h
 */

#ifndef __TOKENREADER_H
#define __TOKENREADER_H


#include <string>

using namespace std;

/**
 *    Implementa un scaner sencillo, que se encarga de separar en token's
 * la entrada, y provee estos componentes lexicos al EMIParser para su
 * posterior analisis sintactico.
 *
 */
class TokenReader
{
	// Constructores
	public:
	/** Crea el objeto TokenString con el comando introducido. */
	   TokenReader( string s );

	// Atributos
	private:
	/** Almacena el comando introducido. */
	   string str;

	public:
	/** Posicion en la que se esta haciendo el scanning. */
	   unsigned int pos;

	   enum tok_type {
		TOK_NUMERO = 0,/**< Cualquier valor numerico. */
		TOK_OP_SUMA,   /**< + */
		TOK_OP_RESTA,  /**< - */
		TOK_OP_TRAS,   /**< ^ */
		TOK_OP_ADJUN,  /**< * */
		TOK_OP_INVER,  /**< ! */

		TOK_OP_IMPRE,  /**< print */
 
		TOK_COR_IZQ,   /**< [ */
		TOK_COR_DER,   /**< ] */

		TOK_IGUAL,     /**< = */
		TOK_ASIG,      /**< => */

		TOK_COMA,      /**< ,  */

		TOK_DEF,       /**< def */
		TOK_CARGAR,    /**< cargar */
		TOK_HELP,      /**< ? */

		TOK_ID,        /**< A - Z */
	
		TOK_ERROR = 100,
		TOK_EOF,
		TOK_NULL} current_TOK ;



	// Funciones Miembro.
	private:
	/**
	 * Verificar que se ha escrito correctamente la palabra 'def',
	 *    utilizada para definir las matrices. En realidad, puede devolver una 
	 * cadena compuesta por cualquier combinacion de las letras 'd', 'e', y 'f',
	 * y luego se compara el valor devuelto contra la constante 'def'.
	 * 
	 * @return string
	 *
	 *
	   string verif_def();
	 */
	/**
	 * Al igual que verif_def(), verifica si la palabra cargar fue
	 *    introducida. Concatena los caracteres encontrados y luego compara si es o
	 * no igual a la constante 'cargar'.
	 *
	 * @return string
	 *
	 *
	   string verif_cargar();
	 */
	/**
	 * Y lo mismo para la palabra reservada "print", se toman los caracteres
	 *    y luego compara si es o no igual a la constante 'print'.
	 *
	 * @return string
	 *
	 */
	   string verif_pal_res();

	 

	/**
	 * Verifica si es un numero valido de acuerdo a la definicion del 
	 *    problema.
	 * 
	 * @return string
	 *
	 */
	   string es_Numero();

	public:
	/**
	 * Es una funcion de utileria, indica simplemente si a la
	 *    cadena de entrada le restan o no token's.
	 * 
	 * @return bool
	 *
	 */
	   bool hasMoreTokens();

	/**
	 * Es la mas importante de todas, se encarga, con la ayuda de
	 *    las demas funciones, de generar el token que le corresponde a cada
	 * componente lexico encontrada en la cadena de entrada. Una vez encontrado
	 * el token se asigna a current_TOK, que es el atributo que mantiene el
	 * token que se esta procesando actualmente.
	 * 
	 * @return string
	 *
	 */
	   string nextToken();
	   
};

#endif
