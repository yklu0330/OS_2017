// Student ID:
// Name      :
// Date      : 2017.11.03

#include "bmpReader.h"
#include "bmpReader.cpp"
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <pthread.h>
#include <semaphore.h>
using namespace std;

#define MYRED	2
#define MYGREEN 1
#define MYBLUE	0

int imgWidth, imgHeight;
int FILTER_SIZE;
int FILTER_SCALE;
int *filter_G;

pthread_mutex_t mutex;

const char *inputfile_name[5] = {
	"input1.bmp",
	"input2.bmp",
	"input3.bmp",
	"input4.bmp",
	"input5.bmp"
};
const char *outputBlur_name[5] = {
	"Blur1.bmp",
	"Blur2.bmp",
	"Blur3.bmp",
	"Blur4.bmp",
	"Blur5.bmp"
};
/*
const char *outputSobel_name[5] = {
	"Sobel1.bmp",
	"Sobel2.bmp",
	"Sobel3.bmp",
	"Sobel4.bmp",
	"Sobel5.bmp"
};*/

unsigned char *pic_in, *pic_grey, *pic_blur, *pic_final;

unsigned char RGB2grey(int w, int h)
{
	int tmp = (
		pic_in[3 * (h*imgWidth + w) + MYRED] +
		pic_in[3 * (h*imgWidth + w) + MYGREEN] +
		pic_in[3 * (h*imgWidth + w) + MYBLUE] )/3;

	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return (unsigned char)tmp;
}

unsigned char GaussianFilter(int w, int h)
{
	int tmp = 0;
	int a, b;
	int ws = (int)sqrt((float)FILTER_SIZE);
	for (int j = 0; j<ws; j++)
	for (int i = 0; i<ws; i++)
	{
		a = w + i - (ws / 2);
		b = h + j - (ws / 2);

		// detect for borders of the image
		if (a<0 || b<0 || a>=imgWidth || b>=imgHeight) continue;

		tmp += filter_G[j*ws + i] * pic_grey[b*imgWidth + a];
	};
	tmp /= FILTER_SCALE;
	if (tmp < 0) tmp = 0;
	if (tmp > 255) tmp = 255;
	return (unsigned char)tmp;
}

struct RGBidx
{
	int iStart, iEnd;
}rgbIdx[4];

void *RGB2grey_pthread(void *ptr)
{
	pthread_mutex_lock(&mutex);
	struct RGBidx* rgb = (struct RGBidx*)ptr;
	for (int j = 0; j<imgHeight; j++)
	{
		for (int i = rgb->iStart; i < rgb->iEnd; i++)
		{
			pic_grey[j*imgWidth + i] = RGB2grey(i, j);
		}
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

struct Gaussianidx
{
	int iStart, iEnd;
}GaussianIdx[4];

void *Gaussian_pthread(void *ptr)
{
	pthread_mutex_lock(&mutex);
	struct Gaussianidx* gau = (struct Gaussianidx*)ptr;
	for (int j = 0; j<imgHeight; j++)
	{
		for (int i = gau->iStart; i < gau->iEnd; i++)
		{
			pic_blur[j*imgWidth + i] = GaussianFilter(i, j);
		}
	}
	pthread_mutex_unlock(&mutex);
	pthread_exit(0);
}

int main()
{
	// read mask file
	FILE* mask;
	mask = fopen("mask_Gaussian.txt", "r");
	fscanf(mask, "%d", &FILTER_SIZE);
	fscanf(mask, "%d", &FILTER_SCALE);

	filter_G = new int[FILTER_SIZE];
	for (int i = 0; i<FILTER_SIZE; i++)
		fscanf(mask, "%d", &filter_G[i]);
	fclose(mask);


	BmpReader* bmpReader = new BmpReader();
	for (int k = 0; k<5; k++){
		// read input BMP file
		pic_in = bmpReader->ReadBMP(inputfile_name[k], &imgWidth, &imgHeight);
		// allocate space for output image
		pic_grey = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_blur = (unsigned char*)malloc(imgWidth*imgHeight*sizeof(unsigned char));
		pic_final = (unsigned char*)malloc(3 * imgWidth*imgHeight*sizeof(unsigned char));

		for (int l = 0; l < 4; l++)
		{
			rgbIdx[l].iStart = imgWidth / 4 * l;
			rgbIdx[l].iEnd = (l != 3)? (imgWidth / 4 * (l + 1)) : imgWidth;
		}
		pthread_t rgbPthread[4];
		for (int m = 0; m < 4; m++)
		{	
			pthread_mutex_init(&mutex, NULL);
			pthread_create(&rgbPthread[m], NULL, RGB2grey_pthread, &rgbIdx[m]);
		}		
		for (int l = 0; l < 4; l++)
		{
			pthread_join(rgbPthread[l], NULL);
		}

		pthread_t gauPthread[4];
		for (int m = 0; m < 4; m++)
		{
			pthread_mutex_init(&mutex, NULL);
			pthread_create(&gauPthread[m], NULL, Gaussian_pthread, &rgbIdx[m]);

		}
		for (int l = 0; l < 4; l++)
		{
			pthread_join(gauPthread[l], NULL);
		}
		
		//extend the size form WxHx1 to WxHx3
		for (int j = 0; j<imgHeight; j++) {
			for (int i = 0; i<imgWidth; i++){
				pic_final[3 * (j*imgWidth + i) + MYRED] = pic_blur[j*imgWidth + i];
				pic_final[3 * (j*imgWidth + i) + MYGREEN] = pic_blur[j*imgWidth + i];
				pic_final[3 * (j*imgWidth + i) + MYBLUE] = pic_blur[j*imgWidth + i];
			}
		}

		// write output BMP file
		bmpReader->WriteBMP(outputBlur_name[k], imgWidth, imgHeight, pic_final);

		//free memory space
		free(pic_in);
		free(pic_grey);
		free(pic_blur);
		free(pic_final);
	}

	return 0;
}
