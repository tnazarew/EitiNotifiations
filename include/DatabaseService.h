//
// Created by paulina on 01.06.16.
//

#ifndef TIN_SERVER_DATABASESERVICE_H
#define TIN_SERVER_DATABASESERVICE_H

#include <string>
#include "DatabaseObjects.h"
using std::string;
using std::vector;
using namespace pqxx;
class DatabaseService {

    connection* conn;

public:
    DatabaseService() {};
    bool initConnection(string dbname, string user, string password, string hostaddr, string port);
    bool initOrClearTables();
    int addNewUser(User user);
    int addNewDevice(Device device);

    User getUserByLogin(string login);
    Device getDeviceById(int id);
    Notification getNotificationById(int id);
    void setNotificationRead(Notification notification);

    void updateDevice(Device device);
    void updateDeviceLoginDate(int deviceid);
    bool addNewNotification(Notification notification);
    vector<Notification> getNewDeviceNotifications(Device device);
    vector<Notification> getFreshlyReadDeviceNotifications(Device device);


    ~DatabaseService();



};

#endif //TIN_SERVER_DATABASESERVICE_H
