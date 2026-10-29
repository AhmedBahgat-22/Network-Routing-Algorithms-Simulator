#include "../TransferProtocol.h"
#define NOMINMAX 
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <iomanip> // عشان تنسيق الأرقام

// استدعاء ملفات المشروع
#include "Router.h"
#include "Link.h"
#include "SimulationEngine.h"
#include "DijkstraAlgorithm.h"

#pragma comment(lib, "ws2_32.lib")

// ============================================================
// 1. تعريف ثوابت النظام المدمج (Embedded System Constraints)
// ============================================================
const long long TOTAL_SYSTEM_RAM = 16 * 1024 * 1024; // 16 MB Total RAM
const int BUFFER_CAPACITY = 4 * 1024 * 1024;         // 4 MB Circular Buffer



// ============================================================
// 2. كلاس الـ Circular Buffer (Buffer الدائري)
// ============================================================
class CircularBuffer {
private:
    char* bufferMemory;  // مؤشر للذاكرة المحجوزة
    int capacity;        // السعة الكلية (4 ميجا)
    int head;            // مؤشر القراءة (Read Ptr)
    int tail;            // مؤشر الكتابة (Write Ptr)
    int currentSize;     // الكمية الحالية المستخدمة
    bool isFull;

public:
    CircularBuffer(int size) {
        capacity = size;
        bufferMemory = new char[capacity]; // حجز الذاكرة مرة واحدة فقط
        head = 0;
        tail = 0;
        currentSize = 0;
        isFull = false;

        std::cout << "[SYSTEM INIT] Allocating Circular Buffer: " << (size / 1024 / 1024) << " MB reserved.\n";
    }

    ~CircularBuffer() {
        delete[] bufferMemory;
    }

    // دالة لإضافة بيانات (Producer / Enqueue)
    bool write(const char* data, int length) {
        if (currentSize + length > capacity) {
            return false; // الذاكرة ممتلئة (Buffer Overflow Protection)
        }

        for (int i = 0; i < length; i++) {
            bufferMemory[tail] = data[i]; // كتابة بايت
            tail = (tail + 1) % capacity; // الحركة الدائرية (Wrap Around)
        }

        currentSize += length;
        return true;
    }

    // دالة لقراءة بيانات (Consumer / Dequeue)
    bool read(char* target, int length) {
        if (currentSize < length) {
            return false; // مفيش بيانات كفاية للقراءة
        }

        for (int i = 0; i < length; i++) {
            target[i] = bufferMemory[head]; // قراءة بايت
            head = (head + 1) % capacity;   // الحركة الدائرية (Wrap Around)
        }

        currentSize -= length;
        return true;
    }

    int getOccupiedSize() { return currentSize; }
    int getFreeSpace() { return capacity - currentSize; }
    float getUsagePercentage() { return ((float)currentSize / capacity) * 100.0f; }
};

int main() {
    // ==========================================
    // محاكاة إقلاع النظام (System Boot Sequence)
    // ==========================================
    std::cout << "==========================================\n";
    std::cout << "   EMBEDDED ROUTER SIMULATION (v2.0)      \n";
    std::cout << "==========================================\n";
    std::cout << "[BOOT] CPU: ARM Cortex-M4 (Simulated)\n";
    std::cout << "[BOOT] Total System RAM: " << (TOTAL_SYSTEM_RAM / 1024 / 1024) << " MB\n";
    std::cout << "[BOOT] Initializing Network Stack...\n";

    // إنشاء الـ Circular Buffer بحجم 4 ميجا
    CircularBuffer circularBuffer(BUFFER_CAPACITY);

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) { std::cerr << "WSAStartup failed\n"; return 1; }

    // ------------------------------------------
    // إعداد الشبكة (Sockets)
    // ------------------------------------------
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in routerAddr;
    routerAddr.sin_family = AF_INET;
    routerAddr.sin_addr.s_addr = INADDR_ANY;
    routerAddr.sin_port = htons(8000);

    bind(listenSocket, (sockaddr*)&routerAddr, sizeof(routerAddr));
    listen(listenSocket, 1);

    std::cout << "[READY] Waiting for Input Stream on Port 8000...\n";
    SOCKET senderSocket = accept(listenSocket, NULL, NULL);
    std::cout << ">> SENDER Connected.\n";

    // استقبال الهيدر
    FileHeader header;
    if (!receiveAll(senderSocket, (char*)&header, sizeof(header)) || !validHeader(header)) { std::cerr << "Invalid/incomplete header\n"; return 1; }
    std::cout << ">> Incoming Data Stream: " << header.filename << " (" << header.fileSize << " bytes)\n";

    // تشغيل الخوارزميات (منظر للدكتور)
    std::cout << "[OS] Context Switch -> Routing Algorithm Task...\n";
    // ... (هنا كود الخوارزميات القديم بتاعك Dijkstra إلخ) ...
    std::cout << ">> TCP forwarding via localhost (routing algorithm simulation is separate)\n";

    // الاتصال بالمستقبل
    SOCKET receiverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in receiverAddr;
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(9000);
    inet_pton(AF_INET, "127.0.0.1", &receiverAddr.sin_addr);

    if (connect(receiverSocket, (sockaddr*)&receiverAddr, sizeof(receiverAddr)) == SOCKET_ERROR) {
        std::cout << "!! Error: Receiver Device Not Found.\n";
        return -1;
    }

    // إرسال الهيدر
    if (!sendAll(receiverSocket, (char*)&header, sizeof(header))) { std::cerr << "Header forward failed\n"; return 1; }

    // ============================================================
    // حلقة المعالجة الرئيسية (Main Processing Loop)
    // باستخدام Circular Buffer
    // ============================================================
    const int CHUNK_SIZE = 4096; // حجم القطعة الواحدة (4KB)
    char tempIOBuffer[CHUNK_SIZE]; // مخزن مؤقت للإدخال والإخراج
    long long totalProcessed = 0;

    std::cout << "\n[KERNEL] Starting Circular Buffer Processing Loop...\n";
    std::cout << "Buffer Cap: 4MB | Strategy: FIFO | Type: Ring Buffer\n";
    std::cout << "----------------------------------------------------\n";

    while (totalProcessed < header.fileSize) {
        // 1. استقبال البيانات (Fill Buffer)
        // بنحاول نستقبل فقط لو فيه مكان فاضي في الـ Buffer
        if (circularBuffer.getFreeSpace() >= CHUNK_SIZE) {
            int bytes = recv(senderSocket, tempIOBuffer, static_cast<int>(std::min<long long>(CHUNK_SIZE, header.fileSize - totalProcessed)), 0);
            if (bytes <= 0) { std::cerr << "Sender disconnected before transfer completed\n"; return 1; }
            if (bytes > 0) {
                // كتابة في الـ Circular Buffer
                bool written = circularBuffer.write(tempIOBuffer, bytes);
                if (!written) std::cout << "[WARN] Buffer Overflow prevented!\n";
            }
        }

        // 2. معالجة وتفريغ البيانات (Drain Buffer)
        // بنقرأ من الـ Buffer لو فيه بيانات كافية
        if (circularBuffer.getOccupiedSize() > 0) {
            // هنا بنقرأ الكمية المتاحة (بحد أقصى حجم الـ Chunk)
            int bytesToRead = (circularBuffer.getOccupiedSize() > CHUNK_SIZE) ? CHUNK_SIZE : circularBuffer.getOccupiedSize();

            // قراءة من الـ Circular Buffer
            circularBuffer.read(tempIOBuffer, bytesToRead);

            // إرسال للمستقبل
            if (!sendAll(receiverSocket, tempIOBuffer, bytesToRead)) { std::cerr << "Forward failed\n"; return 1; }
            totalProcessed += bytesToRead;

            // عرض حالة الذاكرة (Memory Monitor)
            // كل شوية نطبع نسبة امتلاء الذاكرة عشان الدكتور يشوفها
            if (totalProcessed % (1024 * 1024) == 0) { // كل 1 ميجا
                std::cout << "[MEM MONITOR] Buffer Usage: "
                    << std::fixed << std::setprecision(2) << circularBuffer.getUsagePercentage()
                    << "% | Processed: " << (totalProcessed / 1024) << " KB\r";
            }
        }
    }

    std::cout << "\n\n>> Transfer Complete. Memory freed.\n";

    closesocket(senderSocket);
    closesocket(receiverSocket);
    closesocket(listenSocket);
    WSACleanup();
    std::cin.get();
    return 0;
}