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
 * Autor : Cristian Cappo
 * Modificaciones : Eliseo Ocampos
 * Nombre de Archivo: TokenReader.cc
 *    Implementa un scaner sencillo, que se encarga de separar en token's
 * la entrada, y provee estos componentes lexicos al EMIParser para su
 * posterior analisis sintactico.
 *
 */

#include <string>

#include "TokenReader.h"

#include "Exceptions.h"

using namespace std;


// 
// Constructores
//
TokenReader::TokenReader( string s ) {
	str = s;
	pos = 0;
	current_TOK = TOK_NULL;
}
    
//
// Funciones Miembro
//

/**
 * Verificar que se ha escrito correctamente la palabra 'def',
 *    utilizada para definir las matrices. En realidad, puede devolver una 
 * cadena compuesta por cualquier combinacion de las letras 'd', 'e', y 'f',
 * y luego se compara el valor devuelto contra la constante 'def'.
 * 
 * @return string
 *
 *
string TokenReader::verif_def() {
	string tok = "";
	char c = str[pos];
	while ( c >= 'd' && c <= 'f' ){
		tok += c;
		pos++;
		if ( pos < str.length() )
			c = str[pos];
		else
			break;
	}//while
	pos--;
	return tok;
}		
 */   
/**
 * Al igual que verif_def(), verifica si la palabra cargar fue
 *    introducida. Concatena los caracteres encontrados y luego compara si es o
 * no igual a la constante 'cargar'.
 *
 * @return string
 *
 *		
string TokenReader::verif_cargar() {
	string tok = "";
	char c = str[pos];
	while ( c >= 'a' && c <= 'r' ){
		tok += c;
		pos++;
		if ( pos < str.length() )
			c = str[pos];
		else
			break;
	}//while
	pos--;
	return tok;
}				
*/
/**
 * 
 * 
 *
 *
 * @return string
 *
 */		
string TokenReader::verif_pal_res() {
	string tok = "";
	char c = str[pos];
	while ( c >= 'a' && c <= 'z' ){
		tok += c;
		pos++;
		if ( pos < str.length() )
			c = str[pos];
		else
			break;
	}//while
	pos--;
	return tok;
}				
		
/**
 * Verifica si es un numero valido de acuerdo a la definicion del 
 *    problema.
 * 
 * @return string
 *
 */		
string TokenReader::es_Numero() {
	string tok = "";
	char c =  str[pos];
	while ( c >= '0' && c  <= '9' ) {
		tok = tok + c;
		pos++;
		if ( pos < str.length() ) 
			c = str[pos];
		else
			break;
	}//while
	pos--;
	return tok;
}

/**
 * Es una funcion de utileria, indica simplemente si a la
 *    cadena de entrada le restan o no token's.
 * 
 * @return bool
 *
 */
bool TokenReader::hasMoreTokens() {
	return ( pos < str.length() );
}    
    
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
string TokenReader::nextToken() {
	string tok = "";
	current_TOK = TOK_ERROR;
	//pos++;
	if ( pos < str.length() ) {
		char c = str[pos];
		// Eliminamos todos los blancos de la expresion
		while ( c == ' ' ) {
			pos++;
			if ( pos < str.length() )
				c = str[pos];
			else {
				current_TOK = TOK_EOF;
				return "";
			}//if
		}//while
		// Verificar si es un numero
		if ( c >= '0' && c <= '9' ) {
			tok = es_Numero();
			current_TOK = TOK_NUMERO;
		} // Verificamos si es un Identificador
		else if ( c >= 'A' && c <= 'Z' || c == 'Ñ' ){
			tok[0] = c; 
			tok[1] = '\n';
			current_TOK = TOK_ID;
		} else // Verificamos los operadores
			switch ( c )  {
			case '+' : tok = "+"; current_TOK = TOK_OP_SUMA; break;
			case '-' : tok = "-"; current_TOK = TOK_OP_RESTA; break;
			case '^' : tok = "^"; current_TOK = TOK_OP_TRAS;  break;
			case '*' : tok = "*"; current_TOK = TOK_OP_ADJUN; break;
			case '!' : tok = "!"; current_TOK = TOK_OP_INVER; break;
			case '?' : tok = "?"; current_TOK = TOK_HELP; break;
			
			case ']' : tok = "]"; current_TOK = TOK_COR_DER; break;
			case '[' : tok = "["; current_TOK = TOK_COR_IZQ; break;
             
			case '=' : tok = "=";
				if ( (c = str[++pos]) == '>' ){
					tok += c;
					current_TOK = TOK_ASIG;
				}else{
					current_TOK = TOK_IGUAL;
					pos--;
				}
				break;
			case ',' : tok = ","; current_TOK = TOK_COMA;  break;
			case 'd' : tok = verif_pal_res();
				if ( tok == "def" ) current_TOK = TOK_DEF; break;
			case 'c' : tok = verif_pal_res();
				if ( tok ==  "cargar" ) current_TOK = TOK_CARGAR; break;              
			case 'p' : tok = verif_pal_res();
				if ( tok == "print" ) current_TOK = TOK_OP_IMPRE; break;
			}//swhitc
	}//if
	pos++;
	return ( tok );
}//nextToken    

