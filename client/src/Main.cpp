#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <string>
#include <iostream>

using namespace std;

void multi_connect(char mode, sf::TcpSocket &socket, sf::IpAddress ip);
void multi_senttext(string text, sf::TcpSocket &socket);
void multi_recievedtext(string &text, bool &check, sf::TcpSocket &socket);

void check_resolutionscreen(double &ScreenScale);

int main(){

	// สำหรับ multiplayer
	sf::TcpSocket socket;
	string ip;
	cout << "input your ip : ";
	cin >> ip;
	char status_connect = 'c'; // s = server, c = client
	multi_connect(status_connect,socket,ip);
	string text_sent, text_recieved;
	bool check = false;

	//สำหรับปรับ resolution หน้าจอ
	double ScreenScale;
	check_resolutionscreen(ScreenScale);


	// สำหรับขนาด windows
	sf::RenderWindow window(sf::VideoMode(1000*ScreenScale,680*ScreenScale), "REAIxProcom : Survive escape from atlantis");

	// โหลดภาพ png
	sf::Texture png;
	png.loadFromFile("image\\bg_startgame.jpg");

	sf::Sprite img;
	img.setTexture(png);
	img.setPosition(0,0);

	//ลูปโปรแกรม
	while (window.isOpen()){
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		multi_recievedtext(text_recieved, check, socket);
		if(check){
			cout << text_recieved << endl;
			check = false;
		}

        window.clear();

		window.draw(img);

        window.display();
    }

	return 0;

}

void multi_connect(char mode, sf::TcpSocket &socket, sf::IpAddress ip){
	if(mode == 's'){
		sf::TcpListener listenner;
		listenner.listen(2000);
		listenner.accept(socket);
	}
	else{
		socket.connect(ip, 2000);
	}
	cout << "Connect Success!!!" << endl;
}

void multi_senttext(string text, sf::TcpSocket &socket){
	socket.send(text.c_str(), text.length()+1);
}

void multi_recievedtext(string &text, bool &check, sf::TcpSocket &socket){
	char buffer[2000];
	size_t recieved;
	socket.receive(buffer, sizeof(buffer), recieved);
	if(recieved > 0){
		text = buffer;
		check = true;
	}
}

void check_resolutionscreen(double &ScreenScale){
	double ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenScale = ScreenWidth/1366;
}