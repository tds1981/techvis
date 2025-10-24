#include "IMG.h"
#include "BMP.h"
#include "Segmentation.h"
#include <filesystem>

std::filesystem::path dir = "data/test_Segmentation/";

void test_Segmentation()
{
	std::filesystem::remove_all(dir);
	std::filesystem::create_directory(dir);

	shared_ptr<IMG> img1 = make_shared<IMG>("data/foto1.bmp");
	Segmentation::DetectBorders(img1)->Save_Bmp(dir.string() + "DB1.bmp");
	Segmentation::FastDetectBorders(img1, 382)->Save_Bmp(dir.string() + "fast_DB1.bmp");

	img1->PutMask({ 0xf0, 0xf0, 0xf0, 0 });
	vector<TColorStat> stat = Segmentation::Histogram(img1, {0, 0, img1->width, img1->height}, true);

	Segmentation::DetectBorders(img1)->Save_Bmp(dir.string() + "DB2.bmp");

	for (unsigned int i = 25; i <= 250; i += 25)
	{
		TColorPix M;
		shared_ptr<IMG> img2 = img1->GetCopy();
		//img2->PutMask();
		unsigned char k = i & 0xff;
		img2->Monochrom({ k, k, k, k});
		//img2->Contrast({ 127, 127, 127, 127 }, i+50);
		img2->Save_Bmp(dir.string() + to_string(i)+"monohrom.bmp");
		/*shared_ptr<IMG> img3 = Segmentation::FastDetectBorders(img2);
		img3->Save_Bmp(dir.string() + to_string(i) + "FastDetectBorders.bmp");
		shared_ptr<IMG> img4 = Segmentation::DetectBorders(img2);
		img4->Save_Bmp(dir.string() + to_string(i) + "DetectBorders.bmp");
		//img1->PaintInConsol();*/
	}
	cin.get();
}