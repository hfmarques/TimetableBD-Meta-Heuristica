/*
 * Ant.cpp
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#include "stdafx.h"
#include "Ant.h"

#include <limits>

vector<vector<double> > Ant::feromoniosHorarios1;
vector<vector<double> > Ant::feromoniosHorarios2;

Ant::Ant() {
	solucao = new Solucoes();
}

Ant::~Ant() {
    // TODO Auto-generated destructor stub
}

void Ant::inicializa(int numeroTurmas){
    feromoniosHorarios1.clear();

	for (auto i = 0; i < numeroTurmas; i++) {
		feromoniosHorarios1.push_back(vector < double >(NUMERO_HORAS_TOTAL));
		for (auto j = 0; j < NUMERO_HORAS_TOTAL; j++) {
			feromoniosHorarios1.at(i).at(j) = E;
		}
	}

	feromoniosHorarios2.clear();

	for (auto i = 0; i < numeroTurmas; i++){
		feromoniosHorarios2.push_back(vector<double>(NUMERO_HORAS_TOTAL));
		for (auto j = 0; j < NUMERO_HORAS_TOTAL; j++) {
			feromoniosHorarios2.at(i).at(j) = E;
		}
	}
}

void Ant::resetaFeromonios(int numeroTurmas) {
	for (auto i = 0; i < numeroTurmas; i++) {
		auto k = feromoniosHorarios1.at(i);
		k.clear();		
	}	
	feromoniosHorarios1.clear();
	for (auto i = 0; i < numeroTurmas; i++) {
		feromoniosHorarios1.push_back(vector<double>());
		for (auto j = 0; j < NUMERO_HORAS_TOTAL; j++){
			feromoniosHorarios1.at(i).push_back(E);
		}
	}

	for (auto i = 0; i < numeroTurmas; i++) {
		auto k = feromoniosHorarios2.at(i);
		k.clear();
	}
	feromoniosHorarios2.clear();
	for (auto i = 0; i < numeroTurmas; i++) {
		feromoniosHorarios2.push_back(vector<double>());
		for (auto j = 0; j < NUMERO_HORAS_TOTAL; j++) {
			feromoniosHorarios2.at(i).push_back(E);
		}
	}
		
}
