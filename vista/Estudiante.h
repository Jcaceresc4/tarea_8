#pragma once
#include <iostream>
#include <string>
#include "Persona.h"
#include "ConexionBD.h"
#include "Validador.h"
#include <mysql.h>

using std::string;
using std::cout;
using std::cin;
using std::endl;

class Estudiante : public Persona {
    // atributos
private:
    string codigo;
    int id_estudiante = 0;

    // constructor
public:
    Estudiante() {}

    Estudiante(string nom, string ape, string dir, int tel, string fn, int id_ts, string cod, int id_e)
        : Persona(nom, ape, dir, tel, fn, id_ts) {
        codigo = cod;
        id_estudiante = id_e;
    }

    // ==================== SETTERS CON VALIDACIÓN ====================

    bool setId_estudiante(int e) {
        if (Validador::validarIdEstudiante(e)) {
            id_estudiante = e;
            return true;
        }
        return false;
    }

    bool setCodigo(string c) {
        if (Validador::validarCodigo(c)) {
            codigo = c;
            return true;
        }
        return false;
    }

    bool setNombres(string n) {
        if (Validador::validarNombres(n)) {
            nombres = n;
            return true;
        }
        return false;
    }

    bool setApellidos(string a) {
        if (Validador::validarApellidos(a)) {
            apellidos = a;
            return true;
        }
        return false;
    }

    bool setDireccion(string d) {
        if (Validador::validarDireccion(d)) {
            direccion = d;
            return true;
        }
        return false;
    }

    bool setTelefono(int t) {
        if (Validador::validarTelefono(t)) {
            telefono = t;
            return true;
        }
        return false;
    }

    bool setFecha_Nacimiento(string f) {
        if (Validador::validarFecha(f)) {
            fecha_nacimiento = f;
            return true;
        }
        return false;
    }

    bool setId_Tipo_Sangre(int ts) {
        if (Validador::validarTipoSangre(ts)) {
            id_tipo_sangre = ts;
            return true;
        }
        return false;
    }

    // ==================== GETTERS ====================

    int getId_estudiante() { return id_estudiante; }
    string getCodigo() { return codigo; }
    string getNombres() { return nombres; }
    string getApellidos() { return apellidos; }
    string getDireccion() { return direccion; }
    int getTelefono() { return telefono; }
    string getFecha_nacimiento() { return fecha_nacimiento; }
    int getId_tipo_sangre() { return id_tipo_sangre; }

    // ==================== MÉTODOS CRUD CON VALIDACIÓN ====================

    /**
     * CREAR: Inserta un nuevo estudiante en la BD
     * Valida todos los campos antes de insertar
     */
    bool crear() {
        // Validar todos los campos antes de insertar
        if (!Validador::validarCodigo(codigo)) {
            cout << "❌ Creación fallida: código inválido." << endl;
            return false;
        }

        if (Validador::codigoExistente(codigo)) {
            cout << "❌ Creación fallida: el código ya existe." << endl;
            return false;
        }

        if (!Validador::validarNombres(nombres)) {
            cout << "❌ Creación fallida: nombres inválidos." << endl;
            return false;
        }

        if (!Validador::validarApellidos(apellidos)) {
            cout << "❌ Creación fallida: apellidos inválidos." << endl;
            return false;
        }

        if (!Validador::validarDireccion(direccion)) {
            cout << "❌ Creación fallida: dirección inválida." << endl;
            return false;
        }

        if (!Validador::validarTelefono(telefono)) {
            cout << "❌ Creación fallida: teléfono inválido." << endl;
            return false;
        }

        if (!Validador::validarFecha(fecha_nacimiento)) {
            cout << "❌ Creación fallida: fecha de nacimiento inválida." << endl;
            return false;
        }

        if (!Validador::validarTipoSangre(id_tipo_sangre)) {
            cout << "❌ Creación fallida: tipo de sangre inválido." << endl;
            return false;
        }

        // Si todas las validaciones pasaron, proceder con la inserción
        int q_estado = 0;
        ConexionBD cn = ConexionBD();
        cn.abrir_conexion();

        if (!cn.getConector()) {
            cout << "❌ Error: No se pudo conectar a la base de datos." << endl;
            return false;
        }

        string t = std::to_string(telefono);
        string id_ts = std::to_string(id_tipo_sangre);

        // Usar escaped SQL para prevenir inyección
        string codigoEscapado = Validador::escaparSQL(codigo);
        string nombresEscapado = Validador::escaparSQL(nombres);
        string apellidosEscapado = Validador::escaparSQL(apellidos);
        string direccionEscapada = Validador::escaparSQL(direccion);
        string fechaEscapada = Validador::escaparSQL(fecha_nacimiento);

        string consulta = "INSERT INTO estudiantes(codigo,nombres,apellidos,direccion,telefono,fecha_nacimiento,id_tipo_sangre) "
            "VALUES ('" + codigoEscapado + "','" + nombresEscapado + "','" + apellidosEscapado + "','"
            + direccionEscapada + "'," + t + ",'" + fechaEscapada + "'," + id_ts + ");";

        const char* c = consulta.c_str();
        q_estado = mysql_query(cn.getConector(), c);

        if (!q_estado) {
            cout << "✅ Ingreso de Datos Exitoso..." << endl;
            cn.cerrar_conexion();
            return true;
        }
        else {
            cout << "❌ Error: Consulta fallida. " << mysql_error(cn.getConector()) << endl;
            cn.cerrar_conexion();
            return false;
        }
    }

    /**
     * LEER: Obtiene todos los estudiantes de la BD
     */
    bool leer() {
        int q_estado = 0;
        ConexionBD cn = ConexionBD();
        MYSQL_ROW fila;
        MYSQL_RES* resultado;

        cn.abrir_conexion();

        if (!cn.getConector()) {
            cout << "❌ Error: No se pudo conectar a la base de datos." << endl;
            return false;
        }

        cout << "\n___________ Datos del Estudiante ___________" << endl;
        string consulta = "SELECT e.id_estudiante as id, e.codigo, e.nombres, e.apellidos, "
            "e.direccion, e.telefono, e.fecha_nacimiento, ts.sangre "
            "FROM estudiantes as e "
            "INNER JOIN tipos_sangre as ts ON e.id_tipo_sangre = ts.id_tipo_sangre;";

        const char* c = consulta.c_str();
        q_estado = mysql_query(cn.getConector(), c);

        if (!q_estado) {
            resultado = mysql_store_result(cn.getConector());
            if (!resultado) {
                cout << "❌ Error al obtener resultados." << endl;
                cn.cerrar_conexion();
                return false;
            }

            int num_filas = mysql_num_rows(resultado);
            if (num_filas == 0) {
                cout << "📋 No hay estudiantes registrados." << endl;
            }
            else {
                cout << "📋 Total de estudiantes: " << num_filas << endl << endl;
                cout << "ID\t| Código\t| Nombres\t| Apellidos\t| Dirección\t| Teléfono\t| Fecha\t\t| Sangre" << endl;
                cout << "───────────────────────────────────────────────────────────────────────────────────────────────" << endl;

                while (fila = mysql_fetch_row(resultado)) {
                    cout << fila[0] << "\t| " << fila[1] << "\t| " << fila[2] << "\t| "
                        << fila[3] << "\t| " << fila[4] << "\t| " << fila[5] << "\t| "
                        << fila[6] << "\t| " << fila[7] << endl;
                }
            }
            mysql_free_result(resultado);
        }
        else {
            cout << "❌ Error: Consulta fallida. " << mysql_error(cn.getConector()) << endl;
            cn.cerrar_conexion();
            return false;
        }

        cn.cerrar_conexion();
        return true;
    }

    /**
     * ACTUALIZAR: Modifica un estudiante existente
     * Valida todos los campos antes de actualizar
     */
    bool actualizar() {
        // Validar todos los campos antes de actualizar
        if (!Validador::validarIdEstudiante(id_estudiante)) {
            cout << "❌ Actualización fallida: ID inválido." << endl;
            return false;
        }

        if (!Validador::validarCodigo(codigo)) {
            cout << "❌ Actualización fallida: código inválido." << endl;
            return false;
        }

        // Verificar que el código no exista en otro registro
        if (Validador::codigoExistente(codigo, id_estudiante)) {
            cout << "❌ Actualización fallida: el código ya existe en otro registro." << endl;
            return false;
        }

        if (!Validador::validarNombres(nombres)) {
            cout << "❌ Actualización fallida: nombres inválidos." << endl;
            return false;
        }

        if (!Validador::validarApellidos(apellidos)) {
            cout << "❌ Actualización fallida: apellidos inválidos." << endl;
            return false;
        }

        if (!Validador::validarDireccion(direccion)) {
            cout << "❌ Actualización fallida: dirección inválida." << endl;
            return false;
        }

        if (!Validador::validarTelefono(telefono)) {
            cout << "❌ Actualización fallida: teléfono inválido." << endl;
            return false;
        }

        if (!Validador::validarFecha(fecha_nacimiento)) {
            cout << "❌ Actualización fallida: fecha de nacimiento inválida." << endl;
            return false;
        }

        if (!Validador::validarTipoSangre(id_tipo_sangre)) {
            cout << "❌ Actualización fallida: tipo de sangre inválido." << endl;
            return false;
        }

        // Si todas las validaciones pasaron, proceder con la actualización
        int q_estado = 0;
        ConexionBD cn = ConexionBD();
        cn.abrir_conexion();

        if (!cn.getConector()) {
            cout << "❌ Error: No se pudo conectar a la base de datos." << endl;
            return false;
        }

        string t = std::to_string(telefono);
        string id_ts = std::to_string(id_tipo_sangre);
        string id_e = std::to_string(id_estudiante);

        // Usar escaped SQL para prevenir inyección
        string codigoEscapado = Validador::escaparSQL(codigo);
        string nombresEscapado = Validador::escaparSQL(nombres);
        string apellidosEscapado = Validador::escaparSQL(apellidos);
        string direccionEscapada = Validador::escaparSQL(direccion);
        string fechaEscapada = Validador::escaparSQL(fecha_nacimiento);

        string consulta = "UPDATE estudiantes SET "
            "codigo = '" + codigoEscapado + "', "
            "nombres = '" + nombresEscapado + "', "
            "apellidos = '" + apellidosEscapado + "', "
            "direccion = '" + direccionEscapada + "', "
            "telefono = " + t + ", "
            "fecha_nacimiento = '" + fechaEscapada + "', "
            "id_tipo_sangre = " + id_ts + " "
            "WHERE id_estudiante = " + id_e + ";";

        const char* c = consulta.c_str();
        q_estado = mysql_query(cn.getConector(), c);

        if (!q_estado) {
            cout << "✅ Modificación de Datos Exitoso..." << endl;
            cn.cerrar_conexion();
            return true;
        }
        else {
            cout << "❌ Error: Consulta fallida. " << mysql_error(cn.getConector()) << endl;
            cn.cerrar_conexion();
            return false;
        }
    }

    /**
     * BORRAR: Elimina un estudiante de la BD
     * Valida el ID antes de borrar
     */
    bool borrar() {
        // Validar ID antes de eliminar
        if (!Validador::validarIdEstudiante(id_estudiante)) {
            cout << "❌ Eliminación fallida: ID inválido." << endl;
            return false;
        }

        // Verificar que el estudiante existe
        ConexionBD cnVerif = ConexionBD();
        cnVerif.abrir_conexion();

        if (!cnVerif.getConector()) {
            cout << "❌ Error: No se pudo conectar a la base de datos." << endl;
            return false;
        }

        string consultaVerif = "SELECT COUNT(*) as cantidad FROM estudiantes WHERE id_estudiante = "
            + std::to_string(id_estudiante) + ";";
        const char* cVerif = consultaVerif.c_str();
        int q_estadoVerif = mysql_query(cnVerif.getConector(), cVerif);

        bool existe = false;
        if (!q_estadoVerif) {
            MYSQL_RES* resultadoVerif = mysql_store_result(cnVerif.getConector());
            MYSQL_ROW filaVerif = mysql_fetch_row(resultadoVerif);
            if (filaVerif && atoi(filaVerif[0]) > 0) {
                existe = true;
            }
            mysql_free_result(resultadoVerif);
        }

        cnVerif.cerrar_conexion();

        if (!existe) {
            cout << "❌ Error: No existe estudiante con ID " << id_estudiante << endl;
            return false;
        }

        // Proceder con la eliminación
        int q_estado = 0;
        ConexionBD cn = ConexionBD();
        cn.abrir_conexion();

        if (!cn.getConector()) {
            cout << "❌ Error: No se pudo conectar a la base de datos." << endl;
            return false;
        }

        string id_e = std::to_string(id_estudiante);
        string consulta = "DELETE FROM estudiantes WHERE id_estudiante = " + id_e + ";";

        const char* c = consulta.c_str();
        q_estado = mysql_query(cn.getConector(), c);

        if (!q_estado) {
            cout << "✅ Eliminación de Datos Exitoso..." << endl;
            cn.cerrar_conexion();
            return true;
        }
        else {
            cout << "❌ Error: Consulta fallida. " << mysql_error(cn.getConector()) << endl;
            cn.cerrar_conexion();
            return false;
        }
    }

    /**
     * Método auxiliar para validar todos los campos
     */
    bool validarTodos() {
        return Validador::validarCodigo(codigo) &&
            !Validador::codigoExistente(codigo, id_estudiante) &&
            Validador::validarNombres(nombres) &&
            Validador::validarApellidos(apellidos) &&
            Validador::validarDireccion(direccion) &&
            Validador::validarTelefono(telefono) &&
            Validador::validarFecha(fecha_nacimiento) &&
            Validador::validarTipoSangre(id_tipo_sangre);
    }

    /**
     * Muestra todos los datos del estudiante
     */
    void mostrarDatos() const {
        cout << "\n════════════════════════════════════════" << endl;
        cout << "ID: " << id_estudiante << endl;
        cout << "Código: " << codigo << endl;
        cout << "Nombres: " << nombres << endl;
        cout << "Apellidos: " << apellidos << endl;
        cout << "Dirección: " << direccion << endl;
        cout << "Teléfono: " << telefono << endl;
        cout << "Fecha Nacimiento: " << fecha_nacimiento << endl;
        cout << "ID Tipo Sangre: " << id_tipo_sangre << endl;
        cout << "════════════════════════════════════════\n" << endl;
    }
};