#include <iostream>
#include "LeituraASM.h"

//variaveis globais
int PC = 1;
int contInstr = 0;
bool flag = false;

int main() {

    LeituraASM arquivo("teste2.txt");

    while (!arquivo.fetch(PC)) {
        flag = false;
        arquivo.decode(arquivo);
        if(arquivo.passo_execute.opCode == "j"){
            PC = arquivo.getRAuxiliares()[0];
            arquivo.fetch(PC);
            arquivo.decode(arquivo);
            flag = true;
        }
        arquivo.execute();
        arquivo.memoria();
        arquivo.WriteBack();
        if(flag == false){
            PC++;
        }

        contInstr++;
    }

    cout << "Instrucoes executadas: " << contInstr << endl;
    cout << arquivo.getR()[2] << endl;
    cout << arquivo.getR()[6] << endl;
    cout << arquivo.getR()[12] << endl;
    cout << arquivo.getR()[15] << endl;

}
