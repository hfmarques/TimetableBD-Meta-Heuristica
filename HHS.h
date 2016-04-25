#ifndef HHS_H
#define HHS_H

#include "MetaHeuristicas.h"
#include "Turma.h"

#include <vector>

using namespace std;
class HHS : public MetaHeuristicas {
public:
    HHS(int, int, int);
    virtual ~HHS();

    executa(vector<Turma>);
protected:

private:
};

#endif // HHS_H
