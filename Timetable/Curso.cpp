#include "stdafx.h"
#include "Curso.h"


Curso::Curso(){
}


Curso::~Curso(){
}

istream& operator >> (istream& is, Curso& c) {
	is >> c.codigo >> c.qtdPeriodos >> c.turno;
	return is;
}

void Curso::toString() {
	cout << "Codigo: " << codigo << " qtdPeriodos: " << qtdPeriodos << " turno: " << turno << endl;
	for (int i = 0;i < grade->size(); i++) {
		for (int j = 0; j < grade->at(i)->size(); j++) {
			cout << grade->at(i)->at(j)->getDisciplina()->getCodigo() << " " << grade->at(i)->at(j)->getCodigo() << " ";
		}
		cout << endl;
	}
	cout << endl;
}