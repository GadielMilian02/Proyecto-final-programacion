#pragma once
#include <iostream>
#include <mysql.h>

using namespace std;
class DB
{
private:
	MYSQL* conexion;

public:
	void Iniciar_Conexion() {
		conexion = mysql_init(0);
		conexion = mysql_real_connect(conexion, "127.0.0.1", "root", "", "proyecto_final", 3306, NULL, 0);
	}

	void Cerrar_Conexion() {
		mysql_close(conexion);
	}

	MYSQL* GET_conexion() {
		return conexion;
	}
};

