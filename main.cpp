#include <iostream>
#include "LeituraASM.h"

//variaveis globais
int PC = 1;

int main() {

    LeituraASM arquivo("teste2.txt");

    while (!arquivo.analizarLinha(PC)) {


        arquivo.decode(arquivo.passo_search, arquivo);
        arquivo.execute();
        arquivo.memoria();
        arquivo.WriteBack();
        PC++;
    }


    cout << arquivo.getR()[2] << endl << endl;
    cout << arquivo.getR()[6] << endl << endl;
    cout << arquivo.getR()[12] << endl << endl;
    cout << arquivo.getR()[15] << endl << endl;

}