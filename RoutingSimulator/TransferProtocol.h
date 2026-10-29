#pragma once
#include <winsock2.h>
#include <algorithm>
#include <cstring>
#include <cstdint>
struct FileHeader { char filename[256]; long long fileSize; };
inline bool sendAll(SOCKET s, const char* p, int count) {
    int sent = 0;
    while (sent < count) {
        int n = send(s, p + sent, count - sent, 0);
        if (n <= 0) return false;
        sent += n;
    }
    return true;
}
inline bool receiveAll(SOCKET s, char* p, int count) {
    int received = 0;
    while (received < count) {
        int n = recv(s, p + received, count - received, 0);
        if (n <= 0) return false;
        received += n;
    }
    return true;
}
inline bool validHeader(const FileHeader& h) {
    return std::memchr(h.filename, '\0', sizeof(h.filename)) != nullptr && h.fileSize >= 0;
}
