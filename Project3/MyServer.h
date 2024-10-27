#pragma once

#include <iostream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <winsock2.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace System;
using namespace System::Threading;

enum CommandOpcode {
    CLEAR_DISPLAY_OPCODE,
    DRAW_PIXEL_OPCODE,
    DRAW_LINE_OPCODE,
    DRAW_RECTANGLE_OPCODE,
    FILL_RECTANGLE_OPCODE,
    DRAW_ELLIPSE_OPCODE,
    FILL_ELLIPSE_OPCODE
};

// Базовий клас команди
public ref class Command {
public:
    const CommandOpcode opcode;
    Command(const CommandOpcode opcode) : opcode(opcode) {}
    virtual ~Command() {}
};

// Клас команди для очищення дисплея
public ref class ClearDisplay : public Command {
public:
    const uint16_t color;
    ClearDisplay(const uint16_t color) : Command(CLEAR_DISPLAY_OPCODE), color(color) {}
};

// Клас команди для малювання пікселя
public ref class DrawPixel : public Command {
public:
    const int x, y;
    const uint16_t color;
    DrawPixel(int x, int y, uint16_t color) : Command(DRAW_PIXEL_OPCODE), x(x), y(y), color(color) {}
};

// Клас команди для малювання лінії
public ref class DrawLine : public Command {
public:
    const int x1, y1, x2, y2;
    const uint16_t color;
    DrawLine(int x1, int y1, int x2, int y2, uint16_t color)
        : Command(DRAW_LINE_OPCODE), x1(x1), y1(y1), x2(x2), y2(y2), color(color) {}
};

// Клас команди для малювання прямокутника
public ref class DrawRectangle : public Command {
public:
    const int x, y, width, height;
    const uint16_t color;
    DrawRectangle(int x, int y, int width, int height, uint16_t color)
        : Command(DRAW_RECTANGLE_OPCODE), x(x), y(y), width(width), height(height), color(color) {}
};

// Клас команди для заповнення прямокутника
public ref class FillRectangle : public Command {
public:
    const int x, y, width, height;
    const uint16_t color;
    FillRectangle(int x, int y, int width, int height, uint16_t color)
        : Command(FILL_RECTANGLE_OPCODE), x(x), y(y), width(width), height(height), color(color) {}
};

// Клас команди для малювання еліпса
public ref class DrawEllipse : public Command {
public:
    const int x, y, width, height;
    const uint16_t color;
    DrawEllipse(int x, int y, int width, int height, uint16_t color)
        : Command(DRAW_ELLIPSE_OPCODE), x(x), y(y), width(width), height(height), color(color) {}
};

// Клас команди для заповнення еліпса
public ref class FillEllipse : public Command {
public:
    const int x, y, width, height;
    const uint16_t color;
    FillEllipse(int x, int y, int width, int height, uint16_t color)
        : Command(FILL_ELLIPSE_OPCODE), x(x), y(y), width(width), height(height), color(color) {}
};

public ref class MyServer {
private:
    Thread^ serverThread;
    bool running;

    void run() {
        // Ініціалізація WinSock
        WSAData wsaData;
        WORD DLLVersion = MAKEWORD(2, 2);
        if (WSAStartup(DLLVersion, &wsaData) != 0) {
            std::cerr << "Error initializing WinSock" << std::endl;
            return;
        }

        // Налаштування сокета сервера
        SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (serverSocket == INVALID_SOCKET) {
            std::cerr << "Error creating socket" << std::endl;
            WSACleanup();
            return;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(1111); // Порт сервера
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind error" << std::endl;
            closesocket(serverSocket);
            WSACleanup();
            return;
        }

        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        std::vector<uint8_t> buffer(8192);

        while (running) {
            int recvSize = recvfrom(serverSocket, (char*)buffer.data(), buffer.size(), 0, (sockaddr*)&clientAddr, &clientAddrSize);
            if (recvSize == SOCKET_ERROR) {
                int error = WSAGetLastError();
                std::cerr << "Receive error: " << error << std::endl;
                continue;
            }

            if (recvSize > buffer.size()) {
                std::cerr << "Received packet is too large, size: " << recvSize << std::endl;
                continue;
            }

            Command^ command = nullptr; // Використовуйте управлінські типи
            try {
                buffer.resize(recvSize);
                parseCommand(buffer, command);
                if (command) {
                    executeCommand(command);
                }
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }

        closesocket(serverSocket);
        WSACleanup();
    }

public:
    void sendCommand(Command^ command) {
        std::vector<uint8_t> buffer;

        // Serialize the command based on its opcode
        switch (command->opcode) {
        case CLEAR_DISPLAY_OPCODE: {
            ClearDisplay^ clearCommand = dynamic_cast<ClearDisplay^>(command);
            buffer.push_back(static_cast<uint8_t>(CLEAR_DISPLAY_OPCODE)); // Opcode
            buffer.push_back(static_cast<uint8_t>(clearCommand->color & 0xFF)); // Low byte of color
            buffer.push_back(static_cast<uint8_t>((clearCommand->color >> 8) & 0xFF)); // High byte of color
            break;
        }
        case DRAW_PIXEL_OPCODE: {
            DrawPixel^ pixelCommand = dynamic_cast<DrawPixel^>(command);
            buffer.push_back(static_cast<uint8_t>(DRAW_PIXEL_OPCODE)); // Opcode
            buffer.push_back(static_cast<uint8_t>(pixelCommand->x & 0xFF)); // Low byte of x
            buffer.push_back(static_cast<uint8_t>((pixelCommand->x >> 8) & 0xFF)); // High byte of x
            buffer.push_back(static_cast<uint8_t>(pixelCommand->y & 0xFF)); // Low byte of y
            buffer.push_back(static_cast<uint8_t>((pixelCommand->y >> 8) & 0xFF)); // High byte of y
            buffer.push_back(static_cast<uint8_t>(pixelCommand->color & 0xFF)); // Low byte of color
            buffer.push_back(static_cast<uint8_t>((pixelCommand->color >> 8) & 0xFF)); // High byte of color
            break;
        }
        case DRAW_LINE_OPCODE: {
            DrawLine^ lineCommand = dynamic_cast<DrawLine^>(command);
            buffer.push_back(static_cast<uint8_t>(DRAW_LINE_OPCODE)); // Opcode
            buffer.push_back(static_cast<uint8_t>(lineCommand->x1 & 0xFF)); // Low byte of x1
            buffer.push_back(static_cast<uint8_t>((lineCommand->x1 >> 8) & 0xFF)); // High byte of x1
            buffer.push_back(static_cast<uint8_t>(lineCommand->y1 & 0xFF)); // Low byte of y1
            buffer.push_back(static_cast<uint8_t>((lineCommand->y1 >> 8) & 0xFF)); // High byte of y1
            buffer.push_back(static_cast<uint8_t>(lineCommand->x2 & 0xFF)); // Low byte of x2
            buffer.push_back(static_cast<uint8_t>((lineCommand->x2 >> 8) & 0xFF)); // High byte of x2
            buffer.push_back(static_cast<uint8_t>(lineCommand->y2 & 0xFF)); // Low byte of y2
            buffer.push_back(static_cast<uint8_t>((lineCommand->y2 >> 8) & 0xFF)); // High byte of y2
            buffer.push_back(static_cast<uint8_t>(lineCommand->color & 0xFF)); // Low byte of color
            buffer.push_back(static_cast<uint8_t>((lineCommand->color >> 8) & 0xFF)); // High byte of color
            break;
        }
        case DRAW_RECTANGLE_OPCODE: {
            DrawRectangle^ rectCommand = dynamic_cast<DrawRectangle^>(command);
            buffer.push_back(static_cast<uint8_t>(DRAW_RECTANGLE_OPCODE)); // Opcode
            buffer.push_back(static_cast<uint8_t>(rectCommand->x & 0xFF)); // Low byte of x
            buffer.push_back(static_cast<uint8_t>((rectCommand->x >> 8) & 0xFF)); // High byte of x
            buffer.push_back(static_cast<uint8_t>(rectCommand->y & 0xFF)); // Low byte of y
            buffer.push_back(static_cast<uint8_t>((rectCommand->y >> 8) & 0xFF)); // High byte of y
            buffer.push_back(static_cast<uint8_t>(rectCommand->width & 0xFF)); // Low byte of width
            buffer.push_back(static_cast<uint8_t>((rectCommand->width >> 8) & 0xFF)); // High byte of width
            buffer.push_back(static_cast<uint8_t>(rectCommand->height & 0xFF)); // Low byte of height
            buffer.push_back(static_cast<uint8_t>((rectCommand->height >> 8) & 0xFF)); // High byte of height
            buffer.push_back(static_cast<uint8_t>(rectCommand->color & 0xFF)); // Low byte of color
            buffer.push_back(static_cast<uint8_t>((rectCommand->color >> 8) & 0xFF)); // High byte of color
            break;
        }
        case FILL_RECTANGLE_OPCODE: {
            FillRectangle^ fillRectCommand = dynamic_cast<FillRectangle^>(command);
            buffer.push_back(static_cast<uint8_t>(FILL_RECTANGLE_OPCODE)); // Opcode
            buffer.push_back(static_cast<uint8_t>(fillRectCommand->x & 0xFF)); // Low byte of x
            buffer.push_back(static_cast<uint8_t>((fillRectCommand->x >> 8) & 0xFF)); // High byte of x
            buffer.push_back(static_cast<uint8_t>(fillRectCommand->y & 0xFF)); // Low byte of y
            buffer.push_back(static_cast<uint8_t>((fillRectCommand->y >> 8) & 0xFF)); // High byte of y
            buffer.push_back(static_cast<uint8_t>(fillRectCommand->width & 0xFF)); // Low byte of width
            buffer.push_back(static_cast<uint8_t>((fillRectCommand->width >> 8) & 0xFF)); // High byte of width
            buffer.push_back(static_cast<uint8_t>(fillRectCommand->height & 0xFF)); // Low byte of height
            buffer.push_back(static_cast<uint8_t>((fillRectCommand->height >> 8) & 0xFF)); // High byte of height
            buffer.push_back(static_cast<uint8_t>(fillRectCommand->color & 0xFF)); // Low byte of color
            buffer.push_back(static_cast<uint8_t>((fillRectCommand->color >> 8) & 0xFF)); // High byte of color
            break;
        }
        case DRAW_ELLIPSE_OPCODE: {
            DrawEllipse^ ellipseCommand = dynamic_cast<DrawEllipse^>(command);
            buffer.push_back(static_cast<uint8_t>(DRAW_ELLIPSE_OPCODE)); // Opcode
            buffer.push_back(static_cast<uint8_t>(ellipseCommand->x & 0xFF)); // Low byte of x
            buffer.push_back(static_cast<uint8_t>((ellipseCommand->x >> 8) & 0xFF)); // High byte of x
            buffer.push_back(static_cast<uint8_t>(ellipseCommand->y & 0xFF)); // Low byte of y
            buffer.push_back(static_cast<uint8_t>((ellipseCommand->y >> 8) & 0xFF)); // High byte of y
            buffer.push_back(static_cast<uint8_t>(ellipseCommand->width & 0xFF)); // Low byte of width
            buffer.push_back(static_cast<uint8_t>((ellipseCommand->width >> 8) & 0xFF)); // High byte of width
            buffer.push_back(static_cast<uint8_t>(ellipseCommand->height & 0xFF)); // Low byte of height
            buffer.push_back(static_cast<uint8_t>((ellipseCommand->height >> 8) & 0xFF)); // High byte of height
            buffer.push_back(static_cast<uint8_t>(ellipseCommand->color & 0xFF)); // Low byte of color
            buffer.push_back(static_cast<uint8_t>((ellipseCommand->color >> 8) & 0xFF)); // High byte of color
            break;
        }
        case FILL_ELLIPSE_OPCODE: {
            FillEllipse^ fillEllipseCommand = dynamic_cast<FillEllipse^>(command);
            buffer.push_back(static_cast<uint8_t>(FILL_ELLIPSE_OPCODE)); // Opcode
            buffer.push_back(static_cast<uint8_t>(fillEllipseCommand->x & 0xFF)); // Low byte of x
            buffer.push_back(static_cast<uint8_t>((fillEllipseCommand->x >> 8) & 0xFF)); // High byte of x
            buffer.push_back(static_cast<uint8_t>(fillEllipseCommand->y & 0xFF)); // Low byte of y
            buffer.push_back(static_cast<uint8_t>((fillEllipseCommand->y >> 8) & 0xFF)); // High byte of y
            buffer.push_back(static_cast<uint8_t>(fillEllipseCommand->width & 0xFF)); // Low byte of width
            buffer.push_back(static_cast<uint8_t>((fillEllipseCommand->width >> 8) & 0xFF)); // High byte of width
            buffer.push_back(static_cast<uint8_t>(fillEllipseCommand->height & 0xFF)); // Low byte of height
            buffer.push_back(static_cast<uint8_t>((fillEllipseCommand->height >> 8) & 0xFF)); // High byte of height
            buffer.push_back(static_cast<uint8_t>(fillEllipseCommand->color & 0xFF)); // Low byte of color
            buffer.push_back(static_cast<uint8_t>((fillEllipseCommand->color >> 8) & 0xFF)); // High byte of color
            break;
        }
        default:
            std::cerr << "Unknown command" << std::endl;
            return;
        }

        std::cout << "Sending command to server" << std::endl;

        
        executeCommand(command);
    }



    void executeCommand(Command^ command) {
        // Виконання команди
        switch (command->opcode) {
        case CLEAR_DISPLAY_OPCODE: {
            ClearDisplay^ clearCommand = dynamic_cast<ClearDisplay^>(command);
            std::cout << "Clearing display with color: " << clearCommand->color << std::endl;
            break;
        }
        case DRAW_PIXEL_OPCODE: {
            DrawPixel^ pixelCommand = dynamic_cast<DrawPixel^>(command);
            std::cout << "Drawing pixel at (" << pixelCommand->x << ", " << pixelCommand->y << ") with color: " << pixelCommand->color << std::endl;
            break;
        }
        case DRAW_LINE_OPCODE: {
            DrawLine^ lineCommand = dynamic_cast<DrawLine^>(command);
            std::cout << "Drawing line from (" << lineCommand->x1 << ", " << lineCommand->y1 << ") to ("
                << lineCommand->x2 << ", " << lineCommand->y2 << ") with color: " << lineCommand->color << std::endl;
            break;
        }
        case DRAW_RECTANGLE_OPCODE: {
            DrawRectangle^ rectCommand = dynamic_cast<DrawRectangle^>(command);
            std::cout << "Drawing rectangle at (" << rectCommand->x << ", " << rectCommand->y << ") with width: "
                << rectCommand->width << " and height: " << rectCommand->height << " with color: " << rectCommand->color << std::endl;
            break;
        }
        case FILL_RECTANGLE_OPCODE: {
            FillRectangle^ fillRectCommand = dynamic_cast<FillRectangle^>(command);
            std::cout << "Filling rectangle at (" << fillRectCommand->x << ", " << fillRectCommand->y << ") with width: "
                << fillRectCommand->width << " and height: " << fillRectCommand->height << " with color: " << fillRectCommand->color << std::endl;
            break;
        }
        case DRAW_ELLIPSE_OPCODE: {
            DrawEllipse^ ellipseCommand = dynamic_cast<DrawEllipse^>(command);
            std::cout << "Drawing ellipse at (" << ellipseCommand->x << ", " << ellipseCommand->y << ") with width: "
                << ellipseCommand->width << " and height: " << ellipseCommand->height << " with color: " << ellipseCommand->color << std::endl;
            break;
        }
        case FILL_ELLIPSE_OPCODE: {
            FillEllipse^ fillEllipseCommand = dynamic_cast<FillEllipse^>(command);
            std::cout << "Filling ellipse at (" << fillEllipseCommand->x << ", " << fillEllipseCommand->y << ") with width: "
                << fillEllipseCommand->width << " and height: " << fillEllipseCommand->height << " with color: " << fillEllipseCommand->color << std::endl;
            break;
        }
        default:
            std::cerr << "Unknown command opcode: " << command->opcode << std::endl;
            break;
        }
    }

public:
    MyServer() : running(false) {}



    void start() {
        if (!running) {
            running = true;
            serverThread = gcnew Thread(gcnew ThreadStart(this, &MyServer::run));
            serverThread->Start();
          
        }
    }

    void stop() {
        running = false;
        if (serverThread != nullptr && serverThread->IsAlive) {
            serverThread->Join(); // Wait for the thread to finish
        }
    }


    void parseCommand(const std::vector<uint8_t>& buffer, Command^& command) {
        if (buffer.size() < sizeof(CommandOpcode)) {
            throw std::invalid_argument("Buffer size is too small to contain a command.");
        }

        CommandOpcode opcode = static_cast<CommandOpcode>(buffer[0]);
        switch (opcode) {
        case CLEAR_DISPLAY_OPCODE:
            command = gcnew ClearDisplay(*reinterpret_cast<const uint16_t*>(&buffer[1]));
            break;
        case DRAW_PIXEL_OPCODE:
            if (buffer.size() < sizeof(uint16_t) * 3) {
                throw std::invalid_argument("Buffer size is too small for DRAW_PIXEL command.");
            }
            command = gcnew DrawPixel(buffer[1], buffer[2], *reinterpret_cast<const uint16_t*>(&buffer[3]));
            break;
        case DRAW_LINE_OPCODE:
            if (buffer.size() < sizeof(int) * 4 + sizeof(uint16_t)) {
                throw std::invalid_argument("Buffer size is too small for DRAW_LINE command.");
            }
            command = gcnew DrawLine(
                *reinterpret_cast<const int*>(&buffer[1]),
                *reinterpret_cast<const int*>(&buffer[5]),
                *reinterpret_cast<const int*>(&buffer[9]),
                *reinterpret_cast<const int*>(&buffer[13]),
                *reinterpret_cast<const uint16_t*>(&buffer[17])
            );
            break;
        case DRAW_RECTANGLE_OPCODE:
            if (buffer.size() < sizeof(int) * 4 + sizeof(uint16_t)) {
                throw std::invalid_argument("Buffer size is too small for DRAW_RECTANGLE command.");
            }
            command = gcnew DrawRectangle(
                *reinterpret_cast<const int*>(&buffer[1]),
                *reinterpret_cast<const int*>(&buffer[5]),
                *reinterpret_cast<const int*>(&buffer[9]),
                *reinterpret_cast<const int*>(&buffer[13]),
                *reinterpret_cast<const uint16_t*>(&buffer[17])
            );
            break;
        case FILL_RECTANGLE_OPCODE:
            if (buffer.size() < sizeof(int) * 4 + sizeof(uint16_t)) {
                throw std::invalid_argument("Buffer size is too small for FILL_RECTANGLE command.");
            }
            command = gcnew FillRectangle(
                *reinterpret_cast<const int*>(&buffer[1]),
                *reinterpret_cast<const int*>(&buffer[5]),
                *reinterpret_cast<const int*>(&buffer[9]),
                *reinterpret_cast<const int*>(&buffer[13]),
                *reinterpret_cast<const uint16_t*>(&buffer[17])
            );
            break;
        case DRAW_ELLIPSE_OPCODE:
            if (buffer.size() < sizeof(int) * 4 + sizeof(uint16_t)) {
                throw std::invalid_argument("Buffer size is too small for DRAW_ELLIPSE command.");
            }
            command = gcnew DrawEllipse(
                *reinterpret_cast<const int*>(&buffer[1]),
                *reinterpret_cast<const int*>(&buffer[5]),
                *reinterpret_cast<const int*>(&buffer[9]),
                *reinterpret_cast<const int*>(&buffer[13]),
                *reinterpret_cast<const uint16_t*>(&buffer[17])
            );
            break;
        case FILL_ELLIPSE_OPCODE:
            if (buffer.size() < sizeof(int) * 4 + sizeof(uint16_t)) {
                throw std::invalid_argument("Buffer size is too small for FILL_ELLIPSE command.");
            }
            command = gcnew FillEllipse(
                *reinterpret_cast<const int*>(&buffer[1]),
                *reinterpret_cast<const int*>(&buffer[5]),
                *reinterpret_cast<const int*>(&buffer[9]),
                *reinterpret_cast<const int*>(&buffer[13]),
                *reinterpret_cast<const uint16_t*>(&buffer[17])
            );
            break;
        default:
            throw std::invalid_argument("Unknown command opcode.");
        }
    }

};
