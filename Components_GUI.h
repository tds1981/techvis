#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <memory>
#include <cstddef>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class TButton
{
public:
     void SetButton(float X, float Y, string nameButton, bool typeButton = false);
     void draw(sf::RenderWindow &window);
     bool EventControl(sf::Event &event);
     bool state = false;
     bool ficsion = false;  
     float x; float y;
     const int w =135;
     const int h = 50; 
private:
	sf::Font font; 
	sf::Text caption;
    sf::RectangleShape rect;
};
 
class TScrolValue
{
public:
     float x; float y;
     const int w =35;
     const int h = 30;
     sf::Font font;   
     sf::Text ValuePrint;
     sf::RectangleShape Rectang; 
     sf::Color fon;  
     int Value = 0;
     int d_Value = 0;
     bool minus = false;
     bool shift = false;
     void Set(float X, float Y, sf::Color cl, int v = 0, int d_v = 5, bool sh = false);
     void draw(sf::RenderWindow &window);
     void EventControl(sf::Event &event, sf::Vector2i mouseP);
};


