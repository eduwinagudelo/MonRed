#ifndef CIPVAL_H
#define CIPVAL_H

#include <string>
#include "include/CUtiles.h"
//#include "mysql/mysql.h"

using namespace std;
using namespace SBI;

class CIpVal
{
    public:
        CIpVal();
        CIpVal(string strLin);
        virtual ~CIpVal();

        unsigned int Getid() { return id; }
        void Setid(unsigned int val) { id = val; }
        string Getipadd() { return ipadd; }
        void Setipadd(string val) { ipadd = val; }
        string GetdateMod() { return dateMod; }
        void SetdateMod(string val) { dateMod = val; }
        long Getuptime() { return uptime; }
        void Setuptime(long val) { uptime = val; }
        void printAddress();
        bool operator==(CIpVal &rhs);

    protected:

    private:
        unsigned int id;
        string ipadd;
        string dateMod;
        long uptime;
};

#endif // CIPVAL_H
