#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <json.h>
#include <DatabaseService.h>
#include <JsonResolver.h>

using namespace std;
using namespace pqxx;

using namespace std;

int main() {

    string json;
    DatabaseService s;

    s.initConnection("tin", "tin", "tin", "127.0.0.1", "5433");

    Notification n(0, "wiadomosc", "", 1, 0);
    s.addNewNotification(n);

}