#include "include/CConexion.h"

CConexion::CConexion()
{
    //ctor
    estado = 0;
    respuesta = 0;
    myconn = mysql_init(NULL);
    host = string("127.0.0.1");
    user = string("osmonitor");
    pass = string("osmonitor");
    dbname = string("desarrollos");
    puerto = 3306;
    idbg = 1;
}

CConexion::~CConexion()
{
    //dtor
    desconectar();
}

void CConexion::Setdbparams(string vhost, string vuser, string vpass, string vdbname, unsigned int vpuerto){
    host = vhost;
    user = vuser;
    pass = vpass;
    dbname = vdbname;
    puerto = vpuerto;
}


int CConexion::conectar(){
    int retorno = 0;
    mensaje("Iniciando la conexion");
    if(!mysql_real_connect(myconn, host.c_str(), user.c_str(), pass.c_str(), dbname.c_str(), puerto, NULL, 0)){
        retorno = 2;
        estado = retorno;
        if(idbg > 0)
            show_error();
    }
    if(retorno == 0){
        mensaje("Conexion realizada");
        estado = 1;
        retorno = estado;
    }
    return retorno;
}

void CConexion::desconectar(){
    mensaje("Iniciando desconexion");
    if(estado == 1)
        mysql_close(myconn);
}

int CConexion::traerDirecciones(vector<CIpVal> &vecIp){
    MYSQL_RES *res;
    MYSQL_ROW fila;
    int retorno = 0;

    // Realizo la consulta
    //if(mysql_query(myconn, "Select a.dir_ip from direccion a where eje_id =(Select max(b.eje_id) from ejecucion b)")){
    if(mysql_query(myconn, SQL_GETEXE.c_str())){
        show_error();
        retorno = -1;
    }
    else{
        res = mysql_use_result(myconn);
        if(res == NULL){
            show_error();
            retorno = -2;
        }
        else{
            while( (fila = mysql_fetch_row(res)) ){
                CIpVal iptmp;
                iptmp.Setipadd(string(fila[0]));
                vecIp.push_back(iptmp);
                retorno++;
            }
            mysql_free_result(res);
        }
    }
    return retorno;
}

bool CConexion::validarIP(CIpVal val){
    MYSQL_STMT *stmt;
    MYSQL_BIND vari[1];
    MYSQL_BIND varo[1];
    unsigned int cantidad;

    stmt = mysql_stmt_init(myconn);
    if(stmt == NULL){
        show_error();
        return false;
    }
    if(mysql_stmt_prepare(stmt, SQL_VALIP.c_str(), SQL_VALIP.size() ) ){
        show_error();
        return false;
    }
    memset(vari, 0, sizeof(vari));
    memset(varo, 0, sizeof(varo));

    vari[0].buffer_type = MYSQL_TYPE_STRING;
    vari[0].buffer = (char *)val.Getipadd().c_str();
    vari[0].buffer_length = val.Getipadd().length();

    varo[0].buffer_type = MYSQL_TYPE_LONG;
    varo[0].buffer = &cantidad;

    mysql_stmt_bind_param(stmt, vari);
    mysql_stmt_bind_result(stmt, varo);

    if(mysql_stmt_execute(stmt)){
        show_error();
        return false;
    }
    mysql_stmt_store_result(stmt);
    mysql_stmt_fetch(stmt);
    mysql_stmt_close(stmt);
    if(cantidad != 0)
        return true;
    return false;
}

unsigned int CConexion::registrarEjecucion(string archivo){
    MYSQL_STMT *stmt;
    MYSQL_BIND vari[1];
    unsigned int retorno;

    stmt = mysql_stmt_init(myconn);
    if(stmt == NULL){
        show_error();
        return 0;
    }
    if(mysql_stmt_prepare(stmt, SQL_INSERT_EJE.c_str(), SQL_INSERT_EJE.length() ) ){
        show_error();
        return 0;
    }
    memset(vari, 0, sizeof(vari));
    vari[0].buffer_type = MYSQL_TYPE_STRING;
    vari[0].buffer = (char *)archivo.c_str();
    vari[0].buffer_length = archivo.length();

    mysql_stmt_bind_param(stmt, vari);

    if(mysql_stmt_execute(stmt)){
        show_error();
        return 0;
    }
    retorno = mysql_stmt_insert_id(stmt);
    if(retorno == 0)
        show_error();
    mysql_stmt_close(stmt);
    return retorno;
}

int CConexion::registrarDireccion(CIpVal val, unsigned int idEje){
    int retorno = 1;
    MYSQL_STMT *stmt;
    MYSQL_BIND vari[2];

    stmt = mysql_stmt_init(myconn);
    if(stmt == NULL){
        show_error();
        return 0;
    }
    if(mysql_stmt_prepare(stmt, SQL_INSERT_DIR.c_str(), SQL_INSERT_DIR.length() ) ){
        show_error();
        return 0;
    }
    memset(vari, 0, sizeof(vari));
    vari[0].buffer_type = MYSQL_TYPE_LONG;
    vari[0].buffer = &idEje;
    vari[1].buffer_type = MYSQL_TYPE_STRING;
    vari[1].buffer = (char *)val.Getipadd().c_str();
    vari[1].buffer_length = val.Getipadd().length();

    mysql_stmt_bind_param(stmt, vari);

    if(mysql_stmt_execute(stmt)){
        show_error();
        return 0;
    }
    mysql_stmt_close(stmt);
    return retorno;
}

string CConexion::traerVersionMotor(){
    MYSQL_RES *res;
    MYSQL_ROW fila;
    string retorno = "";

    if(mysql_query(myconn, SQL_GETVERSION.c_str() )){
        show_error();
        retorno = "-1";
    }
    else{
        res = mysql_use_result(myconn);
        if(res == NULL){
            show_error();
            retorno = "-2";
        }
        else{
            while( (fila = mysql_fetch_row(res)) ){
                retorno = string(fila[0]);
            }
            mysql_free_result(res);
        }
    }
    return retorno;
}
