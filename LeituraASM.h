//
// Created by felipe on 27/03/18.
//

#ifndef PREDITOR2_LEITURAASM_H
#define PREDITOR2_LEITURAASM_H


#include "iostream"
#include "string"
#include "fstream"
#include "sstream"
#include "vector"
#include "algorithm"
#include "iterator"

using namespace std;

class LeituraASM {
    ifstream arquivo;
    vector<string> infos;
    string nomeArquivo;
    string linhaASerLida;
public:

    //ler informacao da linha do arquivo para
    //isolar informaoes
    const string &getLinhaASerLida() const;

    //construtor que abre o arquivo
    LeituraASM(const string &nomeArquivo);

    //recebe o valor da linha atual do arquivo
    bool analizarLinha(int linha);

    //ler strings da linha atual
    string lerDadosLinha(int posicao);


};


#endif //PREDITOR2_LEITURAASM_H
