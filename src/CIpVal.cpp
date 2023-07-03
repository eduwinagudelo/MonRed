#include "include/CIpVal.h"

CIpVal::CIpVal()
{
    //ctor
}

CIpVal::CIpVal(string strLin){
    vector<string> vStr;
    CUtiles::split(strLin,' ', vStr);
    this->ipadd = vStr[3];
}

CIpVal::~CIpVal()
{
    //dtor
}

void CIpVal::printAddress(){
    cout<<"Direccion IP:"<<ipadd<<endl;
}

bool CIpVal::operator==(CIpVal& rhs){
    if(ipadd.size() != rhs.Getipadd().size())
        return false;
    if(strcmp( ipadd.c_str() , rhs.Getipadd().c_str() ) != 0)
        return false;
    return true;
}
