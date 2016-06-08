//
// Created by paulina on 02.06.16.
//

#include <JsonResolver.h>
#include <json.h>
#include <JsonBuilder.h>

JsonResolver::JsonResolver(string jsonString) {
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(jsonString.c_str(), parsedJson);
    if (!parsingSuccessful) {
        parsed = false;
        return;
    }
    parsed =  true;
}

void JsonResolver::execute(DatabaseService * service) {
    if(!parsed)
        return;
    this->service = service;
    string action = parsedJson.get("action", "invalid").asString();
    if (action == "invalid") {
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
        return;
    }
    if (action == "registerRequest")
        resolveRegisterRequest();
    else if (action == "loginRequest")
        resolveLoginRequest();
    else if (action == "newMessagesRequest")
        resolveNewMessagesRequest();
    else if (action == "messageRead")
        resolveMessageRead();
    else if (action == "messageReceived")
        resolveMessageReceived();
    else
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
    return;
}

void JsonResolver::resolveLoginRequest() {
    Json::Value cipheredData = parsedJson["cipheredData"];
    string login = cipheredData.get("user", "").asString();
    string hashpassword = cipheredData.get("password", "").asString();
    int deviceid = cipheredData.get("deviceId", 0).asInt();
    if(login == "" || hashpassword == "") {
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
        return;
    }
    User user = service->getUserByLogin(login);
    if(user.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchUser");
        return;
    }
    if(user.hashpassword != hashpassword) {
        returnedJson = JsonBuilder::failureResponse("invalidPassword");
        return;
    }
    int did;
    if(deviceid == 0) {
        Device device(0, "", user.id, 0, 0, 0, 0);
        did = service->addNewDevice(device);
    } else {
        Device device = service->getDeviceById(deviceid);
        if(device.id == 0) {
            returnedJson = JsonBuilder::failureResponse("noSuchDevice");
            return;
        }
        if(user.id != device.userid) {
            returnedJson = JsonBuilder::failureResponse("noSuchDevice");
            return;
        }
        did = device.id;
        service->updateDeviceLoginDate(device.id);
    }
    returnedJson = JsonBuilder::loginOrRegisterSuccess(did);
    return;
}

void JsonResolver::resolveNewMessagesRequest() {
    int deviceId = parsedJson.get("deviceID", 0).asInt();
    Device device = service->getDeviceById(deviceId);
    if(device.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchDevice");
        return;
    }
    vector<Notification> read = service->getFreshlyReadDeviceNotifications(device);
    vector<Notification> newNotifications = service->getNewDeviceNotifications(device);
    returnedJson = JsonBuilder::messagesResponse(newNotifications, read);
    return;
}

void JsonResolver::resolveMessageRead() {
    int deviceId = parsedJson.get("deviceID", 0).asInt();
    if(deviceId == 0) {
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
        return;
    }
    Json::Value cipheredData = parsedJson["cipheredData"];
    string messageId = cipheredData.get("messageId", "").asString();
    Device device = service->getDeviceById(deviceId);
    if(device.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchDevice");
        return;
    }
    Notification notification = service->getNotificationById(atoi(messageId.c_str()));
    if(notification.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchMessage");
        return;
    }
    if(device.userid != notification.userid) {
        returnedJson = JsonBuilder::failureResponse("accessDenied");
        return;
    }
    if(notification.readid != -1) {
        returnedJson = JsonBuilder::successResponse();
        return;
    }
    service->setNotificationRead(notification);
    returnedJson = JsonBuilder::successResponse();
    return;
}

void JsonResolver::resolveRegisterRequest() {
    Json::Value cipheredData = parsedJson["cipheredData"];
    string login = cipheredData.get("user", "").asString();
    string hashpassword = cipheredData.get("password", "").asString();
    if(login == "" || hashpassword == ""){
        returnedJson = JsonBuilder::failureResponse("invalidQuery");
        return;
    }
    User user = service->getUserByLogin(login);
    if(user.id != 0) {
        returnedJson =  JsonBuilder::failureResponse("userAlreadyExists");
        return;
    }
    User newUser(0, login, hashpassword);
    int userid = service->addNewUser(newUser);
    Device device(0, "", userid, 0, 0, 0, 0);
    int deviceid = service->addNewDevice(device);
    returnedJson = JsonBuilder::loginOrRegisterSuccess(deviceid);
    return;
}

void JsonResolver::resolveMessageReceived() {
    int deviceId = parsedJson.get("deviceID", 0).asInt();
    Device device = service->getDeviceById(deviceId);
    if(device.id == 0) {
        returnedJson = JsonBuilder::failureResponse("noSuchDevice");
        return;
    }
    service->updateDevice(device);
    returnedJson = JsonBuilder::successResponse();
    return;
}

void JsonResolver::addToReturnedCiphered(string label, string value) {
    JsonBuilder::addToJson(label, value, returnedJson["cipheredData"]);
}

void JsonResolver::addToReturnedRoot(string label, string value) {
    JsonBuilder::addToJson(label, value, returnedJson);
}

string JsonResolver::getFromParsedCiphered(string label) {
    return parsedJson["cipheredData"].get(label, "").asString();
}

string JsonResolver::getFromParsedRoot(string label) {
    return parsedJson.get(label, "").asString();
}