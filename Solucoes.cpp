#include "Solucoes.h"

#include <limits>

Solucoes::Solucoes() {
    turmas  = vector<Turma>();
    hardConstraints = numeric_limits<double>::max();
    softConstraints = numeric_limits<double>::max();
}

Solucoes::Solucoes(int numTurmas) {
    turmas  = vector<Turma>(numTurmas);
    hardConstraints = numeric_limits<double>::max();
    softConstraints = numeric_limits<double>::max();
}

Solucoes::~Solucoes() {

}
