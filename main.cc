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
 * Nombre del Archivo: main.cc
 * Descripcion: 
 *    Se encarga de desplegar un mensaje inicial para el proyecto,
 * luego presenta un prompt, lo cual indica que esta a la espera
 * de algun comando.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>

#include "EMIParser.h"
#include "SymTable.h"

#include "Exceptions.h"

using namespace std;

string getCommand()
{
	char cad[256];
	string c = "";
	do{
		fgets( cad, 256, stdin );
		c = ( string ) cad;
	}while ( c == "\n" );

	if ( c == "QUIT\n" || c == "quit\n" ) c = "";
	return c;
}

void cabecera()
{
	cout	<< "                         ___           ___                   \n"
		<< "  Proyecto:             /\\  \\         /\\__\\          ___    \n"
		<< "                       /::\\  \\       /::|  |        /\\  \\   \n"
		<< "                      /:/\\:\\  \\     /:|:|  |        \\:\\  \\  \n"
		<< "                     /::\\~\\:\\  \\   /:/|:|__|__      /::\\__\\ \n"
		<< "                    /:/\\:\\ \\:\\__\\ /:/ |::::\\__\\  __/:/\\/__/ \n"
		<< "  Autor: Eliseo     \\:\\~\\:\\ \\/__/ \\/__/~~/:/  / /\\/:/  /    \n"
		<< "         Ocampos     \\:\\ \\:\\__\\         /:/  /  \\::/__/     \n"
		<< "                      \\:\\ \\/__/        /:/  /    \\:\\__\\     \n"
		<< "  - Set, 2005 -        \\:\\__\\         /:/  /      \\/__/     \n"
		<< "                        \\/__/         \\/__/                 \n"
		<< endl;

}

int main()
{
	EMIParser::tabla = new SymTable();
	string prompt = "EMT > ";
	string command = "";
	// Mensaje Inicial
	
	cabecera();

	cout << "EMI - Evaluador de Matrices Interactivo" << endl;
	cout << "    - Ingrese '?' para ver los comandos disponibles." << endl;
	cout << "    - Ingrese quit o QUIT para salir." << endl;
	cout << prompt;

	while (( command = getCommand() ) != "" ){
		try{
			// El cuerpo en si.
			EMIParser * ps = new EMIParser( command );
			cout << "Sintaxis OK" << endl;
		}
		// Captura los errores que se cometieron en durante
		// la evaluacion semantica.
		// Este manejador no hace nada, pues cualquier error,
		// que no haya sido de sintaxis, ya se manipulo adecuadamente.
		// Su unica funcion es la de 'cortar' el parsing, y solicitar
		// un nuevo comando.
		catch( ExceptionError &e ){}
		// Captura errores que sean referentes a la sintaxis.
		catch( ExceptionParser &e ){
			cout << "** Error de sintaxis **" << endl;
			cout << command;
			int d = e.posicion;
			for ( int k = 0; k < d - 1; k++ )
				cout << " ";
			cout << "^" << endl;
			
		}
		catch(...){
			cout << "--- Excepcion Imprevista. ---" << endl;
		}//try
		cout << prompt;
	}//while

	cout << "Fin de Sesion." << endl;

	return 0;
}

