#pragma once
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::endl;

class Persona {
    // atributos protegidos para que la clase hija pueda acceder
protected:
    string nombres;
    string apellidos;
    string direccion;
    string fecha_nacimiento;
    int telefono = 0;
    int id_tipo_sangre = 0;

    // constructor
public:
    Persona() {}

    Persona(string nom, string ape, string dir, int tel, string fn, int id_ts) {
        nombres = nom;
        apellidos = ape;
        direccion = dir;
        telefono = tel;
        fecha_nacimiento = fn;
        id_tipo_sangre = id_ts;
    }

    // Getters (sin validación - son solo accesores)
    virtual string getNombres() const { return nombres; }
    virtual string getApellidos() const { return apellidos; }
    virtual string getDireccion() const { return direccion; }
    virtual int getTelefono() const { return telefono; }
    virtual string getFecha_nacimiento() const { return fecha_nacimiento; }
    virtual int getId_tipo_sangre() const { return id_tipo_sangre; }

    // Setters básicos (sin validación)
    // NOTA: Los setters con validación están en la clase Estudiante
    virtual void setNombres(string nom) { nombres = nom; }
    virtual void setApellidos(string ape) { apellidos = ape; }
    virtual void setDireccion(string dir) { direccion = dir; }
    virtual void setTelefono(int tel) { telefono = tel; }
    virtual void setFecha_nacimiento(string fn) { fecha_nacimiento = fn; }
    virtual void setId_tipo_sangre(int id_ts) { id_tipo_sangre = id_ts; }

    // Destructor virtual
    virtual ~Persona() {}
};