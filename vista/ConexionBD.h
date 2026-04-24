#pragma once
#include <mysql.h>
#include <iostream>
using namespace std;
class ConexionBD {
private: 
	MYSQL* conectar;
public : MYSQL * conector;
	   void abrir_conexion() {
		   conector = mysql_init(0);
		   conector = mysql_real_connect(conector, "127.0.0.1", "root", "jose2006", "db_escuela", 3306, NULL, 0);

}
MYSQL* getConector() {
	return conector;
}
void cerrar_conexion() {
	mysql_close(conector);
}
};

