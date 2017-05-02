/*
 * Ant.h
 *
 *  Created on: 18 de fev de 2016
 *      Author: Héber Marques
 */

#ifndef ANT_H_
#define ANT_H_

#include "Turma.h"
#include "Docente.h"
#include "Solucoes.h"

#include <vector>


#define E 0.1 // tolerancia minima de feromonios
#define NUMERO_HORAS_TOTAL 70

class Ant {
public:
    Ant();
    virtual ~Ant();

   inline static double getFeromoniosHorario1(int i, int j) {
        return feromoniosHorarios1.at(i).at(j);
    }

    static void setFeromoniosHorario1(double feromonios, int i, int j) {
        feromoniosHorarios1.at(i).at(j) = feromonios;
    }

	inline static double getFeromoniosHorario2(int i, int j) {
		return feromoniosHorarios2.at(i).at(j);
	}

	static void setFeromoniosHorario2(double feromonios, int i, int j) {
		feromoniosHorarios2.at(i).at(j) = feromonios;
	}

	inline void setSolucao(Solucoes *solucao) { this->solucao = solucao; }
	inline Solucoes *getSolucao() { return solucao; }

    static void inicializa(int numeroTurmas);

	static void resetaFeromonios(int);


private:
    static std::vector<std::vector<double> >feromoniosHorarios1; //turma x horario do dia 1
	static std::vector<std::vector<double> >feromoniosHorarios2; //turma x horario do dia 2
	Solucoes *solucao;
};

#endif /* ANT_H_ */
