#include "GUI.h"

void ppr1(int a)
{
    printf("button presed \n");
}

void ppr2(int a)
{
    
}

int GUI::LoadFoto()
{
    string nameF = "f/f"+ to_string(Nfile) +".bmp";
    string command2 = "rm -f " + nameF;
    //BITMAPINFOHEADER head; head.biSize = 0; 
    //head = Load_Bmp24(nameF, img , false);
    if(ImgBuf.loadFromFile(nameF))
    {
      Seg.Img = make_shared<IMG>(img_width, img_height, ImgBuf.getPixelsPtr(), Nfile);

      if (button[3].state)  Seg.Img->Monochrom({90, 90, 90, 0});
      if (button[4].state)  Seg.Img = Seg.DetectBorders();
      if (button[5].state)  Seg.Img = Difference(); 
      
      texture.update( Seg.Img->GetPtr() );  
      sprite.setTexture(texture);

      int result = system(command2.c_str()); 
      Nfile++; 
    }
    else printf("file %s not found", nameF);
    return 1;  
}

bool  GUI::EventControl(sf::Event &event)
{
    for(int i=0; i < 6; i++) button[i].EventControl(event);
    
    if (button[0].state) TuningFon();
    if (button[1].state) ProcessingImg();
    if (button[2].state) PutMask();
   // if (button[3].state) Monohrom();  
    
    if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right && SelectArea.x1 ==0 && SelectArea.y1 ==0  && event.mouseButton.x > 0 && event.mouseButton.x < img_width && event.mouseButton.y > 0 && event.mouseButton.y < img_height)      
   {
        SelectRec.setPosition(event.mouseButton.x, event.mouseButton.y); 
        SelectArea.x1 = event.mouseButton.x; SelectArea.y1 = event.mouseButton.y;
        Message.setString("Message: x1 ="+to_string(SelectArea.x1)+"  y1 = "+to_string(SelectArea.y1)+ " x2 = "+ to_string(SelectArea.x2)+ " y2 = "+    to_string(SelectArea.y2));
    SelectArea.name[0] = 'A';
   }

  if (event.type == sf::Event::MouseMoved && event.mouseMove.x > SelectArea.x1  && event.mouseMove.y > SelectArea.y1 && SelectArea.name[0] == 'A' )
    {  SelectArea.x2 = event.mouseMove.x; SelectArea.y2 = event.mouseMove.y;
	   SelectRec.setSize(Vector2f(SelectArea.x2 - SelectArea.x1, SelectArea.y2 - SelectArea.y1));  
        Message.setString("Message: x1 ="+to_string(SelectArea.x1)+"  y1 = "+to_string(SelectArea.y1)+ " x2 = "+ to_string(SelectArea.x2)+ " y2 = "+    to_string(SelectArea.y2));
    }
    if(event.type == sf::Event::MouseButtonReleased) SelectArea.name[0] = 0;

    return true;
}
void GUI::ProcessingImg()
{
    printf("button presed \n");
    // shared_ptr<IMG> img = make_shared<IMG>(img_width, img_height, 0);
    // Segmentation Seg(img);
    // Seg.Monochrom();
    //shared_ptr<IMG> imgOut = Seg.DetectBorders();
    TArea A = Seg.FindColorArea();
    SelectRec.setPosition(A.x1, A.y1);
    SelectRec.setSize(Vector2f(A.x2 - A.x1, A.y2 - A.y1));

    shared_ptr<IMG> img_A = Seg.Img->GetSubImg(A); 
    string NameFile = "f/Result"+ to_string(Nfile) +".bmp";
    img_A ->Save_Bmp(NameFile);

    Message.setString("Message: find aria x1="+to_string(A.x1)+"  y1 = "+to_string(A.y1)+ " x2 ="+ to_string(A.x2)+ " y2 ="+ to_string(A.y2));     
}

void GUI::TuningFon()
{
    printf("button presed \n");
    for(int i = 25; i<255; i+=25)
    { 
        shared_ptr<IMG> buf = Seg.Img->GetCopy();
        buf->Monochrom({i, i, i, 0});
        buf->Save_Bmp("f/tun/IMG_mono"+ to_string(i) +".bmp");
    }
    
    map<int, int> fon = Seg.TuningBackground(SelectArea);
    string NameFile = "f/Result"+ to_string(Nfile) +".bmp";
    
    shared_ptr<IMG> img_A = Seg.Img->GetSubImg(SelectArea); 
    img_A ->Save_Bmp("f/Fon"+ to_string(Nfile) +".bmp");

    Message.setString("Message: fon saved. Size aria = "+ to_string(Seg.SizeSegment.x)+" x "+to_string(Seg.SizeSegment.y)+"   "+" R = "+to_string(Seg.fon.Red)+" G = "+to_string(Seg.fon.Green)+" B = "+to_string(Seg.fon.Blue));
   
    SelectArea = {0, 0, 0, 0}; 
    SelectRec.setSize(Vector2f(0, 0));          
}

void GUI::PutMask()
{
    shared_ptr<IMG> bufR = Seg.Img->GetCopy();
    bufR->PutMask({0, 0, 0xff, 0});
    bufR->Save_Bmp("f/Red"+ to_string(Nfile) +".bmp");
    //bufR->Monochrom({0, 0, 0x7f, 0});
    //bufR->Save_Bmp("f/Red_mono"+ to_string(Nfile) +".bmp");
    // Segmentation Seg_R(bufR);
    // Seg_R.DetectBorders();
    // Seg_R.OutImg->Save_Bmp("f/Red_br"+ to_string(Nfile) +".bmp");

    shared_ptr<IMG> bufG = Seg.Img->GetCopy();
    bufG->PutMask({0, 0xff, 0, 0});
    bufG->Save_Bmp("f/Green"+ to_string(Nfile) +".bmp");
    //bufG->Monochrom({0, 0x7f, 0, 0});
    //bufG->Save_Bmp("f/Green_mono"+ to_string(Nfile) +".bmp");
   // Segmentation Seg_G(bufG);
   // Seg_G.DetectBorders();
   // Seg_G.OutImg->Save_Bmp("f/Green_br"+ to_string(Nfile) +".bmp");

    shared_ptr<IMG> bufB = Seg.Img->GetCopy();
    bufB->PutMask({0xff, 0, 0, 0});
    bufB->Save_Bmp("f/Blue"+ to_string(Nfile) +".bmp");
    //bufB->Monochrom({0x7f, 0, 0, 0});
    //bufB->Save_Bmp("f/Blue_mono"+ to_string(Nfile) +".bmp");
    //Segmentation Seg_B(bufB);
    //Seg_B.DetectBorders();
    //Seg_B.OutImg->Save_Bmp("f/Blue_br"+ to_string(Nfile) +".bmp");

    Message.setString("Message: put mask");     
}

void GUI::Monohrom()
{
    //Seg.Img->Monochrom({90, 90, 90, 0});
    //Seg.Img->Save_Bmp("f/IMG_mono"+ to_string(Nfile) +".bmp");
    //Seg.DetectBorders()->Save_Bmp("f/IMG_br"+ to_string(Nfile) +".bmp");

    Message.setString("Message: get monohrom");     
}
void GUI::DetectBorders()
{
    //Seg.Img->Save_Bmp("f/IMG_mono"+ to_string(Nfile) +".bmp");
    //->Save_Bmp("f/IMG_br"+ to_string(Nfile) +".bmp");

    Message.setString("Message: get Borders");     
}

shared_ptr<IMG> GUI::Difference()
{
    static shared_ptr<IMG> Previous = nullptr;
    static shared_ptr<IMG> Dif = nullptr;
    int countDif = 0;
    if (Previous != nullptr) 
    { 
        Dif = Seg.Difference(Previous, Seg.Img, countDif); 
        //texture.update( Dif->GetPtr() ); 
        if (countDif > Dif->size()/20) Dif->Save_Bmp("f/dif/"+ to_string(Nfile) +".bmp");
        Message.setString("Message: countDif="+to_string(countDif)+" size="+to_string(Dif->size()));
    };  
   Previous = Seg.Img;  
   if (Dif != nullptr) return Dif; else return Seg.Img; 
}

// -------------------- Button ---------------------------
void TButton::SetButton(void(*op)(int), float X, float Y, string nameButton, bool typeButton)
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

    click = op; ficsion = typeButton;
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
   {  click(1); 
      if (!ficsion) state = false; 
   }
      
    return false;
}

void TButton::draw(sf::RenderWindow &window)
{
     window.draw(rect);
     window.draw(caption);
}
