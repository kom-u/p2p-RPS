#include <iostream>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 12346;

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        cerr << "Socket creation failed." << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Bind to all available network interfaces
    serverAddr.sin_port = htons(PORT);

    // Bind the socket
    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cerr << "Bind failed." << endl;
        return 1;
    }

    if (listen(serverSocket, 1) == SOCKET_ERROR)
    {
        cerr << "Listen failed." << endl;
        return 1;
    }

    cout << "Waiting for Player 2 to connect..." << endl;

    sockaddr_in clientAddr;
    int clientAddrLen = sizeof(clientAddr);

    SOCKET clientSocket = accept(serverSocket, (sockaddr *)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET)
    {
        cerr << "Accept failed." << endl;
        return 1;
    }

    cout << "Player 2 connected. Let's play Rock-Paper-Scissors!" << endl;

    // Rock-Paper-Scissors logic
    char player1Choice, player2Choice;

    while (true)
    {
        // Get player 1's choice (for example, via cin)
        cout << "Player 1, enter your choice (R/P/S): ";
        cin >> player1Choice;

        // Send player 1's choice to player 2
        send(clientSocket, &player1Choice, sizeof(char), 0);

        // Get player 2's choice
        recv(clientSocket, &player2Choice, sizeof(char), 0);

        // Game logic
        if (player1Choice == 'R' && player2Choice == 'S' ||
            player1Choice == 'S' && player2Choice == 'P' ||
            player1Choice == 'P' && player2Choice == 'R')
        {
            cout << "Player 1 wins!" << endl;
        }
        else if (player1Choice == player2Choice)
        {
            cout << "It's a tie!" << endl;
        }
        else
        {
            cout << "Player 2 wins!" << endl;
        }

        // Display the result
        cout << "Player 1 chose: " << player1Choice << endl;
        cout << "Player 2 chose: " << player2Choice << endl;
    }

    // Clean up
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
