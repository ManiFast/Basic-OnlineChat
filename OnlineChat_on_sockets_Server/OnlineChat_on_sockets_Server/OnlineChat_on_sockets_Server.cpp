//////////////////
//		Server		//
/////////////////

#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
//#include <WS2tcpip.h>

#include <string>
#include <fstream>

#pragma warning(disable: 4996) // Для ошибки в 25 inet_addr("127.0.0.1")

using namespace std;

SOCKET Connections[100];
int counter = 0;
int countUsers = 0;

// Create file
ofstream file("Chat.txt");

void writeToFile(char msg[])
{
	for (int i = 0; i < sizeof(msg); i++)
	{
		file << msg[i];
	}

	file << endl;
}

int sendAll(char msg[256], int counter)
{
	for (int i = 0; i < counter; i++)
	{
		send(Connections[i], msg, sizeof(msg), NULL);
	}

	return 0;
}

void ClientHandler(int index)
{
	char msg[256];

	while (true)
	{
		recv(Connections[index], msg, sizeof(msg), NULL);


		for (int i = 0; i < counter; i++)
		{
			writeToFile(msg);
			if (i == index)
			{
				continue;
			}

			send(Connections[i], msg, sizeof(msg), NULL);
		}
	}
}

int main(int argc, char* argv[])
{
	// Russian
	setlocale(LC_ALL, "ru");

	//// Загрузка библиотеки
	WSAData wsaData;	// Структура
	WORD DLLVersion = MAKEWORD(2, 2);		// Переменная, версия
	if (WSAStartup(DLLVersion, &wsaData) != 0)		// Версия, ссылка
	{
		std::cout << "Error load #01" << std::endl;
		exit(1);
	}

	cout << "Starting..." << endl;


	// Информация о хосте
	//struct adderinfo hints;
	//struct adderinfo * result;


	//// Информация об Адрессе сокета
	SOCKADDR_IN addr;		// Хранине адреса
	int sizeofadder = sizeof(addr);
	int port = 446860832;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");		// Айпи хранит
	addr.sin_port = htons(port);		// Порт (почти любой)
	addr.sin_family = AF_INET;		// Симество протаколов (надо писать)



	//// Прослушивание на опрделенном порту
	SOCKET sListen;
	sListen = socket(AF_INET, SOCK_STREAM, NULL);	//	Сокет который передает Симество, соединение, не надо
	// Привязка адресса сокета
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));	// Сокет, Указатель на структуру, Размер структуры
	// Прослушивание порта
	listen(sListen, SOMAXCONN);	// Сокет с адрессом, макс число запросов

	if (!file)
	{
		cout << "Error opening file !" << endl;
		Sleep(3000);
		exit(1);
	}


	cout << "Success! " << "port: " << port << endl << endl;



	//// Сокет для удержания клента
	SOCKET newConnection;
	for (int i = 0; i < 100; i++)
	{
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofadder);	// Сокет запущенный, Указатель на структуру, ссылка

		// Проверка на подключение юзера
		if (newConnection == 0)
		{
			std::cout << "Error connection #02" << std::endl;
		}
		else
		{
			countUsers++;
			std::cout << "New client connected! " << countUsers << std::endl;

			char msg[256] = "Welcome !";
			//cout << "> ";
			//cin >> msg;
			send(newConnection, msg, sizeof(msg), NULL);

			Connections[i] = newConnection;
			counter++;

			//sendAll(msg, counter);
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);

		}
	}




	return 0;
}