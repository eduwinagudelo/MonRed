#include <iostream>
#include <vector>
#include <cstdlib>
#include "include/CPlano.h"
#include "include/CUtiles.h"
#include "include/CConexion.h"
#include "Monred.h"


using namespace std;
using namespace SBI;

int main(int argc, char** argv)
{
    CPlano config;
    string strPath; // Ruta del archivo
    string strRDest;
    ifstream ifs;
    string strlin;
    long numLin = 0;
    vector<CIpVal> vIpAdd;
    vector<CIpVal> vIpBd;
    map<string, string> mpAnalisis;
    map<string, string>::iterator it;
    bool registrar = false;
    unsigned int idEje = 0;
    int itmp;


    if(argc < 2){
        ayuda();
        return convRetorno(ERETORNOS::EARGS);
    }
    if(getenv("CCONF") == NULL){
        cout<<"No se tiene acceso a la variable de entorno del archivo de configuracion\n"<<endl;
        return convRetorno(ERETORNOS::ECONF);
    }
    config.setRuta(string(getenv("CCONF")));
    if(config.cargar() < 1){
        cout<<"No se puede cargar la configuracion del archivo [" << string(getenv("CCONF")) <<"]" <<endl;
        return convRetorno(ERETORNOS::ECONF);
    }
    cout << "Parametros cargados correctamente" << endl;

    // ahora hago set a la ruta del archivo
    strPath = string(argv[1]);
    if(CUtiles::valAcceso(strPath.c_str()) == 1){
        cout<< "No se tiene acceso al archivo de entrada [" << strPath << "], por favor valide"<<endl;
        return convRetorno(ERETORNOS::EARCH);
    }

    // Ahora procedo a realizar la lectura del archivo
    ifs.open(strPath);
    if(!ifs.is_open()){
        cout<<"No se puede abrir el archivo [" << strPath <<"], por favor valide"<<endl;
        return convRetorno(ERETORNOS::EARCH);
    }

    while(!ifs.eof()){
        getline(ifs,strlin);
        numLin++;
        cout<<"Linea "<<numLin<<":"<<strlin<<endl;
        if(strlin.length() < 2)
            continue;
        CIpVal tmpIp(strlin);
        vIpAdd.push_back(tmpIp);
    }
    ifs.close();
    for(unsigned int i = 0; i < vIpAdd.size() ; i++){
        vIpAdd[i].printAddress();
    }

    // Ahora debo conectarme a la BD
    CConexion myconn;
    myconn.Setdbparams( config.getParam("server"), config.getParam("user"), config.getParam("pass"), config.getParam("dbname"), strtoul(config.getParam("port").c_str(), NULL, 0) );
    if(myconn.conectar() != 1){
        cout<<"No se puede iniciar la conexion a la base de datos."<<endl;
        return convRetorno(ERETORNOS::ENODB);
    }
    cout <<"Version del motor:"<< myconn.traerVersionMotor() <<"."<<endl;
    itmp = myconn.traerDirecciones(vIpBd);
    if( static_cast<unsigned int>(itmp) != vIpAdd.size() ){
        cout<<"Diferente cantidad, hay que registrar."<<endl;
        registrar = true;
    }

    if(!registrar){
        if(validarDirecciones(vIpAdd, vIpBd) != 1){
            cout<<"Diferentes IP, hay que registrar."<<endl;
            cout<<"Ip en la ejecucion:"<<endl;
            for(auto val : vIpAdd){
                val.printAddress();
            }
            cout<<"Ip en el servidor:"<<endl;
            for(auto val : vIpBd){
                val.printAddress();
            }
            registrar = true;
        }
    }

    if(!registrar){
        cout <<"Validando las IP contra la base de datos."<<endl;
        for(unsigned int i = 0; i < vIpAdd.size() ; i++){
            if(!myconn.validarIP(vIpAdd[i])){
                cout<<"Esta IP no esta -> " << vIpAdd[i].Getipadd() << "." << endl;
                registrar = true;
            }
        }
    }

    // Si de la validacion salio algo
    if(registrar){
        cout << "Analizando las direcciones." <<endl;
        mpAnalisis = analisisDireccion(vIpAdd, vIpBd);
        for(it = mpAnalisis.begin(); it != mpAnalisis.end(); ++it){
            cout<<it->first <<":"<<it->second << "." << endl;
        }
        cout<<"Registrando."<<endl;
        idEje = myconn.registrarEjecucion(strPath);
        if(idEje != 0){
            for(unsigned int i = 0; i < vIpAdd.size() ; i++){
                if(myconn.registrarDireccion(vIpAdd[i], idEje) != 1){
                    cout<<"Esta IP no se pudo registrar -> " << vIpAdd[i].Getipadd() << endl;
                }
                cout<<"Esta IP registrada correctamente -> " << vIpAdd[i].Getipadd() << endl;
            }
        }
        return convRetorno(ERETORNOS::ENVMAIL);
    }
    return convRetorno(ERETORNOS::OK);
}
