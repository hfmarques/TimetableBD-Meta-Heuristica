#include "stdafx.h"
#include "GeracaoGrades.h"


#include<fstream>
#include<iostream>
#include<limits>



GeracaoGrades::GeracaoGrades() {
	aco = NULL;
	hhs = NULL;
}

GeracaoGrades::~GeracaoGrades() {
    //dtor
}



void GeracaoGrades::geraSolucaoACO() {
	aco->executa();
}

void GeracaoGrades::geraSolucaoHHS() {
	hhs->executa();
}

