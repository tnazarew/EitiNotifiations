#ifndef THREAD_H
#define THREAD_H
#define _WINSOCKAPI_
#include <windows.h>
#include <iostream>
#include "Socket.h"
DWORD WINAPI socketThreadFunction(LPVOID lpParameter);
DWORD WINAPI signalHandler(LPVOID lpParameter);
BOOL CtrlHandler(DWORD fwdCtrlType);

#endif //THREAD_H