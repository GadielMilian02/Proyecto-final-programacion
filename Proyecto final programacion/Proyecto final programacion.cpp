#include <iostream>
#include <string>
#include <Windows.h>
#include <sstream>
#include "Consultas.h"
#include "ConsultarReserva.h"

using namespace std;

// Esta función cambia el color del texto en la pantalla
void setConsoleColor(int textColor, int backgroundColor) {
    int colorAttribute = (backgroundColor << 4) | textColor;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorAttribute);
}

int main() {
    // Creamos objetos para manejar las reservas
    Consultas consulta_reservar;
    ConsultarReserva consultar_reserva;

    // Cambiamos el color del texto a azul claro
    setConsoleColor(9, 0);

    // Un bucle que se repite hasta que decidamos salir
    while (true)
    {
        // Limpia la pantalla
        system("cls");

        string seleccion;

        // Muestra el menú principal
        cout << "+-----------------------------------------+" << endl;
        cout << "|            Menu principal               |" << endl;
        cout << "|-----------------------------------------|" << endl;
        cout << "|Seleccione lo que desea realizar:        |" << endl;
        cout << "|1) Ver mi reserva                        |" << endl;
        cout << "|2) Realizar nueva reserva                |" << endl;
        cout << "|3) Salir                                 |" << endl;
        cout << "+-----------------------------------------+" << endl;

        // Lee lo que el usuario quiere hacer
        getline(cin, seleccion);

        // Limpia la pantalla nuevamente
        system("cls");

        // Si el usuario elige "1", ver su reserva
        if (seleccion == "1")
        {
            consultar_reserva.Consultar_Reserva();
        }
        // Si el usuario elige "2", hacer una nueva reserva
        else if (seleccion == "2") {
            consulta_reservar.Ingresar_Fecha();       // Pedir la fecha del viaje
            consulta_reservar.Consultar_Vuelos();     // Mostrar los vuelos disponibles
            consulta_reservar.Consultar_Horarios();   // Mostrar los horarios disponibles
            consulta_reservar.Ingresar_Nombre();      // Pedir el nombre del usuario
            consulta_reservar.Ingresar_Asiento();     // Pedir el asiento que quiere el usuario
            consulta_reservar.Registrar_Reserva();    // Registrar la reserva en la base de datos
        }
        // Si el usuario elige "3", salir del programa
        else if (seleccion == "3")
        {
            break;
        }
        // Si el usuario escribe algo incorrecto, pedirle que lo intente de nuevo
        else {
            cout << "Ingrese un dato valido" << endl << endl;
        }
    }
}
