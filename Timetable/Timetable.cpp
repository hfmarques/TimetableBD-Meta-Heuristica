// Timetable.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <time.h>
#include <iostream>

#include "ACO.h"
#include "HHS.h"

int main()
{
	auto* hhs = new HHS();
	auto* aco = new ACO();	
	
	srand(time(nullptr));
	
	hhs->inicializaDados();

	/*aco->setIteradorSolucoes();
	aco->setW(0.3);
	aco->setP(0.5);
	aco->setY(0.2);

	for (auto j = 5; j < 10; j++) {
		aco->setW(0.1);
		for (auto k = 0; k < 10; k++) {
			aco->executa();

			aco->imprimeSolucao();
			aco->escreveSolucao();

			aco->setIteradorSolucoes(aco->getIteradorSolucoes() + 1);
			aco->setW(aco->getW() + 0.1);
		}
		aco->setP(aco->getP() + 0.1);
	}


	for (auto i = 0; i < 10; i++){
		aco->setP(0.1);
		for (auto j = 0; j < 10; j++){
			aco->setW(0.1);
			for (auto k = 0; k < 10; k++){				
				aco->executa();

				aco->imprimeSolucao();
				aco->escreveSolucao();

				aco->setIteradorSolucoes(aco->getIteradorSolucoes() + 1);
				aco->setW(aco->getW() + 0.1);
			}
			aco->setP(aco->getP() + 0.1);
		}
		aco->setY(aco->getY() + 0.1);
	}*/
	aco->executa();
	hhs->executa();

	hhs->imprimeSolucao();
	hhs->escreveSolucao();

	system("pause");

	return 0;
}

