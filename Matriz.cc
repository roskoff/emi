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

/*****************************************************************************
 * Autor: Eliseo Ocampos
 * Nombre Archivo: Matriz.cc
 * Descripcion:
 *    Aqui se implementa todo lo referenciado en Matriz.h
 *
 *****************************************************************************/

#include <iostream>
#include <string>

#include "Matriz.h"

#include "Exceptions.h"


//
// Constructores.
//
/** Por defecto, todos los elementos se inicializan a 0.0.
 * - Se puede indicar una sola dimension ( matriz cuadrada ).
 */
Matriz::Matriz( int dimen ) {
        fila = dimen;
	columna = dimen;
	//mat = new double[MAX_FILA][MAX_COLUMNA];
	for( int i = 0; i < dimen; i++ ){
		for(int j = 0; j < dimen; j++){
			mat [i][j] = 0.0;
		}
	}
}

/** - Se indican ambas dimensiones. */
Matriz::Matriz( int f, int c ) {
	fila = f;
	columna = c;
	//mat = new double[fila][columna];
	for(int i= 0; i < f ; i++){
		for(int j = 0; j < c; j++){
			mat [i][j] = 0.0;
		}
	}
}
    

//
// Funciones Miembro.
//
   
/**
 * esCuadrada(): Verifica la matriz es cuadrada. 
 *    Se utiliza para las operaciones en las cuales se precisa matrices 
 * cuadradas, por ejemplo, inversa().
 *
 * @return bool
 *
 */	
bool Matriz::esCuadrada(){
	return ( this->fila == this->columna );
}

/**
 * Verifica si la matriz es o no Regular.
 *    Es utilizada como condicion para calcular las operaciones con las 
 * matrices, por ejemplo en adjunta().
 *    Se lanza la excepcion porque una matriz que no es cuadrada ya no es
 * regular, para que lo sea, su determinante debe ser un valor distinto a 
 * cero.
 *
 * @return bool
 *  
 */
bool Matriz::esRegular() {
	Matriz * temp = new Matriz( this->fila, this->columna );
	for( int f = 0; f < this->fila; f++ )
		for( int c = 0; c < this->columna; c++)
			temp->mat[f][c] = this->mat[f][c];

	return ( temp->determinante() != 0.0 ); 	
}
		  

/**
 * cargaInteractiva: Carga interactivamente una matriz. 
 *    Esta funcion es llamada desde el EMIParser
 * para realizar una carga interactiva, solicitando
 * los elementos de la matriz al usuario.
 *
 * @return void
 *
 */
 void Matriz::cargaInteractiva(){
	int f = this->fila;
	int c = this->columna;
	
	for (int i = 0 ; i < f; i++)
		for (int j = 0 ; j < c ; j++) {
			backTrack : { 
				cout << "[" << i << "," << j << "] = " ;
				try { 						
					cin >> this->mat[i][j];
				}
				//XXX Encontrar excepcion que controle
				// entrada de numeros.
				catch (.../*Exception e*/) {
					cout << "El elemento debe ser numerico.\n";
					j--;  
					//vuelve a la iteracion donde se produjo el error
					goto backTrack;
				}
			}// end backTrack	
		}//for
	cout << endl;
}//cargaInteractiva
	
/**
 * Suma dos matrices.
 *    Esta funcion es invocada por el EMIParser para realizar 
 * la operacion correspondiente. Se suman la Matriz actual (this)
 * y la Matriz recibida (a), el resultado se devuelve en 'resultado'
 *
 * @param a como Matriz *
 * @param resultado como Matriz *
 * 
 * @return void
 *
 */	
void Matriz::suma( Matriz * a, Matriz * resultado ){
	if ( a->fila == this->fila && a->columna == this->columna ){

		for(int i = 0; i < a->fila; i++){
			for(int j = 0; j < a->columna; j++){
				resultado->mat[i][j] = a->mat[i][j] + this->mat[i][j];
			}
		}
         	// El resultado de la suma se guardo en 'resultado'
	}
	else 
	   throw ExceptionMatrizDistintoOrden();		
}

/**
 * Resta dos matrices.
 *    Esta funcion es invocada por el EMIParser para realizar 
 * la operacion correspondiente. Se restan la Matriz actual (this)
 * y la Matriz recibida (a), el resultado se devuelve en 'resultado'
 *
 * @param a como Matriz *
 * @param resultado como Matriz *
 * 
 * @return void
 *
 */
void Matriz::resta( Matriz * a, Matriz * resultado ){
	if ( a->fila == this->fila && a->columna == this->columna ){

		for(int i = 0; i < a->fila; i++){
			for(int j = 0; j < a->columna; j++){
				resultado->mat[i][j] = this->mat[i][j] - a->mat[i][j];
			}
		}
         	// El resultado de la suma se guardo en 'resultado'
	}
	else throw ExceptionMatrizDistintoOrden();
}
 
/**
 * Calcula la traspuesta de una matriz.
 *    Esta funcion se invoca en el EMIParser, halla la traspuesta
 * colocando las filas en lugar de las columnas y viceversa.
 * @param resultado como Matriz *
 * @return void
 *
 */
void Matriz::traspuesta( Matriz * resultado ){

	int f = this->fila;
	int c = this->columna;
	
	for(int i = 0; i < f; i++){
		for(int j = 0; j < c; j++){
			resultado->mat[j][i] = this->mat[i][j];
		}
	}
       	// El resultado de la traspuesta se guardo en 'resultado'
}
    
/**
 * Calcula la inversa de una matriz.
 *    Aplica la operacion inversa() en el parametro recibido, el mismo
 * debe ser regular para poder realizarse la operacion.
 *
 * @param c como Matriz *
 * @return void
 *
 */
void Matriz::inversa( Matriz * c ){

        // Debe ser regular para que tenga Inversa.
	if ( !(this->esRegular()) )
		throw ExceptionNoEsMatrizRegular();
		int dim  = this->fila;		//dimension de la matriz

		//La Matriz a es una copia de this
		Matriz * a = new Matriz(dim);
		for( int i = 0; i < dim; i++ )
			for( int j = 0; j < dim; j++ )
				a->mat[i][j] = this->mat[i][j];

		
		Matriz * b = new Matriz(dim);  	//matriz de los terminos independientes
		//c es la matriz de las incognitas
        
		// Matriz unidad (indentidad)
		for(int i = 0; i < dim; i++){
			b->mat[i][i] = 1.0;
		}
        
		// Transformacion de la matriz y de los terminos independientes
		for(int k = 0; k < dim - 1; k++){
			for(int i = k + 1; i < dim; i++){

			        // Terminos independientes 
				for(int s = 0; s < dim; s++){
					b->mat[i][s] -= a->mat[i][k] * b->mat[k][s] / a->mat[k][k];
				}
                
				// Elementos de la matriz
               	 		for(int j = k+1; j < dim; j++){
                	    		a->mat[i][j] -= a->mat[i][k] * a->mat[k][j] / a->mat[k][k];
                		}
            		}
        	}
        
	// Calculo de las incognitas, elementos de la matriz inversa.
	for(int s = 0; s < dim; s++){
		c->mat[dim-1][s] = b->mat[dim-1][s] / a->mat[dim-1][dim-1];
		for(int i = dim-2; i >= 0; i--){
			c->mat[i][s] = b->mat[i][s] / a->mat[i][i];
			for(int k = dim-1; k > i; k--){
				c->mat[i][s] -= a->mat[i][k] * c->mat[k][s] / a->mat[i][i];
			}
		}
        }
	// El resultado se devuelve en 'c'.
}
 
  
/**
 * Calcula la adjunta de una matriz.
 *    Para este calculo la funcion utiliza determinante(). Por definicion 
 * de la operacion en el modelo matematico.
 *    Cuando no es cuadrada lanza una excepcion porque no se puede
 * realizar esta operacion si no cumple esta propiedad.
 *
 * @param adj como Matriz *
 * @return void
 *
 */   
void Matriz::adjunta( Matriz * adj ){
	// Tiene que ser cuadrada para que se pueda aplicar 
	// la operacion adjunta().
	if ( !(this->esCuadrada()) ) throw ExceptionNoEsMatrizCuadrada();
  	
	int dim = this->fila;
	Matriz * aux = new Matriz ( dim - 1 );
	int ca, fa, exp, sig;
	ca = 0;
	fa = 0;
			  	
	for (int f = 0; f < dim; f++){
		for (int c = 0 ; c < dim ; c++){
			// Ciclos para obtener la submatriz de la que se halla la determinante
  			for (int i = 0; i < dim; i++){
				for (int j = 0; j < dim; j++){
					if ((f != i) & (c != j)){ 
	  					if (ca < dim-1){
							aux->mat[fa][ca] = this->mat[i][j];
							ca++;
						}
						else {
							ca = 0;
							fa++;
							aux->mat[fa][ca] = this->mat[i][j];
							ca++;
						}//if
					}//if
 				}//for				  	
			}//for
			sig = -1;
			exp = f + c + 2;								  
			for (int i = 1; i < exp; i++  ){
				sig = sig * -1;
			}									
			adj->mat [c][f] = sig * ( aux->determinante()); 
			//adj->mat [c][f] = sig * ( x->determinante( aux )); 
			ca = 0;
			fa = 0;
  		}//for 
    	}//for
	// El resultado se devuelve en 'adj'
}
   	
/**
 * Halla la determinante de una matriz. 
 *    Lanza una excepcion si la matriz no es cuadrada.
 * 
 * @return double
 *
 */   	
double Matriz::determinante() {
	if ( !(this->esCuadrada()) ) throw ExceptionNoEsMatrizCuadrada();
		
	int dim = this->fila;       
	for(int k = 0; k < dim - 1; k++){
		for(int i = k + 1; i < dim; i++){
			for(int j = k + 1; j < dim; j++){
				this->mat[i][j] -= this->mat[i][k] * this->mat[k][j] / this->mat[k][k];
			}
		}
	}

	double deter = 1.0;
	for(int i = 0; i < dim; i++){
		deter *= this->mat[i][i];
	}				
	
	return deter;
}  	
		
/**
 * Se utiliza_para formatear la impresion de una 
 *    matriz de acuerdo a los requerimiento del enunciado del 
 * proyecto.
 * 
 * @return void 
 * 
 */ 	
void Matriz::imprimir(){

	for(int i = 0; i < fila; i++) {
		if (i == 0) 
			cout << "[";
		else
			cout << "\n ";
            
		for(int j = 0; j < columna; j++){
			if (j == 0)
				cout << mat[i][j];
			else
				cout << "\t " << mat[i][j];
		}//for
            
	}//for
	cout << "]\n";

}

