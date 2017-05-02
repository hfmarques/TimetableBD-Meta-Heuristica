#include "Docente.h"
#include "Disciplina.h"
#include "Turma.h"
#include "Curso.h"
#include "ACO.h"
#include "HHS.h"

#include<iostream>
#include<vector>

using namespace std;

class GeracaoGrades : MetaHeuristicas{
public:
    GeracaoGrades();
    virtual ~GeracaoGrades();

    inline static vector<Docente*> *getDocentes() { return &docente; }
	inline static vector<Disciplina*> *getDisciplinas() { return &disciplinas; }
	inline static vector<Turma*> *getTurmas() { return &turmas; }
	inline static vector<Curso*> *getCursos() { return &cursos; }

private:
    static vector<Docente*> docente;
    static vector<Disciplina*> disciplinas;
    static vector<Turma*> turmas;
	static vector<Curso*> cursos;
	ACO *aco;
	HHS *hhs;
};