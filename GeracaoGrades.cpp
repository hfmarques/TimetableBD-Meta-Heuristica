#include "GeracaoGrades.h"

#include"ACO.h"

#include<fstream>
#include<iostream>
#include<limits>
#include "stdlib.h"

GeracaoGrades::GeracaoGrades() {
}

GeracaoGrades::~GeracaoGrades() {
    //dtor
}

void GeracaoGrades::inicializaDados() {
    ifstream file("dados.tbd");

    if(file.fail()) {
        cerr << "ERRO: Arquivo dados.tdb nao encontrado" << endl;
        exit(EXIT_FAILURE);
    }

    int numeroTurmas;
    int numeroDisciplinas;
    int numeroDocentes;

    file >> numeroDocentes;
    file >> numeroDisciplinas;
    file >> numeroTurmas;

    cout << "numeroDocentes: " << numeroDocentes << " numeroDisciplinas: " << numeroDisciplinas << " numeroTurmas: " << numeroTurmas << endl;

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for(int i=0; i < numeroDocentes; i++) {
        Docente *d = new Docente();
        file >> *d;
        docente.push_back(d);
//        d->toString();
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for(int i=0; i < numeroDisciplinas; i++) {
        Disciplina *d = new Disciplina();
        file >> *d;
        disciplinas.push_back(d);
//        d->toString();
    }

    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for(int i=0; i < numeroTurmas; i++) {
        Turma *t = new Turma();
        int idDisc;
        file >> *t;
        file >> idDisc;
        for(Disciplina *d: disciplinas) {
            if(d->getIdDisciplina() == idDisc) {
                t->setDisciplina(d);
                break;
            }
        }
//        t->toString();
    }

    file.close();
}

void GeracaoGrades::geraSolucao() {
}
