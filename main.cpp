#include <iostream>
#include "LeituraASM.h"

//variaveis globais
int PC = 0; //linha 1
int contInstr = 0;

int main() {

    LeituraASM arquivo("teste2.txt");

    while (!arquivo.fetch(PC,arquivo)) {
        //cout << arquivo.passo_search.opCode << arquivo.passo_decode.opCode << arquivo.passo_execute.opCode << arquivo.passo_memory.opCode << arquivo.passo_register.opCode << endl;
        arquivo.WriteBack();
        arquivo.memoria();
        arquivo.execute();
        arquivo.decode();
        contInstr++;
        PC++;
    }

    cout << "Instrucoes executadas: " << contInstr/2 << endl;

    for(int i = 0; i <= 22; i++){
        cout << "R[" << i << "] - " << arquivo.getR()[i] << endl;
    }

}