#include"Image_to_BinaryData.h"
#include<cstdlib>
#include<fstream>
#include<stdio.h>
#include<unistd.h>
#include<dirent.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<string.h>
using namespace cv;

void Image2BinaryData::ReverseInt(ofstream &file,int i)
{
	uchar ch1 = (uchar)(i>>24);
	uchar ch2 = (uchar)((i<<8)>>24);
	uchar ch3 = (uchar)((i<<16)>>24);
	uchar ch4 = (uchar)((i<<24)>>24);
	file.write((char*)&ch1,sizeof(ch4));
	file.write((char*)&ch2,sizeof(ch3));
	file.write((char*)&ch3,sizeof(ch2));
	file.write((char*)&ch4,sizeof(ch1));
}

vector<string> Image2BinaryData::getFileLists(string file_folder)
{
	const char *mystr = file_folder.c_str();
	vector<string> flist;
	string lineStr;
	vector<string> extendName;
	extendName.push_back("JPG");	
	extendName.push_back("jpg");
	extendName.push_back("bmp");
	extendName.push_back("png");
	extendName.push_back("tiff");

	const char* c_file_folder;
	c_file_folder=file_folder.c_str();
	struct stat s;
	lstat(c_file_folder,&s);
	if(! S_ISDIR(s.st_mode))
	{
		cout<<file_folder<<"路径不存在 !"<<endl;
		return flist;
	}
	struct dirent * file;
	DIR * dir;
	dir = opendir(c_file_folder);
	if(NULL==dir)
	{cout<<"无法打开" <<file_folder<<endl;}
	cout<<"成功打开"<<file_folder<<endl;

	while((file=readdir(dir))!=NULL)
	{
		if(strcmp(file->d_name,".")==0 || strcmp(file->d_name,"..")==0)
			continue;
		lineStr=file->d_name;
		flist.push_back(lineStr);
	}
	return flist;
}

string Image2BinaryData::getFileName(string & filename)
{
	int iBeginIndex = filename.find_last_of("/")+1;
	int iEndIndex = filename.length();
	return filename.substr(iBeginIndex,iEndIndex - iBeginIndex);
	cout<<"完成获取文件名工作"<<endl;
}

void Image2BinaryData::ReadImage(string filefolder,vector<string>& img_Lists,vector<int>& img_Labels,vector<cv::Mat> &ImageMat)
{
	const int size_list = img_Lists.size();
	for (int i=0;i<size_list;i++)
	{
		string curPath = filefolder + "/"+img_Lists[i];
		Mat image=imread(curPath,IMREAD_UNCHANGED);
		ImageMat.push_back(image);
		char label = img_Lists[i][0];
		img_Labels[i]=label-'0';
		printf("正在读取图片，请等候:%.2f%%\r",i*100.0/(size_list-1));
	}
	printf("\n图片读取完成!\n\n");
}

void Image2BinaryData::Image2BinaryFile(string filefolder, vector<cv::Mat>& ImagesMat, vector<int>& img_Labels)
{
	
	ofstream file(filefolder.c_str(),ios::binary);
	int idx = filefolder.find_last_of("/")+1;
	string subName = filefolder.substr(idx);
	if(file.is_open())
	{
		cout<<subName<<" 成功创建."<<endl;
		int magic_number=2051;
		int number_of_images=img_Labels.size();
		int n_rows = Height;
		int n_cols = Width;
		ReverseInt(file,magic_number);
		ReverseInt(file,number_of_images);
		ReverseInt(file,n_rows);
		ReverseInt(file,n_cols);

		cout<<"There are "<<ImagesMat.size()<<" 图片需要转换."<<endl;
		for(int i =0;i<ImagesMat.size();i++)
		{
			Mat dstImg;
			cvtColor(ImagesMat[i],dstImg,CV_BGR2GRAY);//灰度化图片
			for(int r=0;r<n_rows;r++)
			{
				for(int c=0;c<n_cols;c++)
				{
					uchar tmp=dstImg.at<uchar>(r,c);
					file.write((char*)&tmp,sizeof(tmp));
				}
			}
			printf("正在转换图片，请等候%.2f%%\r",i*100.0/(ImagesMat.size()-1));
		}
		printf("\n*******转化完成*******\n\n");
	}
	else
		cout<<subName<<"文件创建失败."<<endl<<endl;
	if(file.is_open())
		file.close();
	return;
}


void Image2BinaryData::Label2BinaryFile(string filefolder, vector<int>& img_Labels)
{
	ofstream file(filefolder.c_str(), ios::binary);
	int idx = filefolder.find_last_of("/") + 1;
	string subName = filefolder.substr(idx);
	if (file.is_open()) {
		cout << subName << "文件创建成功." << endl;

		int magic_number = 2049;
		int number_of_images = img_Labels.size();
		ReverseInt(file, magic_number);
		ReverseInt(file, number_of_images);
		cout << "需要转换的标签数为: " << img_Labels.size() << endl;
		for (int i = 0; i < img_Labels.size(); ++i) {
			uchar tmp = (uchar)img_Labels[i];
			file.write((char*)&tmp, sizeof(tmp));
			printf("标签正在转换，请稍等......%.2lf%%\r", i*100.0 / (img_Labels.size() - 1));
		}
		printf("\n******转换完成!******\n");
	}
	else
		cout << subName << "文件创建失败." << endl;
	if (file.is_open())
		file.close();
	return;
}

