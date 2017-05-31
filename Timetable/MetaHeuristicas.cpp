#include "stdafx.h"
#include "MetaHeuristicas.h"

#include<fstream>
#include<iostream>
#include<limits>
#include <stdio.h>
#include <stdlib.h>

vector<Solucoes*> *MetaHeuristicas::populacaoSolucoes = NULL;
int MetaHeuristicas::NUM_CURSO = 0;
int MetaHeuristicas::NUM_TURMA = 0;
int MetaHeuristicas::NUM_DISCIPLINA = 0;
int MetaHeuristicas::NUM_DOCENTE = 0;
vector<Docente*> MetaHeuristicas::docente;
vector<Disciplina*> MetaHeuristicas::disciplinas;
vector<Turma*> MetaHeuristicas::turmas;
vector<Curso*> MetaHeuristicas::cursos;

MetaHeuristicas::MetaHeuristicas(){
	populacaoSolucoes = new vector<Solucoes*>();
	interadorSolucoes = -1;
	Y = 0.2;
	P = 0.2;
	W = 0.8;
}

MetaHeuristicas::MetaHeuristicas(int iterador) {
	populacaoSolucoes = new vector<Solucoes*>();
	interadorSolucoes = iterador;
}

MetaHeuristicas::~MetaHeuristicas() {
	
}

bool MetaHeuristicas::ordena(Solucoes *s1, Solucoes *s2) {
	if (s1->getHardConstraints() == s2->getHardConstraints()) {
		return (s1->getSoftConstraints() < s2->getSoftConstraints());
	}
	return (s1->getHardConstraints() < s2->getHardConstraints());
}

void MetaHeuristicas::ordenaSolucao() {
	sort(populacaoSolucoes->begin(), populacaoSolucoes->end(), ordena);
}

double MetaHeuristicas::classificaHC(vector<Turma*> *turma) {
	double pontuacao = 0;

	//Respeitar as turmas que tem horarios fixos pre definidos.

	//Levar em considerac~ao o turno da disciplina ao determinar os horarios.
	for (auto i = 0; i < NUM_TURMA; i++) {
		if (!turma->at(i)->getHorarioFixo()) {
			if (turma->at(i)->getTurno() == "diurno" || turma->at(i)->getTurno() == "Diurno") {
				if (turma->at(i)->getDisciplina()->getCreditos() == 2) {
					if (turma->at(i)->getHorario1() >= NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
						pontuacao++;
					}
				}
				else {
					if (turma->at(i)->getHorario1() > NUMERO_HORAS_DIA - HORAS_NOTURNAS || turma->at(i)->getHorario2() > NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
						pontuacao++;
					}
				}
			}
			else if (turma->at(i)->getTurno() == "misto" || turma->at(i)->getTurno() == "Misto") {
				if (turma->at(i)->getDisciplina()->getCreditos() == 2) {
					if (turma->at(i)->getHorario1() != HORA_MISTA) {
						pontuacao++;
					}
				}
				else {
					if (turma->at(i)->getHorario1() != HORA_MISTA || turma->at(i)->getHorario2() != HORA_MISTA) {
						pontuacao++;
					}
				}
			}
			else if (turma->at(i)->getTurno() == "noturno" || turma->at(i)->getTurno() == "Noturno") {
				if (turma->at(i)->getDisciplina()->getCreditos() == 2) {
					if (turma->at(i)->getHorario1() < NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
						pontuacao++;
					}
				}
				else {
					if (turma->at(i)->getHorario1() <= NUMERO_HORAS_DIA - HORAS_NOTURNAS || turma->at(i)->getHorario2() <= NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
						pontuacao++;
					}
				}
			}
		}
	}

	//Garantir que as turmas daquela grade não coincidam seus horários
	for (auto i = 0; i < MetaHeuristicas::NUM_CURSO; i++){ // para todos os cursos
		
		for (auto j = 0; j < MetaHeuristicas::getCursos()->at(i)->getGrades()->size(); j++){ // para todos os periodos dos cursos
			
			for (auto k = 0; k < MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->size(); k++){ //para todas as diciplinas do periodo verifica se a turma está naquele período

				for (auto t = 0; t < turma->size(); t++) //para todas as turmas da solução
				{
					if( turma->at(t)->getDisciplina()->getCodigo() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(k)->getDisciplina()->getCodigo() &&
						turma->at(t)->getCodigo() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(k)->getCodigo()) //se a turma está no período
					{
						for (auto n = 0; n < MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->size(); n++)
						{
							if(n != k) // se não é a mesma turma
							{
								if (turma->at(t)->getDisciplina()->getCreditos() == 4 &&
									MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDisciplina()->getCreditos() == 4)
								{
									if (turma->at(t)->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1()) {
										if( turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
										{
											pontuacao++;
										}
									}
									if(turma->at(t)->getDia2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia2())
									{
										if( turma->at(t)->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
											turma->at(t)->getHorario2() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
											turma->at(t)->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() + 1)
										{
											pontuacao++;
										}
									}
									if(turma->at(t)->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia2())
									{
										if( turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
											turma->at(t)->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
											turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() + 1)
										{
											pontuacao++;
										}
									}
									if(turma->at(t)->getDia2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1())
									{
										if (turma->at(t)->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario2() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1) 
										{
											pontuacao++;
										}
									}
								}
								else if (turma->at(t)->getDisciplina()->getCreditos() == 4 &&
											MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDisciplina()->getCreditos() == 2)
								{
									if (turma->at(t)->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1())
									{
										if( turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
										{
											pontuacao++;
										}
									}
									if(turma->at(t)->getDia2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1())
									{
										if (turma->at(t)->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario2() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
										{
											pontuacao++;
										}
									}
								}
								else if (turma->at(t)->getDisciplina()->getCreditos() == 2 &&
									MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDisciplina()->getCreditos() == 4)
								{
									if (turma->at(t)->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1()) {
										if( turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
										{
											pontuacao++;
										}
									}
									if(turma->at(t)->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia2())
									{
										if( turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
											turma->at(t)->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
											turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() + 1)
										{
											pontuacao++;
										}
									}
								}
								else
								{
									if (turma->at(t)->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1())
									{
										if (turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
											turma->at(t)->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
										{
											pontuacao++;
										}
									}
								}
							}
						}
						break;
					}
				}
			}
		}
	}

	//dois horarios não serem iguais
	for (int i = 0; i < NUM_TURMA; i++) {
		if (!turma->at(i)->getHorarioFixo()) {
			if (turma->at(i)->getDisciplina()->getCreditos() == 4) {
				if (turma->at(i)->getDia1() == turma->at(i)->getDia2()) {
					if (turma->at(i)->getHorario1() == turma->at(i)->getHorario2()) {
						pontuacao++;
					}
					if (turma->at(i)->getHorario1() + 1 == turma->at(i)->getHorario2()) {
						pontuacao++;
					}
					if (turma->at(i)->getHorario2() + 1 == turma->at(i)->getHorario1()) {
						pontuacao++;
					}
				}
			}
		}
	}
	return pontuacao;
}

double MetaHeuristicas::classificaSC(vector<Turma*> *turma) {
	double pontuacao = 0;
	
	//Atender as restric~oes de horario dos professores.
	for (auto i = 0; i < NUM_TURMA; i++) {
		if (!turma->at(i)->getHorarioFixo()) {
			if (turma->at(i)->getDisciplina()->getCreditos() == 2) {
				if (!turma->at(i)->getDocente()->getPrefHorarios(turma->at(i)->getDia1(), turma->at(i)->getHorario1())) {
					pontuacao++;
				}
			}
			else {
				if (!turma->at(i)->getDocente()->getPrefHorarios(turma->at(i)->getDia1(), turma->at(i)->getHorario1()) || !turma->at(i)->getDocente()->getPrefHorarios(turma->at(i)->getDia2(), turma->at(i)->getHorario2())) {
					pontuacao++;
				}
			}
		}
	}
	
	//Disciplinas n~ao terem quatro horas seguidas de durac~ao.
	for (auto i = 0; i < NUM_TURMA; i++) {
		if (turma->at(i)->getDisciplina()->getCreditos() == 4) {
			if (turma->at(i)->getDia1() == turma->at(i)->getDia2()) {
				if (turma->at(i)->getHorario1() == turma->at(i)->getHorario2() + 2) {
					pontuacao += 1;
				}
			}
		}
	}

	//N~ao possuir dois horarios as 21 horas para a mesma disciplina.
	for (auto i = 0; i < NUM_TURMA; i++) {
		if (turma->at(i)->getDisciplina()->getCreditos() == 4) {
			if (turma->at(i)->getDia1() != turma->at(i)->getDia2()) {
				if (turma->at(i)->getHorario1() >= NUMERO_HORAS_DIA - 1 && turma->at(i)->getHorario2() >= NUMERO_HORAS_DIA - 1) {
					pontuacao += 1;
				}
			}
		}
	}

	//Laboratorios de programac~ao n~ao serem as 21 horas.
	for (auto i = 0; i < NUM_TURMA; i++) {
		if (turma->at(i)->getDisciplina()->getCodigo() == "DCC120" && turma->at(i)->getHorario1() >= NUMERO_HORAS_DIA - 1) {
			pontuacao += 1;
		}
	}

	return pontuacao;
}

bool MetaHeuristicas::validaSolucao(int dia1, int dia2, int horario1, int horario2) {
	//dois horarios não serem iguais
	if (dia1 == dia2) {
		if (horario1 == horario2) {
			return false;
		}
		if (horario1 + 1 == horario2) {
			return false;
		}
		if (horario2 + 1 == horario1) {
			return false;
		}
	}

	return true;
}


double MetaHeuristicas::classificaElementoHC(Turma *turma) {
	double pontuacao = 0;

	//Respeitar as turmas que tem horarios fixos pre definidos.

	//Levar em considerac~ao o turno da disciplina ao determinar os horarios.
	if (!turma->getHorarioFixo()) {
		if (turma->getTurno() == "diurno" || turma->getTurno() == "Diurno") {
			if (turma->getDisciplina()->getCreditos() == 2) {
				if (turma->getHorario1() >= NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
					pontuacao++;
				}
			}
			else {
				if (turma->getHorario1() > NUMERO_HORAS_DIA - HORAS_NOTURNAS || turma->getHorario2() > NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
					pontuacao++;
				}
			}
		}
		else if (turma->getTurno() == "misto" || turma->getTurno() == "Misto") {
			if (turma->getDisciplina()->getCreditos() == 2) {
				if (turma->getHorario1() != HORA_MISTA) {
					pontuacao++;
				}
			}
			else {
				if (turma->getHorario1() != HORA_MISTA || turma->getHorario2() != HORA_MISTA) {
					pontuacao++;
				}
			}
		}
		else if (turma->getTurno() == "noturno" || turma->getTurno() == "Noturno") {
			if (turma->getDisciplina()->getCreditos() == 2) {
				if (turma->getHorario1() < NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
					pontuacao++;
				}
			}
			else {
				if (turma->getHorario1() <= NUMERO_HORAS_DIA - HORAS_NOTURNAS || turma->getHorario2() <= NUMERO_HORAS_DIA - HORAS_NOTURNAS) {
					pontuacao++;
				}
			}
		}
	}

	//Garantir que as turmas daquela grade não coincidam seus horários
	for (auto i = 0; i < MetaHeuristicas::NUM_CURSO; i++) { // para todos os cursos

		for (auto j = 0; j < MetaHeuristicas::getCursos()->at(i)->getGrades()->size(); j++) { // para todos os periodos dos cursos

			for (auto k = 0; k < MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->size(); k++) { //verifica se aquela turma está na grade de algum curso

				if (turma->getId() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(k)->getId()) {
					
					for (auto n = 0; n < MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->size(); n++) { //para todas as outras disciplinas daquele periodo

						if (k != n) { // se a turma não é a mesma
							if (turma->getDisciplina()->getCreditos() == 4 &&
								MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDisciplina()->getCreditos() == 4)
							{
								if (turma->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1()) {
									if (turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
									{
										pontuacao++;
									}
								}
								if (turma->getDia2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia2())
								{
									if (turma->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
										turma->getHorario2() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
										turma->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() + 1)
									{
										pontuacao++;
									}
								}
								if (turma->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia2())
								{
									if (turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
										turma->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
										turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() + 1)
									{
										pontuacao++;
									}
								}
								if (turma->getDia2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1())
								{
									if (turma->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario2() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
									{
										pontuacao++;
									}
								}
							}
							else if (turma->getDisciplina()->getCreditos() == 4 &&
								MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDisciplina()->getCreditos() == 2)
							{
								if (turma->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1())
								{
									if (turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
									{
										pontuacao++;
									}
								}
								if (turma->getDia2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1())
								{
									if (turma->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario2() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario2() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
									{
										pontuacao++;
									}
								}
							}
							else if (turma->getDisciplina()->getCreditos() == 2 &&
								MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDisciplina()->getCreditos() == 4)
							{
								if (turma->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia1()) {
									if (turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
										turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
									{
										pontuacao++;
									}
								}
								if (turma->getDia1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getDia2())
								{
									if (turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
										turma->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() ||
										turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario2() + 1)
									{
										pontuacao++;
									}
								}
							}
							else
							{
								if (turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
									turma->getHorario1() + 1 == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() ||
									turma->getHorario1() == MetaHeuristicas::getCursos()->at(i)->getGrades()->at(j)->at(n)->getHorario1() + 1)
								{
									pontuacao++;
								}
							}
						}
					}
					break;
				}
			}
		}
	}

	//dois horarios não serem iguais
	if (!turma->getHorarioFixo()) {
		if (turma->getDisciplina()->getCreditos() == 4) {
			if (turma->getDia1() == turma->getDia2()) {
				if (turma->getHorario1() == turma->getHorario2()) {
					pontuacao++;
				}
				if (turma->getHorario1() + 1 == turma->getHorario2()) {
					pontuacao++;
				}
				if (turma->getHorario2() + 1 == turma->getHorario1()) {
					pontuacao++;
				}
			}
		}
	}

	//Respeitar o numero maximo de salas por horarios.
	return pontuacao;
}


double MetaHeuristicas::classificaElementoSC(Turma *turma) {
	double pontuacao = 0;
	
	//Atender as restric~oes de horario dos professores.
	if (!turma->getHorarioFixo()) {
		if (turma->getDisciplina()->getCreditos() == 2) {
			if (!turma->getDocente()->getPrefHorarios(turma->getDia1(), turma->getHorario1())) {
				pontuacao++;
			}
		}
		else {
			if (!turma->getDocente()->getPrefHorarios(turma->getDia1(), turma->getHorario1()) || !turma->getDocente()->getPrefHorarios(turma->getDia2(), turma->getHorario2())) {
				pontuacao++;
			}
		}
	}

	if (turma->getDisciplina()->getCreditos() == 4) {
		//Disciplinas n~ao terem quatro horas seguidas de durac~ao.
		if (turma->getDia1() == turma->getDia2()) {
			if (turma->getHorario1() == turma->getHorario2() + 2) {
				pontuacao += 1;
			}
		}

		//N~ao possuir dois horarios as 21 horas para a mesma disciplina.
		if (turma->getDia1() != turma->getDia2()) {
			if (turma->getHorario1() >= NUMERO_HORAS_DIA - 2 && turma->getHorario2() >= NUMERO_HORAS_DIA - 2) {
				pontuacao += 1;
			}
		}
	}

	//Laboratorios de programac~ao n~ao serem as 21 horas.
	if (turma->getDisciplina()->getCodigo() == "DCC120" && turma->getHorario1() >= NUMERO_HORAS_DIA - 2) {
		pontuacao += 1;
	}
	return pontuacao;
}


void MetaHeuristicas::buscaLocal(Solucoes * solucao){
	double pontuacao1 = 0;
	double pontuacao2 = 0;
	int turma1 = -1;
	int turma2 = -1;

	//realiza trocas entre as turmas
	for (int j = 0; j < REPETICAO_BUSCA_LOCAL; j++)
	{
		for (int i = 0; i < solucao->getTurmas()->size(); i++) {
			if (turma1 == -1) {
				pontuacao1 = classificaElementoHC(solucao->getTurmas()->at(i));
				if (pontuacao1 != 0) {
					turma1 = i;
				}
			}
			else {
				pontuacao2 = classificaElementoHC(solucao->getTurmas()->at(i));
				if (pontuacao2 != 0) {
					turma2 = i;
				}
			}

			if (turma1 != -1 && turma2 != -1) {
				int rnd = rand() % 2;

				if (rnd == 0) {
					int dia = solucao->getTurmas()->at(turma1)->getDia1();
					int hora = solucao->getTurmas()->at(turma1)->getHorario1();

					solucao->getTurmas()->at(turma1)->setDia1(solucao->getTurmas()->at(turma2)->getDia1());
					solucao->getTurmas()->at(turma1)->setHorario1(solucao->getTurmas()->at(turma2)->getHorario1());

					solucao->getTurmas()->at(turma2)->setDia1(dia);
					solucao->getTurmas()->at(turma2)->setHorario1(hora);

					if (classificaElementoHC(solucao->getTurmas()->at(turma1)) > pontuacao1 && classificaElementoHC(solucao->getTurmas()->at(turma2)) > pontuacao2) {
						int dia = solucao->getTurmas()->at(turma2)->getDia1();
						int hora = solucao->getTurmas()->at(turma2)->getHorario1();

						solucao->getTurmas()->at(turma2)->setDia1(solucao->getTurmas()->at(turma1)->getDia1());
						solucao->getTurmas()->at(turma2)->setHorario1(solucao->getTurmas()->at(turma1)->getHorario1());

						solucao->getTurmas()->at(turma1)->setDia1(dia);
						solucao->getTurmas()->at(turma1)->setHorario1(hora);
					}
				}else if (rnd == 1) {
					int dia = solucao->getTurmas()->at(turma1)->getDia2();
					int hora = solucao->getTurmas()->at(turma1)->getHorario2();

					solucao->getTurmas()->at(turma1)->setDia2(solucao->getTurmas()->at(turma2)->getDia2());
					solucao->getTurmas()->at(turma1)->setHorario2(solucao->getTurmas()->at(turma2)->getHorario2());

					solucao->getTurmas()->at(turma2)->setDia2(dia);
					solucao->getTurmas()->at(turma2)->setHorario2(hora);

					if (classificaElementoHC(solucao->getTurmas()->at(turma1)) > pontuacao1 && classificaElementoHC(solucao->getTurmas()->at(turma2)) > pontuacao2) {
						int dia = solucao->getTurmas()->at(turma2)->getDia2();
						int hora = solucao->getTurmas()->at(turma2)->getHorario2();

						solucao->getTurmas()->at(turma2)->setDia2(solucao->getTurmas()->at(turma1)->getDia2());
						solucao->getTurmas()->at(turma2)->setHorario2(solucao->getTurmas()->at(turma1)->getHorario2());

						solucao->getTurmas()->at(turma1)->setDia2(dia);
						solucao->getTurmas()->at(turma1)->setHorario2(hora);
					}
				}

				turma1 = -1;
				turma2 = -1;
				pontuacao1 = 0;
				pontuacao2 = 0;
			}

		}
	}
	turma1 = -1;
	pontuacao1 = 0; 

	//coloca horários randomicos na turma
	for (int j = 0; j < REPETICAO_BUSCA_LOCAL; j++)
	{
		for (int i = 0; i < solucao->getTurmas()->size(); i++) {
			if (turma1 == -1) {
				pontuacao1 = classificaElementoHC(solucao->getTurmas()->at(i));
				if (pontuacao1 != 0) {
					turma1 = i;
					
					int rnd = rand() % 2; //rand para decidir se irá alterar o dia 1 ou dia 2

					if (rnd == 0) {
						int dia = solucao->getTurmas()->at(turma1)->getDia1();
						int hora = solucao->getTurmas()->at(turma1)->getHorario1();

						solucao->getTurmas()->at(turma1)->setDia1(rand() % NUMERO_DIAS);
						solucao->getTurmas()->at(turma1)->setHorario1(rand() % NUMERO_HORAS_DIA);

						if (validaSolucao(solucao->getTurmas()->at(turma1)->getDia1(), solucao->getTurmas()->at(turma1)->getDia2(), solucao->getTurmas()->at(turma1)->getHorario1(), solucao->getTurmas()->at(turma1)->getHorario2()) && 
							classificaElementoHC(solucao->getTurmas()->at(turma1)) > pontuacao1)
						{
							solucao->getTurmas()->at(turma1)->setDia1(dia);
							solucao->getTurmas()->at(turma1)->setHorario1(hora);
						}
					}
					else {
						int dia = solucao->getTurmas()->at(turma1)->getDia2();
						int hora = solucao->getTurmas()->at(turma1)->getHorario2();

						solucao->getTurmas()->at(turma1)->setDia2(rand() % NUMERO_DIAS);
						solucao->getTurmas()->at(turma1)->setHorario2(rand() % NUMERO_HORAS_DIA);

						if (validaSolucao(solucao->getTurmas()->at(turma1)->getDia1(), solucao->getTurmas()->at(turma1)->getDia2(), solucao->getTurmas()->at(turma1)->getHorario1(), solucao->getTurmas()->at(turma1)->getHorario2()) && 
							classificaElementoHC(solucao->getTurmas()->at(turma1)) > pontuacao1)
						{
							solucao->getTurmas()->at(turma1)->setDia2(dia);
							solucao->getTurmas()->at(turma1)->setHorario2(hora);
						}
					}
				}
			}
		}

	}
}


void MetaHeuristicas::inicializaDados() {
	ifstream file("dados.tbd");

	if (file.fail()) {
		cerr << "ERRO: Arquivo dados.tdb nao encontrado" << endl;
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < 13; i++) {
		file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	int periodo;
	int numeroTurmas;
	int numeroDisciplinas;
	int numeroDocentes;
	int numeroCursos;

	file >> periodo;
	file >> numeroDocentes;
	file >> numeroDisciplinas;
	file >> numeroTurmas;
	file >> numeroCursos;

	cout << "Periodo " << periodo << " numeroDocentes: " << numeroDocentes << " numeroDisciplinas: " << numeroDisciplinas << " numeroTurmas: " << numeroTurmas << " numeroCursos " << numeroCursos << endl;

	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	for (int i = 0; i < numeroDocentes; i++) {
		Docente *d = new Docente();
		file >> *d;
		docente.push_back(d);
		d->toString();
	}

	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	for (int i = 0; i < numeroDisciplinas; i++) {
		Disciplina *d = new Disciplina();
		file >> *d;
		disciplinas.push_back(d);
		d->toString();
		cout << endl;
	}

	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	for (int i = 0; i < numeroTurmas; i++) {
		Turma *t = new Turma();
		int idDisc;
		int idDoc;
		file >> *t;
		file >> idDisc;
		for (Disciplina *d : disciplinas) {
			if (d->getIdDisciplina() == idDisc) {
				t->setDisciplina(d);
				break;
			}
		}
		file >> idDoc;
		for (Docente *d : docente) {
			if (d->getIdDocente() == idDoc) {
				t->setDocente(d);
				break;
			}
		}
		turmas.push_back(t);
		t->toString();
		cout << endl;
	}

	file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

	for (int i = 0; i < numeroCursos; i++) {
		Curso *c = new Curso();
		file >> *c;
		vector<vector<Turma*>* > *grade = new vector<vector<Turma*>* >();
		for (int k = 0; k<c->getQtdPeriodos(); k++) {
			int numDisciplinas;
			file >> numDisciplinas;
			grade->push_back(new vector<Turma*>());
			for (int j = 0; j < numDisciplinas; j++) {
				int idDisc;
				string codTurma;
				file >> idDisc;
				file >> codTurma;

				for (Turma *d : turmas) {
					if (d->getDisciplina()->getIdDisciplina() == idDisc && d->getCodigo() == codTurma) {
						grade->at(k)->push_back(d);
						break;
					}
				}
			}
		}
		c->setGrades(grade);
		cursos.push_back(c);
		c->toString();
	}

	file.close();

	NUM_DOCENTE = numeroDocentes;
	NUM_DISCIPLINA = numeroDisciplinas;
	NUM_TURMA = numeroTurmas;
	NUM_CURSO = numeroCursos;
}

void MetaHeuristicas::escreveSolucao() {
	if (interadorSolucoes == -1) {
		Solucoes * solucao = MetaHeuristicas::getPopulacaoSolucoes()->at(0);
		ofstream saida;
		saida.open("solucao.tbd", ios::out);

		saida << "numero de turmas\nTurma: \n\tid turma, dia1, horario1, dia2, horario2\n\n";
		saida << solucao->getTurmas()->size() << "\n";
		for (int i = 0; i < solucao->getTurmas()->size(); i++) {
			saida << solucao->getTurmas()->at(i)->getId() << " " << solucao->getTurmas()->at(i)->getDia1() << " " << solucao->getTurmas()->at(i)->getHorario1() << " " << solucao->getTurmas()->at(i)->getDia2() << " " << solucao->getTurmas()->at(i)->getHorario2() << "\n";
		}
		saida.close();
	}
	else {
		Solucoes * solucao = MetaHeuristicas::getPopulacaoSolucoes()->at(0);
		ofstream saida;
		saida.open("dados/solucao" + to_string(interadorSolucoes) + ".tbd", ios::out);

		saida << "numero de turmas\nTurma: \n\tid turma, dia1, horario1, dia2, horario2\n\n";
		saida << solucao->getTurmas()->size() << "\n";
		for (int i = 0; i < solucao->getTurmas()->size(); i++) {
			saida << solucao->getTurmas()->at(i)->getId() << " " << solucao->getTurmas()->at(i)->getDia1() << " " << solucao->getTurmas()->at(i)->getHorario1() << " " << solucao->getTurmas()->at(i)->getDia2() << " " << solucao->getTurmas()->at(i)->getHorario2() << "\n";
		}
		saida.close();

		saida.open("dados/qualidade solucao.csv", ios::out | ios::app);

		saida.seekp(0, saida.end);

		saida << interadorSolucoes << ";" << solucao->getHardConstraints() << ";" << solucao->getSoftConstraints() << ";" << Y << ";" << P << ";" << W << ";" << timer.elapsed() << "\n";
		saida.close();
	}
}


void MetaHeuristicas::imprimeSolucao() {
	Solucoes * solucao = MetaHeuristicas::getPopulacaoSolucoes()->at(0);

	for (int i = 0; i < solucao->getTurmas()->size(); i++) { // imprime a melhor solução
		cout << "turma:" << i << " " <<
			solucao->getTurmas()->at(i)->getDisciplina()->getCodigo() << " " <<
			solucao->getTurmas()->at(i)->getCodigo() << " ";
		switch (solucao->getTurmas()->at(i)->getDia1()) {
		case 0:
			cout << "SEG ";
			break;
		case 1:
			cout << "TER ";
			break;
		case 2:
			cout << "QUA ";
			break;
		case 3:
			cout << "QUI ";
			break;
		case 4:
			cout << "SEX ";
			break;
		default:
			break;
		}
		cout << solucao->getTurmas()->at(i)->getHorario1() + 8 << "h ";
		switch (solucao->getTurmas()->at(i)->getDia2()) {
		case 0:
			cout << "SEG ";
			break;
		case 1:
			cout << "TER ";
			break;
		case 2:
			cout << "QUA ";
			break;
		case 3:
			cout << "QUI ";
			break;
		case 4:
			cout << "SEX ";
			break;
		default:
			break;
		}
		cout << solucao->getTurmas()->at(i)->getHorario2() + 8 << "h " << endl;
	}
}