//
// Created by paulina on 02.06.16.
//

#ifndef TIN_SERVER_JSONBUILDER_H
#define TIN_SERVER_JSONBUILDER_H
#include <string>
#include <DatabaseObjects.h>
#include <json.h>
#include <vector>
using std::vector;
using std::string;

class JsonBuilder {

public:
    static Json::Value loginOrRegisterSuccess(int deviceid);
    static Json::Value messagesResponse(vector<Notification>& newMessages, vector<Notification> &read);
    static Json::Value failureResponse(string msg);
    static Json::Value successResponse();
    static void addToJson(string label, string value, Json::Value & root);
};


#endif //TIN_SERVER_JSONBUILDER_H
