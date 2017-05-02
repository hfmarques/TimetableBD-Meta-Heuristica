#ifndef HHS_H
#define HHS_H

#include "MetaHeuristicas.h"
#include "Turma.h"

#include <vector>

using namespace std;
class HHS : public MetaHeuristicas {
public:
    HHS();
    virtual ~HHS();

    void executa();
protected:

private:

	int selecionaDias(int*, Solucoes*, int);
	int selecionaHoras(int*, Solucoes*, int);

};

#endif // HHS_H
