#ifndef METAHEURISTICAS_H
#define METAHEURISTICAS_H

#include "Turma.h"
#include "Solucoes.h"

#include <vector>
#include <algorithm>

#define NUM_SOLUCOES 20

using namespace std;

class MetaHeuristicas {
public:
    MetaHeuristicas(int numProf, int numDisc, int numTurmas);
    virtual ~MetaHeuristicas();

    inline vector<Solucoes>* getPopulacaoSolucoes() {
        return &populacaoSolucoes;
    }
    void ordenaSolucao();

    virtual void executa(vector<Turma*>) = 0;
protected:

private:
    static bool ordena(const Solucoes &s1,const Solucoes &s2);

    vector<Solucoes> populacaoSolucoes;
    const int NUM_DOCENTE;
    const int NUM_DISCIPLINA;
    const int NUM_TURMA;
};

#endif // METAHEURISTICAS_H
