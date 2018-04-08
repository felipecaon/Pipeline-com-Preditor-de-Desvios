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
#include "iomanip"

using namespace std;

class LeituraASM {
    ifstream arquivo;
    vector<string> infos;
    string nomeArquivo;
    string linhaASerLida;
    int RAuxiliares[3] = {0};
    int R[32] = {0};
    int i = 1;
    int contadorInstr = 0;
    int PC = 1;

public:

    struct instr {
        bool hasLabel = false;
        string opCode = "", Op1, Op2, Op3;
        bool valido = true;
    } passo_search, passo_decode, passo_execute, passo_memory, passo_register;


    //ler informacao da linha do arquivo para
    //isolar informaoes
    const string &getLinhaASerLida() const;

    //construtor que abre o arquivo
    LeituraASM(const string &nomeArquivo);

    //recebe o valor da linha atual do arquivo
    void fetch(int linha, LeituraASM &arq);

    //ler strings da linha atual
    string lerDadosLinha(int posicao);

    //decode na linha a fim de isolar OPCode, Op1, Op2 e OP3
    void decode();

    //executa comandos de acordo com instrução e grava seus elementos em um registrador temporário
    void execute();

    //faz nada
    void memoria();

    //escreve o resultado da operação no registrador final
    void WriteBack();

    //método auxiliares
    void zerarRegsAuxs();
    const int *getR() const;
    int getContadorInstr() const;
    int getPC() const;
    void setPC(int PC);
};


#endif //PREDITOR2_LEITURAASM_H