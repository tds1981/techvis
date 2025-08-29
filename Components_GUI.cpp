#include "Components_GUI.h"

// -------------------- Button ---------------------------
void TButton::SetButton(float X, float Y, string nameButton, bool typeButton)
{
    x=X; y=Y;
    font.loadFromFile("Textures/arial.ttf");
    caption.setString(nameButton);
    caption.setFont(font);
    caption.setPosition(X+10, Y+20);
    caption.setCharacterSize(18);
    caption.setColor(sf::Color::Black);
    
    rect.setPosition(X, Y);
	rect.setSize(Vector2f(w, h));
	rect.setOutlineColor(Color::Magenta);
	rect.setOutlineThickness(3);
    rect.setFillColor(sf::Color::Yellow);

    ficsion = typeButton;
}
bool TButton::EventControl(sf::Event &event)
{  
   rect.setOutlineColor(Color::Red);  rect.setOutlineThickness(3);
 
   if (!ficsion) rect.setFillColor(sf::Color::Yellow);
   else if (state) rect.setFillColor(sf::Color::Green); else  rect.setFillColor(sf::Color::Yellow);

   if (event.type == sf::Event::MouseMoved && event.mouseMove.x > x && event.mouseMove.x < x+w && event.mouseMove.y > y && event.mouseMove.y < y+h)
      { rect.setOutlineColor(sf::Color::Blue);  rect.setOutlineThickness(5);   }
    
   if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
            && event.mouseButton.x > x && event.mouseButton.x < x+w && event.mouseButton.y > y && event.mouseButton.y < y+h)
      {  if (!ficsion) state = true; else state = !state;    }

   if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left
            && event.mouseButton.x > x && event.mouseButton.x < x+w && event.mouseButton.y > y && event.mouseButton.y < y+h)
   {  
      if (!ficsion) state = false; 
   }
      
    return false;
}

void TButton::draw(sf::RenderWindow &window)
{
     window.draw(rect);
     window.draw(caption);
}

//-----------------------------------------------------------------
void TScrolValue::Set(float X, float Y, sf::Color cl, int v, int d_v, bool sh)
{
    x=X; y=Y; fon= cl;
    font.loadFromFile("Textures/arial.ttf");
    ValuePrint.setFont(font);
    ValuePrint.setPosition(X+2, Y);
    ValuePrint.setCharacterSize(14);
    ValuePrint.setFillColor(sf::Color::Black);

    Rectang.setPosition(X, Y);
	Rectang.setSize(Vector2f(w, h));
	Rectang.setOutlineColor(sf::Color::Yellow);
	Rectang.setOutlineThickness(2);
    Rectang.setFillColor(fon);
    Value=v; ValuePrint.setString(to_string(Value));
    d_Value = d_v;
    shift = sh;
}

void TScrolValue::EventControl(sf::Event &event, sf::Vector2i mouseP)
{
    
    Rectang.setOutlineColor(sf::Color::Yellow); Rectang.setFillColor(fon); Rectang.setOutlineThickness(2);

    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left
            && event.mouseButton.x > x && event.mouseButton.x < x+w && event.mouseButton.y > y && event.mouseButton.y < y+h)
      {  uint8_t b = fon.r; fon.r = fon.g; fon.g = b; minus = !minus;  }
    
    if (event.type == sf::Event::MouseMoved && mouseP.x > x && mouseP.x < x+w && mouseP.y > y && mouseP.y < y+h)
      { Rectang.setOutlineColor(sf::Color::Blue);  Rectang.setOutlineThickness(3);   }
    
    if(event.type == sf::Event::MouseWheelScrolled && mouseP.x > x && mouseP.x < x+w && mouseP.y > y && mouseP.y < y+h)
    {
        Rectang.setOutlineColor(sf::Color::Blue);  Rectang.setOutlineThickness(3);
       // if(event.mouseWheel.delta>0) 
        if (!shift)
            if (minus) Value -= d_Value; else Value += d_Value;
        else if (minus) Value = Value>>1; else Value = (Value<<1) | 1;
        ValuePrint.setString(to_string(Value));
    }
       
}

void TScrolValue::draw(sf::RenderWindow &window)
{
    window.draw(Rectang);
    window.draw(ValuePrint);  
}

