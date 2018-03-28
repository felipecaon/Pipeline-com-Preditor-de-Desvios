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

bool LeituraASM::analizarLinha(int linha) {
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

