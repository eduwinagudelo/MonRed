#ifndef MONRED_H_INCLUDED
#define MONRED_H_INCLUDED

#include <iostream>
#include <cstdlib>
#include <bits/stdc++.h>
#include <map>
#include <iterator>
#include "CIpVal.h"

using namespace std;

enum class ERETORNOS{
    OK,
    ENVMAIL,
    EARGS,
    ECONF,
    EARCH,
    ENODB
};

static void ayuda(){
    cout<<"Metodo de invocacion:\n\n\tMonRed <Archivo a validar>\n\nIng. Edwin Agudelo\nSBI\n2018"<<endl;
}

int convRetorno(ERETORNOS val){
    return static_cast<int>(val);
}

bool compararDirecciones(CIpVal a, CIpVal b){
    int i = a.Getipadd().compare(b.Getipadd());
    return( i >= 0 );
}

int validarDirecciones(vector<CIpVal> &va, vector<CIpVal> &vb){
    vector<int> validar;
    int retorno = 0;
    sort(va.begin(), va.end(), compararDirecciones);
    sort(vb.begin(), vb.end(), compararDirecciones);
    for(unsigned int i = 0; i < va.size(); i++){
        cout<< "Validando A:" << va[i].Getipadd() << endl;
        for(unsigned int j = 0; j < vb.size() ; j++){
            if( va[i] == vb[j] ){
                cout<<"Validado B:"<<vb[j].Getipadd() << endl;
                validar.push_back(j);
                break;
            }
        }
    }
    if(validar.size() == va.size())
        retorno = 1;
    return retorno;
}

map<string, string> analisisDireccion(vector<CIpVal> &va, vector<CIpVal> &vb){
    map<string, string> mretorno;
    sort(va.begin(), va.end(), compararDirecciones);
    sort(vb.begin(), vb.end(), compararDirecciones);
    for(unsigned int i = 0; i < va.size(); i++){
        mretorno.insert(pair<string, string>(va[i].Getipadd(), "Direccion Nueva"));
    }
    for(unsigned int i = 0; i < vb.size(); i++){
        if(mretorno.count(vb[i].Getipadd()) == 1){ // la direccion existe
            mretorno[vb[i].Getipadd()] = "Direccion Persistente";
        }
        else{
            mretorno.insert(pair<string, string>(vb[i].Getipadd(), "Direccion Eliminada"));
        }
    }
    return mretorno;

}

#endif // MONRED_H_INCLUDED
