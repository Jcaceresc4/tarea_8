#pragma once
#include <iostream>
#include <string>
#include "ConexionBD.h"
#include "Persona.h"
#include <mysql.h>

using namespace std;
class Estudiante : Persona {
	//atributos
private: string codigo;
	   int id_estudiante = 0;
	   //constructor
public:
	Estudiante() {}
	Estudiante(string nom, string ape, string dir, int tel, string fn, int id_ts, string cod, int id_e) : Persona(nom, ape, dir, tel, fn, id_ts) {
		codigo = cod;
		id_estudiante = id_e;
	}
	void setid_estudainte(int e) { id_estudiante = e; }
	void setCodigo(string c) { codigo = c; }
	void setnombres(string n) { nombres = n; }
	void setapellidos(string a) { apellidos = a; }
	void setdireccion(string d) { direccion = d; }
	void settelefono(int t) { telefono = t; }
	void setfecha_nacimiento(string f) { fecha_nacimiento = f; }
	void setid_tipo_sangre(int ts) { id_tipo_sangre = ts; }
	
	//get 
	int getid_estudainte() { return id_estudiante; }
	string getnombres() { return nombres; }
	string getapellidos() { return apellidos; }
	string getdireccion() { return direccion; }
	int gettelefono() { return telefono; }
	string getfecha_nacimiento() { return fecha_nacimiento; }
	int getid_tipo_sangre() {return id_tipo_sangre; }

	//metodos
	void crear() {}
	void leer (){
		int q_estado = 0;
		ConexionBD cn = ConexionBD();
		MYSQL_ROW fila;
		MYSQL_RES* resultado;
		cn.abrir_conexion();
		if (cn.getConector()) {
			cout << "____________Datos del estudiante___________" << endl;
			string consulta = "select * from estudiantes;";
			const char* c = consulta.c_str();
			q_estado = mysql_query(cn.getConector(), c);
			if (!q_estado) {
				resultado = mysql_store_result(cn.getConector());
				while (fila = mysql_fetch_row(resultado)) {
					cout << fila[0] << "," << fila[1] << "," << fila[2] << "," << fila[3] << "," << fila[4] << "," << fila[5] << "," << fila[6] << "," << fila[7]<<endl;
				}
			}
			else {
				cout << "xxxx Consulta fallida xxxx" << endl;
			}
		}
		else {
			cout << "  xxxx fallo la conexion xxx"<<endl;
		}
		cn.cerrar_conexion();
	}
	void actualizar (){}
	void borrar (){}
};