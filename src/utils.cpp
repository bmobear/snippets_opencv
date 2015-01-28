#include "main.h"

// do not declare any Eigen variable in here
// will result in directive conflict with boost
using namespace boost::filesystem;	

void extractImageFromVideo(string input_dir, string output_dir);
void scanDirForInputFiles(const string& input_dir, const vector<string>& ext_list, vector<string>& out_filename_list);

//==============================================================

void extractImageFromVideo(string input_dir, string output_dir)
{
	string vidExt = ".mp4";
	string imgExt = ".jpg";
	vector<string> ext_list;
	vector<string> stem_list;

	ext_list.push_back(vidExt);	
	scanDirForInputFiles(input_dir, ext_list, stem_list);

	for(int i=0; i<stem_list.size(); i++) {
		path vidFile = input_dir/(stem_list[i]+vidExt);

		VideoCapture vid(vidFile.string());
		if(!vid.isOpened()) {
			printf("** error: failed to open file %s\n", vidFile.string().c_str());
			continue;
		}
		printf("processing file %s\n", vidFile);

		// create an output directory
		path outDir = path(output_dir)/stem_list[i];
		create_directory(outDir);
		Mat img;
		int frameId = 0;
		int maxFrames = 9999;

		while(vid.read(img) && frameId <= maxFrames) {
			char frameId_str[10];
			sprintf_s(frameId_str, "%04d", frameId++);

			// rotate by 90 degree CW
			Mat rotatedImg;
			flip(img, img, 0);
			transpose(img, rotatedImg);

			// write grabbed frame to file
			string outFile = (outDir/("frame_"+string(frameId_str)+".jpg")).string(); 
			printf("out to %s\n", outFile);
			imwrite(outFile, rotatedImg);
		}

		
	}
}

/*
	scan input directory for input files with given extensions
*/
void scanDirForInputFiles(const string& input_dir, const vector<string>& ext_list, vector<string>& out_filename_list)
{
	if(!is_directory(input_dir)) {
		cout << "input_dir not found!" << endl;
		exit(-1);
	}

	path root_dir = input_dir;
	string search_ext = ext_list.front();
	out_filename_list.clear();

	// scan for input files
	directory_iterator end_itr; // default construction yields past-the-end
	for(directory_iterator itr(root_dir); itr != end_itr; ++itr)
	{
	    path itr_path = itr->path();
	    if(itr_path.extension() == search_ext) 
	    {
			// add to input list if file exists in all requested extensions
			bool good = true;
			for(int i=0; i<(int)ext_list.size(); i++) {
				path f_path = itr_path.parent_path()/(itr_path.stem().string()+ext_list[i]);
				good &= is_regular_file(f_path);
			}
			
			if(good) {
				out_filename_list.push_back(itr_path.stem().string());
			}
		}
	}
}