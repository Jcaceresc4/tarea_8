#pragma once
#include <iostream>
#include <string>
#include "ConexionBD.h"
#include <regex>
#include <ctime>
#include <limits>
#include <mysql.h>

using std::string;
using std::cout;
using std::cin;
using std::endl;
using std::regex;
using std::regex_match;

class Validador {
public:

    // ==================== VALIDACIÓN DE CÓDIGO ====================
    /**
     * Valida el código del estudiante
     * Formato: E### (Ejemplo: E001, E002, etc.)
     * Máximo 4 caracteres
     */
    static bool validarCodigo(const string& codigo) {
        if (codigo.empty()) {
            cout << "❌ Error: El código no puede estar vacío." << endl;
            return false;
        }

        if (codigo.length() > 4) {
            cout << "❌ Error: El código debe tener máximo 4 caracteres. Ingresado: "
                << codigo.length() << " caracteres." << endl;
            return false;
        }

        // Expresión regular para formato E### (mayúscula o minúscula)
        regex formatoCodigo("^[Ee]\\d{3}$");
        if (!regex_match(codigo, formatoCodigo)) {
            cout << "❌ Error: El código debe seguir el formato E### (ejemplo: E001, E002)." << endl;
            return false;
        }

        return true;
    }

    /**
     * Verifica si el código ya existe en la base de datos
     */
    static bool codigoExistente(const string& codigo, int id_actual = 0) {
        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.getConector()) {
            cout << "⚠️  Advertencia: No se pudo verificar duplicidad (sin conexión BD)." << endl;
            return false;
        }

        string consulta = "SELECT COUNT(*) as cantidad FROM estudiantes WHERE codigo = '" + codigo + "'";

        // Si estamos actualizando, excluir el ID actual
        if (id_actual > 0) {
            consulta += " AND id_estudiante != " + std::to_string(id_actual);
        }

        consulta += ";";
        const char* c = consulta.c_str();
        int q_estado = mysql_query(cn.getConector(), c);

        bool existe = false;
        if (!q_estado) {
            MYSQL_RES* resultado = mysql_store_result(cn.getConector());
            MYSQL_ROW fila = mysql_fetch_row(resultado);
            if (fila && atoi(fila[0]) > 0) {
                cout << "❌ Error: El código '" << codigo << "' ya existe en la base de datos." << endl;
                existe = true;
            }
            mysql_free_result(resultado);
        }

        cn.cerrar_conexion();
        return existe;
    }

    // ==================== VALIDACIÓN DE NOMBRES ====================
    /**
     * Valida nombres del estudiante
     * Solo letras y espacios, máximo 60 caracteres
     */
    static bool validarNombres(const string& nombres) {
        if (nombres.empty()) {
            cout << "❌ Error: Los nombres no pueden estar vacíos." << endl;
            return false;
        }

        if (nombres.length() > 60) {
            cout << "❌ Error: Los nombres deben tener máximo 60 caracteres. Ingresado: "
                << nombres.length() << " caracteres." << endl;
            return false;
        }

        // Solo letras (incluyendo acentos españoles) y espacios
        regex formatoNombres("^[a-zA-ZáéíóúÁÉÍÓÚàèìòùÀÈÌÒÙäëïöüÄËÏÖÜãõÃÕñÑ ]+$");
        if (!regex_match(nombres, formatoNombres)) {
            cout << "❌ Error: Los nombres solo deben contener letras y espacios." << endl;
            return false;
        }

        return true;
    }

    // ==================== VALIDACIÓN DE APELLIDOS ====================
    /**
     * Valida apellidos del estudiante
     * Solo letras y espacios, máximo 60 caracteres
     */
    static bool validarApellidos(const string& apellidos) {
        if (apellidos.empty()) {
            cout << "❌ Error: Los apellidos no pueden estar vacíos." << endl;
            return false;
        }

        if (apellidos.length() > 60) {
            cout << "❌ Error: Los apellidos deben tener máximo 60 caracteres. Ingresado: "
                << apellidos.length() << " caracteres." << endl;
            return false;
        }

        // Solo letras (incluyendo acentos españoles) y espacios
        regex formatoApellidos("^[a-zA-ZáéíóúÁÉÍÓÚàèìòùÀÈÌÒÙäëïöüÄËÏÖÜãõÃÕñÑ ]+$");
        if (!regex_match(apellidos, formatoApellidos)) {
            cout << "❌ Error: Los apellidos solo deben contener letras y espacios." << endl;
            return false;
        }

        return true;
    }

    // ==================== VALIDACIÓN DE DIRECCIÓN ====================
    /**
     * Valida dirección
     * Máximo 100 caracteres, sin caracteres peligrosos SQL
     */
    static bool validarDireccion(const string& direccion) {
        if (direccion.empty()) {
            cout << "❌ Error: La dirección no puede estar vacía." << endl;
            return false;
        }

        if (direccion.length() > 100) {
            cout << "❌ Error: La dirección debe tener máximo 100 caracteres. Ingresado: "
                << direccion.length() << " caracteres." << endl;
            return false;
        }

        // Detectar caracteres peligrosos para SQL injection
        const string caracteresPeligrosos = "'\";<>%\\";
        for (char c : caracteresPeligrosos) {
            if (direccion.find(c) != string::npos) {
                cout << "❌ Error: La dirección contiene caracteres peligrosos: " << c << endl;
                return false;
            }
        }

        return true;
    }

    // ==================== VALIDACIÓN DE TELÉFONO ====================
    /**
     * Valida teléfono
     * Solo números, exactamente 8 dígitos
     */
    static bool validarTelefono(int telefono) {
        string telStr = std::to_string(telefono);

        if (telStr.length() != 8) {
            cout << "❌ Error: El teléfono debe tener exactamente 8 dígitos. Ingresado: "
                << telStr.length() << " dígitos." << endl;
            return false;
        }

        if (telefono < 0) {
            cout << "❌ Error: El teléfono no puede ser negativo." << endl;
            return false;
        }

        return true;
    }

    // ==================== VALIDACIÓN DE FECHA ====================
    /**
     * Valida fecha de nacimiento
     * Formato: AAAA-MM-DD
     * No debe ser fecha futura
     * Debe ser una fecha válida
     */
    static bool validarFecha(const string& fecha) {
        if (fecha.empty()) {
            cout << "❌ Error: La fecha de nacimiento no puede estar vacía." << endl;
            return false;
        }

        // Validar formato AAAA-MM-DD
        regex formatoFecha("^\\d{4}-(0[1-9]|1[0-2])-(0[1-9]|[12][0-9]|3[01])$");
        if (!regex_match(fecha, formatoFecha)) {
            cout << "❌ Error: La fecha debe tener formato AAAA-MM-DD (ejemplo: 2005-03-15)." << endl;
            return false;
        }

        // Extraer componentes de la fecha
        int anio = std::stoi(fecha.substr(0, 4));
        int mes = std::stoi(fecha.substr(5, 2));
        int dia = std::stoi(fecha.substr(8, 2));

        // Validar rango de mes
        if (mes < 1 || mes > 12) {
            cout << "❌ Error: El mes debe estar entre 01 y 12. Ingresado: " << mes << endl;
            return false;
        }

        // Validar rango de día según el mes
        int diasEnMes[] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        // Validar año bisiesto
        bool esBisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
        if (esBisiesto) {
            diasEnMes[2] = 29;
        }

        if (dia < 1 || dia > diasEnMes[mes]) {
            cout << "❌ Error: El día no es válido para el mes " << mes << ". Máximo: "
                << diasEnMes[mes] << " días." << endl;
            return false;
        }

        // Validar que no sea fecha futura
        time_t ahora = time(nullptr);
        tm* fechaActual = localtime(&ahora);

        int anioActual = 1900 + fechaActual->tm_year;
        int mesActual = 1 + fechaActual->tm_mon;
        int diaActual = fechaActual->tm_mday;

        if (anio > anioActual || (anio == anioActual && mes > mesActual) ||
            (anio == anioActual && mes == mesActual && dia > diaActual)) {
            cout << "❌ Error: La fecha de nacimiento no puede ser en el futuro." << endl;
            return false;
        }

        // Validar edad mínima (5 años)
        int edadMinima = 5;
        int edadActual = anioActual - anio;
        if (mes > mesActual || (mes == mesActual && dia > diaActual)) {
            edadActual--;
        }

        if (edadActual < edadMinima) {
            cout << "❌ Error: El estudiante debe tener al menos " << edadMinima
                << " años de edad. Edad actual: " << edadActual << " años." << endl;
            return false;
        }

        return true;
    }

    // ==================== VALIDACIÓN DE TIPO DE SANGRE ====================
    /**
     * Valida tipo de sangre
     * Verifica que exista en la tabla tipos_sangre
     */
    static bool validarTipoSangre(int idTipoSangre) {
        if (idTipoSangre <= 0) {
            cout << "❌ Error: El tipo de sangre debe ser un número positivo." << endl;
            return false;
        }

        // Validar que existe en la BD
        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.getConector()) {
            cout << "⚠️  Advertencia: No se pudo verificar tipo de sangre (sin conexión BD)." << endl;
            return false;
        }

        string consulta = "SELECT COUNT(*) as cantidad FROM tipos_sangre WHERE id_tipo_sangre = "
            + std::to_string(idTipoSangre) + ";";
        const char* c = consulta.c_str();
        int q_estado = mysql_query(cn.getConector(), c);

        bool existe = false;
        if (!q_estado) {
            MYSQL_RES* resultado = mysql_store_result(cn.getConector());
            MYSQL_ROW fila = mysql_fetch_row(resultado);
            if (fila && atoi(fila[0]) > 0) {
                existe = true;
            }
            mysql_free_result(resultado);
        }

        cn.cerrar_conexion();

        if (!existe) {
            cout << "❌ Error: El tipo de sangre ID " << idTipoSangre
                << " no existe en la base de datos." << endl;
            mostrarTiposSangre();
            return false;
        }

        return true;
    }

    // ==================== VALIDACIÓN DE ID ESTUDIANTE ====================
    /**
     * Valida ID del estudiante
     * Debe ser positivo
     */
    static bool validarIdEstudiante(int idEstudiante) {
        if (idEstudiante <= 0) {
            cout << "❌ Error: El ID del estudiante debe ser un número positivo." << endl;
            return false;
        }

        return true;
    }

    // ==================== FUNCIONES AUXILIARES ====================
    /**
     * Muestra los tipos de sangre disponibles en la BD
     */
    static void mostrarTiposSangre() {
        cout << "\n📋 Tipos de sangre disponibles:" << endl;

        ConexionBD cn;
        cn.abrir_conexion();

        if (!cn.getConector()) {
            cout << "⚠️  No se pudo conectar para mostrar tipos de sangre." << endl;
            return;
        }

        string consulta = "SELECT id_tipo_sangre, sangre FROM tipos_sangre ORDER BY id_tipo_sangre;";
        const char* c = consulta.c_str();
        int q_estado = mysql_query(cn.getConector(), c);

        if (!q_estado) {
            MYSQL_RES* resultado = mysql_store_result(cn.getConector());
            MYSQL_ROW fila;
            while (fila = mysql_fetch_row(resultado)) {
                cout << "  " << fila[0] << " = " << fila[1] << endl;
            }
            mysql_free_result(resultado);
        }

        cn.cerrar_conexion();
    }

    // ==================== FUNCIONES DE ENTRADA VALIDADA ====================
    /**
     * Obtiene código validado del usuario con reintentos
     */
    static string obtenerCodigoValidado(int id_actual = 0) {
        string codigo;
        while (true) {
            cout << "\nIngrese Código (formato E###): ";
            cin >> codigo;

            if (!validarCodigo(codigo)) {
                continue;
            }

            if (codigoExistente(codigo, id_actual)) {
                continue;
            }

            return codigo;
        }
    }

    /**
     * Obtiene nombres validados del usuario con reintentos
     */
    static string obtenerNombresValidados() {
        string nombres;
        while (true) {
            cout << "Ingrese Nombres (máx 60 caracteres, solo letras): ";
            std::cin.ignore();
            std::getline(std::cin, nombres);
            if (validarNombres(nombres)) {
                return nombres;
            }
        }
    }

    /**
     * Obtiene apellidos validados del usuario con reintentos
     */
    static string obtenerApellidosValidados() {
        string apellidos;
        while (true) {
            cout << "Ingrese Apellidos (máx 60 caracteres, solo letras): ";
            std::cin.ignore();
            std::getline(std::cin, apellidos);
            if (validarApellidos(apellidos)) {
                return apellidos;
            }
        }
    }

    /**
     * Obtiene dirección validada del usuario con reintentos
     */
    static string obtenerDireccionValidada() {
        string direccion;
        while (true) {
            cout << "Ingrese Dirección (máx 100 caracteres): ";
            std::cin.ignore();
            std::getline(std::cin, direccion);
            if (validarDireccion(direccion)) {
                return direccion;
            }
        }
    }

    /**
     * Obtiene teléfono validado del usuario con reintentos
     */
    static int obtenerTelefonoValidado() {
        int telefono;
        while (true) {
            cout << "Ingrese Teléfono (8 dígitos): ";
            if (!(std::cin >> telefono)) {
                std::cin.clear();
                (std::cin.ignore)((std::numeric_limits<std::streamsize>::max)(), '\n');
                cout << "❌ Error: Ingrese un número válido." << endl;
                continue;
            }

            if (validarTelefono(telefono)) {
                return telefono;
            }
        }
    }

    /**
     * Obtiene fecha validada del usuario con reintentos
     */
    static string obtenerFechaValidada() {
        string fecha;
        while (true) {
            cout << "Ingrese Fecha de Nacimiento (formato AAAA-MM-DD): ";
            std::cin.ignore();
            std::getline(std::cin, fecha);
            if (validarFecha(fecha)) {
                return fecha;
            }
        }
    }

    /**
     * Obtiene tipo de sangre validado del usuario con reintentos
     */
    static int obtenerTipoSangreValidado() {
        int tipo;
        mostrarTiposSangre();
        while (true) {
            cout << "Ingrese Tipo de Sangre (ID): ";
            if (!(std::cin >> tipo)) {
                std::cin.clear();
                (std::cin.ignore)((std::numeric_limits<std::streamsize>::max)(), '\n');
                cout << "❌ Error: Ingrese un número válido." << endl;
                continue;
            }

            if (validarTipoSangre(tipo)) {
                return tipo;
            }
        }
    }

    /**
     * Obtiene ID validado del usuario con reintentos
     */
    static int obtenerIdValidado(const string& mensaje) {
        int id;
        while (true) {
            cout << mensaje;
            if (!(std::cin >> id)) {
                std::cin.clear();
                (std::cin.ignore)((std::numeric_limits<std::streamsize>::max)(), '\n');
                cout << "❌ Error: Ingrese un número válido." << endl;
                continue;
            }

            if (validarIdEstudiante(id)) {
                return id;
            }
        }
    }

    /**
     * Escapa caracteres peligrosos para SQL
     * NOTA: Se recomienda usar prepared statements en producción
     */
    static string escaparSQL(const string& texto) {
        string resultado = "";
        for (char c : texto) {
            if (c == '\'') {
                resultado += "\\'";
            }
            else if (c == '"') {
                resultado += "\\\"";
            }
            else if (c == '\\') {
                resultado += "\\\\";
            }
            else {
                resultado += c;
            }
        }
        return resultado;
    }
};