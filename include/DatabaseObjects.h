//
// Created by paulina on 01.06.16.
//

#ifndef TIN_SERVER_DATABASEOBJECTS_H
#define TIN_SERVER_DATABASEOBJECTS_H

#include <string>
#include <pqxx/pqxx>
#include <iostream>

using std::string;

struct User {
    int id;
    string login;
    string hashpassword;
    User() {};
    User(int i, string l, string h) : id(i), login(l), hashpassword(h) {};
    User(pqxx::result::const_iterator row) {
        try {
            id = row[0].as<int>();
            login = row[1].as<string>();
            hashpassword = row[2].as<string>();
        } catch(...) {
            std::cout<<"error user";
        }
    }
};

struct Notification {
    int id;
    string text;
    string created;
    int userid;
    int readid;
    Notification (int i, string t, string c, int uid, int rid) : id(i), text(t), created(c), userid(uid), readid(rid) {};
    Notification (pqxx::result::const_iterator row) {
        try {
            id = row[0].as<int>();
            text = row[1].as<string>();
            created = row[2].as<string>();
            userid = row[3].as<int>();
            readid = row[4].as<int>();
        } catch(...) {
            std::cout<<"error notification";
        }
    }
};

struct Device {
    int id;
    string lastlogindate;
    int userid;
    int lastqueryid;
    int lastreadid;
    int lastquerytmp;
    int lastreadtmp;
    Device (int i, string lld, int uid, int lqi, int lri, int lqt, int lrt) :
            id(i), lastlogindate(lld), userid(uid), lastqueryid(lqi), lastreadid(lri), lastquerytmp(lqt), lastreadtmp(lrt) {};
    Device (pqxx::result::const_iterator row) {
        try {
            id = row[0].as<int>();
            lastlogindate = row[1].as<string>();
            userid = row[2].as<int>();
            lastqueryid = row[3].as<int>();
            lastreadid = row[4].as<int>();
            lastquerytmp = row[5].as<int>();
            lastreadtmp = row[6].as<int>();
        } catch(...) {
            std::cout<<"error device";
        }
    }
};

#endif //TIN_SERVER_DATABASEOBJECTS_H
