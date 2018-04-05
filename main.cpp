#include <iostream>
#include "LeituraASM.h"

//variaveis globais
int PC = 0; //linha 1
int contInstr = 0;

int main() {

    LeituraASM arquivo("teste2.txt");

    while (!arquivo.fetch(PC)) {
        arquivo.WriteBack();
        arquivo.memoria();
        arquivo.execute();
        arquivo.decode(arquivo);
        contInstr++;
        PC++;
    }

    cout << "Instrucoes executadas: " << contInstr << endl;

    for(int i = 0; i <= 22; i++){
        cout << "R[" << i << "] - " << arquivo.getR()[i] << endl;
    }

}