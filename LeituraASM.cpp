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

bool LeituraASM::fetch(int linha) {
    int i = 0;
    if (arquivo.is_open()) {
        //busca linha a ser de acordo com o valor do PC
        i++;
        getline(arquivo, linhaASerLida);
        if (linhaASerLida.empty()) {
            return true;
        }
    }

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

string LeituraASM::lerDadosLinha(int posicao) {
    int i = 0;
    for (auto it = infos.begin(); it != infos.end(); it++, i++) {
        if (i == posicao) {
            return *it;
        }
    }
}

void LeituraASM::decode(LeituraASM &arq) {

    zerarRegsAuxs();

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

    RAuxiliares[0] = stoi(passo_search.Op1);
    RAuxiliares[1] = stoi(passo_search.Op2);
    RAuxiliares[2] = stoi(passo_search.Op3);

    passo_decode = passo_search;



}

void LeituraASM::execute() {

    passo_execute = passo_decode;

    if(passo_execute.opCode == "daddi"){
        //R[RAuxiliares[0]] = R[RAuxiliares[1]] + RAuxiliares[2];
        RAuxiliares[0] = R[RAuxiliares[1]] + RAuxiliares[2];
    }

    if(passo_execute.opCode == "dsubi"){
        //R[RAuxiliares[0]] = R[RAuxiliares[1]] + RAuxiliares[2];
        RAuxiliares[0] = R[RAuxiliares[1]] - RAuxiliares[2];
    }

    if(passo_execute.opCode == "dadd"){
        //R[RAuxiliares[0]] = R[RAuxiliares[1]] + RAuxiliares[2];
        RAuxiliares[0] = R[RAuxiliares[1]] + R[RAuxiliares[2]];
    }

    if(passo_execute.opCode == "dsub"){
        //R[RAuxiliares[0]] = R[RAuxiliares[1]] + RAuxiliares[2];
        RAuxiliares[0] = R[RAuxiliares[1]] - R[RAuxiliares[2]];
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

    R[stoi(passo_memory.Op1)] = R[stoi(passo_memory.Op1)] + RAuxiliares[0];

}

const int *LeituraASM::getRAuxiliares() const {
    return RAuxiliares;
}

