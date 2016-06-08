//
// Created by paulina on 02.06.16.
//

#ifndef TIN_SERVER_ANSWERBUILDER_H
#define TIN_SERVER_ANSWERBUILDER_H
#include <string>
#include <iostream>
#include "json.h"
#include "DatabaseService.h"

using std::string;
using std::cout;

class JsonResolver {

    DatabaseService * service;
    Json::Value parsedJson;
    Json::Value returnedJson;

    void resolveRegisterRequest();
    void resolveLoginRequest();
    void resolveNewMessagesRequest();
    void resolveMessageRead();
    void resolveMessageReceived();
    bool parsed;
public:
    JsonResolver(string jsonString);
    void execute( DatabaseService * service);
    void print() { std::cout<<returnedJson.toStyledString() ;};
    string getReturnedJsonString() { return returnedJson.asString(); };
    bool isParsed() { return parsed; }

    string getFromParsedRoot(string label);
    string getFromParsedCiphered(string label);

    void addToReturnedRoot(string label, string value);
    void addToReturnedCiphered(string label, string value);
};


#endif //TIN_SERVER_ANSWERBUILDER_H
