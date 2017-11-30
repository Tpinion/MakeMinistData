#include"Image_to_BinaryData.h"

int main()
{
	Image2BinaryData IBD(28,28);
	cout<<"---------创建mnist数据集.-----------\n";
	string filefolder="/home/snowy/caffe/tools/mnist/creat_my_data/input_data/";//原始图片
	vector<string> fileLists = IBD.getFileLists(filefolder);
	
	const int size_list = fileLists.size();
	cout<<"Images Number : " <<size_list << endl;
	
	string translated_image__path = "/home/snowy/caffe/tools/mnist/creat_my_data/output_data/my_test_image_data";//转化后的图片数据集
	string translated_label_path = "/home/snowy/caffe/tools/mnist/creat_my_data/output_data/my_test_label_data";//转化后的标签数据集
	vector<cv::Mat> ImageMat;
	vector<int> image_labels(size_list);
	IBD.ReadImage(filefolder,fileLists,image_labels,ImageMat);
	IBD.Image2BinaryFile(translated_image__path,ImageMat,image_labels);
	IBD.Label2BinaryFile(translated_label_path,image_labels);
	
	return 0;
}
