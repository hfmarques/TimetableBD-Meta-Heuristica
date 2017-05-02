/*
 * Disciplina.cpp
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#include "stdafx.h"
#include "Disciplina.h"

Disciplina::Disciplina() {
}

Disciplina::Disciplina(int idDisciplina, string codigo, int creditos, string nome, string perfil) {
    this->idDisciplina = idDisciplina;
    this->codigo = codigo;
    this->creditos = creditos;
    this->nome = nome;
    this->perfil = perfil;
}

Disciplina::~Disciplina() {
    // TODO Auto-generated destructor stub
}

istream& operator >> (istream& is, Disciplina& d) {
    is >> d.idDisciplina >> d.codigo >> d.creditos >> d.perfil;
    return is;
}

void Disciplina::toString() {
    cout << "ID: " << idDisciplina <<" Codigo: "<< codigo << " Creditos: "<< creditos<< " Perfil " << perfil << endl;
}
