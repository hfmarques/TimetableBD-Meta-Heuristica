#include "stdafx.h"
#include "HHS.h"
#include "GeracaoGrades.h"
#include "Turma.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define MAXIMO_ITERACOES 10000
#define CONTROLA_OTIMO_LOCAL 500 // número minimo de vezes que uma solução ótima (otimo local) deve aparecer antes de haver evaporação de feromônios
#define CONTROLA_MAXIMO_OTIMO_LOCAL 1500 // número minimo de vezes que uma solução ótima (otimo local) deve aparecer antes de haver evaporação de feromônios
#define HMCR 0.7 //Porcentagem randomica de consideração da memória
#define PAR 0.3 //Porcentagem de consideração randomica (ajuste de tom)
#define TAMANHO_PITCH 2 //Controla o máximo para mais e para menos que pode haver alteração com o ajudate de pitch

HHS::HHS() {
    //ctor
}

HHS::~HHS() {
    //dtor
}

bool compare(Solucoes* i, Solucoes* j) {
	if (i->getHardConstraints() == j->getHardConstraints())
		return i->getSoftConstraints() < j->getSoftConstraints();
	else
		return i->getHardConstraints() < j->getHardConstraints();
}

void HHS::executa() {
	//Passo 1: Inicializar os parametros
	auto turmaAtual = 0, iatual = 0; // controle de quantas turmas ja foram alocadas e o indice de interações atualmente
	//Passo 2: Inicializar a memória harmonica
	if (getPopulacaoSolucoes()->empty()) {
		for (auto j = 0; j < NUM_SOLUCOES; j++) {
			auto *s = new Solucoes();
			for (auto i = 0; i < NUM_TURMA; i++) {
				auto *t = new Turma(getTurmas()->at(i)->getId(),
									 getTurmas()->at(i)->getCodigo(),
									 getTurmas()->at(i)->getTurno(),
									 getTurmas()->at(i)->getDisciplina(),
									 getTurmas()->at(i)->getDocente(),
									 getTurmas()->at(i)->getHorarioFixo(),
									 rand() % NUMERO_DIAS,
									 rand() % NUMERO_HORAS_DIA,
									 rand() % NUMERO_DIAS,
									 rand() % NUMERO_HORAS_DIA);

				if (validaSolucao(t->getDia1(), t->getDia2(), t->getHorario1(), t->getHorario2())) {
					s->getTurmas()->push_back(t);
				}
				else {
					i--;
					delete t;
				}
			}
			s->setHardConstraints(classificaHC(s->getTurmas()));
			s->setSoftConstraints(classificaSC(s->getTurmas()));
			populacaoSolucoes->push_back(s);
		}
		ordenaSolucao();
	}
	
	//Passo 3: improvisar uma nova harmonia
	
	while (iatual < MAXIMO_ITERACOES) { // inicia o loop principal
		auto s = new Solucoes();
		for (turmaAtual = 0;turmaAtual < NUM_TURMA;turmaAtual++) { //enquanto há turmas sem horarios ele encontra um horario para ela
			auto *t = new Turma(getTurmas()->at(turmaAtual)->getId(),
								 getTurmas()->at(turmaAtual)->getCodigo(),
								 getTurmas()->at(turmaAtual)->getTurno(),
								 getTurmas()->at(turmaAtual)->getDisciplina(),
								 getTurmas()->at(turmaAtual)->getDocente(),
								 getTurmas()->at(turmaAtual)->getHorarioFixo(),
								 -1,
								 -1,
								 -1,
								 -1);

			auto* dias1 = new int[NUM_SOLUCOES];
			auto* horas1 = new int[NUM_SOLUCOES];
			auto* dias2 = new int[NUM_SOLUCOES];
			auto* horas2 = new int[NUM_SOLUCOES];
			for (auto i = 0; i < NUM_SOLUCOES; i++) { //salva em um vetor auxiliar todos os horários já definidos para aquela turma
				dias1[i] = populacaoSolucoes->at(i)->getTurmas()->at(turmaAtual)->getDia1();
				horas1[i] = populacaoSolucoes->at(i)->getTurmas()->at(turmaAtual)->getHorario1();
				dias2[i] = populacaoSolucoes->at(i)->getTurmas()->at(turmaAtual)->getDia2();
				horas2[i] = populacaoSolucoes->at(i)->getTurmas()->at(turmaAtual)->getHorario2();
			}

			//seleciona dias e horarios para a turma
			auto dia1 = selecionaDias(dias1, s, 1);
			auto hora1 = selecionaHoras(horas1, s, 1);
			auto dia2 = selecionaDias(dias2, s, 2);
			auto hora2 = selecionaHoras(horas2, s, 2);

			//verifica se os dias e horários selecionados são válidos
			if (validaSolucao(dia1, dia2, hora1, hora2)) {
				t->setDia1(dia1);
				t->setDia2(dia2);
				t->setHorario1(hora1);
				t->setHorario2(hora2);
				s->getTurmas()->push_back(t);
			}
			else {
				turmaAtual--;
				delete t;
			}
			delete[] dias1;
			delete[] dias2;
			delete[] horas1;
			delete[] horas2;
		}
		s->setHardConstraints(classificaHC(s->getTurmas()));
		s->setSoftConstraints(classificaSC(s->getTurmas()));

		//imprime os dados encontrados.
		cout << "DFT:" << s->getHardConstraints()
			<< "  SCP:" << s->getSoftConstraints()
			<< "  bestDFT:" << populacaoSolucoes->at(0)->getHardConstraints()
			<< "  bestSCP:" << populacaoSolucoes->at(0)->getSoftConstraints()
			<< "  numero de iteracoes:" << iatual << endl;

		//Passo 4: Atualiza a memória harmônica;
		if (!compare(s, populacaoSolucoes->at(NUM_SOLUCOES - 1))) {
			delete s;
		}
		else {
			for (auto j = 0; j < NUM_SOLUCOES; j++) {
				if (compare(s, populacaoSolucoes->at(j))) {
					populacaoSolucoes->insert(populacaoSolucoes->begin() + j, s);
					s = populacaoSolucoes->at(NUM_SOLUCOES);
					delete s;
					populacaoSolucoes->pop_back();					
					break;
				}
			}
		}

		iatual++;
	}
	cout << "  bestDFT:" << populacaoSolucoes->at(0)->getHardConstraints()
	<< "  bestSCP:" << populacaoSolucoes->at(0)->getSoftConstraints() << endl;
}

int HHS::selecionaDias(int* dias, Solucoes* s, int dia) {
	auto diaSelecionado = -1;
	auto rnd = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	if (rnd <= HMCR) {
		auto posicao = rand() % NUM_SOLUCOES;
		diaSelecionado = dias[posicao];
		rnd = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
		if (rnd < PAR/3)// 0 < rnd < PAR1 - move para qualquer slot livre na nova solução
		{
			auto pitch = (rand() % TAMANHO_PITCH * 2) - TAMANHO_PITCH;
			if (posicao + pitch >= NUM_SOLUCOES)
				posicao = NUM_SOLUCOES - 1;
			else if (posicao + pitch < 0)
				posicao = 0;
			else
				posicao = posicao + pitch;
			diaSelecionado = dias[posicao];
		}
		else if (rnd > PAR/3 && rnd < (PAR/3) * 2)// PAR1 < rnd < PAR2 - troca os slots com outro existente na nova solução
		{
			if (s->getTurmas()->size() != 0)
			{
				rnd = static_cast<int>(rand() % s->getTurmas()->size());
				if (dia == 1)
					diaSelecionado = s->getTurmas()->at(rnd)->getDia1();
				else if (dia == 2)
					diaSelecionado = s->getTurmas()->at(rnd)->getDia2();
			}				
		}
		else if (rnd > (PAR / 3) * 2 && rnd < PAR)// PAR2 < rnd < PAR -  troca slots conforme os existentes na memoria harmônica
		{
			auto xIMod = dias[posicao] % NUMERO_DIAS;
			for (auto i = 0; i < s->getTurmas()->size(); i++)
			{
				if(dia == 1)
				{
					if (s->getTurmas()->at(i)->getDia1() % NUMERO_DIAS == xIMod)
					{
						auto diaNovo = s->getTurmas()->at(i)->getDia1() + dias[posicao];
						if (diaNovo >= NUMERO_DIAS)
							s->getTurmas()->at(i)->setDia1(NUMERO_DIAS - 1);
					}
					if (s->getTurmas()->at(i)->getDia1() % NUMERO_DIAS == dias[posicao])
					{
						auto diaNovo = s->getTurmas()->at(i)->getDia1() + xIMod;
						if (diaNovo >= NUMERO_DIAS)
							s->getTurmas()->at(i)->setDia1(NUMERO_DIAS - 1);
					}
				}
			}
			
		}
		
	}
	else {
		diaSelecionado = rand() % NUMERO_DIAS;
	}
	return diaSelecionado;
}

int HHS::selecionaHoras(int *horas, Solucoes* s, int dia) {
	auto horaSelecionada = -1;
	auto rnd = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
	if (rnd <= HMCR) {
		auto posicao = rand() % NUM_SOLUCOES;
		horaSelecionada = horas[posicao];
		rnd = static_cast<double>(rand()) / static_cast<double>(RAND_MAX);
		if (rnd < PAR / 3)// 0 < rnd < PAR1 - move para qualquer slot livre na nova solução
		{
			auto pitch = (rand() % TAMANHO_PITCH * 2) - TAMANHO_PITCH;
			if (posicao + pitch >= NUM_SOLUCOES)
				posicao = NUM_SOLUCOES - 1;
			else if (posicao + pitch < 0)
				posicao = 0;
			else
				posicao = posicao + pitch;
			horaSelecionada = horas[posicao];
		}
		else if (rnd > PAR / 3 && rnd < (PAR / 3) * 2)// PAR1 < rnd < PAR2 - troca os slots com outro existente na nova solução
		{
			if (s->getTurmas()->size() != 0)
			{
				rnd = static_cast<int>(rand() % s->getTurmas()->size());
				if (dia == 1)
					horaSelecionada = s->getTurmas()->at(rnd)->getDia1();
				else if (dia == 2)
					horaSelecionada = s->getTurmas()->at(rnd)->getDia2();
			}
		}
		else if (rnd >(PAR / 3) * 2 && rnd < PAR)// PAR2 < rnd < PAR -  troca slots conforme os existentes na memoria harmônica
		{
			auto xIMod = horas[posicao] % NUMERO_HORAS_DIA;
			for (auto i = 0; i < s->getTurmas()->size(); i++)
			{
				if (dia == 1)
				{
					if (s->getTurmas()->at(i)->getDia1() % NUMERO_HORAS_DIA == xIMod)
					{
						auto diaNovo = s->getTurmas()->at(i)->getDia1() + horas[posicao];
						if (diaNovo >= NUMERO_HORAS_DIA)
							s->getTurmas()->at(i)->setDia1(NUMERO_HORAS_DIA - 1);
					}
					if (s->getTurmas()->at(i)->getDia1() % NUMERO_HORAS_DIA == horas[posicao])
					{
						auto diaNovo = s->getTurmas()->at(i)->getDia1() + xIMod;
						if (diaNovo >= NUMERO_HORAS_DIA)
							s->getTurmas()->at(i)->setDia1(NUMERO_HORAS_DIA - 1);
					}
				}
			}

		}

	}
	else {
		horaSelecionada = rand() % NUMERO_HORAS_DIA;
	}
	return horaSelecionada;
}