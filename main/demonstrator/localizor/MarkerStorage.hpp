/*
 * MarkerStorage.hpp
 *
 *  Created on: Jan 8, 2013
 *      Author: Bigontina
 */

#ifndef MARKERSTORAGE_HPP_
#define MARKERSTORAGE_HPP_

#include "opencv2/opencv.hpp"

#include "MarkerPose.hpp"

using namespace std;


/*
 * Loads the marker poses from a file
 * and provides the information as MarkerPose objects
 */
class MarkerStorage {

private:
	static const String defaultMarkersFile;

	String markersFile;

	std::map<int,MarkerPose*> markers;

	void createMarkerYaml(FileStorage & fs, MarkerPose & m) {
		fs << "{:";
		fs << "id" << m.id;
		fs << "position" << m.position;
		fs << "normal" << m.rotation;
		fs << "}";
	}

	void pointToYaml(FileStorage & fs, Point3f & p) {
		fs << "x" << p.x;
		fs << "y" << p.y;
		fs << "z" << p.z;
	}

	Point3f* pointFromYaml(FileNode & fs) {
		FileNodeIterator it = fs.begin(), it_end = fs.end();
		float x,y,z;
		(*it) >> x;
		it++;
		(*it) >> y;
		it++;
		(*it) >> z;
		return new Point3f(x,y,z);
	}

	// for 3x1 Mats only
	void mat3FromYaml(FileNode & fs, Mat & m) {
		m = cv::Mat(3,1,CV_32FC1);
		int i=0;
		for(FileNodeIterator it = fs.begin(); it != fs.end(); it++, i++)
			(*it) >> m.at<float>(i, 0);
	}

public:

	MarkerStorage() {
		this->markersFile = defaultMarkersFile;
	}

	MarkerStorage(String markersFile) {
		this->markersFile = markersFile;
	}

	void load() {
		cv::FileStorage fs(markersFile, cv::FileStorage::READ & cv::FileStorage::FORMAT_YAML);
		FileNode markersnode = fs["markers"];
		FileNodeIterator it = markersnode.begin(), it_end = markersnode.end();		
		for( ; it != it_end; ++it) {
			int id;
			Mat rotation, position;
			float size;
			(*it)["id"] >> id;
			FileNode fn = (*it)["position"];
			mat3FromYaml(fn, position);
			fn = (*it)["rotation"];
			mat3FromYaml(fn, rotation);
			(*it)["size"] >> size;
			MarkerPose* pose = new MarkerPose(id, position, rotation, size);			
			this->markers.insert( pair<int,MarkerPose*>(id,pose) );
		}		
	}

	bool contains(int id) {
		return ( markers.find(id) != markers.end() );
	}

	MarkerPose* getInfo(int id) {
		return this->markers.at(id);
	}
	
   int getStroageCount(){
   	return markers.size();
   }
   
	void getAll(vector<MarkerPose*> & poses) {
		map<int, MarkerPose*>::iterator it;
		for(it = markers.begin();it != markers.end(); it ++) {
			poses.push_back((MarkerPose*)it->second);
		}
	}

};

const String MarkerStorage::defaultMarkersFile = "config/default/markers.yaml";


#endif /* MARKERSTORAGE_HPP_ */
