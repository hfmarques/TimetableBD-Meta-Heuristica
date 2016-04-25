#ifndef GERACAOGRADES_H
#define GERACAOGRADES_H

#include "Docente.h"
#include "Disciplina.h"
#include "Turma.h"
#include "ACO.h"

#include<iostream>
#include<vector>

using namespace std;
class GeracaoGrades {
public:
    GeracaoGrades();
    virtual ~GeracaoGrades();

    void inicializaDados();
    void geraSolucao();
    void escreveSolucao();

protected:

private:
    vector<Docente*> docente;
    vector<Disciplina*> disciplinas;
    vector<Turma*> turmas;
};

#endif // GERACAOGRADES_H
