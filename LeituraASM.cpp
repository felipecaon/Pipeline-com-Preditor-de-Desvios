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

    arquivo.close();
    arquivo.open(dir);

    if(i >= linha){
        linha = i;
    }

    if(arquivo.is_open()) {
        for (int i = 1; i <= linha; i++) {
            getline(arquivo, linhaASerLida);
            if (linhaASerLida.empty()) {
                //j++;
                infos.clear();
                for (int p = 1; p <= 5; p++) {
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
        passo_search.opCode = arq.lerDadosLinha(1);
        passo_search.Op1 = arq.lerDadosLinha(2);
        passo_search.Op2 = arq.lerDadosLinha(3);
        passo_search.Op3 = arq.lerDadosLinha(4);
    } else {
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
         PC = RAuxiliares[0] - 1;
        return;
    }

    if(!isPredicaoBimodalLigada()){
        if(passo_execute.opCode == "beq") {
            isBranch = true;
            if (R[RAuxiliares[0]] == R[RAuxiliares[1]]) {
                PCAux = RAuxiliares[2];
                predicaoCertaContador++;
            } else {
                predicaoErradaContador++;
            }
            return;
        }

        if(passo_execute.opCode == "beqz") {
            isBranch = true;
            if (R[RAuxiliares[0]] == 0) {
                PCAux = RAuxiliares[1];
                predicaoErradaContador++;
            } else {
                predicaoCertaContador++;
            }
            return;
        }

        if(passo_execute.opCode == "bnez"){
            isBranch = true;
            if(R[RAuxiliares[0]] != 0){
                PCAux = RAuxiliares[1];
                predicaoErradaContador++;
            } else{
                predicaoCertaContador++;
            }
            return;
        }

        if(passo_execute.opCode == "bne") {
            isBranch = true;
            if (R[RAuxiliares[0]] != R[RAuxiliares[1]]) {
                PCAux = RAuxiliares[2];
                predicaoErradaContador++;
            } else {
                predicaoCertaContador++;
            }
            return;
        }
    }else{


        if(passo_execute.opCode == "beq" || passo_execute.opCode == "bne" ||
                passo_execute.opCode == "beqz" || passo_execute.opCode == "bnez") {
            LeituraASM arquivoPred;

            abrirPredTableEProcurarPC(PCAuxAux, arquivoPred);


            switch (resposta) {
                case 0 :
                    predicaoNaoTomada++;
                    break;
                case 1 :
                    predicaoNaoTomada++;
                    break;
                case 2 :
                    predicaoTomada++;
                    break;
                case 3 :
                    predicaoTomada++;
                    break;
                default:
                    break;
            }
        }

        if(passo_execute.opCode == "beq") {
            isBranch = true;
            if (R[RAuxiliares[0]] == R[RAuxiliares[1]]) {
                if(resposta == 2 || resposta == 3){
                    predicaoCertaContador++;
                }else{
                    predicaoErradaContador++;
                }
                PCAux = RAuxiliares[2];
            } else {
                if(resposta == 2 || resposta == 3){
                    predicaoErradaContador++;
                }else{
                    predicaoCertaContador++;
                }
                //PCAux = PC + 4;
            }
            return;
        }

        if(passo_execute.opCode == "beqz") {
            isBranch = true;
            if (R[RAuxiliares[0]] == 0) {
                PCAux = RAuxiliares[1];
                contadorBimodal++;
                if(seraTomado){
                    predicaoCertaContador++;
                }else{
                    predicaoErradaContador++;
                }
            } else {
                if(naoSeraTomado){
                    predicaoCertaContador++;
                }else{
                    predicaoErradaContador++;
                }
                if(contadorBimodal <= 0){
                    contadorBimodal = 0;
                }else{
                    contadorBimodal--;
                }
                //PCAux = PC + 4;
            }
            return;
        }

        if(passo_execute.opCode == "bnez"){
            isBranch = true;
            if(R[RAuxiliares[0]] != 0){
                PCAux = RAuxiliares[1];
                contadorBimodal++;
                if(seraTomado){
                    predicaoCertaContador++;
                }else{
                    predicaoErradaContador++;
                }
            } else {
                if(naoSeraTomado){
                    predicaoCertaContador++;
                }else{
                    predicaoErradaContador++;
                }
                if(contadorBimodal <= 0){
                    contadorBimodal = 0;
                }else{
                    contadorBimodal--;
                }
                //PCAux = PC + 4;
            }
            return;
        }

        if(passo_execute.opCode == "bne") {
            isBranch = true;
            if (R[RAuxiliares[0]] != R[RAuxiliares[1]]) {
                PCAux = RAuxiliares[2];
                contadorBimodal++;
                if(seraTomado){
                    predicaoCertaContador++;
                }else{
                    predicaoErradaContador++;
                }
            } else {
                if(naoSeraTomado){
                    predicaoCertaContador++;
                }else{
                    predicaoErradaContador++;
                }
                if(contadorBimodal <= 0){
                    contadorBimodal = 0;
                }else{
                    contadorBimodal--;
                }
                //PCAux = PC + 4;
            }
            return;
        }
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


void LeituraASM::incrementarPC() {
    PC++;
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
    cout << "Predicoes Corretas: " << predicaoCertaContador << endl;
    cout << "Predicoes Erradas: " << predicaoErradaContador << endl;
    cout << "Predito como tomado: " << predicaoTomada << endl;
    cout << "Predito como nao tomado: " << predicaoNaoTomada << endl;

    cout << endl;
}


LeituraASM::LeituraASM() {}

void LeituraASM::ativarContadorBimodal() {

    if(contadorBimodal > 3){
        contadorBimodal = 0;
    }

    if(contadorBimodal == 0 || contadorBimodal == 1){
        naoSeraTomado = true;
    }else if(contadorBimodal == 2 || contadorBimodal == 3){
       seraTomado = true;
    }

}

void LeituraASM::setPredicaoBimodalLigada(bool predicaoBimodalLigada) {
    LeituraASM::predicaoBimodalLigada = predicaoBimodalLigada;
}

void LeituraASM::abrirPredTableEProcurarPC(int PCAuxAux, LeituraASM &arq){

    arquivo.close();
    arquivo.open(dirPred);


    if(arquivo.is_open()) {
        for (int p = 1; p <= PCAuxAux; p++) {
            getline(arquivo, linhaASerLidaPCAux);
        }
    }

    resposta =  stoi(linhaASerLidaPCAux);

    arquivo.close();


}

int LeituraASM::getPCAuxAux() const {
    return PCAuxAux;
}

void LeituraASM::setPCAuxAux(int PCAuxAux) {
    LeituraASM::PCAuxAux = PCAuxAux;
}

