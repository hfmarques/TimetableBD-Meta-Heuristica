/*
 * Turma.cpp
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#include "Turma.h"

Turma::Turma() {
    this->idTurma = 0;
    this->codigo = "";
    this->turno = "";
    this->disciplina = NULL;
}

Turma::~Turma() {
    // TODO Auto-generated destructor stub
}

istream& operator >> (istream& is, Turma& t) {
    is >> t.idTurma >> t.codigo >> t.turno;
    return is;
}

void Turma::toString() {
    cout << "ID: " << idTurma <<" Codigo: "<< codigo <<" Turno: "<< turno << " Disciplina: "<< disciplina->getCodigo() <<endl;
}
