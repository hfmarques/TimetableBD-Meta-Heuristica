/*
 * Disciplina.h
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#ifndef DISCIPLINA_H_
#define DISCIPLINA_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Disciplina {
public:
    Disciplina();
    Disciplina(int idDisciplina, string codigo, int creditos, string nome, string perfil);
    virtual ~Disciplina();

    inline const string& getCodigo() const {
        return codigo;
    }

    inline void setCodigo(const string& codigo) {
        this->codigo = codigo;
    }

    inline int getCreditos() const {
        return creditos;
    }

    inline void setCreditos(int creditos) {
        this->creditos = creditos;
    }

    inline int getIdDisciplina() const {
        return idDisciplina;
    }

    inline void setIdDisciplina(int idDisciplina) {
        this->idDisciplina = idDisciplina;
    }

    inline const string& getNome() const {
        return nome;
    }

    inline void setNome(const string& nome) {
        this->nome = nome;
    }

    inline const string& getPerfil() const {
        return perfil;
    }

    inline void setPerfil(const string& perfil) {
        this->perfil = perfil;
    }

    void toString();

    friend istream& operator >> (istream& is, Disciplina& d);

private:
    int idDisciplina;
    string codigo;
    int creditos;
    string nome;
    string perfil;
};

#endif /* DISCIPLINA_H_ */
