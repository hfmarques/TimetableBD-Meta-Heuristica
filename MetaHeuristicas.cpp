#include "MetaHeuristicas.h"

MetaHeuristicas::MetaHeuristicas(int numProf, int numDisc, int numTurmas):NUM_DOCENTE(numProf), NUM_DISCIPLINA(numDisc), NUM_TURMA(numTurmas) {
    populacaoSolucoes = vector<Solucoes>(NUM_SOLUCOES);
    for(int i=0; i<NUM_SOLUCOES; i++) {
        populacaoSolucoes.at(i) = Solucoes(NUM_TURMA);
    }
}

MetaHeuristicas::~MetaHeuristicas() {
    //dtor
}

bool MetaHeuristicas::ordena(const Solucoes &s1,const Solucoes &s2) {
    if(s1.getHardConstraints() == s2.getHardConstraints()) {
        return (s1.getSoftConstraints() < s2.getSoftConstraints());
    }
    return (s1.getHardConstraints() < s2.getHardConstraints());
}

void MetaHeuristicas::ordenaSolucao() {
    sort(populacaoSolucoes.begin(), populacaoSolucoes.end(), ordena);
}
