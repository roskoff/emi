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
 * Nombre Archivo: Matriz.h
 * Descripcion:
 *    Esta clase describe objetos del tipo Matriz, con los
 * atributos y funciones miembro necesarios para poder
 * manipularlos de acuerdo al problema planteado.
 *    Se listan todos los componentes, y se deja cualquier
 * implementacion para el archivo Matriz.cc
 *
 */
#ifndef __MATRIZ_H
#define __MATRIZ_H


#include <string>

#define MAX_FILA 25
#define MAX_COLUMNA 25


using namespace std;

/**
 *    Esta clase describe objetos del tipo Matriz, con los
 * atributos y funciones miembro necesarios para poder
 * manipularlos de acuerdo al problema planteado.
 *    Se listan todos los componentes, y se deja cualquier
 * implementacion para el archivo Matriz.cc
 */
class Matriz
{
	// Constructores
	public:
	/** Por defecto, todos los elementos se inicializan a 0.0.
	 * - Se puede indicar una sola dimension ( matriz cuadrada ).
	 */
	   Matriz( int dimen );


	/** - Se indican ambas dimensiones. */
	   Matriz( int f, int c );


	// Atributos
	public:
	/** Guarda el valor correspondiente a la fila de la matriz. */
	   int fila;
	/** Guarda el valor correspondiente a la columna de la matriz. */
	   int columna;
	/** Este array bidimensional guarda todos los elementos de la matriz. */
	   double mat [MAX_FILA][MAX_COLUMNA];

	// Funciones Miembro
	private:
	/**
	 * esCuadrada(): Verifica la matriz es cuadrada. 
	 *    Se utiliza para las operaciones en las cuales se precisa matrices 
	 * cuadradas, por ejemplo, inversa().
	 *
	 * @return bool
	 *
	 */
	   bool esCuadrada();

	/**
	 * Verifica si la matriz es o no Regular.
	 *    Es utilizada como condicion para calcular las operaciones con las 
	 * matrices, por ejemplo en adjunta().
	 *    Se lanza la excepcion porque una matriz que no es cuadrada ya no es
	 * regular, para que lo sea, su determinante debe ser un valor distinto a 
	 * cero.
	 * @param r como Matriz * 
	 * @return bool
	 *  
	 */
	   bool esRegular();

	public:
	/**
	 * cargaInteractiva: Carga interactivamente una matriz. 
	 *    Esta funcion es llamada desde el EMIParser
	 * para realizar una carga interactiva, solicitando
	 * los elementos de la matriz al usuario.
	 *
	 * @return void
	 *
	 */
	   void cargaInteractiva();

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
	   void suma( Matriz * a, Matriz * resultado );

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
	   void resta( Matriz * a, Matriz * resultado );

	/**
	 * Calcula la traspuesta de una matriz.
	 *    Esta funcion se invoca en el EMIParser, halla la traspuesta
	 * colocando las filas en lugar de las columnas y viceversa.
	 * @param resultado como Matriz *
	 * @return void
	 *	
	 */
	   void traspuesta( Matriz * resultado );

	/**
	 * Calcula la inversa de una matriz.
	 *    Aplica la operacion inversa() en el parametro recibido, el mismo
	 * debe ser regular para poder realizarse la operacion.
	 *
	 * @param c como Matriz *
	 * @return void
	 *
	 */
	   void inversa( Matriz * resultado );
  
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
	   void adjunta( Matriz * resultado );

	/**
	 * Halla la determinante de una matriz. 
	 *    Lanza una excepcion si la matriz no es cuadrada.
	 * 
	 * @return double
	 *
	 */ 
	   double determinante();

	/**
	 * Se utiliza_para formatear la impresion de una 
	 *    matriz de acuerdo a los requerimiento del enunciado del 
	 * proyecto.
	 * 
	 * @return void 
	 * 
	 */ 
	   void imprimir();
};

#endif 
