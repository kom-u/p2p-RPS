#include <iostream>
#include <Winsock2.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

const int PORT = 12346;
char SERVER_IP[] = "127.0.0.1"; // Replace with the IP of Player 1's machine

int main()
{
    cout << "Enter IP: ";
    cin >> SERVER_IP;

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "WSAStartup failed." << endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET)
    {
        cerr << "Socket creation failed." << endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    if (connect(clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        cerr << "Connection to Player 1 failed." << endl;
        return 1;
    }

    cout << "Connected to Player 1. Let's play Rock-Paper-Scissors!" << endl;

    // Rock-Paper-Scissors logic
    char player1Choice, player2Choice;

    while (true)
    {
        // Get player 1's choice
        recv(clientSocket, &player1Choice, sizeof(char), 0);

        // Get player 2's choice (for example, via cin)
        cout << "Player 2, enter your choice (R/P/S): ";
        cin >> player2Choice;

        // Send player 2's choice to player 1
        send(clientSocket, &player2Choice, sizeof(char), 0);

        // Game logic (same as in Player 1)
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
    WSACleanup();

    return 0;
}
