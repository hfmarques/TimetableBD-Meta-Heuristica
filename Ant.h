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

#include <vector>


#define E 1 // tolerancia minima de feromonios
#define NUMERO_HORAS 15

class Ant {
public:
    Ant();
    virtual ~Ant();

    inline static double getFeromoniosT(int i, int j) {
        return feromoniosTurmas.at(i).at(j);
    }

    static void setFeromoniosT(double feromonios, int i, int j) {
        feromoniosTurmas.at(i).at(j) = feromonios;
    }

    inline static double getFeromoniosHorario(int i, int j) {
        return feromoniosTurmas.at(i).at(j);
    }

    static void setFeromoniosHorario(double feromonios, int i, int j) {
        feromoniosTurmas.at(i).at(j) = feromonios;
    }

    static void inicializa(vector<Docente*> professor, vector<Turma*> turmas);
    static void inicializa(vector<Turma*> turmas);

private:
    static std::vector<std::vector<double> >feromoniosTurmas; // professor x turma
    static std::vector<std::vector<double> >feromoniosHorarios; //turma x horario
};

#endif /* ANT_H_ */
