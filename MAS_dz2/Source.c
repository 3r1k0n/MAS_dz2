#include <stdio.h>
#include <string.h>
#define BLOCK_SIZE 16
#define SEARCH_WINDOW 16

void ispisStatistike(char filePath[], int bucketNumber);
char* loadPicture(char pictureAddress[], int* height, int* width);
char* loadBlock(int y, int x, char* picture, int pictureWidth, int blockSize);
float MAD(char* block1, char* block2, int blockSize);
void calculateMovement(int* outX, int* outY, int blockNumber,int blockSize,int width, int height);

int main(int argc, char *argv[])
{
	unsigned char *picture;
	int height, width,resultX,resultY;
	int inputNumber = atoi(argv[1]);

	unsigned char *picture1 = loadPicture("lenna1.pgm", &height, &width);
	unsigned char *picture2 = loadPicture("lenna.pgm", &height, &width);

	if (inputNumber<0 || inputNumber>=1024) {
		printf("Block number must be between 0 and 1023.\n");
		return 0;
	}

	calculateMovement(&resultX,&resultY,inputNumber,BLOCK_SIZE,width,height,picture1,picture2);
	printf("(%d, %d)",resultX,resultY);

	/*ispisStatistike("lenna.pgm", BLOCK_SIZE);*/
	
	return 0;
}

void calculateMovement(int* outX, int* outY, int blockNumber, int blockSize, int width, int height,char* picture1,char* picture2) {

	float minMAD = 999999;
	int maxI, maxJ;
	float mad;
	
	int startRow = (blockNumber / (width / BLOCK_SIZE))*BLOCK_SIZE;
	int startColumn = (blockNumber % (width / BLOCK_SIZE))*BLOCK_SIZE;

	unsigned char *startBlock = loadBlock(startRow, startColumn, picture1, width, BLOCK_SIZE);

	for (int i = -SEARCH_WINDOW; i <= SEARCH_WINDOW; i++) {
		if ((startRow + i) >= 0 && (startRow + i + BLOCK_SIZE) < height) {
			for (int j = -SEARCH_WINDOW; j <= SEARCH_WINDOW; j++) {
				
				if ((startColumn + j) >= 0 && (startColumn + j + BLOCK_SIZE) < width) {
					unsigned char *secondBlock = loadBlock(startRow+i, startColumn+j, picture2, width, BLOCK_SIZE);
					mad = MAD(startBlock, secondBlock, BLOCK_SIZE);

					if (mad <= minMAD) {
						minMAD = mad;
						*outY = i;
						*outX = j;
					}
				}
			}
		}
	}
	
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

			unsigned char a = *(block1 + i*blockSize + j);
			unsigned char b = *(block2 + i*blockSize + j);

			sum += abs( a - b );
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