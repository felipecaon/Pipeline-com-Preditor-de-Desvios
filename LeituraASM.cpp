//
// Created by felipe on 27/03/18.
//


#include "LeituraASM.h"

LeituraASM::LeituraASM(const string &nomeArquivo) : nomeArquivo(nomeArquivo) {
    arquivo.open(nomeArquivo);
}

const string &LeituraASM::getLinhaASerLida() const {
    return linhaASerLida;
}

void LeituraASM::fetch(int linha, LeituraASM &arq) {
    if (arquivo.is_open()) {
        //busca linha a ser lida de acordo com o valor do PC
        for(i; i <= linha; i++) {
            getline(arquivo, linhaASerLida);
            if (linhaASerLida.empty()) {
                //j++;
                infos.clear();
                for(int p = 1; p <= 5; p++) {
                    infos.emplace_back("");
                }
            }
        }
    }
    if (!linhaASerLida.empty()) {
        istringstream iss(getLinhaASerLida());

        //splita a linha de modo a remover espaços e vírgulas
        infos.clear();
        do {
            string subs;
            iss >> subs;
            subs.erase(remove(subs.begin(), subs.end(), ','), subs.end());
            infos.push_back(subs);
        } while (iss);
    }

    if (arq.lerDadosLinha(0).find(':') != string::npos) {
        passo_search.hasLabel = true;
        passo_search.opCode = arq.lerDadosLinha(1);
        passo_search.Op1 = arq.lerDadosLinha(2);
        passo_search.Op2 = arq.lerDadosLinha(3);
        passo_search.Op3 = arq.lerDadosLinha(4);
    } else {
        passo_search.hasLabel = false;
        passo_search.opCode = arq.lerDadosLinha(0);
        passo_search.Op1 = arq.lerDadosLinha(1);
        passo_search.Op2 = arq.lerDadosLinha(2);
        passo_search.Op3 = arq.lerDadosLinha(3);
    }

}

string LeituraASM::lerDadosLinha(int posicao) {
    int i = 0;
    for (auto it = infos.begin(); it != infos.end(); it++, i++) {
        if (i == posicao) {
            return *it;
        }
    }
}

void LeituraASM::decode() {

    zerarRegsAuxs();

    if(passo_search.opCode.empty()){
        RAuxiliares[0] = 0;
        RAuxiliares[1] = 0;
        RAuxiliares[2] = 0;
    }else if(passo_search.opCode == "j"){
        RAuxiliares[0] = stoi(passo_search.Op1);
        RAuxiliares[1] = 0;
        RAuxiliares[2] = 0;
    }else if(passo_search.opCode == "beqz" || passo_search.opCode == "bnez"){
        RAuxiliares[0] = stoi(passo_search.Op1);
        RAuxiliares[1] = stoi(passo_search.Op2);
        RAuxiliares[2] = 0;
    }else{
        RAuxiliares[0] = stoi(passo_search.Op1);
        RAuxiliares[1] = stoi(passo_search.Op2);
        RAuxiliares[2] = stoi(passo_search.Op3);
    }


    passo_decode = passo_search;



}

void LeituraASM::execute() {

    passo_execute = passo_decode;

    if (passo_execute.opCode == "daddi") {
        R[RAuxiliares[0]] = R[RAuxiliares[1]] + RAuxiliares[2];
        return;
    }

    if (passo_execute.opCode == "dsubi") {
        R[RAuxiliares[0]] = R[RAuxiliares[1]] - RAuxiliares[2];
        return;
    }

    if (passo_execute.opCode == "dadd") {
        R[RAuxiliares[0]] = R[RAuxiliares[1]] + R[RAuxiliares[2]];
        return;
    }

    if (passo_execute.opCode == "dsub") {
        R[RAuxiliares[0]] = R[RAuxiliares[1]] - R[RAuxiliares[2]];
        return;
    }

    if (passo_execute.opCode == "j") {
         PC = RAuxiliares[0];
        return;
    }

    if(!isPredicaoBimodalLigada()){
        if(passo_execute.opCode == "beq") {
            isBranch = true;
            if (R[RAuxiliares[0]] == R[RAuxiliares[1]]) {
                PCAux = RAuxiliares[2];
                predicaoCertaContador++;
            } else {
                PCAux = RAuxiliares[2];
                predicaoCertaContador++;
                predicaoErradaContador++;
            }
            return;
        }

        if(passo_execute.opCode == "beqz") {
            isBranch = true;
            if (R[RAuxiliares[0]] == 0) {
                PCAux = RAuxiliares[1];
                predicaoCertaContador++;
            } else {
                PCAux = RAuxiliares[2];
                predicaoCertaContador++;
                predicaoErradaContador++;
            }
            return;
        }

        if(passo_execute.opCode == "bnez"){
            isBranch = true;
            if(R[RAuxiliares[0]] != 0){
                PCAux = RAuxiliares[1];
                predicaoCertaContador++;
            } else{
                PCAux = RAuxiliares[2];
                predicaoCertaContador++;
                predicaoErradaContador++;
            }
            return;
        }

        if(passo_execute.opCode == "bne") {
            isBranch = true;
            if (R[RAuxiliares[0]] != R[RAuxiliares[1]]) {
                PCAux = RAuxiliares[2];
                predicaoCertaContador++;
            } else {
                PCAux = RAuxiliares[2];
                predicaoCertaContador++;
                predicaoErradaContador++;
            }
            return;
        }
    }

    if(passo_execute.opCode == "beq") {
        isBranch = true;
        if (R[RAuxiliares[0]] == R[RAuxiliares[1]]) {
            PCAux = RAuxiliares[2];
        } else {
            PCAux = PC + 4;
        }
        return;
    }

    if(passo_execute.opCode == "beqz") {
        isBranch = true;
        if (R[RAuxiliares[0]] == 0) {
            PCAux = RAuxiliares[1];
        } else {
            PCAux = PC + 4;
        }
        return;
    }

    if(passo_execute.opCode == "bnez"){
        isBranch = true;
        if(R[RAuxiliares[0]] != 0){
            PCAux = RAuxiliares[1];
        } else{
            PCAux = PC + 4;
        }
        return;
    }

    if(passo_execute.opCode == "bne") {
        isBranch = true;
        if (R[RAuxiliares[0]] != R[RAuxiliares[1]]) {
            PCAux = RAuxiliares[2];
        } else {
            PCAux = PC + 4;
        }
        return;
    }

}

const int *LeituraASM::getR() const {
    return R;
}

void LeituraASM::zerarRegsAuxs() {
    RAuxiliares[0] = 0;
    RAuxiliares[1] = 0;
    RAuxiliares[2] = 0;
}

void LeituraASM::memoria() {

    passo_memory = passo_execute;
}

void LeituraASM::WriteBack() {
    passo_register = passo_memory;

    if(!passo_register.opCode.empty()){
        contadorInstr++;
    }

}

int LeituraASM::getContadorInstr() const {
    return contadorInstr;
}

int LeituraASM::getPC() const {
    return PC;
}

void LeituraASM::setPC(int PC) {
    LeituraASM::PC = PC;
}

void LeituraASM::incrementarPC() {
    PC++;
}

bool LeituraASM::isIsBranch() const {
    return isBranch;
}

void LeituraASM::setIsBranch(bool isBranch) {
    LeituraASM::isBranch = isBranch;
}

void LeituraASM::PosChecagem() {


    passo_decode.opCode = "";
    passo_decode.Op1 = "";
    passo_decode.Op2 = "";
    passo_decode.Op3 = "";
    RAuxiliares[0,1,2] = 0;

    incrementarPC();

}

int LeituraASM::getPCAux() const {
    return PCAux;
}

bool LeituraASM::isPredicaoBimodalLigada() const {
    return predicaoBimodalLigada;
}

int LeituraASM::getPredicaoCertaContador() const {
    return predicaoCertaContador;
}

int LeituraASM::getPredicaoErradaContador() const {
    return predicaoErradaContador;
}

void LeituraASM::mostrarEstatisticas() {
    cout << "Instrucoes executadas: " << getContadorInstr() << endl;
    cout << "Predicoes Corretas: " << getPredicaoCertaContador() << endl;
    cout << "Predicoes Erradas: " << getPredicaoErradaContador() << endl;
    cout << "Instrucoes Invalidas: " << getPredicaoErradaContador() << endl;
    cout << endl;
}


