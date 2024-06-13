#pragma once
#include <mysql.h>
#include <iostream>
#include <string>
#include "DB.h"

using namespace std;

// Esta clase se llama ConsultarReserva y se usa para ver y eliminar reservaciones de vuelos
class ConsultarReserva
{
public:
    // Aquí guardamos información como el número de reserva y la selección del usuario
    string id_reserva;
    string seleccion;

    // Variables para manejar la conexión con la base de datos
    int q_estado;
    MYSQL_ROW fila;
    MYSQL_RES* resultado;
    DB db = DB();

    // Método para consultar una reserva
    void Consultar_Reserva() {
        db.Iniciar_Conexion(); // Inicia la conexión con la base de datos

        system("cls"); // Limpia la pantalla

        cout << "Ingrese su numero de reserva: " << endl; // Pide el número de reserva al usuario
        getline(cin, id_reserva);

        system("cls"); // Limpia la pantalla nuevamente

        // Prepara la consulta para obtener la información de la reserva
        string consulta_reserva = "SELECT reserva.id, reserva.pasajero, asientos.asiento, rutas.direccion, horarios.horario, reserva.fecha, reserva.volado FROM reserva JOIN asientos ON reserva.id_asiento = asientos.id JOIN vuelos ON reserva.id_vuelo = vuelos.id JOIN rutas ON vuelos.id_ruta = rutas.id JOIN horarios ON horarios.id = vuelos.id_horario WHERE reserva.id = " + id_reserva;
        const char* c_r = consulta_reserva.c_str();
        q_estado = mysql_query(db.GET_conexion(), c_r); // Ejecuta la consulta

        if (!q_estado) {
            resultado = mysql_store_result(db.GET_conexion());
            while (fila = mysql_fetch_row(resultado)) {
                if (fila[0] == id_reserva) { // Verifica si el ID de la reserva coincide

                    // Muestra la información de la reserva
                    if (fila[6] == "0") { // Si el vuelo no ha sido volado
                        cout << "id\tReserva a nombre de:\tAsiento:\tRuta:\t\t\tHorario:\tFecha:\t\tVolado:" << endl;
                        cout << fila[0] << "\t" << fila[1] << "\t\t" << fila[2] << "\t\t" << fila[3] << "\t\t" << fila[4] << "\t" << fila[5] << "\t" << "No" << endl << endl;
                    }
                    else { // Si el vuelo ya ha sido volado
                        cout << "id\tReserva a nombre de:\tAsiento:\tRuta:\t\t\tHorario:\tFecha:\t\tVolado:" << endl;
                        cout << fila[0] << "\t" << fila[1] << "\t\t" << fila[2] << "\t\t" << fila[3] << "\t\t" << fila[4] << "\t" << fila[5] << "\t" << "Si" << endl << endl;
                    }
                }
                else {
                    break;
                }
            }
        }

        // Pregunta al usuario si quiere eliminar la reserva o regresar
        cout << "1) Eliminar reserva" << endl;
        cout << "2) Regresar" << endl;
        getline(cin, seleccion);
        if (seleccion == "1") {
            Eliminar_Reserva(); // Llama al método para eliminar la reserva
        }

        db.Cerrar_Conexion(); // Cierra la conexión con la base de datos
    }

    // Método para eliminar una reserva
    void Eliminar_Reserva() {
        db.Iniciar_Conexion(); // Inicia la conexión con la base de datos

        system("cls"); // Limpia la pantalla

        // Prepara la consulta para eliminar la reserva
        string eliminar = "DELETE FROM reserva WHERE reserva.id = " + id_reserva;
        const char* e = eliminar.c_str();
        q_estado = mysql_query(db.GET_conexion(), e); // Ejecuta la consulta para eliminar la reserva

        if (!q_estado) {
            cout << "Se ha eliminado la reserva con exito"; // Muestra un mensaje de éxito
            system("pause"); // Pausa la pantalla para que el usuario vea el mensaje
        }

        db.Cerrar_Conexion(); // Cierra la conexión con la base de datos
    }
};
