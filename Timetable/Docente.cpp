/*
 * Docente.cpp
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#include "stdafx.h"
#include "Docente.h"
#include <numeric>

Docente::Docente() {
	this->prefHorarios = new bool*[5];
	for (int i = 0; i<5; i++) {
		this->prefHorarios[i] = new bool[15];
	}
}

Docente::Docente(int idDocente,	string codigo, string nome,	string nomeCompleto, int creditacaoEsperada) {
    this->idDocente = idDocente;
    this->codigo = codigo;
    this->nome = nome;
    this->nomeCompleto = nomeCompleto;
    this->creditacaoEsperada = creditacaoEsperada;
	this->prefHorarios = new bool*[5];
	for (int i = 0; i<5; i++) {
		this->prefHorarios[i] = new bool[15];
	}
}

Docente::~Docente() {
    
}

istream& operator >> (istream& is, Docente& d) {
    is >> d.idDocente >> d.codigo >> d.nome >> d.creditacaoEsperada;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 15; j++) {
			string s;
			is >> s;
			if (s == "true")
				d.prefHorarios[i][j] = true;
			else
				d.prefHorarios[i][j] = false;
		}
	}
    return is;
}

void Docente::toString() {
    cout << "ID: " << idDocente <<" Codigo: "<< codigo <<" Nome: "<< nome << " Creditcao Esperada: "<< creditacaoEsperada<<endl;
	for (int i = 2;i < 7;i++) {
		for (int j = 8; j < 23; j++) {
			cout << getPrefHorarios(i, j) << " ";
		}
		cout << endl;
	}
	cout << endl;
}
