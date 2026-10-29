#include "../TransferProtocol.h"
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")



int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { std::cerr << "WSAStartup failed\n"; return 1; }

    std::cout << "=== RECEIVER NODE (Destination - Node B) ===\n";

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(9000);

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    std::cout << "Waiting for Router on Port 9000...\n";

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);
    std::cout << ">> Router Connected.\n";

    // 1. (Decoding Step 1): استقبال الهيدر
    FileHeader header;
    bool headerReceived = receiveAll(clientSocket, (char*)&header, sizeof(header));

    if (headerReceived && validHeader(header)) {
        std::cout << ">> Incoming File: " << header.filename << "\n";
        std::cout << ">> Size: " << header.fileSize << " bytes\n";

        // تغيير الاسم عشان ميحصلش تعارض
        std::string safeName(header.filename);
        safeName = safeName.substr(safeName.find_last_of("/\\") == std::string::npos ? 0 : safeName.find_last_of("/\\") + 1);
        if (safeName.empty() || safeName == "." || safeName == "..") { std::cerr << "Invalid filename\n"; return 1; }
        std::string outputName = "received_" + safeName;
        std::ofstream outFile(outputName, std::ios::binary);

        if (!outFile.is_open()) {
            std::cout << "Error creating output file!\n";
            return -1;
        }

        // 2. (Decoding Step 2): استقبال البيانات وتجميع الملف
        const int BUFFER_SIZE = 4096;
        char buffer[BUFFER_SIZE];
        long long totalReceived = 0;

        std::cout << "Receiving Data...\n";
        while (totalReceived < header.fileSize) {
            int bytes = recv(clientSocket, buffer, static_cast<int>(std::min<long long>(BUFFER_SIZE, header.fileSize - totalReceived)), 0);
            if (bytes <= 0) break;

            outFile.write(buffer, bytes); // كتابة في الملف
            totalReceived += bytes;

            std::cout << "\rProgress: " << (header.fileSize ? totalReceived * 100 / header.fileSize : 100) << "%" << std::flush;
        }

        if (totalReceived != header.fileSize || !outFile.good()) { std::cerr << "\nTransfer incomplete\n"; return 1; }
        std::cout << "\n>> File Received Successfully!\n";
        std::cout << ">> Saved as: " << outputName << "\n";
        outFile.close();
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}