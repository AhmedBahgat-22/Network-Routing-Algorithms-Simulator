
#include "../TransferProtocol.h"
#include <iostream>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cstring>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData{};

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    std::cout << "=== SENDER NODE (Source - Node A) ===\n";

    // 1. Open the image file
    std::string filename = "test.jpg";

    std::ifstream file(
        filename,
        std::ios::binary | std::ios::ate
    );

    if (!file.is_open()) {
        std::cerr << "ERROR: Could not find 'test.jpg'!\n";
        std::cerr << "Please put test.jpg in the working directory.\n";
        WSACleanup();
        return 1;
    }

    // 2. Calculate file size
    std::streampos endPosition = file.tellg();

    if (endPosition < 0) {
        std::cerr << "ERROR: Could not determine file size.\n";
        file.close();
        WSACleanup();
        return 1;
    }

    long long fileSize =
        static_cast<long long>(endPosition);

    file.seekg(0, std::ios::beg);

    std::cout << "File Loaded: " << filename << "\n";
    std::cout << "Size: " << fileSize << " bytes\n";
    std::cout << "Press Enter to Connect & Send...\n";
    std::cin.get();

    // 3. Create TCP socket
    SOCKET clientSocket = socket(
        AF_INET,
        SOCK_STREAM,
        IPPROTO_TCP
    );

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "ERROR: Socket creation failed.\n";
        file.close();
        WSACleanup();
        return 1;
    }

    // 4. Configure Router address
    sockaddr_in routerAddr{};
    routerAddr.sin_family = AF_INET;
    routerAddr.sin_port = htons(8000);

    if (inet_pton(
        AF_INET,
        "127.0.0.1",
        &routerAddr.sin_addr
    ) != 1) {
        std::cerr << "ERROR: Invalid Router address.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // 5. Connect to Router
    if (connect(
        clientSocket,
        reinterpret_cast<sockaddr*>(&routerAddr),
        sizeof(routerAddr)
    ) == SOCKET_ERROR) {
        std::cerr << "Connection Failed! Is Router running?\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << ">> Connected to Router.\n";

    // 6. Prepare file metadata
    FileHeader header{};

    if (filename.size() >= sizeof(header.filename)) {
        std::cerr << "ERROR: Filename is too long.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    strcpy_s(
        header.filename,
        sizeof(header.filename),
        filename.c_str()
    );

    header.fileSize = fileSize;

    // 7. Send file header
    if (!sendAll(
        clientSocket,
        reinterpret_cast<const char*>(&header),
        sizeof(header)
    )) {
        std::cerr << "ERROR: Header send failed.\n";
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << ">> Header Sent.\n";

    // 8. Send image data in chunks
    const int BUFFER_SIZE = 4096;
    char buffer[BUFFER_SIZE];

    long long totalSent = 0;

    std::cout << "Sending Data...\n";

    while (totalSent < fileSize) {

        file.read(buffer, BUFFER_SIZE);

        std::streamsize bytesRead = file.gcount();

        if (bytesRead <= 0) {
            std::cerr << "\nERROR: File read failed.\n";
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        if (!sendAll(
            clientSocket,
            buffer,
            static_cast<int>(bytesRead)
        )) {
            std::cerr << "\nERROR: File send failed.\n";
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        totalSent += bytesRead;

        int progress = fileSize > 0
            ? static_cast<int>((totalSent * 100) / fileSize)
            : 100;

        std::cout << "\rProgress: "
            << progress << "%"
            << std::flush;
    }

    std::cout << "\n>> File Sent Successfully!\n";

    // 9. Cleanup
    file.close();

    closesocket(clientSocket);
    WSACleanup();

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}
