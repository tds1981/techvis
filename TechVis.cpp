#include <iostream>
#include <cmath>
#include "IMG.h"
#include "Segmentation.h"
#include "IMGtoVector.h"
#include "Test_IMGtoVector.h"
#include "Test_IMG.h"
#include "Test_Segmentation.h"
#include <filesystem>


int main()
{
    std::cout << "Tests for TechVis\n";
    std::filesystem::path dir = "data/TestVector";
    std::filesystem::remove_all(dir);
    std::filesystem::create_directory(dir);

    dir = "data/rhombus"; 
    std::filesystem::remove_all(dir);
    std::filesystem::create_directory(dir);

    dir = "data/Leters";  
    std::filesystem::remove_all(dir); 
    std::filesystem::create_directory(dir);

    test_IMG();
    test_IMGtoVector();  
  //  test_Segmentation();
}

