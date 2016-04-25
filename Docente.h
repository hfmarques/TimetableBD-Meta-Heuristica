/*
 * Docente.h
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#ifndef DOCENTE_H_
#define DOCENTE_H_

#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Docente {
public:
    Docente();
    Docente(int idDocente,	string codigo, string nome,	string nomeCompleto, int creditacaoEsperada);
    virtual ~Docente();

    inline const string& getCodigo() const {
        return codigo;
    }

    inline void setCodigo(const string& codigo) {
        this->codigo = codigo;
    }

    inline int getCreditacaoEsperada() const {
        return creditacaoEsperada;
    }

    inline void setCreditacaoEsperada(int creditacaoEsperada) {
        this->creditacaoEsperada = creditacaoEsperada;
    }

    inline int getIdDocente() const {
        return idDocente;
    }

    inline void setIdDocente(int idDocente) {
        this->idDocente = idDocente;
    }

    inline const string& getNome() const {
        return nome;
    }

    inline void setNome(const string& nome) {
        this->nome = nome;
    }

    inline const string& getNomeCompleto() const {
        return nomeCompleto;
    }

    inline void setNomeCompleto(const string& nomeCompleto) {
        this->nomeCompleto = nomeCompleto;
    }

    inline bool getPrefHorarios(int dia, int hora) const {
        if((dia >= 2 && dia <= 6) && (hora >= 8 && hora <= 22)) {
            return prefHorarios[dia-2][hora-8];
        }
    }

    inline void setPrefHorarios(int dia, int hora, bool valor) {
        if((dia >= 2 && dia <= 6) && (hora >= 8 && hora <= 22)) {
            prefHorarios[dia-2][hora-8] = valor;
        }
    }

    void toString();

    friend istream& operator >> (istream& is, Docente& d);
private:
    int idDocente;
    string codigo;
    string nome;
    string nomeCompleto;
    int creditacaoEsperada;
    bool **prefHorarios; // preferencia de horarios

};

#endif /* DOCENTE_H_ */
