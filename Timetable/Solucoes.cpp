#include "stdafx.h"
#include "Solucoes.h"

#include <limits>

Solucoes::Solucoes() {
    turmas  = new vector<Turma*>();
    hardConstraints = numeric_limits<double>::max();
    softConstraints = numeric_limits<double>::max();
}

Solucoes::Solucoes(int numTurmas) {
    turmas  = new vector<Turma*>(numTurmas);
    hardConstraints = numeric_limits<double>::max();
    softConstraints = numeric_limits<double>::max();
}

Solucoes::~Solucoes() {
	for (int i = turmas->size()-1; i >= 0; i--) {
		auto& t = turmas->at(i);
		delete t;
		turmas->pop_back();
	}
	turmas->clear();
	delete turmas;
}
