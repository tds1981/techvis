#include "GUI.h"

int GUI::LoadFoto()
{
    string nameF = "f/f"+ to_string(Nfile) +".bmp";
    string command2 = "rm -f " + nameF;
    //BITMAPINFOHEADER head; head.biSize = 0; 
    //head = Load_Bmp24(nameF, img , false);
    if(ImgBuf.loadFromFile(nameF))
    {
      Seg.Img = make_shared<IMG>(img_width, img_height, ImgBuf.getPixelsPtr(), Nfile);
 
      if (button[3].state)  Seg.Img->Monochrom({0xff & (uint8_t)LevelMonohrom[0].Value, 0xff & (uint8_t)LevelMonohrom[1].Value, 0xff & (uint8_t)LevelMonohrom[2].Value, 0xff});
      if (button[4].state)  {  Seg.Img = Seg.FastDetectBorders(nullptr, LevelBorders.Value);} //Seg.DetectBorders();
      if (button[5].state)  Seg.Img = Difference(); 
      if (button[6].state)  Brightness_value.setString( to_string(Seg.Img->Brightness(-1*LevelBrightness.Value)) );
      if (button[7].state)  Seg.Img->Contrast({127, 127, 127, 0}, LevelContrast.Value);
      if (button[2].state)  Seg.Img->PutMask({0xff & (uint8_t)LevelMask[0].Value, 0xff & (uint8_t)LevelMask[1].Value, 0xff & (uint8_t)LevelMask[2].Value, 0xff}); 
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
    for(int i=0; i < 8; i++) button[i].EventControl(event);
    for(int i=0; i < 3; i++) LevelMonohrom[i].EventControl(event, mouseP);

    LevelBrightness.EventControl(event, mouseP);
    LevelContrast.EventControl(event, mouseP);
    LevelBorders.EventControl(event, mouseP);
    for(int i=0; i < 3; i++) LevelMask[i].EventControl(event, mouseP);

    if (button[1].state)  ProcessingImg1(); //else {Seg.FonImg = nullptr; timer = 0;}
    if (button[0].state) TuningFon();
    
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
    static bool ProcesStart = false;
    int procentDiff = Seg.CompareFrames();

    if (time(NULL) - timer < 5)  Message.setString("Message: whaiting timer="+to_string(timer)); else timer =0;
   
    if(ProcesStart == false && timer == 0)
      if (procentDiff < 2 &&  Seg.FonImg ==nullptr ) { Seg.FonImg = Seg.Img; Message.setString("Message: fon seted, whaiting image timer="+to_string(timer));}
      else 
      if (procentDiff > 20) 
      { 
        ProcesStart = true; timer=0;
        Message.setString("Message: start processing");
      }
      
    if(ProcesStart && procentDiff < 20)  
      {
          int count;
          shared_ptr<IMG> Dif = Seg.Difference(Seg.FonImg, Seg.Img, count); 
        
          //Dif->Save_Bmp("f/seg/f_"+to_string(Seg.Img->Nimg)+".bmp");
          //Seg.FonImg->Save_Bmp("f/seg/fon_"+to_string(Seg.FonImg->Nimg)+".bmp");
          Seg.Img->Save_Bmp("f/seg/img_"+to_string(Seg.Img->Nimg)+".bmp");
          Message.setString("Message: start colors, Save file f/seg/f_"+to_string(Seg.Img->Nimg)+".bmp");  

          shared_ptr<IMG> im = Seg.Img->GetCopy();
          if (! button[3].state)  im->Monochrom(Seg.LevelMonohrom); 
          vector<TArea> Clusters;     
          shared_ptr<IMG> out = Seg.PaintClusters(im, 1, Clusters);
          for(auto it=Clusters.begin(); it<Clusters.end(); it++)
                    Seg.Img->GetSubImg(*it)->Save_Bmp("f/seg/s_"+to_string(it->x1)+"_"+to_string(it->y1)+".bmp");  
          im->Save_Bmp("f/seg/mono"+to_string(Seg.Img->Nimg)+".bmp");  
          out->Save_Bmp("f/seg/colors"+to_string(Seg.Img->Nimg)+".bmp"); 

          ProcesStart = false;
          timer = time(NULL);
      }    
}

void GUI::ProcessingImg1()
{     
      auto t0 = chrono::high_resolution_clock::now();    

      shared_ptr<IMG> im = Seg.Img->GetCopy();
      //if (! button[3].state)  im->Monochrom(Seg.LevelMonohrom); 
      shared_ptr<IMG> im_db = Seg.FastDetectBorders(im, 110);  
      vector<TArea> Clusters;     
      shared_ptr<IMG> out = Seg.PaintClusters(im_db, 5, Clusters, {0xff, 0xff, 0xff, 0xff});
      
      auto t1 = chrono::high_resolution_clock::now();
      Message.setString("Message: prosess time: "+to_string(chrono::duration_cast<chrono::milliseconds>(t1-t0).count())+" ms");  
      
      for(auto it=Clusters.begin(); it<Clusters.end(); it++) 
        if(Seg.FilterBrightnessClusters(*it) && Seg.FilterSizeClusters(*it))  
            Seg.Img->GetSubImg(*it)->Save_Bmp("f/seg/s_"+to_string(it->x1)+"_"+to_string(it->y1)+".bmp"); 
          
      im_db->Save_Bmp("f/seg/mono"+to_string(Seg.Img->Nimg)+".bmp");  
      out->Save_Bmp("f/seg/colors"+to_string(Seg.Img->Nimg)+".bmp"); 
      Seg.Img->Save_Bmp("f/seg/img_"+to_string(Seg.Img->Nimg)+".bmp");
  
  
}

void GUI::TuningFon()
{
   /* for(int i = 25; i<255; i+=25)
    { 
        shared_ptr<IMG> buf = Seg.Img->GetCopy();
        buf->Monochrom({i, i, i, 0});
        buf->Save_Bmp("f/tun/IMG_mono"+ to_string(i) +".bmp");
    }*/
    if(SelectArea.x1 != SelectArea.x2 && SelectArea.y1 != SelectArea.y2)
    {
        Seg.LevelMonohrom = {Seg.LevelMonohrom.Blue+10, Seg.LevelMonohrom.Green+10, Seg.LevelMonohrom.Red+10, Seg.LevelMonohrom.A+10};
        map<int, int> fon = Seg.TuningBackground(SelectArea);
        shared_ptr<IMG> findSegment = Seg.Img->GetSubImg(SelectArea); 
        findSegment = Seg.FastDetectBorders(findSegment, 110);  
        findSegment->Save_Bmp("f/seg/findSegment"+ to_string(Nfile) +".bmp");
        VectorPoints Vp(findSegment, {0xff, 0xff, 0xff, 0xff});
        Vp.GetImg(false)->Save_Bmp("f/seg/restored"+ to_string(Nfile) +".bmp");
        Vp.SortLength();
        Vp.PrintLog();
    }
    
    Message.setString("Message: Size segment = "+ to_string(Seg.FindSegmentSize.x)+" x "+to_string(Seg.FindSegmentSize.y)+"   "+" R = "+to_string(Seg.LevelMonohrom.Red)+" G = "+to_string(Seg.LevelMonohrom.Green)+" B = "+to_string(Seg.LevelMonohrom.Blue));
   
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
        Message.setString("Message: countDif="+to_string(countDif)+" size="+to_string(Dif->size())+" procent= "+to_string(countDif * 100 / Dif->size()));
    };  
   Previous = Seg.Img;  
   if (Dif != nullptr) return Dif; else return Seg.Img; 
}

void GUI::TextSetting(sf::Text &t, unsigned int x, unsigned int y, string s)
{
    font.loadFromFile("Textures/arial.ttf");
    t.setString(s.c_str());
    t.setFont(font);          t.setPosition(x, y);
    t.setCharacterSize(16);   t.setColor(sf::Color::White); 
}


