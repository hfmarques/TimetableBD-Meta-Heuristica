#ifndef METAHEURISTICAS_H
#define METAHEURISTICAS_H

#include "Turma.h"
#include "Solucoes.h"
#include "Docente.h"
#include "Disciplina.h"
#include "Curso.h"
#include "Timer.hpp"

#include <vector>
#include <algorithm>
#include <memory>
#include <iostream>

#define NUM_SOLUCOES 20
#define NUMERO_HORAS_TOTAL 70
#define NUMERO_HORAS_DIA 14
#define HORA_MISTA 9
#define NUMERO_DIAS 5
#define HORA_INICIAL 8
#define HORAS_NOTURNAS 4
#define REPETICAO_BUSCA_LOCAL 50

using namespace std;
using namespace avr;

class MetaHeuristicas {
public:
    MetaHeuristicas();
	MetaHeuristicas(int);
    virtual ~MetaHeuristicas();

    inline static vector<Solucoes*> *getPopulacaoSolucoes() {
        return populacaoSolucoes;
    }
    void ordenaSolucao();
	bool validaSolucao(int dia1, int dia2, int horario1, int horario2);
	double classificaHC(vector<Turma*> *turma);
	double classificaSC(vector<Turma*> *turma);
	double classificaElementoHC(Turma *turma);
	double classificaElementoSC(Turma *turma);

	void buscaLocal(Solucoes * solucao);

    virtual void executa() = 0;

	void inicializaDados();
	void escreveSolucao();
	void imprimeSolucao();

	inline static vector<Docente*> *getDocentes() { return &docente; }
	inline static vector<Disciplina*> *getDisciplinas() { return &disciplinas; }
	inline static vector<Turma*> *getTurmas() { return &turmas; }
	inline static vector<Curso*> *getCursos() { return &cursos; }
	
	inline void setY(double y) { Y = y; }
	inline void setP(double y) { P = y; }
	inline void setW(double y) { W = y; }
	inline void setIteradorSolucoes(int i) { interadorSolucoes = i; }

	inline double getY() { return Y; }
	inline double getP() { return P; }
	inline double getW() { return W; }
	inline int getIteradorSolucoes() { return interadorSolucoes; }
protected:
	static int NUM_DOCENTE;
    static int NUM_DISCIPLINA;
    static int NUM_TURMA;
	static int NUM_CURSO;
    static vector<Solucoes*> *populacaoSolucoes;
	static vector<Docente*> docente;
	static vector<Disciplina*> disciplinas;
	static vector<Turma*> turmas;
	static vector<Curso*> cursos;

	int interadorSolucoes;
	
	double Y; // fator de penalidade
	double P; // fator de evaporação dos feromonios
	double W; // variavel omega que define o quanto será a normalização dos feromonios

	Timer timer;
private:
    static bool ordena(Solucoes *s1, Solucoes *s2);
    
};

#endif // METAHEURISTICAS_H
