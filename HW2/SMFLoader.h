#pragma once

#ifndef SMFLOADER_H
#define SMFLOADER_H

/*
   �@��vertex 3��float  �@��size
   �Y��3��vertex  vertexSize ��3  ���Overtex���Ҧ���float��9

   face�]�O�@��    */

class SMFLoader
{
public:
	SMFLoader();
	void load(const char* filename);
	void clear();
	float* getVertices();
	int* getFaces();
	float* getColors();

	float* getFace(int index);
	float* getFaceColor(int index);

	int getVertexSize();
	int getFaceSize();
	int getColorSize();
private:
	float* vertex;
	int* face;
	float* color;

	int vertexSize;
	int faceSize;
	int colorSize;
};

#endif
