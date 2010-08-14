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

/************************************************************************
 * Autor: Eliseo Ocampos
 * Nombre del Archivo: EMIParser.cc
 * Descripcion: 
 *    Esta clase es el 'motor' del proyecto. Cuenta con dos atributos
 * que son: una variable del tipo 'TokenReader' que es 'parseada', y 
 * otra que es del tipo 'SymTable' que es la tabla de simbolos. Tiene 
 * metodos para ir aplicando la siguiente sintaxis expresada en EBNF:
 *<BR>* 
 *<BR>*	<instr>		=> <def> | <operacion> | <impr> | <cargar> | <help>
 *<BR>* 	<impr>		=> 'print' <expr>
 *<BR>*	<expr>		=> <expr_unaria> {'+' <expr_unaria> }
 *<BR>*	<expr_unaria>	=> [<op_unario>] ( <ident> | <matriz> )
 *<BR>*	<operacion>	=> <ident> '=' <expr>
 *<BR>*	<help>		=> '?' ['def'|'print'|'cargar']
 *<BR>*	<op_unario>	=> '*' | '^' | '!'
 *<BR>*	<cargar>	=> 'cargar' <ident>
 *<BR>*	<def>		=> 'def' <ident> ('=' <expr> 
 *<BR>*			   | '['<nro>','<nro>']' [ '=>' <matriz>] ) 
 *<BR>*	<matriz>	=> '[' <fila> {',' <fila>}']'
 *<BR>*	<fila>		=> '[' <nro> {',' <nro>} ']'
 *<BR>* 	<ident>		=> 'A' | .. | 'Z'
 *<BR>* 	<nro>		=> '0' | .. | '9'
 *<BR>* 
 **************************************************************************/

#include <iostream>
#include <string>
#include <stdlib.h>

#include "EMIParser.h"
#include "Matriz.h"
#include "TokenReader.h"
#include "Variable.h"

#include "Exceptions.h"


//
// Atributos Estaticos
//

/** Un puntero estatico a la tabla de simbolos. */
SymTable * EMIParser::tabla;

//
// Atributos Auxiliares
//
int fila_Cont;
int columna_Cont;

//
// Constructores
// 

// Constructor
/**
  Construye un objeto con el comando solicitado, y sobre
  el realiza el 'parsing' segun la sintaxis propuesta.
 */
EMIParser::EMIParser( string s ) {

	fila_Aux = 26;
	columna_Aux = 26;

	mayor_Col = 0;

	st = new TokenReader( s );
	while ( st->hasMoreTokens() ){
		tok_Aux = st->nextToken();
		instr();
	}
}

//
// Funciones Miembro
//

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
void EMIParser::instr() {
	if ( st->current_TOK ==  st->TOK_DEF )
		def();
	else if ( st->current_TOK ==  st->TOK_CARGAR )
		cargar();
	else if ( st->current_TOK ==  st->TOK_OP_IMPRE )
		impr();
	else if ( st->current_TOK ==  st->TOK_HELP)
		help();
	else if ( st->current_TOK == st->TOK_ID){
		// Se guarda el Id
		id_Aux = tok_Aux[ 0 ];
		operacion();
	}else
		error ();					
}  

/**
 * Se encarga de imprimir una expresion. La excepcion
 *    ExceptionParser es lanzada si el siguiente caracter introducido
 * despues del '?' no esta permitido en la sintaxis.
 * 
 * @return void
 * 
 */		
void EMIParser::impr (){	     
	tok_Aux = st->nextToken();
	if ( st->current_TOK != st->TOK_ERROR ){	
		expr()->imprimir();
	}else 
		error(); 		
}
	
/**
 * Imprime un texto de ayuda, por defecto imprime los 
 * comandos disponibles, y si se le pasa como argumento
 * uno de esos comandos presenta una ayuda especifica
 * sobre el mismo.
 */
void EMIParser::help()
{
	tok_Aux = st->nextToken();

	if ( st->current_TOK == st->TOK_OP_IMPRE )
		help_print();
	else if ( st->current_TOK == st->TOK_DEF )
		help_def();
	else if ( st->current_TOK == st->TOK_CARGAR )
		help_cargar();
	else {
	   cout << "Listado de comandos disponibles -- EMI \n\n"
	        << " :.: print <expresion> \n"
	        << "       Imprime una salida formateada de <expresion>,\n"
	        << "     que puede ser una variable o constante de Matriz o\n"
        	<< "     una expresion entre ellas.\n\n"
	        << " :.: def <id> <dim> [ => <matriz> ]\n"
	        << "       Define una matriz llamada <id>, de dimension <dim> y \n"
		<< "     con valores iniciales opcionales.\n\n"
	        << " :.: cargar <id>\n"
	        << "       Carga en forma \"interactiva\" la matriz llamada\n"
		<< "     <id> solicitando al usuario que ingrese cada elemento.\n\n"
		<< " Obs.: Para obtener una ayuda especifica, puede tipear '?' seguido\n"
		<< "       de alguno de los comandos citados mas arriba.\n"
	        << endl;
        }
}

/** Ayuda especifica sobre el comando 'print'.
 */
void EMIParser::help_print()
{
	cout 	<< " :.: print <expresion>\n"
		<< "       Donde <expresion> puede ser:\n"
		<< "     . constante    : [[1,2,3],[4,5,6],[7,8,9]]\n"
		<< "     . identificador: 'A' a la 'Z'\n"
		<< "     . operaciones  : A + *D - ^[[1,2],[3,4]]\n"
		<< "\n"
		<< "     Los operadores disponibles son:\n"
		<< "     + suma dos matrices: A + B\n"
		<< "     - resta dos matrices: A - B\n"
		<< "     ^ traspuesta de una matriz: ^A\n"
		<< "     * adjunta de una matriz: *B\n"
		<< "     ! inversa de una matriz: !B\n"
		<< endl;	

	tok_Aux = st->nextToken();
}

/** Ayuda especifica para el comando 'def'.
 */
void EMIParser::help_def()
{
	cout	<<  ":.: def <id> <dim> [ => <matriz> ]\n"
		<<  "    Donde <id>  : caracter de la 'A' a la 'Z' (solo mayusculas)\n"
		<<  "          <dim> : dimensiones de la matriz.\n"
		<<  "		      Ej.: [2,2] matriz de 2x2,\n"
		<<  "		           [5,3] matriz de 5x3 \n"
		<<  "	y opcionalmente el signo de asignacion '=>' y\n"
		<<  "    una matriz constante de la forma:\n"
		<<  "    [ [<elem>,<elem>... ],[<elem>,<elem>...]...]\n"
		<<  "    Ej.: def A[2,2] => [[1,0],[0,1]]\n"
		<< endl;
	tok_Aux = st->nextToken();
}

/** Ayuda especifica para el comando 'cargar'.
 * Presenta el siguiente texto:
 */
void EMIParser::help_cargar()
{
	cout	<< " :.: cargar <id>\n"
		<< "     Donde <id> puede ser cualquier identificador\n"
		<< "     valido.\n"
		<< endl;
	tok_Aux = st->nextToken();
}

/**
 * Es llamado luego de haber encontrado algun token legal de la 
 * siantaxis y evalua la expresion que corresponda. Lanza 
 * ExceptionParser si el siguiente caracter es ilegal en la sintaxis.
 * 
 * @return Variable *
 *
 */
Variable * EMIParser::expr (){		   
	Variable * temp = new Variable();	
	Matriz * resultado;
	
	try{		
		temp = expr_unaria();	 
		while( st->current_TOK == st->TOK_OP_SUMA || st->current_TOK == st->TOK_OP_RESTA ){
			TokenReader::tok_type operador = st->current_TOK;

			tok_Aux = st->nextToken();
			resultado = new Matriz( temp->getValue()->fila, temp->getValue()->columna );

			if ( operador == st->TOK_OP_SUMA )
				temp->getValue()->suma( expr_unaria()->getValue(), resultado );
			else if ( operador == st->TOK_OP_RESTA )
				temp->getValue()->resta( expr_unaria()->getValue(), resultado );

			temp->setValue( resultado );
		}			 
	}catch( ExceptionMatrizDistintoOrden &e ){
		cout << "**Error: Las Matrices son de distinto orden." << endl;
		throw ExceptionError();
	}
	return ( temp );	 
}

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
Variable * EMIParser::expr_unaria () {
	Variable * temp = new Variable();
	Matriz * resultado;
	char id_Aux;

	try{	   
		if ( st->current_TOK == st->TOK_OP_ADJUN ||
		     st->current_TOK == st->TOK_OP_TRAS ||
		     st->current_TOK == st->TOK_OP_INVER ) {
			
			char operador_Aux = tok_Aux[ 0 ];
			tok_Aux = st->nextToken();
			if ( st->current_TOK == st->TOK_ID ){	
			// Se guarda el id para recuperarlo de la tabla de simbolos,
			// si es que existe.
				id_Aux = tok_Aux[ 0 ];
		       
			// Asi resultado es una copia de la variable que esta en la 
		       	// tabla de simbolos.
				temp = tabla->clonar( id_Aux );	
		       	      
			// Se hace la operacion correspondiente sobre una Variable
				select_operacion( operador_Aux, temp, resultado );

				tok_Aux = st->nextToken();			 
			}//if adentro
			else if ( st->current_TOK == st->TOK_COR_IZQ ){//de adentro
		        // Este intecambio se hace para salvar la operacion de traspuesta
		        // en una operacion que implique matriz constante, no afecta para las
		        // demas operaciones ya que para ellas las matrices deben ser cuadradas
				int aux = fila_Aux;
				fila_Aux = columna_Aux;
				columna_Aux = aux;

				temp = matriz();

				// Se hace la operacion correspondiente sobre una constante.
				select_operacion( operador_Aux, temp, resultado );

	         	}//else if del de adentro
			else error();
		} //del if de afuera
		else if ( st->current_TOK == st->TOK_ID ){
		// Procesar ID
			id_Aux = tok_Aux[ 0 ];
			temp = tabla->clonar( id_Aux );
			tok_Aux = st->nextToken();
		}else if ( st->current_TOK == st->TOK_COR_IZQ ){
			temp = matriz();
		} else error();
	}catch( ExceptionVariableNoDefinida &e ){
		cout << "**Error : Variable no esta definida. " << e.id << endl;
		throw ExceptionError();
   	}catch( ExceptionNoEsMatrizCuadrada &e){
		cout << "**Error : Matriz no es cuadrada. " << endl;
		throw ExceptionError();
	}catch( ExceptionNoEsMatrizRegular &e ){
		cout << "**Error : Matriz no es regular. " << endl;
		throw ExceptionError();
	}
			
	return ( temp );     
}//de exp_unaria()

void EMIParser::select_operacion( char operador, Variable * temp, Matriz * resultado ){

	// Se hace la operacion correspondiente sobre una Variable
	switch( operador ){
	// Se evalua el simbolo de la adjunta
		case '*':
			resultado = new Matriz( temp->getValue()->fila );
			temp->getValue()->adjunta( resultado );
			temp->setValue( resultado );
			//resultado->setValue (Matriz.adjunta( resultado->getValue()));
			break;
	// Se evalua el simbolo de la traspuesta
		case '^':
			resultado = new Matriz( temp->getValue()->columna, temp->getValue()->fila );
			temp->getValue()->traspuesta( resultado );
			temp->setValue( resultado );
			//resultado->setValue (Matriz.traspuesta( resultado->getValue()));
			break;		          	  
	// Se evalua el simbolo de la inversa
		case '!': {
			try {
				resultado = new Matriz( temp->getValue()->fila );
				temp->getValue()->inversa( resultado );
				temp->setValue( resultado );
				// Asi la 'mat_aux' es la inversa de una copia de resultado.
				//Matriz mat_aux = Matriz.inversa( (Matriz)resultado->getValue());
				//resultado->setValue( mat_aux );
	 	                break;
			} catch ( ExceptionNoEsMatrizRegular &e ){
				// Imprime mjs error.
				cout << "**Error: Matriz no es regular. " << endl;
			}//try
		}//case
	}//switch

}
		
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
void EMIParser::operacion (){
	tok_Aux = st->nextToken();
	if ( st->current_TOK == st->TOK_IGUAL ){       	
		try {
			// Se hace una copia de la variable solicitada
			// Recordar que id_Aux fue asignado *antes* de
			// llamar a operacion().
			Variable * vieja = tabla->clonar( id_Aux );

			// expr() devuelve una Variable, cuyo valor se 
			// asignara a la Variable nueva.

			// Se recupera la fila y la columna de la Variable, 
			// esto nos sirve para crear Variables anonimas con 
			// el campo 'value' ( la matriz ) con las dimensiones
 			// correctas.
			// Estos valores se usan mas adelante en el metodo matriz().
			fila_Aux = vieja->getFila();
			columna_Aux = vieja->getColumna();

			tok_Aux = st->nextToken();
			Variable * nueva = expr();
	 
			// Se hace la modificacion
			tabla->modify( vieja, nueva );

		}catch( ExceptionVariableNoDefinida &e ){
			cout << "**Error : Variable no definida. " << endl;
			throw ExceptionError();
		}
	}else error();    
}
		
/**
 * Efectua la carga interactiva con el operador llamando al 
 * metodo cargaInteracativa() definido en Matriz.
 * ExceptionParser es lanzada si el siguiente caracter es ilegal en 
 * la sintaxis.
 * 
 * @return void
 * 
 */		
void EMIParser::cargar (){
	tok_Aux = st->nextToken();
	if ( st->current_TOK == st->TOK_ID ){
		try{
			id_Aux = tok_Aux[ 0 ];
			// Imprimir variable 'id_Aux'
			cout << "**Carga ";  tabla->retrieve( id_Aux )->imprimir();
			//Llamar al procedimiento para cargar, dependiendo 
			// del ID.
			tabla->retrieve( id_Aux )->getValue()->cargaInteractiva();
			tok_Aux = st->nextToken();

		}catch ( ExceptionVariableNoDefinida &e ){
			cout << "**Error : Variable no definida. " << endl;
			throw ExceptionError();
		}
	}else error();
}	
		
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
void EMIParser::def() {    
	// Almacena momentaneamente el token devuelto por nextToken()
	tok_Aux = st->nextToken();
	if ( st->current_TOK == st->TOK_ID ){        
		// Convierte a caracter el identificador.
		// Se supone que es una cadena de longitud 1, es por
		// eso que se usa tok_Aux[0]
		id_Aux = tok_Aux[ 0 ];
		tok_Aux = st->nextToken();
		
		// Si se encuentra '=' tiene que parsearse una expresion.
		if ( st->current_TOK == st->TOK_IGUAL ){ 
			tok_Aux = st->nextToken();                    
			try{
				Variable * temp = expr();                      
				if ( temp->getValue() != NULL )
					tabla->add( new Variable( id_Aux, temp->getFila(), 
						    temp->getColumna(), temp->getValue()));

			}catch( ExceptionIdentificadorDuplicado &e ){
				cout << "**Error: Identificador Duplicado: " << e.id << endl;
				throw ExceptionError();
			}catch (...){
				cout << "**Error: La matriz no se pudo definir." << endl;
				throw ExceptionError();
			}
		}//if
    
		// De lo contrario una defincion y opcionalmente una inicializacion.
		else if ( st->current_TOK == st->TOK_COR_IZQ ) {            
			tok_Aux = st->nextToken(); 
			if ( st->current_TOK == st->TOK_NUMERO ) {
				// Se guarda temporalmente la fila
				fila_Aux = atoi( tok_Aux.c_str() );

				// Hay que evitar que se definan matrices con
				// dimensiones negativas, o mayores al limite
				// permitido.
				if ( fila_Aux < 1 || fila_Aux > MAX_FIL ) error();
                
				// Ahora se toma el otro token ( tiene que ser coma )
				tok_Aux = st->nextToken();
				if ( st->current_TOK == st->TOK_COMA ) {
					tok_Aux = st->nextToken();
					if ( st->current_TOK == st->TOK_NUMERO ){
						// Aqui se guarda la columna
						columna_Aux = atoi( tok_Aux.c_str() );
                                
						// Hay que evitar que se definan matrices con
						// dimensiones negativas, o mayores al limite
						// permitido.
						if ( columna_Aux < 1 || columna_Aux > MAX_COL ) error();

						tok_Aux = st->nextToken();
						if ( st->current_TOK == st->TOK_COR_DER ){
							// Opcionalmente se puede dar valores iniciales
							// a la matriz, pero si no se quiere, solo se
							// inicializa con ceros. 
							tok_Aux = st->nextToken();
							if ( st->current_TOK == st->TOK_ASIG ){
								tok_Aux = st->nextToken();
								try{
									tabla->add( new Variable( id_Aux, fila_Aux, columna_Aux, matriz()->getValue() ));
									// El metodo matriz() devuelve una Variable, 
									// la cual contiene una matriz con los valores 
									// para inicializar nuestra nueva Variable.
								}catch( ExceptionIdentificadorDuplicado &e ){
									cout << "**Error: Identificador Duplicado: "<< e.id << endl;
									throw ExceptionError();
								}//try
							}else{
							// Si entra aqui es por que fue una definicion 
							// similar a 'def B[3,4]', o sea que debemos
							//  poner una nueva variable en la tabla de 
							//  simbolos con los valores que obtuvimos.
								try {  
								// Se realiza un control para el caso de que el
								// siguiente token sea un igual, en este caso
								// no de debe almacenar la variable en la tabla de simbolo
									if ( st->current_TOK != st->TOK_IGUAL ) 
										bool flag  = tabla->add( new Variable( id_Aux, fila_Aux, 
										columna_Aux, new Matriz( fila_Aux, columna_Aux )));
								}catch ( ExceptionIdentificadorDuplicado &e ){
									cout << "**Error: Identificador Duplicado: "<< e.id << endl;
									throw ExceptionError();
								}catch ( ... ){
									cout << "--- Excepcion Imprevista (EMIParser::def) ---" << endl;
									throw ExceptionError();
								}//try
							}//If
						}else error();
					}else error();
				}else error();  //Por ej.: 'def K = [ 39 ]'
			}else error(); // Por ej.: 'def A = 15', o 'def A'
		}else error(); // Por ejemplo: 'def []', o 'def 42'.                
	}else error();
}
    
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
void EMIParser::fila ( double matriz_Aux[25][25] ){
	// Con esta variable se tiene en cuenta la cantidad de columnas a definir
	// en la maatriz
	columna_Cont = 0;
			
	// string temporal para guardar un numero (elemento) de la matriz
	string temp_numero = "";
	tok_Aux = st->nextToken();	
	if (st->current_TOK == st->TOK_COR_IZQ ){
		tok_Aux = st->nextToken();
		if ( st->current_TOK == st->TOK_NUMERO ){
			// Se almacena en el array
			matriz_Aux[fila_Cont][columna_Cont] = ( double ) atof( tok_Aux.c_str() );
			columna_Cont++;  // Se capto un elemento en esta columna

			tok_Aux = st->nextToken();
			while( (st->current_TOK == st->TOK_COMA) && (columna_Cont < columna_Aux)){

				tok_Aux = st->nextToken();  // Busca un numero.
				if ( st->current_TOK == st->TOK_NUMERO){
				// Se almacena en el array
					matriz_Aux[fila_Cont][columna_Cont] = ( double ) atof( tok_Aux.c_str() );
					columna_Cont++;  // Se capto un elemento en esta columna
				}//if

				tok_Aux = st->nextToken(); // Busca una coma o un corchete derecho..
			}//while
			if ( st->current_TOK == st->TOK_COR_DER ){
				// Se actualiza el valor de mayor_Col
				if ( columna_Cont > mayor_Col ) mayor_Col = columna_Cont;
				tok_Aux = st->nextToken();
			}else error();					 
		}//if
	}//if
}//fila()

/**
* matriz(): Parsea una constante del tipo Matriz. Llama al metodo 
*    fila() para parsear una fila.
* ExceptionParser es lanzada si el siguiente caracter es ilegal
* en la sintaxis.
* 
* @return Variable *
*
*/
Variable * EMIParser::matriz (){
	//Devuelve el resultado
	Variable * resultado = new Variable();
	
	// Array temporal para guardar los elementos de la constante,
	// es utilizado en el metodo 'fila()', es inicializada 
	// por defecto con 25x25 de otro modo inicializa con los 
	// valores modificados de fila_Aux y columna_Aux
	double matriz_Aux [25][25];
		  
	// Con fila_cont se tiene en cuenta el valor de la fila de la matriz
	// se esta definiendo.
	fila_Cont = 0;

	fila( matriz_Aux );
	fila_Cont++;
	while ( ( st->current_TOK == st->TOK_COMA) && ( fila_Cont < fila_Aux)){
		fila( matriz_Aux );
		fila_Cont++;
	}
	if ( st->current_TOK == st->TOK_COR_DER ){
		// Se traspasan los valores de matriz_Aux a matriz_temp, 
		// recordemos que matriz_Aux pudo haber sido definida 
		// con 25x25, y posiblemente no se utilizaron todas
		// las posiciones.
		double matriz_temp[25][25]; // = new double[fila_Cont][mayor_Col];
		for( int i = 0; i < fila_Cont; i++)
			for ( int j = 0; j < mayor_Col; j++ )
				matriz_temp[i][j] = matriz_Aux[i][j];

		// Aqui ya se cargaron todos los elementos en 'matriz_Aux[][]',
		// solo resta crear una referencia a un objeto Variable con los 
		// siguientes parametros:
		//	Un identificador ' ' ( sin nombre )
		// 	Un valor para la fila, cuyo valor es fila_Cont o fila_Aux
		//	  - Ver observacion abajo.
		//	Un valor para la columna, columna_Aux, o columna_Aux, y
		//	  - Ver observacion abajo.
		//	un nuevo objeto Matriz, inicializado con el array matriz_Aux[][].

		// Obs.: Hay que realizar un control referente a las dimenciones en 
		//       este punto. La definicion de EMI nos indica que esta permitida
		//       la definicion de variables con valores opcionales y tambien
		//	 que estos elementos pueden citarse en forma parcial. Esto es,
		//	 tenemos la posibilidad de hacer una definicion como la sgte.:
		//
		//	EMI > def B[3,3] => [[1,2],[2,4]]
		//
		//	 quedando la variable B como:
		//	B = [ 1	  2   0
		//	      2   4   0
		//	      0   0   0 ] , pues se definio con la dimension 3x3, pero
		// 	 solo se cargaron los primeros valores.	       

		//	 El tema esta en hacer 'new' con los parametros adecuados, es
		//	 decir, con fila_Aux o fila_Cont ( o con columna_Aux o mayor_Col ).
		//	 Si fila_Aux o columna_Aux tienen valor '26', significa que en
		//	 ningun momento se indico que se queria alguna dimension, entonces
		//	 usamos a fila_Cont y mayor_Col. De lo contrario se utilizan los
		//	 valores capturados ( fila_Aux y columna_Aux ).

		{ int f; int c;

		f = ( fila_Aux == 26 ) ? fila_Cont : fila_Aux;
		c = ( columna_Aux == 26 ) ? mayor_Col : columna_Aux;

		resultado = new Variable( ' ', f, c, new Matriz( f, c ));
		
		// Ahora pasamos los valores obtenidos donde corresponden.
		for( int i = 0; i < f; i++)
			for ( int j = 0; j < c; j++ )
				resultado->getValue()->mat[i][j] = matriz_Aux[i][j];

		}		

		tok_Aux = st->nextToken();
	}else error();
	return ( resultado );
}

/**
 * Construye el objeto ExceptionParser con un int
 * que es un atributo de la variable 'st' (TokenReader) para que la consola 
 * pueda indicar la posicion de la cadena en donde esta el error de sintaxis.
 *
 * @return void
 * 
 */
void EMIParser::error(){
	throw ExceptionParser (st->pos );
}		

