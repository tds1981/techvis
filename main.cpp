#include <iostream>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <thread>

#include "GUI.h"
#include "IMG.h"
#include "Segmentation.h"

void takeFoto()
{
        string s = "ffmpeg -f v4l2  -s 640x480 -i /dev/video2 -vf \"fps=3\" f/f%d.bmp &";
        int result = system(s.c_str());
}

int main() 
{   
try
{
  setlocale(LC_CTYPE, "Russian"); // корректное отображение Кириллицы  
  //IMG *img1 = new IMG(640, 480, 0);  
  GUI W;

  takeFoto();   

  sf::RenderWindow window(sf::VideoMode(SiseXWindow,SiseYWindow), "Technical vision", sf::Style::Titlebar | sf::Style::Close);
  window.setVerticalSyncEnabled(true);
  window.clear(sf::Color::White);
       
  while (window.isOpen()) 
  {
        W.LoadFoto();
        
		sf::Event event;
        while(window.pollEvent(event))
		{
            if(event.type == sf::Event::Closed) 
            {
                string com = "kill 0";
                int result = system(com.c_str());
                com = "rm -r f";
                result = system(com.c_str());    
                window.close();
            };
            W.EventControl(event);
        }
        window.clear();
        W.draw(window);
		window.display();
  }
 
}
catch(...)
{printf("Error in main");} 
  return 0;
}
