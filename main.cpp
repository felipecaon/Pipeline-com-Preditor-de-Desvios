#include <iostream>
#include "LeituraASM.h"

//struct para instrucoes
struct instr {
    string opCode, Op1, Op2, Op3;
    int valido;
} estagio[5];

//variaveis auxiliares
int PC = 1;
unsigned int R[32];
int RAuxiliares[2];


int main() {

    LeituraASM arquivo("teste2.txt");

    while (!arquivo.analizarLinha(PC)) {

        cout << PC << endl;
        if (arquivo.lerDadosLinha(0).find(':') != string::npos) {
            estagio[0].opCode = arquivo.lerDadosLinha(1);
            estagio[0].Op1 = arquivo.lerDadosLinha(2);
            estagio[0].Op2 = arquivo.lerDadosLinha(3);
            estagio[0].Op3 = arquivo.lerDadosLinha(4);
        } else {
            estagio[0].opCode = arquivo.lerDadosLinha(0);
            estagio[0].Op1 = arquivo.lerDadosLinha(1);
            estagio[0].Op2 = arquivo.lerDadosLinha(2);
            estagio[0].Op3 = arquivo.lerDadosLinha(3);
        }
        PC++;
        cout << "OpCode: " << estagio[0].opCode << endl
             << "Op1: " << estagio[0].Op1 << endl
             << "Op2: " << estagio[0].Op2 << endl
             << "Op3: " << estagio[0].Op3 << endl;
    }
}