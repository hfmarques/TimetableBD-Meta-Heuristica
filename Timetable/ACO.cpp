/*
 * ACO.cpp
 *
 *  Created on: 22 de fev de 2016
 *      Author: Héber Marques
 */

#include "stdafx.h"
#include "ACO.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define METODO 1 // 1 para roleta, 2 para NPI, 3 para torneio
#define NUMERO_FORMIGAS 50
#define MAXIMO_ITERACOES 20000
#define NUMERO_HORAS_TOTAL 70
#define NUMERO_HORAS_DIA 14
#define NUMERO_DIAS 5
#define HORA_INICIAL 8
#define HORAS_NOTURNAS 4
//#define Y 0.1 // fator de penalidade
//#define P 0.1 // fator de evaporação dos feromonios
//#define W 0.8 // variavel omega que define o quanto será a normalização dos feromonios
#define C 5 // chance de escolher a menor solução no metodo do torneio
#define D 5 // quantidade de elementos do torneio
#define CONTROLA_OTIMO_LOCAL 100 // número minimo de vezes que uma solução ótima (otimo local) deve aparecer antes de haver evaporação de feromônios
#define CONTROLA_MAXIMO_OTIMO_LOCAL 500 // número minimo de vezes que uma solução ótima (otimo local) deve aparecer antes de haver evaporação de feromônios

///TODO: restrição de horas iguais
///TODO: alterar o proximoItemPermutado
///TODO: valida solução
///TODO: arrumar o posT em proxima permutação
///TODO: definir um padrão para selecionar a próxima hora e salva-la

using namespace std;
ACO::ACO(){
	formigas = vector<Ant*>(NUMERO_FORMIGAS);
}

ACO::ACO(int iterador){
	this->interadorSolucoes = iterador;		
	formigas = vector<Ant*>(NUMERO_FORMIGAS);
}

ACO::~ACO() {
    // TODO Auto-generated destructor stub
}

bool sortSolution(Solucoes* i, Solucoes* j) {
	if (i->getHardConstraints() == j->getHardConstraints())
		return i->getSoftConstraints() < j->getSoftConstraints();
	else
		return i->getHardConstraints() < j->getHardConstraints();
}

bool sortFeromonios(vector<double> i, vector<double> j) {
	return i.at(0) > j.at(0);
}

void ACO::executa() {
	auto& tauH = vector<vector<double>>(); 
	auto& tauH2 = vector<vector<double>>();  // vetor omega de horas, onde 0 é o nivel de feromonios e 1 é a posição em que ela estava inicialmente
	auto turmaAtual = 0, iatual = 0; // controle de quantas turmas ja foram alocadas e o indice de interações atualmente
	auto numTentativasH1 = 0, numTentativasH2 = 0;// vezes em que se tentou criar uma solução
	auto maxDiag = 0; // controle para a matriz imaginaria passar sempre na diagonal
	auto freeze = 0, freeze2 = 0; // conta quantas vezes a solução nao muda
	double bestDFTAtual = 10000, bestDFTGlobal = 10000; // melhor dft atualmente(desconsiderando o global)

	Ant::inicializa(MetaHeuristicas::NUM_TURMA); // incializa os feromonios

	for (auto i = 0; i < NUMERO_HORAS_TOTAL; i++)
	{
		tauH.push_back(vector<double>(2));
		tauH2.push_back(vector<double>(2));
	}

	for (auto i = 0;i < NUMERO_FORMIGAS;i++)
		formigas.at(i) = new Ant();

	timer.start();
	while (iatual<MAXIMO_ITERACOES) { // inicia o loop principal
		for (auto g = 0;g<NUMERO_FORMIGAS;g++) { // constroi e gera as soluções
			for (auto i = 0; i < NUMERO_HORAS_TOTAL; i++) { // irá buscar todas os horarios possiveis do professor que irá ministrar esta disciplina
				tauH.at(i).at(0) = Ant::getFeromoniosHorario1(turmaAtual, i);
				tauH.at(i).at(1) = i;
			}
			if (getTurmas()->at(turmaAtual)->getDisciplina()->getCreditos() == 4) {
				for (auto i = 0; i < NUMERO_HORAS_TOTAL; i++) { // irá buscar todas os horarios possiveis do professor em que o nextPermItem retornou e tentar achar um horario para ele
					tauH2.at(i).at(0) = Ant::getFeromoniosHorario2(turmaAtual, i);
					tauH2.at(i).at(1) = i;
				}
			}
			while (turmaAtual < MetaHeuristicas::NUM_TURMA) { //enquanto há turmas sem horarios ele encontra um horario para ela
				//cout << "turma: " << turmaAtual << endl;
				if (getTurmas()->at(turmaAtual)->getHorarioFixo() == false) {
					if (METODO == 1)
						posH1 = roleta(tauH, NUMERO_HORAS_TOTAL); // usa a roleta para retornar um horario adequado
					else if (METODO == 2)
						posH1 = getProxItemPermutado(tauH, numTentativasH1, NUMERO_HORAS_TOTAL, posH1); // usa o nextPermItem para retornar um horario adequado
					else if (METODO == 3)
						posH1 = torneio(tauH, NUMERO_HORAS_TOTAL);// usa o torneio para retornar um horario adequado
					if (getTurmas()->at(turmaAtual)->getDisciplina()->getCreditos() == 4) {
						if (METODO == 1)
							posH2 = roleta(tauH2, NUMERO_HORAS_TOTAL); // usa a roleta para retornar um horario adequado
						else if (METODO == 2)
							posH2 = getProxItemPermutado(tauH2, numTentativasH2, NUMERO_HORAS_TOTAL, posH2); // usa o nextPermItem para retornar um horario adequado
						else if (METODO == 3)
							posH1 = torneio(tauH2, NUMERO_HORAS_TOTAL);// usa o torneio para retornar um horario adequado
					}else {
						tauH2.at(numTentativasH2).at(0) = -1;
					}
					//cout << posH1 << endl; 
					//cout << posH2 << endl;
					auto dia1 = 0;
					auto horario1 = 0;
					auto dia2 = 0;
					auto horario2 = 0;
					if (METODO == 1 || METODO == 3) {
						dia1 = static_cast<int>(floor(tauH.at(posH1).at(1)) / NUMERO_HORAS_DIA);
						horario1 = static_cast<int>(tauH.at(posH1).at(1)) % NUMERO_HORAS_DIA;
						dia2 = -1;
						horario2 = -1;
						if (tauH2.at(posH2).at(1) != -1) {
							dia2 = static_cast<int>(floor(tauH2.at(posH2).at(1)) / NUMERO_HORAS_DIA);
							horario2 = static_cast<int>(tauH2.at(posH2).at(1)) % NUMERO_HORAS_DIA;
						}
					}
					if (METODO == 2) {
						dia1 = static_cast<int>(floor(tauH.at(posH1).at(1)) / NUMERO_HORAS_DIA);
						horario1 = static_cast<int>(tauH.at(posH1).at(1)) % NUMERO_HORAS_DIA;
						dia2 = -1;
						horario2 = -1;
						if (tauH2.at(numTentativasH2).at(1) != -1) {
							dia2 = static_cast<int>(floor(tauH2.at(posH2).at(1)) / NUMERO_HORAS_DIA);
							horario2 = static_cast<int>(tauH2.at(posH2).at(1)) % NUMERO_HORAS_DIA;
						}
					}
					
					//cout << "Dia 1: "<< dia1 << " Hora 1: " << horario1 << " Dia 2: "<<dia2 << " Hora 2: " << horario2 << endl;
					if (validaSolucao(dia1, dia2, horario1, horario2)) { // confere se a solução é uma solução valida, se sim dimunui o numero de disciplinas alocadas e seta essa solução						
						formigas[g]->getSolucao()->getTurmas()->push_back(new Turma(getTurmas()->at(turmaAtual)->getId(),
																				getTurmas()->at(turmaAtual)->getCodigo(),
																				getTurmas()->at(turmaAtual)->getTurno(),
																				getTurmas()->at(turmaAtual)->getDisciplina(),
																				getTurmas()->at(turmaAtual)->getDocente(),
																				getTurmas()->at(turmaAtual)->getHorarioFixo(),
																				dia1, horario1, dia2, horario2));

						// reseta as variaveis de controle e aumenta o número de turma alocada
						turmaAtual++;
						numTentativasH1 = 0;
						numTentativasH2 = 0;
						maxDiag = 0;
						posH1 = 0;
						posH2 = 0;	
						//cout << "ENTROU AQUI " << turmaAtual << endl;

						if(turmaAtual < MetaHeuristicas::NUM_TURMA) // atualiza o vetor tau, ordena o vetor omega para o nextPermItem realizar suas operações
						{
							for (auto i = 0; i < NUMERO_HORAS_TOTAL; i++) { // irá buscar todas os horarios possiveis do professor que irá ministrar esta disciplina
								tauH.at(i).at(0) = Ant::getFeromoniosHorario1(turmaAtual, i);
								tauH.at(i).at(1) = i;
							}
							if (getTurmas()->at(turmaAtual)->getDisciplina()->getCreditos() == 4) {
								for (auto i = 0; i < NUMERO_HORAS_TOTAL; i++) { // irá buscar todas os horarios possiveis do professor em que o nextPermItem retornou e tentar achar um horario para ele
									tauH2.at(i).at(0) = Ant::getFeromoniosHorario2(turmaAtual, i);
									tauH2.at(i).at(1) = i;
								}
							}
						}

					}

					else { // aumenta o numero de tentativas para o getNestxPermItem recriar uma nova possivel solução e garante que ele sempre vai percorrer a matriz de forma diagonal
						if (numTentativasH1 == NUMERO_HORAS_TOTAL || numTentativasH2 == NUMERO_HORAS_TOTAL) {
							//                        numtentativasT =0;
							//                        numtentativasH =0;
							cout << "ERRO 13";
							exit(13);
						}
						if (maxDiag == numTentativasH1 || numTentativasH2 == 0) {
							maxDiag++;
							numTentativasH1 = 0;
							numTentativasH2 = maxDiag;
						}
						else {
							numTentativasH1++;
							numTentativasH2--;
						}

					}
				}
				else {
					formigas[g]->getSolucao()->getTurmas()->push_back(new Turma(getTurmas()->at(turmaAtual)->getId(),
																			getTurmas()->at(turmaAtual)->getCodigo(),
																			getTurmas()->at(turmaAtual)->getTurno(),
																			getTurmas()->at(turmaAtual)->getDisciplina(),
																			getTurmas()->at(turmaAtual)->getDocente(),
																			getTurmas()->at(turmaAtual)->getHorarioFixo(),
																			getTurmas()->at(turmaAtual)->getDia1(),
																			getTurmas()->at(turmaAtual)->getHorario1(),
																			getTurmas()->at(turmaAtual)->getDia2(), 
																			getTurmas()->at(turmaAtual)->getHorario2()));

					turmaAtual++;
					
				}

			}
			// classifica a solução encontrada

			buscaLocal(formigas[g]->getSolucao());
			formigas[g]->getSolucao()->setHardConstraints(classificaHC(formigas[g]->getSolucao()->getTurmas()));
			formigas[g]->getSolucao()->setSoftConstraints(classificaSC(formigas[g]->getSolucao()->getTurmas()));
			turmaAtual = 0;
		}

		//////////////////////////// atualização dos feromônios

		auto solucoes = new vector<Solucoes*>(); //vetor que contem todas as soluções encontradas na iteração atual

		for (auto x = 0;x<NUMERO_FORMIGAS;x++) { // salva em um  vetor auxiliar a pontuação e todas as soluções e qual formiga tem essa pontuação
			solucoes->push_back(formigas[x]->getSolucao());
		}

		std::sort(solucoes->begin(), solucoes->end(), sortSolution);

		double f = 0; // f e g sao parametros para se atualizar o numero de feromonios
		double g = 0;
		double At = 0; // delta tau
		double nAt = 0; // negação para o delta tau

		for (auto i = 0;i<10;i++) { // aumenta o nivel de feromonios para as 10 melhores soluções

	        // calculo do f
			if (solucoes->at(i)->getHardConstraints() == 0)
				f = 200;
			else
				f = 100 / solucoes->at(i)->getHardConstraints();

			// calculo do g
			if (solucoes->at(i)->getSoftConstraints() == 0)
				g = 2000;
			else
				g = 1000 / solucoes->at(i)->getSoftConstraints();


			At = f*g; // formula para calcular o delta tau
			//cout << At << endl;
			for (auto j = 0;j<MetaHeuristicas::NUM_TURMA;j++) { // calcula e aumenta o numero de feromonios
				if (!solucoes->at(i)->getTurmas()->at(j)->getHorarioFixo()) {
					auto h1 = solucoes->at(i)->getTurmas()->at(j)->getDia1() * NUMERO_HORAS_DIA + solucoes->at(i)->getTurmas()->at(j)->getHorario1();
					auto h2 = -1;
					if(solucoes->at(i)->getTurmas()->at(j)->getDia2() != -1)
						h2 = solucoes->at(i)->getTurmas()->at(j)->getDia2() * NUMERO_HORAS_DIA + solucoes->at(i)->getTurmas()->at(j)->getHorario2();
					nAt = (1 - pow((1 - Y), classificaElementoSC(solucoes->at(i)->getTurmas()->at(j))))*f*g;
					Ant::setFeromoniosHorario1(Ant::getFeromoniosHorario1(j, h1) + At - nAt, j, h1);
					if(h2 != -1)
						Ant::setFeromoniosHorario2(Ant::getFeromoniosHorario2(j, h2) + At - nAt, j, h2);
				}
			}
		}

		// faz a evaporação dos feromonios
		for (auto i = 0;i<MetaHeuristicas::NUM_TURMA;i++) {
			for (auto j = 0;j<NUMERO_HORAS_TOTAL;j++) {
				auto t = floor((1 - P)*Ant::getFeromoniosHorario1(i, j));
				if (t>0)
					Ant::setFeromoniosHorario1(t, i, j);
				else
					Ant::setFeromoniosHorario1(0, i, j);

				t = floor((1 - P)*Ant::getFeromoniosHorario2(i, j));
				if (t>0)
					Ant::setFeromoniosHorario2(t, i, j);
				else
					Ant::setFeromoniosHorario2(0, i, j);
			}
		}

		
		for (auto i = 0; i < solucoes->size(); i++) {
			if (populacaoSolucoes->size() >= NUM_SOLUCOES) {
				for (auto j = 0; j < NUM_SOLUCOES; j++) {
					if (sortSolution(solucoes->at(i), populacaoSolucoes->at(j))) {
						auto s = new Solucoes();
						s->setHardConstraints(solucoes->at(i)->getHardConstraints());
						s->setSoftConstraints(solucoes->at(i)->getSoftConstraints());
						for (auto k = 0; k < MetaHeuristicas::NUM_TURMA; k++) {
							s->getTurmas()->push_back(new Turma(solucoes->at(i)->getTurmas()->at(k)->getId(),
																solucoes->at(i)->getTurmas()->at(k)->getCodigo(),
																solucoes->at(i)->getTurmas()->at(k)->getTurno(),
																solucoes->at(i)->getTurmas()->at(k)->getDisciplina(),
																solucoes->at(i)->getTurmas()->at(k)->getDocente(),
																solucoes->at(i)->getTurmas()->at(k)->getHorarioFixo(),
																solucoes->at(i)->getTurmas()->at(k)->getDia1(),
																solucoes->at(i)->getTurmas()->at(k)->getHorario1(),
																solucoes->at(i)->getTurmas()->at(k)->getDia2(),
																solucoes->at(i)->getTurmas()->at(k)->getHorario2()));
						}
						populacaoSolucoes->insert(populacaoSolucoes->begin() + j, s);
						s = populacaoSolucoes->at(NUM_SOLUCOES);
						delete s;
						populacaoSolucoes->erase(populacaoSolucoes->begin() + NUM_SOLUCOES);
						break;
					}
				}
			}
			else {
				auto s = new Solucoes();
				s->setHardConstraints(solucoes->at(i)->getHardConstraints());
				s->setSoftConstraints(solucoes->at(i)->getSoftConstraints());
				for (auto k = 0; k < MetaHeuristicas::NUM_TURMA; k++) {
					s->getTurmas()->push_back(new Turma(solucoes->at(i)->getTurmas()->at(k)->getId(),
														solucoes->at(i)->getTurmas()->at(k)->getCodigo(),
														solucoes->at(i)->getTurmas()->at(k)->getTurno(),
														solucoes->at(i)->getTurmas()->at(k)->getDisciplina(),
														solucoes->at(i)->getTurmas()->at(k)->getDocente(),
														solucoes->at(i)->getTurmas()->at(k)->getHorarioFixo(),
														solucoes->at(i)->getTurmas()->at(k)->getDia1(),
														solucoes->at(i)->getTurmas()->at(k)->getHorario1(),
														solucoes->at(i)->getTurmas()->at(k)->getDia2(),
														solucoes->at(i)->getTurmas()->at(k)->getHorario2()));
				}
				populacaoSolucoes->push_back(s);
				ordenaSolucao();
			}
		}

		// controla quando a formiga converge
		if (bestDFTAtual > solucoes->at(0)->getHardConstraints()) {
				bestDFTAtual = solucoes->at(0)->getHardConstraints();
			}

		if (bestDFTAtual == solucoes->at(0)->getHardConstraints()) { // compara se a melhor solução se repete
			freeze++;
			if (freeze == CONTROLA_OTIMO_LOCAL) { // se ela se repete n vezes ele tenta normaliza-las
				double maxFeromoniosH1 = 0; // armazena o maior feromonio dos horarios
				double mediaFeromoniosH1 = 0; // armazena a media de feromonios das horas
				double maxFeromoniosH2 = 0; // armazena o maior feromonio dos horarios
				double mediaFeromoniosH2 = 0; // armazena a media de feromonios das horas

				for (auto i = 0;i<MetaHeuristicas::NUM_TURMA;i++) { // calcula o somatorio dos feromonios e o maior feromonios
					for (auto j = 0; j<NUMERO_HORAS_TOTAL;j++) {
						mediaFeromoniosH1 = Ant::getFeromoniosHorario1(i, j) + mediaFeromoniosH1;
						if (maxFeromoniosH1 < Ant::getFeromoniosHorario1(i, j)) {
							maxFeromoniosH1 = Ant::getFeromoniosHorario1(i, j);
						}

						mediaFeromoniosH2 = Ant::getFeromoniosHorario2(i, j) + mediaFeromoniosH2;
						if (maxFeromoniosH2 < Ant::getFeromoniosHorario2(i, j)) {
							maxFeromoniosH2 = Ant::getFeromoniosHorario2(i, j);
						}
					}

				}
				// faz a media dos feromonios
				mediaFeromoniosH1 = mediaFeromoniosH1 / (MetaHeuristicas::NUM_TURMA*NUMERO_HORAS_TOTAL);
				mediaFeromoniosH2 = mediaFeromoniosH2 / (MetaHeuristicas::NUM_TURMA*NUMERO_HORAS_TOTAL);

				for (auto i = 0;i<MetaHeuristicas::NUM_TURMA;i++) { // utiliza a formula para normalizar todos os feromonios
					for (auto j = 0; j<NUMERO_HORAS_TOTAL;j++) {
						Ant::setFeromoniosHorario1(((Ant::getFeromoniosHorario1(i, j) - mediaFeromoniosH1)*((mediaFeromoniosH1*W) / maxFeromoniosH1) + mediaFeromoniosH1), i, j);
						Ant::setFeromoniosHorario2(((Ant::getFeromoniosHorario2(i, j) - mediaFeromoniosH2)*((mediaFeromoniosH2*W) / maxFeromoniosH2) + mediaFeromoniosH2), i, j);
					}

				}
				freeze = 0;
				bestDFTAtual = 100000;
			}

		}
		else { // caso contrario ele reseta o freeze pois as solução nao se repetiu
			freeze = 0;
		}

		if (bestDFTGlobal > populacaoSolucoes->at(0)->getHardConstraints()) {
			freeze2 = 0;
			bestDFTGlobal = populacaoSolucoes->at(0)->getHardConstraints();
		}

		if (bestDFTGlobal == populacaoSolucoes->at(0)->getHardConstraints()) { // compara se a melhor solução se repete
			freeze2++;
			if (freeze2 == CONTROLA_MAXIMO_OTIMO_LOCAL) { // se ela se repete n vezes ele tenta normaliza-las
				double maxFeromoniosH1 = 0; // armazena o maior feromonio dos horarios
				double mediaFeromoniosH1 = 0; // armazena a media de feromonios das horas
				double maxFeromoniosH2 = 0; // armazena o maior feromonio dos horarios
				double mediaFeromoniosH2 = 0; // armazena a media de feromonios das horas

				for (auto i = 0;i<MetaHeuristicas::NUM_TURMA;i++) { // calcula o somatorio dos feromonios e o maior feromonios
					for (auto j = 0; j<NUMERO_HORAS_TOTAL;j++) {
						mediaFeromoniosH1 = Ant::getFeromoniosHorario1(i, j) + mediaFeromoniosH1;
						if (maxFeromoniosH1 < Ant::getFeromoniosHorario1(i, j)) {
							maxFeromoniosH1 = Ant::getFeromoniosHorario1(i, j);
						}

						mediaFeromoniosH2 = Ant::getFeromoniosHorario2(i, j) + mediaFeromoniosH2;
						if (maxFeromoniosH2 < Ant::getFeromoniosHorario2(i, j)) {
							maxFeromoniosH2 = Ant::getFeromoniosHorario2(i, j);
						}
					}

				}
				// faz a media dos feromonios
				mediaFeromoniosH1 = mediaFeromoniosH1 / (MetaHeuristicas::NUM_TURMA*NUMERO_HORAS_TOTAL);
				mediaFeromoniosH2 = mediaFeromoniosH2 / (MetaHeuristicas::NUM_TURMA*NUMERO_HORAS_TOTAL);

				for (auto i = 0;i<MetaHeuristicas::NUM_TURMA;i++) { // utiliza a formula para normalizar todos os feromonios
					for (auto j = 0; j<NUMERO_HORAS_TOTAL;j++) {
						Ant::setFeromoniosHorario1(((Ant::getFeromoniosHorario1(i, j) - mediaFeromoniosH1)*((mediaFeromoniosH1*W) / maxFeromoniosH1) + mediaFeromoniosH1), i, j);
						Ant::setFeromoniosHorario2(((Ant::getFeromoniosHorario2(i, j) - mediaFeromoniosH2)*((mediaFeromoniosH2*W) / maxFeromoniosH2) + mediaFeromoniosH2), i, j);
					}

				}
				freeze2 = 0;
				bestDFTGlobal = 100000;
			}

		}

		else { // caso contrario ele reseta o freeze pois as solução nao se repetiu
			freeze2 = 0;
		}
		
		cout << "DFT:" << solucoes->at(0)->getHardConstraints()
			 << "  SCP:" << solucoes->at(0)->getSoftConstraints()
			 << "  bestDFT:" << populacaoSolucoes->at(0)->getHardConstraints()
			 << "  bestSCP:" << populacaoSolucoes->at(0)->getSoftConstraints()
			 << "  numero de stagnacoes:" << freeze
			 << "  numero de stagnacoes globais:" << freeze2
			 << "  numero de iteracoes:" << iatual << endl;

		for (auto i = 0; i < NUMERO_FORMIGAS; i++) {
			delete solucoes->at(i);
			formigas.at(i)->setSolucao(new Solucoes());
		}
		//delete solucoes;
		iatual++;

	}

	cout << "  bestDFT:" << populacaoSolucoes->at(0)->getHardConstraints()
		<< "  bestSCP:" << populacaoSolucoes->at(0)->getSoftConstraints() << endl;

	timer.stop();
}

int ACO::getProxItemPermutado(vector<vector<double> > &omega, int j, int tamanhoVetor, int pos) {
	double o = 0, rnd = 0, m;
	int q;

	for (auto k = pos; k < tamanhoVetor; k++) {
		o += omega.at(k).at(0);
	}

	if (j > pos) {
		for (auto k = pos; k <= j; k++) {
			if (o>E) {
				rnd = o*(static_cast<double>(rand()) / RAND_MAX) + 0.1;
				q = pos;
				m = 0;
				while (m<rnd && q<tamanhoVetor) {
					m = m + omega.at(q).at(0);
					q++;
				}

				o = o - omega.at(q - 1).at(0);
				swap(omega, q - 1, k);
			}
			else {
				swap(omega, rand() % (pos - k + 1) + k, k); // gera numeros randomicos entre pos e k
			}

			pos++;
		}
	}
	return pos;
}

int ACO::roleta(vector<vector<double> > &omega, int tamanhoVetor) {
	auto roleta = vector<int>();
	auto totalFeromonios = 0.0;
	auto qtdCasasRoleta = 0.0;
	for (auto i = 0; i < tamanhoVetor; i++) {
		totalFeromonios += omega.at(i).at(0);
	}
	
	for (auto i = 0; i < tamanhoVetor; i++) {
		if (omega.at(i).at(0) <= 1) {
			roleta.push_back(static_cast<int>(omega.at(i).at(1)));
		}
		else {
			qtdCasasRoleta = ceil((omega.at(i).at(0) * 100) / totalFeromonios);
			for (auto j = 0; j < qtdCasasRoleta; j++)
				roleta.push_back(static_cast<int>(omega.at(i).at(1)));
		}
	}
	if(roleta.size() == 0){
		return rand() % NUMERO_HORAS_TOTAL;
	}
	int rnd = rand() % roleta.size();
	
	//cout << roleta[rnd] << " " << rnd << " " << roleta.size() << " " << tamanhoVetor << " " << totalFeromonios << endl;

	return roleta[rnd];
}


int ACO::torneio(vector<vector<double> > &omega, int tamanhoVetor) {
	int selecionado[D];

	for (auto i = 0; i < D; i++)
	{
		selecionado[i] = rand() % tamanhoVetor;
	}

	auto ganhador = selecionado[0];

	double chanceC = rand() % 100;
	if (chanceC > C) {
		for (auto i = 1; i < D; i++) {
			if (omega.at(selecionado[i]).at(0) > omega.at(ganhador).at(0))
				ganhador = selecionado[i];
		}
	}
	else {
		for (auto i = 1; i < D; i++) {
			if (omega.at(selecionado[i]).at(0) < omega.at(ganhador).at(0))
				ganhador = selecionado[i];
		}
	}

	return omega.at(ganhador).at(1);
}

void ACO::swap(vector<vector<double> > &omega, int i, int j) {
	auto k = omega.at(i).at(0);
	omega.at(i).at(0) = omega.at(j).at(0);
	omega.at(j).at(0) = k;
	k = omega.at(i).at(1);
	omega.at(i).at(1) = omega.at(j).at(1);
	omega.at(j).at(1) = k;
}