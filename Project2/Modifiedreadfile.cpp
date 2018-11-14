#include <vector>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

/*
*  Readfile -- a "basic" routine to read in the obj file and put the
*  values in vectors.  The vectors are passed in by reference so that
*  the modified values are returned to the program. *
*
*  Author:  Jerry Heuring
*  Date:    September 28, 2018
*
*  Bugs:  File open is NOT checked!! a bad file will result in
*         bogus data!!
*
*  Revisions:
*
*/
void readfile(string filename,
	vector<float>& vertices,
	vector<unsigned int>& indices) {
	ifstream inFile;
	inFile.open(filename);
	string inputLine;
	istringstream *lineRdr;
	string prefix;
	float x, y, z;
	unsigned int i1, i2, i3;
	vertices.push_back(0.0);
	vertices.push_back(0.0);
	vertices.push_back(0.0);
	while (inFile.good() && !inFile.eof()) {
		getline(inFile, inputLine);
		lineRdr = new istringstream(inputLine);
		*lineRdr >> prefix;
		if (prefix == "v") { // process a vertex
			*lineRdr >> x >> y >> z;
			vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);
		}
		else if (prefix == "f") { // process a face...
			*lineRdr >> i1 >> i2 >> i3;
			indices.push_back(i1);
			indices.push_back(i2);
			indices.push_back(i3);
		}

	}
}

/*
* readfile2 -- a basic routine that will read the file and set up the data in a
* pair of vectors.  One vector contains the vertices and the second contains the
* normals.  The two vectors are passed in by reference and returned to the calling
* routine.
*/
void readfile2(string filename,
	vector<float>& vertices,
	vector<float>& normals) {

	vector <float> localVertices;
	vector <float> localNormals;
	vector <unsigned int> localIndices;

	ifstream inFile;
	inFile.open(filename);
	if (inFile.is_open()) {
		string inputLine;
		istringstream *lineRdr;
		string prefix;
		float x, y, z;
		float dx, dy, dz;
		unsigned int i1, i2, i, j;
		char ch;

		localVertices.push_back(0.0);
		localVertices.push_back(0.0);
		localVertices.push_back(0.0);
		localNormals.push_back(0.0f);
		localNormals.push_back(0.0f);
		localNormals.push_back(0.0f);
		while (inFile.good() && !inFile.eof()) {
			getline(inFile, inputLine);
			if (!inFile.bad()) {
				lineRdr = new istringstream(inputLine);
				*lineRdr >> prefix;
				if (prefix == "v") { // process a vertex
					*lineRdr >> x >> y >> z;
					localVertices.push_back(x);
					localVertices.push_back(y);
					localVertices.push_back(z);
				}
				else if (prefix == "vn") {
					*lineRdr >> dx >> dy >> dz;
					localNormals.push_back(dx);
					localNormals.push_back(dy);
					localNormals.push_back(dz);
				}
				else if (prefix == "f") { // process a face...
					for (i = 0; i < 3; i++) {
						*lineRdr >> i1 >> ch >> ch >> i2;
						for (j = 0; j < 3; j++) {
							vertices.push_back(localVertices[3 * i1 + j]);
							normals.push_back(localNormals[3 * i2 + j]);
						}
					}
				}

			}

		}
	}
}

/*
* readfile3 -- a basic routine that will read the file and set up the data in a
* 3-tuple of vectors.  One vector contains the vertices, the second contains the
* normals, and the third contains the texture coordinates  The three vectors are
* passed in by reference and returned to the calling routine.
*/
void readfile3(string filename,
	vector<float>& vertices,
	vector<float>& normals,
	vector<float>& textures) {

	vector <float> localVertices;
	vector <float> localNormals;
	vector <float> localTextures;
	vector <unsigned int> localIndices;

	ifstream inFile;
	inFile.open(filename);
	if (inFile.is_open()) {
		string inputLine;
		istringstream *lineRdr;
		string prefix;
		float x, y, z;
		float dx, dy, dz;
		float u, v;
		unsigned int i1, i2, i3, i, j;
		char ch;

		localVertices.push_back(0.0);
		localVertices.push_back(0.0);
		localVertices.push_back(0.0);
		localNormals.push_back(0.0f);
		localNormals.push_back(0.0f);
		localNormals.push_back(0.0f);
		localTextures.push_back(0.0f);
		localTextures.push_back(0.0f);
		while (inFile.good() && !inFile.eof()) {
			getline(inFile, inputLine);
			if (!inFile.bad()) {
				lineRdr = new istringstream(inputLine);
				*lineRdr >> prefix;
				if (prefix == "v") { // process a vertex
					*lineRdr >> x >> y >> z;
					localVertices.push_back(x);
					localVertices.push_back(y);
					localVertices.push_back(z);
				}
				else if (prefix == "vn") {
					*lineRdr >> dx >> dy >> dz;
					localNormals.push_back(dx);
					localNormals.push_back(dy);
					localNormals.push_back(dz);
				}
				else if (prefix == "vt") { // texture coordinates
					*lineRdr >> u >> v;
					localTextures.push_back(u);
					localTextures.push_back(v);
				}
				else if (prefix == "f") { // process a face...
					for (i = 0; i < 3; i++) {
						*lineRdr >> i1 >> ch >> i2 >> ch >> i3;
						for (j = 0; j < 3; j++) {
							vertices.push_back(localVertices[3 * i1 + j]);
							normals.push_back(localNormals[3 * i3 + j]);

						}
						for (j = 0; j < 2; j++) {
							textures.push_back(localTextures[2 * i2 + j]);
						}
					}
				}

			}

		}
	}
}