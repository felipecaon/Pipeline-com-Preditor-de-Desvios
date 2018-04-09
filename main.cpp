#include "LeituraASM.h"

//variaveis globais
int contAux = 1;

int main() {

    LeituraASM arquivo("/home/felipe/Documentos/teste.txt");

    cout << setw(7) << "Fetch" << setw(14) << "Decode" << setw(14) << "Execute" << setw(14) << "Memory" << setw(14) << "WriteBack" << endl;
    cout << setw(7) << "-----------------------------------------------------------------" << endl;

    while (contAux < 12) {
        arquivo.fetch(arquivo.getPC(),arquivo);
        cout << setw(7) << arquivo.passo_search.opCode << setw(14) << arquivo.passo_decode.opCode << setw(14) << arquivo.passo_execute.opCode << setw(14) << arquivo.passo_memory.opCode << setw(14) << arquivo.passo_register.opCode << endl;
        cout << setw(7) << "-----------------------------------------------------------------" << endl;
        arquivo.WriteBack();
        arquivo.memoria();
        arquivo.execute();
        arquivo.decode();
        contAux++;
        if(arquivo.passo_search.opCode == "j"){
            arquivo.setPC(arquivo.getRAuxiliares()[0]);
        }else{
            arquivo.incrementarPC();
        }
    }


    cout << "Instrucoes executadas: " << arquivo.getContadorInstr() << endl;

    for(int i = 0; i <= 8; i++){
        cout << "R[" << i << "] - " << arquivo.getR()[i] << endl;
    }

}