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

    int getProxItemPermutado(double w[][2], int j, int tamanhoVetor, int pos);
    void swap(double w[][2], int i, int j);
	int roleta(double omega[][2], int tamanhoVetor);

    inline void setPosH1(int p){ posH1 = p;}
    inline int getPosH1() const{ return posH1;}
    inline void setPosH2(int p){ posH2 = p;}
    inline int getPosH2() const{ return posH2;}

    int posH1; // posição horarios 1
    int posH2; // posição horarios 2
	
	vector<Ant*> formigas;

};

#endif /* ACO_H_ */
