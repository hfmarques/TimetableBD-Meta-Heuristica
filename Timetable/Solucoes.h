#ifndef SOLUCOES_H
#define SOLUCOES_H

#include "Turma.h"

#include<vector>

using namespace std;
class Solucoes {
public:
    Solucoes();
    Solucoes(int numTurmas);
    virtual ~Solucoes();
    inline vector<Turma*> *getTurmas() {
        return turmas;
    }
    inline double getHardConstraints() const {
        return hardConstraints;
    }
    inline double getSoftConstraints() const {
        return softConstraints;
    }

    inline void setHardConstraints(double hc) {
        hardConstraints = hc;
    }
    inline void setSoftConstraints(double sc) {
        softConstraints = sc;
    }
private:
    vector<Turma*> *turmas;
    double hardConstraints;
    double softConstraints;
};

#endif // SOLUCOES_H
