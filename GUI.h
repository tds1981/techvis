#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <cstddef>
#include <SFML/Graphics.hpp>
#include "IMG.h"
#include "Segmentation.h"

using namespace std;
using namespace sf;

typedef void(*op)(int p1);

void ppr1(int a);
void ppr2(int a);

const int img_width = 640;
const int img_height = 480;
const float SiseXWindow = img_width + 360;
const float SiseYWindow = img_height + 60;

class TButton
{
public:
     void SetButton(void(*op)(int),float X, float Y, string nameButton, bool typeButton = false);
     void draw(sf::RenderWindow &window);
     bool EventControl(sf::Event &event);
     void (*click)(int);
     bool state = false;
     bool ficsion = false;   
private:
    float x; float y;
    const int w =150;
    const int h = 50;
	sf::Font font;
    //sf::Texture texture;   
    //sf::Sprite sprite; // Создание спрайта  
	sf::Text caption;
    sf::RectangleShape rect;
};

// ------------------------------------------------------------------

class GUI
{
public:
 GUI() {
         button[0].SetButton(&ppr1, img_width+15, 20, "Tuning"); 
         button[1].SetButton(ppr2, img_width+15, 80, "Processing");  
         button[2].SetButton(ppr2, img_width+15, 140, "Put mask");   
         
         button[3].SetButton(ppr2, img_width+15, 240, "Monohrom", true); 
         button[4].SetButton(ppr2, img_width+15, 300, "DetectBorders", true);    
         button[5].SetButton(ppr2, img_width+15, 360, "Difference", true);
   
         if(!texture.create(img_width, img_height)) printf("Error create texture \n");      
         Nfile = 1;   
         font.loadFromFile("Textures/arial.ttf");
         Message.setString("Message: ");
         Message.setFont(font);
         Message.setPosition(10, img_height + 10);
         Message.setCharacterSize(18);
         Message.setColor(sf::Color::White);   
         SelectRec.setOutlineColor(sf::Color::Green); 
         SelectRec.setOutlineThickness(2); 
         SelectRec.setFillColor(sf::Color::Transparent);  
       } 
 bool EventControl(sf::Event &event); 
 int LoadFoto();  
 
 sf::Image ImgBuf;   //текущее изображение которое демонстрируется на экране
 TButton button[6];  
    
 int Nfile;
 sf::Texture texture;   
 sf::Sprite sprite; // Создание спрайта 
 sf::Font font;    
 sf::Text Message;
 sf::RectangleShape SelectRec; 
 TArea SelectArea = {0, 0, 0, 0, ""};  
 Segmentation Seg;
 void ProcessingImg();   
 void TuningFon();
 void PutMask();
 void Monohrom();  
 void DetectBorders();   
 shared_ptr<IMG> Difference(); 
 void draw(sf::RenderWindow &window)
 {
        window.draw(sprite);
        window.draw(Message);
        window.draw(SelectRec);
        for(int i=0; i < 6; i++) button[i].draw(window);
       // button2.draw(window);
       // button3.draw(window);
       // button4.draw(window);
       // button5.draw(window);

 }
};








