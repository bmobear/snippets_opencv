#include "main.h"

// do not declare any Eigen variable in here
// will result in directive conflict with boost
using namespace boost::filesystem;	

void scanDirForInputFiles(const string& input_dir, const vector<string>& ext_list, vector<string>& out_filename_list);
bool loadImg(const string imgFile, Mat& out_img);
bool loadLdmks(const string ldmkFile, vector<Point2f>& out_imgPts);
void writeLdmks(const string filename, const vector<Point2f>& pts);

//==============================================================

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

/*
load image into cv::Mat
*/
bool loadImg(const string imgFile, Mat& out_img)
{
	out_img = imread(imgFile, CV_LOAD_IMAGE_COLOR);
	if(!out_img.data) {
		printf("Failed to load image file: %s\n", imgFile.c_str());
		return false;
	}
	return true;
}

/*
load landmark (.pts) file into a vector of cv::Point2f
*/
bool loadLdmks(const string ldmkFile, vector<Point2f>& out_imgPts)
{
	// read 2D ldmks
	ifstream in(ldmkFile); 
	if(!in) {
		printf("Failed to load ldmk file: %s\n", ldmkFile.c_str());
		return false;
	}
	string line;
	getline(in, line); 
	getline(in, line);
	getline(in, line);
	out_imgPts.clear();

	int numInputLdmks = 68;
	for(int i=0; i<numInputLdmks; i++) {
		getline(in, line);
		float x, y;
		stringstream(line) >> x >> y;
		Point2f pt(x, y);
		out_imgPts.push_back(pt);
	} 
	in.close();
	return true;
}

/*
write landmarks (cv::Point2f) to .pts file
*/
void writeLdmks(const string filename, const vector<Point2f>& pts)
{
	ofstream out(filename);
	if (!out.is_open())
	{
		printf("Failed to write to file: %s\n", filename.c_str());
		return;
	}

	out << "version: 1" << endl;
	out << "n_points:  " << pts.size() << endl;
	out << "{" << endl;
	
	for(int i=0; i<(int)pts.size(); i++)
	{
		out << pts[i].x << " " << pts[i].y << " " << endl;
	}

	out << "}" << endl;
	out.close();
}
