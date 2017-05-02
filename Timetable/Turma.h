/*
 * Turma.h
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#ifndef TURMA_H_
#define TURMA_H_

#include "Disciplina.h"
#include "Docente.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Turma {
public:
    Turma();
	Turma(int idTurma, string codigo, string turno, Disciplina *disciplina, Docente *docente, bool horarioFixo, int dia1, int horario1, int dia2, int horario2);
    virtual ~Turma();

    inline const string& getCodigo() const {
        return codigo;
    }

    inline void setCodigo(const string& codigo) {
        this->codigo = codigo;
    }

    inline int getId() const {
        return idTurma;
    }

    inline void setId(int id) {
        this->idTurma = id;
    }

    inline const string& getTurno() const {
        return turno;
    }

    inline void setTurno(const string& turno) {
        this->turno = turno;
    }

    Disciplina *getDisciplina() {
        return disciplina;
    }

    inline void setDisciplina(Disciplina *disciplina) {
        this->disciplina = disciplina;
    }

    inline Docente* getDocente() const {
        return docente;
    }

    inline void setDocente(Docente* p) {
        this->docente = p;
    }

    inline int getDia1() const {
        return dia1;
    }

    inline void setDia1(int d) {
        dia1 = d;
    }

    inline int getDia2() const {
        return dia2;
    }

    inline void setDia2(int d) {
        dia2 = d;
    }

    inline int getHorario1() const {
        return horario1;
    }

    inline void setHorario1(int h) {
        horario1 = h;
    }

    inline int getHorario2() const {
        return horario2;
    }

    inline void setHorario2(int h) {
        horario2 = h;
    }
	inline bool getHorarioFixo() const
	{ 
		return horarioFixo; 
	}
    void toString() const;

    friend istream& operator >> (istream& is, Turma& t);

private:
	int idTurma;
	string codigo;
	string turno;
	Disciplina *disciplina;
	Docente *docente;
	int dia1;
	int dia2;
	int horario1;
	int horario2;

	bool horarioFixo;
	vector<bool> horariosDisponiveis;
};

#endif /* TURMA_H_ */
