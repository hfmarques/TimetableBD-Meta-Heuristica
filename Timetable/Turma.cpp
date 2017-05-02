/*
 * Turma.cpp
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#include "stdafx.h"
#include "Turma.h"

Turma::Turma() {
    this->idTurma = 0;
    this->codigo = "";
    this->turno = "";
    this->disciplina = NULL;
	this->docente = NULL;
}

Turma::Turma(int idTurma, string codigo, string turno, Disciplina *disciplina, Docente *docente, bool horarioFixo, int dia1, int horario1, int dia2, int horario2) {
	this->idTurma = idTurma;
	this->codigo = codigo;
	this->turno = turno;
	this->disciplina = disciplina;
	this->docente = docente;
	this->horarioFixo = horarioFixo;
	this->dia1 = dia1;
	this->horario1 = horario1;
	this->dia2 = dia2;
	this->horario2 = horario2;
}

Turma::~Turma() {
	disciplina = nullptr;
	docente = nullptr;
	horariosDisponiveis.clear();
}

istream& operator >> (istream& is, Turma& t) {
    is >> t.idTurma >> t.codigo >> t.turno;
	string s;
	is >> s;
	if (s == "true")
		t.horarioFixo = true;
	else
		t.horarioFixo = false;
	if (t.horarioFixo == true) {
		is >> t.dia1 >> t.horario1 >> t.dia2 >> t.horario2;
		t.dia1 = t.dia1 - 2;
		if(t.dia2 != -1)
			t.dia2 = t.dia2 - 2;
		t.horario1 = t.horario1 - 8;
		if(t.horario2 != -1)
			t.horario2 = t.horario2 - 8;
	}
    return is;
}

void Turma::toString() const
{
	cout << "ID: " << idTurma << " Codigo: " << codigo << " Turno: " << turno << " Disciplina: " << disciplina->getCodigo() << " docente: " << docente->getNome() << " Horario Fixo " << horarioFixo;
	if (horarioFixo)
		cout << " Dia 1: " << dia1 + 2 << " as " << horario1 + 8 << "h Dia 2: " << dia2 + 2 << " as " << horario2 + 8 << "h";
	cout << endl;
}
