/*
 * Ant.cpp
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#include "Ant.h"

#include <limits>

vector<std::vector<double> > Ant::feromoniosTurmas;
vector<std::vector<double> > Ant::feromoniosHorarios;

Ant::Ant() {
}

Ant::~Ant() {
    // TODO Auto-generated destructor stub
}

void Ant::inicializa(vector<Docente*> professor, vector<Turma*> turmas) {
//    feromoniosTurmas.clear();
//    feromoniosHorarios.clear();
//    melhorSolucao.clear();
//
//    for(int i=0;i<professor.size();i++){
//        feromoniosTurmas.push_back(std::vector<double>(turmas.size()));
//    }
//    for(int i=0;i<turmas.size();i++){
//        melhorSolucao.push_back(std::vector<int>(TAMANHO_SOLUCAO));
//    }
//
//    for(int i=0;i<professor.size();i++){
//        for(int j=0; j<turmas.size();j++){
//            Ant::feromoniosTurmas.at(i).at(j) = 0.3 * professor.at(i)->getPrefDisciplina()->at(turmas.at(j)->getDisciplina()->getIdDisciplina());
//        }
//    }
}

void Ant::inicializa(vector<Turma*> turmas) {
    feromoniosTurmas.clear();
    feromoniosHorarios.clear();

    for(unsigned int i=0; i<turmas.size(); i++)
        feromoniosHorarios.push_back(std::vector<double>(NUMERO_HORAS));
}
