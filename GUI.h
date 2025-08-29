#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <cstddef>
#include <ctime>
#include <chrono>
#include <SFML/Graphics.hpp>
#include "Components_GUI.h"
#include "IMG.h"
#include "Segmentation.h"
#include "IMGtoVector.h"

using namespace std;
using namespace sf;

const int img_width = 640;
const int img_height = 480;
const float SiseXWindow = img_width + 360;
const float SiseYWindow = img_height + 60;

class GUI
{
public:
 GUI() {    
         button[1].SetButton(img_width+15, 20, "Processing"); 
         button[0].SetButton(img_width+180, 20, "Tuning");            
         
         button[3].SetButton(img_width+15, button[1].y+button[1].h+20, "Monohrom", true); 
         LevelMonohrom[0].Set(img_width+180, button[3].y+5, sf::Color::Cyan);  
         LevelMonohrom[1].Set(img_width+205+LevelMonohrom[0].w, button[3].y+5, sf::Color::Cyan);
         LevelMonohrom[2].Set(img_width+235+2*LevelMonohrom[0].w, button[3].y+5, sf::Color::Cyan);    

         button[6].SetButton(img_width+15,  button[3].y+button[3].h+20, "Brightness", true);
         LevelBrightness.Set(img_width+180, button[6].y+5, sf::Color::Cyan);
         TextSetting(Brightness_value, img_width+250, button[3].y+button[3].h+20, "Brightness_value");

         button[7].SetButton(img_width+15,  button[6].y+button[6].h+20, "Contrast", true);
         LevelContrast.Set(img_width+180, button[7].y+5, sf::Color::Cyan);

         button[2].SetButton(img_width+15, button[7].y+button[7].h+20, "Put mask", true);
         LevelMask[0].Set(img_width+180, button[2].y+5, sf::Color::Cyan, 1, 1, true);  
         LevelMask[1].Set(img_width+205+LevelMonohrom[0].w, button[2].y+5, sf::Color::Cyan, 1, 1, true);
         LevelMask[2].Set(img_width+235+2*LevelMonohrom[0].w, button[2].y+5, sf::Color::Cyan, 1, 1, true);   

         button[4].SetButton(img_width+15, button[2].y+button[2].h+20, "DetectBorders", true);  
         LevelBorders.Set(img_width+180,   button[4].y, sf::Color::Cyan);
  
         button[5].SetButton(img_width+15, button[4].y+button[4].h+20, "Difference", true);
   
         if(!texture.create(img_width, img_height)) printf("Error create texture \n");      
         Nfile = 1;   
         TextSetting(Message, 10, img_height + 10, "Message: ");
        // for(int i=0; i<5; i++) { Settings[i].setPosition(img_width+15, 85+i*20); Settings[i].setFont(font); 
        //                          Settings[i].setCharacterSize(16);               Settings[i].setColor(Color::White);}                   
         
         
        // Settings[0].setString("LevelMonohrom");
         SelectRec.setOutlineColor(sf::Color::Green); 
         SelectRec.setOutlineThickness(2); 
         SelectRec.setFillColor(sf::Color::Transparent);  
       } 
 bool EventControl(sf::Event &event); 
 int LoadFoto();  
 
 sf::Image ImgBuf;   //текущее изображение которое демонстрируется на экране
 sf::Vector2i mouseP;
 TButton button[8];  
 TScrolValue LevelMonohrom[3];    
 TScrolValue LevelMask[3]; 
 TScrolValue LevelBrightness;
 TScrolValue LevelContrast;
 TScrolValue LevelBorders;
 sf::Text Brightness_value;
 sf::Text Contrast_value;
 int Nfile;
 sf::Texture texture;   
 sf::Sprite sprite; // Создание спрайта 
 sf::Font font;    
 sf::Text Message;   
 sf::RectangleShape SelectRec; 
 TArea SelectArea = {0, 0, 0, 0, ""}; 
 long int timer = 0; 
 Segmentation Seg;
 void ProcessingImg();  
 void ProcessingImg1(); 
 void TuningFon();
 void PutMask();
 void Monohrom();  
 void DetectBorders();   
 shared_ptr<IMG> Difference(); 
 void TextSetting(sf::Text &t, unsigned int x, unsigned int y, string s);
 void draw(sf::RenderWindow &window)
 {
        window.draw(sprite);
        window.draw(Message);
        window.draw(SelectRec);
        //for(int i = 0; i < 5; i++) window.draw(Settings[i]);
        for(int i = 0; i < 3; i++)   LevelMonohrom[i].draw(window);
        for(int i = 0; i < 3; i++)   LevelMask[i].draw(window);
        LevelBrightness.draw(window);
        LevelContrast.draw(window);
        LevelBorders.draw(window);

        window.draw(Brightness_value);
        for(int i = 0; i < 8; i++) button[i].draw(window);

 }
};








