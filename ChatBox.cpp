#include "ChatBox.h"
#include "Client.h"
#include "MessageBox.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <iostream>

ChatBox::ChatBox() {
}

ChatBox::ChatBox(const ChatBox& orig) {
}

ChatBox::~ChatBox() {
}

void ChatBox::run(){
    
    port = 8080;
    adres= sf::IpAddress("127.0.0.1");
    std::string name;
    std::string schowek;
    std::string text1 = "Podaj nazwe uzytkownika (max 10 bez polskich znakow)";
    std::string text2 =  "A nastepnie kliknij przycisk 'Start Client'";
    //unsigned short port=8080;
   // sf::IpAddress a2("127.0.0.1");
    // 1. odczytać name klienta z pola tekstowego w oknie
    // 2. podstawić client1.name = to co odczytałem
    // 3 uruchomić: client1.connect(); w oddzielnym  wątku:
    // status=client.connect(a2, port);
    // 4. jeśli status ok, to wyświetlić oddzielne okno ( z name w tytule ) do rozmow
    // 5. ja client odbiera komunikat to wyświetlić go w tym oknie,
    // 6. jak wpisałem tekst mojego komunikatu w polu tekstowym okna to wywołać client.send(type, tekst)
    
    
    int stan_okna = 0; //przechowywanie wartości która mówi jakie tło podstawić
     // making window :
    sf::RenderWindow window(sf::VideoMode(400, 800), "Paplanina v1.0");
    
    // prostokat pod tekst wpisywany
    sf::RectangleShape rectangle;
    rectangle.setSize(sf::Vector2f(220, 50));
    rectangle.setFillColor(sf::Color::Cyan);
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(5);
    rectangle.setPosition(90, 270);
    
    // ładowanie textów
    sf::Text texto;
    sf::Font font;
    texto.setFont(font);
    font.loadFromFile("resources/czcionka.ttf");
    sf::Text login;
    sf::Text wskazowka;
    wskazowka.setFont(font);
    sf::Text wskazowka2;
    wskazowka2.setFont(font);
    
    // tekst wpisywany
    login.setFont(font);
    login.setCharacterSize(24);
    login.setColor(sf::Color::Black);
    login.setPosition(100,280);
    // text kontaktu
    texto.setString(name);
    texto.setCharacterSize(26); 
    texto.setColor(sf::Color::Red);
    texto.setPosition(50,60);
    // tekst instrukcji
    wskazowka.setCharacterSize(14);
    wskazowka.setColor(sf::Color::Black);
    wskazowka.setPosition(5,350);
    wskazowka.setString(text1);
    wskazowka2.setCharacterSize(14);
    wskazowka2.setColor(sf::Color::Black);
    wskazowka2.setPosition(50,380);
    wskazowka2.setString(text2);
    
    
    
    // adding images :
    sf::Texture background; //background
    background.loadFromFile("resources/back.png");
    sf::Texture but2; // client button
    but2.loadFromFile("resources/client.png");
    sf::Texture kontakty;//tło kontaktow
    kontakty.loadFromFile("resources/kontakty.png");
    sf::Texture pop; // przycisk wylogowania
    pop.loadFromFile("resources/wyloguj.png");
    
    // creating sprites: 
    
    sf::Sprite spriteb; 
    spriteb.setTexture(background);
    spriteb.setPosition(0, 0);
    
    sf::Sprite buton2;
    buton2.setTexture(but2);
    buton2.setPosition(115, 200);
    
    sf::Sprite kontakt;
    kontakt.setTexture(kontakty);
    kontakt.setPosition(0,0);
    
    sf::Sprite wyloguj;
    wyloguj.setTexture(pop);
    wyloguj.setPosition(280,750);
    
  //  Client client1();
    
    // program main loop:
    
    while (window.isOpen())
    {
        
        
        sf::Event event;
        while (window.pollEvent(event))
        {   
           
            
            if (event.type == sf::Event::Closed){
                window.close();
            }
           
            if(event.type==sf::Event::TextEntered && stan_okna == 0 && schowek.size()<=9 )
            {
		char code=static_cast<char>(event.text.unicode);
		
		if(event.text.unicode==13 && schowek.size()!=0)//enter
		{
                    stan_okna = 1;
                    name = schowek;
                    schowek = "";
		}

		else if(code!='\b'){
			schowek.push_back(code);
                        login.setString(schowek);}
		else if(code=='\b')
		{
			if(schowek.size()>0)
//				schowek.pop_back();
                                login.setString(schowek);
                                
		}
                else login.setString(schowek);
            }
            
            sf::Vector2i position = sf::Mouse::getPosition(window);
            //std::cout<<position.y<<std::endl;
            
            if (stan_okna==0 && schowek.size()!=0 && sf::Mouse::isButtonPressed(sf::Mouse::Left) && position.x>=115 && position.x<=285 && position.y>=200 && position.y<=246)
            {   
                name = schowek;
                schowek = "";
                std::cout<<"Zalogowano"<<std::endl;
                stan_okna = 1;
                
                
                //sf::Socket::Status status;
                status = client.connect(adres, port);
                
                    
                    if(status!=sf::Socket::Done)
                    {
                       std::cout<<"Sorry we couldn't connect\n";
                       //return -1;
                    }
                    client.send(INITIAL_NAME_DATA, name);
                    
                    
                
            }
            if (stan_okna==1 && sf::Mouse::isButtonPressed(sf::Mouse::Left) && position.y>=65 && position.y <=90)
            {
                std::cout<<"Rozpoczynasz rozmowe"<<std::endl;
                mbox.run(& client, & status, name, & potrzebne);
            }
            if (stan_okna==1 && sf::Mouse::isButtonPressed(sf::Mouse::Left) && position.x>=280 && position.y>=750 )
            {   
                
                stan_okna = 0;
                login.setString(schowek);
                
                
                
            }
        }
        status = client.receive(potrzebne);
        
        window.clear();
        if (stan_okna == 0){
            window.draw(spriteb);
            window.draw(rectangle);
            window.draw(buton2);
            window.draw(login);
            window.draw(wskazowka);
            window.draw(wskazowka2);
            }
        if(stan_okna == 1) {
            texto.setString(name);
            window.draw(kontakt);
            window.draw(wyloguj);
            window.draw(texto);
            }
        window.display();
        
    }
    
}

