#ifndef CCONEXION_H
#define CCONEXION_H

#include "mysql.h"
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "include/CIpVal.h"

using namespace std;


class CConexion
{
    public:
        CConexion();
        virtual ~CConexion();

        unsigned int Getestado() { return estado; }
        void Setestado(unsigned int val) { estado = val; }
        const int Getrespuesta() { return respuesta; }
        const string GetError(){ return strerror; }
        void Setdbg(unsigned int val){idbg = val;}
        void Setdbparams(string vhost, string vuser, string vpass, string vdbname, unsigned int vpuerto);
        int conectar();
        void desconectar();
        int traerDirecciones(vector<CIpVal> &vecIp);
        bool validarIP(CIpVal val);
        unsigned int registrarEjecucion(string archivo);
        int registrarDireccion(CIpVal val, unsigned int idEje);
        string traerVersionMotor();

    protected:

    private:
        unsigned int estado;
        int respuesta;
        string strerror;
        unsigned int idbg;

        MYSQL *myconn;
        string host;
        string user;
        string pass;
        string dbname;
        unsigned int puerto;

        const string SQL_VALIP = "Select count(1) cantidad from direccion a where eje_id =(Select max(b.eje_id) from ejecucion b) and a.dir_ip = ?";
        const string SQL_INSERT_EJE = "Insert Into ejecucion(eje_registro, eje_archivo) Values(now(),?)";
        const string SQL_INSERT_DIR = "Insert Into direccion(eje_id, dir_ip, dir_registro) values(?,?,now())";
        const string SQL_GETEXE = "Select a.dir_ip from direccion a where a.eje_id =(Select max(b.eje_id) from ejecucion b) order by a.dir_ip";
        const string SQL_GETVERSION = "Select version()";

        void show_error(){
            printf("Error(%d) [%s] \"%s\"\n", mysql_errno(myconn),mysql_sqlstate(myconn),mysql_error(myconn));
        }

        void mensaje(const string strMen){
            if(idbg > 0)
                cout<<strMen<<endl;
        }

};

#endif // CCONEXION_H
