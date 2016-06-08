//
// Created by paulina on 02.06.16.
//

#include <JsonBuilder.h>
#include <json.h>

Json::Value JsonBuilder::failureResponse(string msg) {
    Json::Value root;
    Json::Value cipheredData;
    cipheredData["success"] = false;
    cipheredData["value"] = msg;
    root["cipheredData"] = cipheredData;
    return root;
}

Json::Value JsonBuilder::loginOrRegisterSuccess(int id) {
    Json::Value root;
    Json::Value cipheredData;
    Json::Value value;
    cipheredData["success"] = true;
    value["deviceId"] = id;
    cipheredData["value"] = value;
    root["cipheredData"] = cipheredData;
    return root;
}

Json::Value JsonBuilder::successResponse() {
    Json::Value root;
    Json::Value cipheredData;
    cipheredData["success"] = true;
    root["cipheredData"] = cipheredData;
    return root;
}

Json::Value JsonBuilder::messagesResponse(vector<Notification> &newMessages, vector<Notification> &read) {
    Json::Value root;
    Json::Value cipheredData;
    cipheredData["success"] = true;
    Json::Value messages;
    for(int i = 0; i < newMessages.size(); ++i) {
        messages[i]["id"] = newMessages[i].id;
        messages[i]["text"] = newMessages[i].text;
    }
    cipheredData["messages"] = messages;
    Json::Value readMessages;
    for(int i = 0; i < read.size(); ++i) {
        readMessages[i]["id"] = read[i].id;
    }
    cipheredData["readMessages"] = readMessages;
    root["cipheredData"] = cipheredData;
    return root;
}

void JsonBuilder::addToJson(string label, string value, Json::Value &root) {
    root[label] = value;
}