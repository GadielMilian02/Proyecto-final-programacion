#pragma once
#include <mysql.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include "DB.h"

using namespace std;

// Esta clase se llama Consultas y se usa para hacer reservaciones de vuelos
class Consultas
{
public:
    // Aquí guardamos información como el vuelo, la ruta, el horario, el asiento, y el nombre del usuario
    string id_vuelo;
    string id_ruta;
    string id_horario;
    string id_asiento;
    string nombre_usuario;
    string asiento;
    string dia, mes, año, fecha;

    // Variables para manejar la conexión con la base de datos
    int q_estado;
    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    DB db = DB();

    // Este método se llama para pedir la fecha en la que quieres viajar
    void Ingresar_Fecha() {
        system("cls"); // Limpia la pantalla
        cout << "Ingrese el dia que quiere viajar: "; // Pide el día del viaje
        getline(cin, dia);
        cout << "Ingrese el mes que quiere viajar (en numeros): "; // Pide el mes del viaje
        getline(cin, mes);
        cout << "Ingrese el año del viaje: "; // Pide el año del viaje
        getline(cin, año);
        fecha = año + "-" + mes + "-" + dia; // Forma la fecha completa
        system("cls"); // Limpia la pantalla nuevamente
    }

    // Método para cambiar el color del texto en la pantalla
    void setConsoleColor(int textColor, int backgroundColor) {
        int colorAttribute = (backgroundColor << 4) | textColor;
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, colorAttribute);
    }

    // Método para consultar las rutas disponibles
    void Consultar_Vuelos()
    {
        db.Iniciar_Conexion(); // Inicia la conexión con la base de datos
        system("cls"); // Limpia la pantalla
        cout << "Seleccione una ruta para viajar" << endl;
        string consulta = "SELECT * FROM rutas"; // Consulta todas las rutas disponibles
        const char* c = consulta.c_str();
        q_estado = mysql_query(db.GET_conexion(), c); // Ejecuta la consulta
        if (!q_estado)
        {
            resultado = mysql_store_result(db.GET_conexion());
            while (fila = mysql_fetch_row(resultado))
            {
                cout << fila[0] << "\t" << fila[1] << endl; // Muestra las rutas
            }
            cout << endl;
        }
        getline(cin, id_ruta); // Lee la ruta seleccionada por el usuario
        db.Cerrar_Conexion(); // Cierra la conexión con la base de datos
    }

    // Método para consultar los horarios disponibles para una ruta específica
    void Consultar_Horarios()
    {
        db.Iniciar_Conexion();
        system("cls");
        cout << "Seleccione el horario en el que desea viajar" << endl;
        string consulta = "SELECT horarios.id, horarios.horario FROM rutas JOIN vuelos ON rutas.id = vuelos.id_ruta JOIN horarios ON vuelos.id_horario = horarios.id WHERE rutas.id = " + id_ruta;
        const char* c = consulta.c_str();
        q_estado = mysql_query(db.GET_conexion(), c); // Ejecuta la consulta para obtener los horarios
        if (!q_estado)
        {
            resultado = mysql_store_result(db.GET_conexion());
            while (fila = mysql_fetch_row(resultado))
            {
                cout << fila[0] << "\t" << fila[1] << endl; // Muestra los horarios
            }
            cout << endl;
        }
        getline(cin, id_horario); // Lee el horario seleccionado por el usuario
        Obtener_Vuelo(); // Obtiene el vuelo específico basado en la ruta y el horario seleccionados
        db.Cerrar_Conexion();
    }

    // Método para pedir el nombre del usuario
    void Ingresar_Nombre() {
        db.Iniciar_Conexion();
        system("cls");
        cout << "Ingrese su nombre y apellido" << endl; // Pide el nombre y apellido del usuario
        getline(cin, nombre_usuario);
        db.Cerrar_Conexion();
    }

    // Método para seleccionar un asiento
    void Ingresar_Asiento() {
        db.Iniciar_Conexion();
        while (true) {
            cout << "Seleccione su asiento (A-D, 1-10)" << endl; // Pide el asiento que quiere el usuario
            getline(cin, asiento);
            system("cls");

            // Consulta para verificar si el asiento está ocupado
            string consulta = "SELECT asientos.asiento FROM reserva JOIN asientos ON asientos.id = reserva.id_asiento JOIN vuelos ON vuelos.id = reserva.id_vuelo WHERE asientos.asiento = '" + asiento + "'  AND vuelos.id = " + id_vuelo;
            const char* c = consulta.c_str();
            q_estado = mysql_query(db.GET_conexion(), c);
            if (!q_estado)
            {
                resultado = mysql_store_result(db.GET_conexion());
                if (!(fila = mysql_fetch_row(resultado)))
                {
                    // Si el asiento no está ocupado, obtener su ID
                    string pedir_asiento = "SELECT asientos.id FROM asientos WHERE asientos.asiento = '" + asiento + "'";
                    const char* p_a = pedir_asiento.c_str();
                    q_estado = mysql_query(db.GET_conexion(), p_a);
                    if (!q_estado) {
                        resultado = mysql_store_result(db.GET_conexion());
                        if ((fila = mysql_fetch_row(resultado))) {
                            id_asiento = fila[0]; // Guarda el ID del asiento
                        }
                    }
                    break; // Sale del bucle si el asiento está disponible
                }
                else {
                    // Si el asiento está ocupado, muestra un mensaje de error
                    setConsoleColor(4, 0);
                    cout << "No se puede, asiento ocupado" << endl << endl;
                    setConsoleColor(9, 0);
                };
            }
        }
        db.Cerrar_Conexion();
    }

    // Método para registrar una reserva en la base de datos
    void Registrar_Reserva() {
        db.Iniciar_Conexion();
        // Inserta la reserva en la base de datos
        string insertar = "INSERT INTO reserva(id_vuelo, id_asiento, pasajero, fecha) values(" + id_vuelo + ", " + id_asiento + ", '" + nombre_usuario + "', '" + fecha + "')";
        const char* i = insertar.c_str();
        q_estado = mysql_query(db.GET_conexion(), i);

        if (!q_estado) {
            // Consulta para obtener el ID de la reserva recién hecha
            string consulta = "SELECT id FROM reserva WHERE reserva.id_vuelo = " + id_vuelo + " AND reserva.id_asiento = " + id_asiento + " AND pasajero = '" + nombre_usuario + "'";
            const char* c = consulta.c_str();
            q_estado = mysql_query(db.GET_conexion(), c);

            if (!q_estado) {
                resultado = mysql_store_result(db.GET_conexion());
                if (fila = mysql_fetch_row(resultado)) {
                    cout << "Tu numero de reserva es el " << fila[0] << endl << endl; // Muestra el número de reserva
                    system("pause");
                }
            }
        }

        db.Cerrar_Conexion();
    }

    // Método para obtener el ID del vuelo basado en la ruta y el horario
    void Obtener_Vuelo() {
        string consulta_vuelo = "SELECT vuelos.id FROM vuelos JOIN horarios ON horarios.id = vuelos.id_horario JOIN rutas ON rutas.id = vuelos.id_ruta WHERE rutas.id = " + id_ruta + " AND horarios.id = " + id_horario;
        const char* cv = consulta_vuelo.c_str();
        q_estado = mysql_query(db.GET_conexion(), cv);
        if (!q_estado) {
            resultado = mysql_store_result(db.GET_conexion());
            if ((fila = mysql_fetch_row(resultado))) {
                id_vuelo = fila[0]; // Guarda el ID del vuelo
            }
        }
    }
};
