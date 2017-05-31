/*
 * ACO.h
 *
 *  Created on: 22 de fev de 2016
 *      Author: Héber Marques
 */

#ifndef ACO_H_
#define ACO_H_

#include "Ant.h"
#include "MetaHeuristicas.h"

#include<vector>
#include<iostream>

using namespace std;
class ACO : public MetaHeuristicas {
public:
    ACO();
	ACO(int);
    virtual ~ACO();
    void executa();

private:
    int convert(char *str);

	int getProxItemPermutado(vector<vector<double> > &omega, int j, int tamanhoVetor, int pos);
	void swap(vector<vector<double> > &omega, int i, int j);
	int roleta(vector<vector<double> > &omega, int tamanhoVetor);
	int torneio(vector<vector<double> > &omega, int tamanhoVetor);

    inline void setPosH1(int p){ posH1 = p;}
    inline int getPosH1() const{ return posH1;}
    inline void setPosH2(int p){ posH2 = p;}
    inline int getPosH2() const{ return posH2;}

    int posH1; // posição horarios 1
    int posH2; // posição horarios 2
	
	vector<Ant*> formigas;

};

#endif /* ACO_H_ */
