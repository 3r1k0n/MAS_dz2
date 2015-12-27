#include <stdio.h>
#include <math.h>
#include <string.h>
#define BLOCK_SIZE 16

float F(int u, int v, short *matrix, int width, int iBlokova, int jBlokova);
void ispisBlokaFloat(float *matrix);
void ispisBlokaUChar(unsigned char *matrix);
void ispisBlokaShort(short *matrix, int rb, int sirinaUBlokovima, int width, FILE *out);

void ispisStatistike(char filePath[], int bucketNumber);
char* loadPicture(char pictureAddress[], int* height, int* width);
char* loadBlock(int y, int x, char* picture, int pictureWidth, int blockSize);
float MAD(char* block1, char* block2, int blockSize);\

int main(int argc, char *argv[])
{
	unsigned char buff[255],*picture;
	int height, width;
	int sampleBlockN = 33;

	unsigned char *picture1 = loadPicture("lenna.pgm", &height, &width);
	unsigned char *picture2 = loadPicture("lenna1.pgm", &height, &width);

	unsigned char *startBlock = loadBlock((sampleBlockN / (width / BLOCK_SIZE))*BLOCK_SIZE, (sampleBlockN % (width / BLOCK_SIZE))*BLOCK_SIZE, picture1, width, BLOCK_SIZE);

	ispisStatistike("lenna.pgm", BLOCK_SIZE);
	return 33;
}
char* loadPicture(char pictureAddress[], int* height, int* width) {
	unsigned char buff[255];

	FILE *fp;
	fp = fopen(pictureAddress, "rb");

	fscanf(fp, "%s", buff);

	if (strcmp(buff, "P5") != 0) {
		printf("Ovaj program je namijenjen samo P5 formatu ppm slika.\n");
		return 0;
	}

	fscanf(fp, "%s", buff);
	*width = atoi(buff);

	fscanf(fp, "%s", buff);
	*height = atoi(buff);

	fscanf(fp, "%s", buff);
	const int max = atoi(buff);

	// reads a new line
	fgetc(fp); 

	unsigned char *picture = (char *)malloc((*height)*(*width)*sizeof(char));

	for (int redak = 0; redak < (*height); redak++) {
		for (int stupac = 0; stupac < (*width); stupac++) {
			*(picture + redak*(*width) + stupac) = fgetc(fp);
		}
	}

	return picture;
}
char* loadBlock(int y, int x, char* picture,int pictureWidth, int blockSize) {
	unsigned char *block = (char *)malloc(blockSize*blockSize*sizeof(char));

	for (int redak = 0; redak < blockSize; redak++) {
		for (int stupac = 0; stupac < blockSize; stupac++) {
			*(block + redak*blockSize + stupac)=*(picture + (y+redak)*pictureWidth + (x+stupac));
		}
	}

	return block;
}
float MAD(char* block1, char* block2,int blockSize) {
	int sum = 0;
	int N2 = blockSize*blockSize;
	for (int i = 0; i < blockSize; i++) {
		for (int j = 0; j < blockSize; j++) {
			sum += abs( *(block1+i*blockSize+j) - *(block2 + i*blockSize + j) );
		}
	}
	return (float)sum / (float)N2;
}
void ispisStatistike(char filePath[], int bucketNumber) {
	unsigned char buff[255], newLineChar, c;
	FILE *fp;
	fp = fopen(filePath, "rb");
	fscanf(fp, "%s", buff);

	if (strcmp(buff, "P5") != 0) {
		printf("Ovaj program je namijenjen samo P5 formatu ppm slika.\n");
		return 0;
	}

	fscanf(fp, "%s", buff);
	int width = atoi(buff);

	fscanf(fp, "%s", buff);
	int height = atoi(buff);

	int totalElements = width*height;

	fscanf(fp, "%s", buff);
	int max = atoi(buff);

	newLineChar = fgetc(fp);

	int bucketCapacity = (max+1) / bucketNumber;
	unsigned int *buckets = (int *)malloc(bucketNumber*sizeof(int));

	for (int i = 0; i < bucketNumber; i++) {
		buckets[i]=0;
	}

	for (int redak = 0; redak < height; redak++) {
		for (int stupac = 0; stupac < width; stupac++) {
			c = fgetc(fp);

			buckets[(int)c / bucketCapacity]++;
		}
	}

	for (int i = 0; i < bucketNumber; i++) {
		printf("%d. %f\n", i, (float)buckets[i] / (float)totalElements);
	}
	fclose(fp);
}