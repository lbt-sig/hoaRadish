// MIT License

// Copyright (c) 2023 LBT_sig

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <iostream>
#include <fstream>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <host> <port> <filename>" << std::endl;
        return 1;
    }

    const std::string host = argv[1];
    const int port = std::stoi(argv[2]);
    const std::string filename = argv[3];

#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock." << std::endl;
        return 1;
    }
#endif

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        std::cerr << "Failed to create socket." << std::endl;
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);

#ifdef _WIN32
    serverAddress.sin_addr.s_addr = inet_addr(host.c_str());
#else
    if (inet_pton(AF_INET, host.c_str(), &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Invalid address or address not supported." << std::endl;
        close(clientSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }
#endif

    if (connect(clientSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Failed to connect to the server." << std::endl;
        close(clientSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    std::ifstream fileStream(filename, std::ios::binary);
    if (!fileStream.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        close(clientSocket);
#ifdef _WIN32
        WSACleanup();
#endif
        return 1;
    }

    // Send the filename first
    send(clientSocket, filename.c_str(), filename.length(), 0);

    char buffer[1024];
    while (!fileStream.eof()) {
        fileStream.read(buffer, sizeof(buffer));
        send(clientSocket, buffer, fileStream.gcount(), 0);
    }

    std::cout << "File sent successfully." << std::endl;

    close(clientSocket);
#ifdef _WIN32
    WSACleanup();
#endif

    return 0;
}
