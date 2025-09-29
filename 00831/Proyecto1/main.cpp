/*
Se importan iostream para functiones cout y cin, asi como namespace para evitar tener que declarar std en todas las invocaciones
de la libreria.

Se importa vector porque vamos a implementar el historial como un struct, que nos va a permitir almacenar varios tipos de datos
en una estructura similar a una matriz en python, pero mas similar a una base de datos no columnar, con vectores de tipos mixtos
en medio.

Se usa CLion en vez de CodeBlocks para indentar todo el programa, sin embargo, se evita usarlo para el compilado final.
*/
#include <iostream>
#include <vector>

//las siguientes librerias son recomendadas segun documentacion de C++ https://en.cppreference.com/w/cpp/io/manip/get_money.html para manipulacion de currency.
#include <iomanip>
#include <locale>
#include <sstream>


using namespace std;
int intentos_Pin = 0;
bool pin_Validado = false;
double total_Cuenta = 150000;
//El pin es falso hasta que introduzcan el verdadero.
vector<int> historial_Transacciones;

/*
Funcion para limpiar pantalla, segun la literatura, no hay un clear universal de pantalla, entonces se implementa segun sea el S.O.
*/
void limpiarPantalla() {
    // logica para limpiar pantalla, segun la comunidad, tomado de: https://es.stackoverflow.com/questions/218943/existe-una-manera-est%C3%A1ndar-de-limpiar-la-pantalla-en-c-c
#ifdef _WIN32
    system("cls");
    // Si el programa se ejecuta en un sistema Unix (Linux, macOS, etc.)
#else
    system("clear");
#endif
}

void print_colones(ostream &os, double amount) {
    //con ostream podemos implementar el cout como funcion en este metodo, y con os, sacamos el locale
    os << (char) 155 << fixed << setprecision(2) << amount;
}


struct Transacciones {
    int orden = 0;
    string tipo_transaccion = "Deposito_Inicial";
    double cantidad = total_Cuenta;
    double estado_De_Cuenta = total_Cuenta;
};

vector<Transacciones> transDB;

/*
Funcion para validar el Pin, esta funcion recibe un numero (el pin) y modifica -o no- un booleano de pin_Validado.

*/
void validacion_De_Pin(int numero) {
    /*Se definen los pines como constantes, no es la mejor forma, pero a falta de una funcion de "Cambiar Pin" los vamos a definir como constantes, ya que
    para nosotros lo son. */


    int const Pin1 = 1234, Pin2 = 2345, Pin3 = 3456, Pin4 = 4567, Pin5 = 5678;
    if (numero == Pin1 || numero == Pin2 || numero == Pin3 || numero == Pin4 || numero == Pin5) {
        pin_Validado = true;
    } else {
        cout << "PIN Erroneo, intentos restantes " << 2 - intentos_Pin << endl;
        cout << "Intente introducirlo de nuevo";
        intentos_Pin++;
        cin.get();
        cin.get();
        limpiarPantalla();
    }
}


/*
Funcion para la consulta de saldo, lee el valor actual del saldo. (Hace falta formatearlo a colones)
e imprime en la consola la cantidad.

*/
void consulta_De_Saldo() {
    cout << "Ud tiene el monto de ";
    print_colones(cout, total_Cuenta);
    cout << " colones";
    //iniciar saldo con 150 000 Colones.
    //debe guardar un log con transacciones
}

/*Funcion para el deposito de dinero, se almacena en el struct y modifica el valor del saldo. (Hace falta un error catch para overflow de valores)
  */
void deposito_De_Dinero(int monto) {
    // Si tengo mas de 5 Transacciones no entro


    if (transDB.size() < 5) {
        Transacciones nueva_Linea;

        //si mi vector esta vacio, el tamaño es 0, pero si no, tengo 1, y el inicio es en 0, es decir siempre voy a tener el orden bien.
        nueva_Linea.orden = transDB.size() + 1;
        nueva_Linea.tipo_transaccion = "Deposito";
        nueva_Linea.cantidad = monto;

        // actualizo el total de dinero
        total_Cuenta += monto;
        nueva_Linea.estado_De_Cuenta = total_Cuenta;

        // Empujo una nueva linea al vector
        transDB.push_back(nueva_Linea);
    } else {
        cout << "Ya tiene mas de 5 Transacciones, no puede seguir." << endl;
    }
}

/*
Igual que la funcion anterior recibe un monto de dinero, lo escribe al struct (de ser valido y tener menos de 5 transact), y
modifica el saldo.


*/
void retiro_De_Dinero(int monto) {
    if (transDB.size() < 5) {
        Transacciones nueva_Linea;

        //si mi vector esta vacio, el tamaño es 0, pero si no, tengo 1, y el inicio es en 0, es decir siempre voy a tener el orden bien.
        nueva_Linea.orden = transDB.size() + 1;
        nueva_Linea.tipo_transaccion = "Retiro  "; //con los espacios llega a 8 caracteres y se alinea el output
        nueva_Linea.cantidad = monto;

        // actualizo el total de dinero
        total_Cuenta -= monto;
        nueva_Linea.estado_De_Cuenta = total_Cuenta;

        // Empujo una nueva linea al vector
        transDB.push_back(nueva_Linea);
    } else {
        cout << "Ya tiene mas de 5 Transacciones o el valor introducido es invalido, no puede seguir." << endl;
    }
}

/*
imprime las transacciones que tenemos (de haber alguna), el for es muy curioso hubo que buscarlo.
Se declara que es tipo Transacciones y que recorra la base original transDB. Es similar a un for
en Python. Pero aca hay que diferenciar entre recorrer la copia y el original.

*/

void historial_De_Transacciones() {
    if (transDB.empty()) {
        cout << "No hay transacciones para mostrar." << endl;
        return;
    }

    cout << "===============================================" << endl;
    cout << "       HISTORIAL DE TRANSACCIONES" << endl;
    cout << "===============================================" << endl;
    cout << "Orden\tTipo \tCantidad\t\tTotal" << endl;
    cout << "-----------------------------------------------" << endl;

    // Use a range-based for loop to iterate through each Transacciones object in the vector.
    for (Transacciones &lineas: transDB) {
        cout << lineas.orden << "\t";
        cout << lineas.tipo_transaccion << "\t ";
        print_colones(cout, lineas.cantidad);
        cout << "\t";
        print_colones(cout, lineas.estado_De_Cuenta);
        cout << endl;
    }
    cout << "===============================================" << endl;
}

/*
Implementacion del menu, es un switch con 5 opciones que llama las funciones declaradas anteriormente.

*/
void implementacion_Del_Menu() {
    int opcion;
    do {
        cout << "Menu Principal" << endl;
        cout << "====================================" << endl;
        cout << "MENU DE OPCIONES" << endl;
        cout << "====================================" << endl;
        cout << "1. Consultar saldo" << endl;
        cout << "2. Retirar dinero" << endl;
        cout << "3. Depositar dinero" << endl;
        cout << "4. Ver historial de transacciones" << endl;
        cout << "5. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;


        switch (opcion) {
            case 1:
                limpiarPantalla();
                cout << "Consultando saldo..." << endl;
                consulta_De_Saldo();
                cout << endl;
                cout << "Presiona Enter para continuar..." << endl;
                cin.get();
                cin.get();
                limpiarPantalla();

                break;
            case 2:

                limpiarPantalla();
                cout << "Retirando dinero..." << endl;
                cout << "Cuanto quiere retirar?, por favor introduzca el monto" << endl;
                double monto_a_retirar;
                cin >> monto_a_retirar;
                if (monto_a_retirar < 0 || monto_a_retirar > total_Cuenta) {
                    cout <<
                            "El monto que quiere retirar es invalido, por favor digite el monto correcto. \n presione enter para continuar.";
                    cin.get();
                    cin.get();
                    limpiarPantalla();
                    break;
                }
                cout << "Desea retirar ";
                print_colones(cout, monto_a_retirar);
                cout << " Colones?" << endl;
                cout << "Presione 1 para si y 2 para no." << endl;
                int confirmacion;
                cin >> confirmacion;
                if (confirmacion == 1 && transDB.size() < 5) {
                    retiro_De_Dinero(monto_a_retirar);
                    cout << "Retiro de ";
                    print_colones(cout, monto_a_retirar);
                    cout << " Colones realizado." << endl;
                    cout << "Ahora su total es de ";
                    print_colones(cout, total_Cuenta);
                    cout << " Colones. \n Gracias" << endl;
                    cin.get();
                    cin.get();
                    limpiarPantalla();
                    break;
                } else {
                    cout << "Retiro cancelado." << endl;
                }

                cout << "Presiona Enter para continuar..." << endl;
                cin.get();
                //es igual que antes, el endl deja un input entonces hay que limpiarlo con ignore o con 2 get.
                cin.get();
                limpiarPantalla();

                break;
            case 3:
                limpiarPantalla();
                cout << "Depositando dinero..." << endl;
                cout << "Cuanto quiere depositar?, por favor introduzca el monto" << endl;
                double monto_a_depositar;
                cin >> monto_a_depositar;
                if (monto_a_depositar <= 0) {
                    cout <<
                            "El monto que quiere depositar es invalido, por favor digite el monto correcto. \n presione enter para continuar.";
                    cin.get();
                    cin.get();
                    limpiarPantalla();
                    break;
                } else if (monto_a_depositar > 1e15L || (monto_a_depositar + total_Cuenta) > 1e15L) {
                    // Límite de 10^15 por problemas de overflow
                    cout <<
                            "\nERROR: El monto excede el limite de transaccion (10^15). Por favor digite una cantidad menor."
                            << endl;
                    cout << "Presione Enter para continuar..." << endl;
                    cin.get();
                    cin.get();
                    limpiarPantalla();
                    break;
                }

                cout << "Desea depositar ";
                print_colones(cout, monto_a_depositar);
                cout << " Colones?" << endl;
                cout << "Presione 1 para si y 2 para no." << endl;
                //int confirmacion;
                cin >> confirmacion;
                if (confirmacion == 1 && transDB.size() < 5) {
                    deposito_De_Dinero(monto_a_depositar);
                    cout << "Deposito de ";
                    print_colones(cout, monto_a_depositar);
                    cout << " Colones realizado." << endl;
                    cout << "Ahora su total es de ";
                    print_colones(cout, total_Cuenta);
                    cout << " Colones. \nGracias" << endl;
                    cout << "Presione Enter para continuar..." << endl;

                    cin.get();
                    cin.get();
                    limpiarPantalla();
                    break;
                } else {
                    cout << "Deposito cancelado." << endl;
                }

                cout << "Presiona Enter para continuar..." << endl;
                cin.get();
                //es igual que antes, el endl deja un input entonces hay que limpiarlo con ignore o con 2 get.
                cin.get();

                break;
            case 4:
                limpiarPantalla();
                cout << "Mostrando historial..." << endl;
                historial_De_Transacciones();
                cin.get();
                cin.get();
                limpiarPantalla();
                break;
            case 5:
                cout << "Saliendo..." << endl;
                break;
            default:
                limpiarPantalla();
                cout << "El numero " << opcion << " es una opcion invalida. Intente nuevamente." << endl;
        }
    } while (opcion != 5);
}


int main() {
    /*
    Dado que el formato de dinero depende del locale de cada computadora, no hay forma de generalizar el output sin
    tener en cuenta el locale del lugar a donde se esta trabajando la ejecucion del programa, por lo que primero intentamos
    invocar el locale de CR (que se presume ser el mas comun), pero a sabiendas que no es universal en CR, tambien seteamos el
    default, "" por si acaso para no evitar la ejecucion del programa completo.

    */
    try {
        locale::global(locale("es_CR"));
    } catch (...) {
        // Si falla, intenta el locale nativo del sistema ("").
        try {
            locale::global(locale(""));
        } catch (...) {
            // Si falla, continua sin formato de 100.000 sino 100000, aunque al tener fixed en la funcion al menos evitamos la notacion cientifica.
        }
    }


    do {
        int numero;
        cout << "Ingrese el pin" << endl;
        cin >> numero;
        limpiarPantalla();

        validacion_De_Pin(numero);
    } while (!pin_Validado && intentos_Pin < 3); //repetir mientras el pin sea falso, y sea menor que 3
    if (pin_Validado != true) {
        cout <<
                "Gracias por Preferirnos, retendremos y destruiremos su tarjeta, por favor contacte al banco para una nueva";
        exit(0);
    } else {
        cout << "Introdujo bien la clave!" << endl;
        cout << "Bienvenido!, Ingresando al menu..." << endl;
        cout << "Presiona Enter para continuar..." << endl;
        cin.get();
        cin.get();
        limpiarPantalla();
        implementacion_Del_Menu();
        exit(0);
    }
}
