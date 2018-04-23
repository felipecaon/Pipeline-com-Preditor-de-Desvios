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

    string dir = ("/home/felipe/Documentos/pipeline/teste.txt");
    ifstream arquivo;
    vector<string> infos;
    string nomeArquivo;
    string linhaASerLida;
    int RAuxiliares[3] = {0};
    bool isBranch = false;
    bool predicaoBimodalLigada = false;
    int predicaoCertaContador = 0;
    int predicaoErradaContador = 0;
    bool seraTomado = false;
    bool naoSeraTomado = false;
    int R[32] = {0};
    int i = 1;
    int contadorInstr = 0;
    int PC = 1;
    int PCAux = PC;
    short int contadorBimodal = 0;

public:

    struct instr {
        string opCode = "", Op1, Op2, Op3;
    } passo_search, passo_decode, passo_execute, passo_memory, passo_register;


    //ler informacao da linha do arquivo para
    //isolar informaoes
    const string &getLinhaASerLida() const;

    //construtor que abre o arquivo
    LeituraASM(const string &nomeArquivo);

    LeituraASM();

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
    void incrementarPC();
    void PosChecagem();
    int getPCAux() const;
    bool isPredicaoBimodalLigada() const;

    void setPredicaoBimodalLigada(bool predicaoBimodalLigada);

    void mostrarEstatisticas();
    int getPredicaoCertaContador() const;
    int getPredicaoErradaContador() const;
    void ativarContadorBimodal();

};

/*
 * daddi 1, 0, 100
daddi 4, 0, 20
daddi 5, 0, 150
dadd 2, 2, 4
daddi 3, 3, 1
beq 3, 1, 8
j 4
dadd 2, 0, 2
dadd 2, 2, 5
daddi 6, 6, 1
beq 5, 6, 13
j 9
dadd 2, 2, 0
daddi 8, 0, 152
 */
#endif //PREDITOR2_LEITURAASM_H