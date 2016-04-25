/*
 * Docente.cpp
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#include "Docente.h"
#include <numeric>

Docente::Docente() {
}

Docente::Docente(int idDocente,	string codigo, string nome,	string nomeCompleto, int creditacaoEsperada) {
    this->idDocente = idDocente;
    this->codigo = codigo;
    this->nome = nome;
    this->nomeCompleto = nomeCompleto;
    this->creditacaoEsperada = creditacaoEsperada;
}

Docente::~Docente() {
    prefHorarios = new bool*[5];
    for(int i=0; i<5; i++) {
        prefHorarios[i] = new bool[15];
    }
}

istream& operator >> (istream& is, Docente& d) {
    is >> d.idDocente >> d.codigo >> d.nomeCompleto >> d.creditacaoEsperada;
    return is;
}

void Docente::toString() {
    cout << "ID: " << idDocente <<" Codigo: "<< codigo <<" Nome: "<< nomeCompleto << " Creditcao Esperada: "<< creditacaoEsperada<<endl;
}
