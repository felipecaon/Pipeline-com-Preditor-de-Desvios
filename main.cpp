#include <iostream>
#include "LeituraASM.h"


struct instr{
    string opCode, Op1, Op2, Op3;
    int valido;
} dados;

int main() {
    LeituraASM arquivo("teste.txt");
    arquivo.analizarLinha(1);


    if(arquivo.lerDadosLinha(0).find(':') != string::npos){
        dados.opCode = arquivo.lerDadosLinha(1);
        dados.Op1 = arquivo.lerDadosLinha(2);
        dados.Op2 = arquivo.lerDadosLinha(3);
        dados.Op3 = arquivo.lerDadosLinha(4);
    }else{
        dados.opCode = arquivo.lerDadosLinha(0);
        dados.Op1 = arquivo.lerDadosLinha(1);
        dados.Op2 = arquivo.lerDadosLinha(2);
        dados.Op3 = arquivo.lerDadosLinha(3);
    }

    cout << "OpCode: " << dados.opCode << endl
         << "Op1: " << dados.Op1 << endl
         << "Op2: " << dados.Op2 << endl
         << "Op3: " << dados.Op3 << endl;

}