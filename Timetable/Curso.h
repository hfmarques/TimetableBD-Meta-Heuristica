/*
* Curso.h
*
*  Created on: 18 de fev de 2016
*      Author: Héber Marques
*/

#ifndef CURSO_H_
#define CURSO_H_

#include <vector>
#include "Turma.h"

using namespace std;
class Curso {
public:
	Curso();
	virtual ~Curso();

	void toString();

	inline vector<vector<Turma*>* > *getGrades() { return grade; }
	inline int getQtdPeriodos() const { return qtdPeriodos; }

	inline void setGrades(int i, int j, Turma *disc) { grade->at(i)->at(j) = disc; }
	inline void setGrades(vector<vector<Turma*>* > *grade) { this->grade = grade; }

	inline const string& getTurno() const {
		return turno;
	}

	friend istream& operator >> (istream& is, Curso& c);
private:
	string codigo;
	string nome;
	string turno;
	int qtdPeriodos;

	vector<vector<Turma*>* > *grade; // grade do curso
};
#endif /* CURSO_H_ */