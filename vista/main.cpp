// Evitar conflicto de byte ANTES de cualquier include
#define _HAS_STD_BYTE 0

#include <iostream>
#include <string>
#include "Estudiante.h"
#include "Validador.h"
#include <regex>
#include <limits>

// Usar prefijos específicos para mayor seguridad
using std::cout;
using std::cin;
using std::endl;
using std::string;

// ==================== FUNCIONES DEL MENÚ ====================

/**
 * Muestra el menú principal
 */
void mostrarMenu() {
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║   SISTEMA DE GESTIÓN DE ESTUDIANTES      ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;
    cout << "1. Crear nuevo estudiante" << endl;
    cout << "2. Leer/Listar estudiantes" << endl;
    cout << "3. Actualizar estudiante" << endl;
    cout << "4. Eliminar estudiante" << endl;
    cout << "5. Salir" << endl;
    cout << "───────────────────────────────────────────" << endl;
    cout << "Seleccione una opción: ";
}

/**
 * Opción 1: Crear un nuevo estudiante
 */
void crearEstudiante() {
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║      CREAR NUEVO ESTUDIANTE               ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;

    // Obtener datos con validación
    string codigo = Validador::obtenerCodigoValidado();
    string nombres = Validador::obtenerNombresValidados();
    string apellidos = Validador::obtenerApellidosValidados();
    string direccion = Validador::obtenerDireccionValidada();
    int telefono = Validador::obtenerTelefonoValidado();
    string fecha_nacimiento = Validador::obtenerFechaValidada();
    int id_tipo_sangre = Validador::obtenerTipoSangreValidado();

    // Crear objeto (id_estudiante = 0 porque es auto-incrementable)
    Estudiante e(nombres, apellidos, direccion, telefono,
        fecha_nacimiento, id_tipo_sangre, codigo, 0);

    // Intentar crear
    if (e.crear()) {
        cout << "\n✅ Estudiante creado exitosamente." << endl;
        // Mostrar todos los registros después de crear
        e.leer();
    }
    else {
        cout << "\n❌ Error al crear el estudiante." << endl;
    }
}

/**
 * Opción 2: Leer/Listar estudiantes
 */
void leerEstudiantes() {
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║      LISTAR TODOS LOS ESTUDIANTES         ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;

    Estudiante e;
    e.leer();
}

/**
 * Opción 3: Actualizar estudiante
 */
void actualizarEstudiante() {
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║      ACTUALIZAR ESTUDIANTE                ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;

    // Primero mostrar todos los estudiantes
    Estudiante eTemp;
    eTemp.leer();

    // Solicitar ID a actualizar
    int id_estudiante = Validador::obtenerIdValidado("\nIngrese el ID del estudiante a modificar: ");

    cout << "\nIngrese los nuevos datos:" << endl;

    // Obtener datos con validación
    string codigo = Validador::obtenerCodigoValidado(id_estudiante);
    string nombres = Validador::obtenerNombresValidados();
    string apellidos = Validador::obtenerApellidosValidados();
    string direccion = Validador::obtenerDireccionValidada();
    int telefono = Validador::obtenerTelefonoValidado();
    string fecha_nacimiento = Validador::obtenerFechaValidada();
    int id_tipo_sangre = Validador::obtenerTipoSangreValidado();

    // Crear objeto con los nuevos datos
    Estudiante e(nombres, apellidos, direccion, telefono,
        fecha_nacimiento, id_tipo_sangre, codigo, id_estudiante);

    // Intentar actualizar
    if (e.actualizar()) {
        cout << "\n✅ Estudiante actualizado exitosamente." << endl;
        // Mostrar todos los registros después de actualizar
        e.leer();
    }
    else {
        cout << "\n❌ Error al actualizar el estudiante." << endl;
    }
}

/**
 * Opción 4: Eliminar estudiante
 */
void eliminarEstudiante() {
    cout << "\n╔═══════════════════════════════════════════╗" << endl;
    cout << "║      ELIMINAR ESTUDIANTE                  ║" << endl;
    cout << "╚═══════════════════════════════════════════╝" << endl;

    // Primero mostrar todos los estudiantes
    Estudiante eTemp;
    eTemp.leer();

    // Solicitar ID a eliminar
    int id_estudiante = Validador::obtenerIdValidado("\nIngrese el ID del estudiante a eliminar: ");

    // Crear objeto y establecer solo el ID
    Estudiante e;
    e.setId_estudiante(id_estudiante);

    // Solicitar confirmación
    char confirmacion;
    cout << "\n⚠️  ¿Está seguro que desea eliminar este estudiante? (s/n): ";
    cin >> confirmacion;

    if (confirmacion == 's' || confirmacion == 'S') {
        // Intentar eliminar
        if (e.borrar()) {
            cout << "\n✅ Estudiante eliminado exitosamente." << endl;
            // Mostrar todos los registros después de eliminar
            e.leer();
        }
        else {
            cout << "\n❌ Error al eliminar el estudiante." << endl;
        }
    }
    else {
        cout << "\n❌ Operación cancelada." << endl;
    }

    cin.ignore();
}

/**
 * Función principal
 */
int main() {
    cout << "\n════════════════════════════════════════════" << endl;
    cout << "🎓 BIENVENIDO AL SISTEMA DE GESTIÓN DE ESTUDIANTES" << endl;
    cout << "════════════════════════════════════════════" << endl;

    int opcion = 0;
    bool salir = false;

    while (!salir) {
        mostrarMenu();

        // Validar entrada de menú
        if (!(cin >> opcion)) {
            cin.clear();
            (cin.ignore)((std::numeric_limits<std::streamsize>::max)(), '\n');
            cout << "\n❌ Error: Ingrese un número válido." << endl;
            continue;
        }

        cin.ignore();

        switch (opcion) {
        case 1:
            crearEstudiante();
            break;
        case 2:
            leerEstudiantes();
            break;
        case 3:
            actualizarEstudiante();
            break;
        case 4:
            eliminarEstudiante();
            break;
        case 5:
            cout << "\n👋 ¡Hasta luego! Gracias por usar el sistema." << endl;
            cout << "════════════════════════════════════════════\n" << endl;
            salir = true;
            break;
        default:
            cout << "\n❌ Opción inválida. Por favor, seleccione una opción entre 1 y 5." << endl;
        }

        if (!salir) {
            cout << "\nPresione Enter para continuar...";
            cin.get();
        }
    }

    return 0;
}