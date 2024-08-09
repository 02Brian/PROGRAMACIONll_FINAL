// ConexionBD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//BRIAN AXEL SANCHEZ 

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <ctime>
#include <iomanip>

using namespace std;

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Asignar un gestor de entorno
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Asignar un gestor de conexión
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Conectarse a la base de datos
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sqlserver", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) { // Operador logico OR
        cout << "Conectado a la base de datos exitosamente." << endl;

        // Ejemplo de ejecución de una consulta
        SQLHSTMT hStmt; //Statement
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt); //

        //imprimir encabezados de la tabla 
        cout << "+----------------+-----------------------------------+-----------------+---------------------+-------------------------+" << endl;
        cout << "| No. Empleado   | Nombre  Completo                  | Fecha Nacimiento|        RFC          |      Centro Trabajo     |" << endl;
        cout << "+----------------+-----------------------------------+-----------------+---------------------+-------------------------+" << endl;

        // Ejemplo de consulta SELECT
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Datos_Empleados", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_empleado;
            SQLCHAR name[50];
            SQLCHAR first_name[50];
            SQLCHAR last_name[50];
            SQLCHAR Fecha[15];
            SQLCHAR RFC[50];
            SQLCHAR Centro_Trabajo[50];

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, name, sizeof(name), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, first_name, sizeof(first_name), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, last_name, sizeof(last_name), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, Fecha, sizeof(Fecha), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, RFC, sizeof(RFC), NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, Centro_Trabajo, sizeof(Centro_Trabajo), NULL);

                //CONCATENAR NOMBRE Y APELLIDOS 
                string first_names = string((char*)first_name) + "  " + string((char*)last_name);
                string full_name = string((char*)name) + " " + string(first_names);

                //imprimir datos de la fila con alineación 

                cout << "| " << setw(15) << left << num_empleado
                    << "| " << setw(34) << left << full_name
                    << "| " << setw(16) << left << Fecha
                    << "| " << setw(20) << left << RFC
                    << "| " << setw(24) << left << Centro_Trabajo << "| " << endl;

            }
        }

        //IMPRIMIR LINEA FINAL DE LA TABLA 
        cout << "+----------------+-----------------------------------+-----------------+---------------------+-------------------------+" << endl;


        // Liberar el manejador de conexion 
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);


    }
    // Ejemplo de ejecución de una consulta
    SQLHSTMT hStmt; //Statement
    ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt); //
    //imprimir encabezados de la tabla 
    cout << "+-------------------------------+---------------------------------------------------------+----------+" << endl;
    cout << "| Puesto                        | Descripcion_Puesto                                      | Directivo| " << endl;
    cout << "+-------------------------------+---------------------------------------------------------+----------+" << endl;

    // Ejemplo de consulta SELECT
    ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Datos_Empleados", SQL_NTS);
    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        SQLCHAR Puesto[50];
        SQLCHAR Descripcion_Puesto[56];
        SQLCHAR Directivo[50];

        while (SQLFetch(hStmt) == SQL_SUCCESS) {
            SQLGetData(hStmt, 8, SQL_C_CHAR,Puesto, sizeof(Puesto), NULL);
            SQLGetData(hStmt, 9, SQL_C_CHAR, Descripcion_Puesto, sizeof(Descripcion_Puesto), NULL);
            SQLGetData(hStmt, 10, SQL_C_CHAR, Directivo, sizeof(Directivo), NULL);
            string dir = string((char*)Directivo);
            string respuesta;

            if (dir == "1") {
                respuesta = "SI";
            }
            else {
                respuesta = "NO";
            }


            //imprimir datos de la fila con alineación 

            cout << "| " << setw(30) << left << Puesto
                << "| " << setw(56) << left << Descripcion_Puesto
                << "| " << setw(9) << left << respuesta  << "| " << endl;

        }


        //IMPRIMIR LINEA FINAL DE LA TABLA 
        cout << "+-------------------------------+---------------------------------------------------------+----------+" << endl;


        // Liberar el manejador de conexion 
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    }


    else {
        cout << "Fallo la conexion a la base de datos" << endl;
    }

    // Desconectar y liberar gestores de entorno
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}