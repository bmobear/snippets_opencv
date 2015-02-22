#include "main.h"

// do not declare any Eigen variable in here
// will result in directive conflict with boost
using namespace boost::filesystem;

void adjustGivenLdmksByHand(string input_dir, string output_dir);
void mouseEventToMovePoint(int evt, int x, int y, int flags, void* param);

//==============================================================

int selectedPtId;
bool dragMode, drawMode;
Point beginDragPt, endDragPt;
vector<Point2f> ldmkPts;

void adjustGivenLdmksByHand(string input_dir, string output_dir)
{
	string ldmkExt = ".pts";
	string imgExt = ".jpg";
	vector<string> ext_list;
	vector<string> stem_list;

	ext_list.push_back(imgExt);	
	ext_list.push_back(ldmkExt);
	scanDirForInputFiles(input_dir, ext_list, stem_list);
	create_directory(output_dir);

	for(int i=0; i<stem_list.size(); i++) {
		path imgFile = input_dir/(stem_list[i]+imgExt);
		path ldmkFile = input_dir/(stem_list[i]+ldmkExt);
		path outputFile = output_dir/(stem_list[i]+ldmkExt);

		Mat img;
		if(!loadImg(imgFile.string(), img) || !loadLdmks(ldmkFile.string(), ldmkPts)) {
			printf("failed to load %s or %s\n", imgFile.string(), ldmkFile.string());
			continue;
		}
		printf("processing file %s\n", imgFile);
		
		selectedPtId = -1;
		dragMode = false;
		drawMode = false;
		namedWindow("Landmarks", 1);
		setMouseCallback("Landmarks", mouseEventToMovePoint, &img);

		int ldmkItr = 0;
		while(true) {
			Mat cloneImg = img.clone();
			// draw landmarks
			for(int i=0; i<ldmkPts.size(); i++) {
				circle(cloneImg, ldmkPts[i], 2, CV_RGB(0, 0, 255), CV_FILLED);
			}
			// draw current landmark
			circle(cloneImg, ldmkPts[ldmkItr], 2, CV_RGB(0, 255, 0), CV_FILLED);
			// draw bounding box
			if(drawMode) {
				rectangle(cloneImg, beginDragPt, endDragPt, CV_RGB(0, 255, 0));
			}
			// draw selected landmark
			else if(selectedPtId >= 0) {
				circle(cloneImg, ldmkPts[selectedPtId], 2, CV_RGB(255, 0, 0), CV_FILLED);
			}
			imshow("Landmarks", cloneImg);

			char k = waitKey(1);
			if(k == 'q') {
				break;
			}
			if(k == 'w') {
				ldmkItr++;
				continue;
			}
		}

		// write updated landmarks to file
		writeLdmks(outputFile.string(), ldmkPts);
	}
}

void mouseEventToMovePoint(int evt, int x, int y, int flags, void* param) 
{                    
    Mat* img = (Mat*) param;

	switch(evt) {
	case CV_EVENT_LBUTTONDOWN:
		if(selectedPtId < 0) {
			beginDragPt = Point(x, y); 
			dragMode = true;
			drawMode = false;
		}
		break;
    
	case CV_EVENT_LBUTTONUP:
		if(selectedPtId < 0) {
			endDragPt = Point(x, y); 
			dragMode = false;
			drawMode = false;

			// pick a point
			Rect rect(beginDragPt, endDragPt);
			for(int i=0; i<ldmkPts.size(); i++) {
				Point pt = Point(ldmkPts[i].x, ldmkPts[i].y);
				if(rect.contains(pt)) {
					selectedPtId = i; 
					break;
				}
			}
		}
		else {
			// update selected point
			ldmkPts[selectedPtId] = Point2f(x, y);
			selectedPtId = -1;
		}
		break;

	case CV_EVENT_MOUSEMOVE:
		if(dragMode) {
			endDragPt = Point2f(x, y);
			drawMode = true;
		}
		break;
	}
}