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
    int RAuxiliares[3] = {0};
    int R[32] = {0};

public:

    struct instr {
        bool hasLabel = false;
        string opCode, Op1, Op2, Op3;
        int valido;
    } passo_search, passo_decode, passo_execute, passo_memory, passo_register;


    //ler informacao da linha do arquivo para
    //isolar informaoes
    const string &getLinhaASerLida() const;

    //construtor que abre o arquivo
    LeituraASM(const string &nomeArquivo);

    //recebe o valor da linha atual do arquivo
    bool analizarLinha(int linha);

    //ler strings da linha atual
    string lerDadosLinha(int posicao);


    void decode(struct instr& str, LeituraASM &arq);

    void execute();

    void memoria();

    void WriteBack();

    void zerarRegsAuxs();

    const int *getRAuxiliares() const;

    const int *getR() const;

};


#endif //PREDITOR2_LEITURAASM_H
