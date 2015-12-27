#include <stdio.h>
#include <math.h>
#include <string.h>
#define PI 3.141592653
#define VELICINA_BLOKA 8

float F(int u, int v, short *matrix, int width, int iBlokova, int jBlokova);
void ispisBlokaFloat(float *matrix);
void ispisBlokaUChar(unsigned char *matrix);
void ispisBlokaShort(short *matrix, int rb, int sirinaUBlokovima, int width, FILE *out);
void ispisStatistike(char filePath[], int bucketNumber);

int main(int argc, char *argv[])
{
	/*unsigned char buff[255], buffPixel[3], buffPixel2[4], newLineChar, testc;
	unsigned char r, g, b;
	short K1[8][8] = {
		16 , 11 , 10 , 16 , 24 , 40 , 51 , 61,
		12 , 12 , 14 , 19 , 26 , 58 , 60 , 55,
		14 , 13 , 16 , 24 , 40 , 57 , 69 , 56,
		14 , 17 , 22 , 29 , 51 , 87 , 80 ,  62,1
		18 , 22 , 37 , 56 , 68 ,  109  ,103,  77,
		24 , 35 , 55 , 64 , 81  , 104  ,113 , 92,
		49 , 64 , 78 , 87 , 103  ,121 , 120 , 101,
		72 , 92 , 95 , 98 , 112  ,100 , 103  ,99 };
	short K2[8][8] = {
		17,	18	,24,	47	,99	,99,	99,	99,
		18	,21	,26	,66	,99,	99	,99,	99,
		24	,26	,56	,99	,99	,99	,99	,99,
		47	,66	,99	,99	,99	,99	,99	,99,
		99	,99	,99	,99	,99	,99	,99	,99,
		99	,99	,99	,99	,99	,99	,99	,99,
		99	,99	,99	,99	,99	,99,99,	99,
		99	,99,99,	99,	99	,99,99	,99
	};

	FILE *fp;
	fp = fopen(argv[1], "rb");

	fscanf(fp, "%s", buff);

	if (strcmp(buff, "P6") != 0) {
		printf("Ovaj program je namijenjen samo P6 formatu ppm slika.\n");
		return 0;
	}

	fscanf(fp, "%s", buff);
	int width = atoi(buff);

	fscanf(fp, "%s", buff);
	const int height = atoi(buff);

	fscanf(fp, "%s", buff);
	const int max = atoi(buff);

	newLineChar = fgetc(fp);

	unsigned char *R = (char *)malloc(height*width*sizeof(char));
	unsigned char *G = (char *)malloc(height*width*sizeof(char));
	unsigned char *B = (char *)malloc(height*width*sizeof(char));

	float *Y = (float *)malloc(height*width*sizeof(float));
	float *Cb = (float *)malloc(height*width*sizeof(float));
	float *Cr = (float *)malloc(height*width*sizeof(float));

	for (int redak = 0; redak < height; redak++) {
		for (int stupac = 0; stupac < width; stupac++) {
			r = fgetc(fp);
			g = fgetc(fp);
			b = fgetc(fp);

			*(R + redak*width + stupac) = r;
			*(G + redak*width + stupac) = g;
			*(B + redak*width + stupac) = b;

			*(Y + redak*width + stupac) = 0.299*r + 0.587*g + 0.114*b - 128; //translacija dodana
			*(Cb + redak*width + stupac) = -0.1687*r - 0.3313*g + 0.5*b + 128 - 128;
			*(Cr + redak*width + stupac) = 0.5*r - 0.4187*g - 0.0813*b + 128 - 128;
		}
	}
	fclose(fp);

	///////RACUNANJE 2D-DCT

	int sirinaUBlokovima = width / VELICINA_BLOKA;
	int visinaUBlokovima = height / VELICINA_BLOKA;

	float *FY = (float *)malloc(width*height*sizeof(float));
	float *FCb = (float *)malloc(width*height*sizeof(float));
	float *FCr = (float *)malloc(width*height*sizeof(float));

	short *FY_kvantizirano = (short *)malloc(width*height*sizeof(short));
	short *FCb_kvantizirano = (short *)malloc(width*height*sizeof(short));
	short *FCr_kvantizirano = (short *)malloc(width*height*sizeof(short));

	float mfy, mfcb, mfcr;

	for (int iB = 0; iB < visinaUBlokovima; iB++) {
		for (int jB = 0; jB < sirinaUBlokovima; jB++) { //iB i jB su koordinate s obzirom na blokova 64x64
			for (int u = 0; u < VELICINA_BLOKA; u++) {
				for (int v = 0; v < VELICINA_BLOKA; v++) { //u i v su koordinate unutar jednog bloka u frekvencijskoj domeni
					mfy = F(u, v, Y, width, iB, jB);
					mfcb = F(u, v, Cb, width, iB, jB);
					mfcr = F(u, v, Cr, width, iB, jB);

					*(FY + (VELICINA_BLOKA*iB + u)*width + VELICINA_BLOKA*jB + v) = mfy;
					*(FCb + (VELICINA_BLOKA * iB + u)*width + VELICINA_BLOKA * jB + v) = mfcb;
					*(FCr + (VELICINA_BLOKA * iB + u)*width + VELICINA_BLOKA * jB + v) = mfcr;

					*(FY_kvantizirano + (VELICINA_BLOKA * iB + u)*width + VELICINA_BLOKA * jB + v) = round(mfy / K1[u][v]);
					*(FCb_kvantizirano + (VELICINA_BLOKA * iB + u)*width + VELICINA_BLOKA * jB + v) = round(mfcb / K2[u][v]);
					*(FCr_kvantizirano + (VELICINA_BLOKA * iB + u)*width + VELICINA_BLOKA * jB + v) = round(mfcr / K2[u][v]);
				}
			}
		}
	}
	FILE *fop = fopen(argv[3], "w");
	ispisBlokaShort(FY_kvantizirano, atoi(argv[2]), sirinaUBlokovima, width, fop);
	ispisBlokaShort(FCb_kvantizirano, atoi(argv[2]), sirinaUBlokovima, width, fop);
	ispisBlokaShort(FCr_kvantizirano, atoi(argv[2]), sirinaUBlokovima, width, fop);
	fclose(fop);
	*/

	ispisStatistike("lenna.pgm", 16);
	return 0;
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

float F(int u, int v, float *matrix, int width, int iB, int jB) {
	float Cu = 1, Cv = 1;
	if (u == 0) Cu = pow(0.5, 0.5);
	if (v == 0) Cv = pow(0.5, 0.5);

	float sum = 0.0;
	for (int i = 0; i < VELICINA_BLOKA; i++) {
		for (int j = 0; j < VELICINA_BLOKA; j++) {
			float elementIJ = *(matrix + (VELICINA_BLOKA * iB + i)*width + (VELICINA_BLOKA * jB + j));
			sum += elementIJ*cos((2 * i + 1)*u*PI / 16)*cos((2 * j + 1)*v*PI / 16);
		}
	}
	return 0.25*Cu*Cv*sum;
}

void ispisBlokaShort(short *matrix, int rb, int sirinaUBlokovima, int width, FILE *out) {

	int PBi = floor(rb / sirinaUBlokovima);
	int PBj = (rb%sirinaUBlokovima)*VELICINA_BLOKA;

	for (int i = 0; i < VELICINA_BLOKA; i++) {
		for (int j = 0; j < VELICINA_BLOKA; j++) {
			fprintf(out, "%hi ", *(matrix + (PBi + i)*width + PBj + j));
		}
		fprintf(out, "\n");
	}
	fprintf(out, "\n");
}
void ispisBlokaUChar(unsigned char *matrix) {
	printf("--------------------\n");
	for (int i = 0; i < VELICINA_BLOKA; i++) {
		for (int j = 0; j < VELICINA_BLOKA; j++) {
			printf("%hhu ", *(matrix + i * 512 + j));
		}
		printf("\n");
	}
	printf("--------------------\n");
}

void ispisBlokaFloat(float *matrix) {
	printf("--------------------\n");
	for (int i = 0; i < VELICINA_BLOKA; i++) {
		for (int j = 0; j < VELICINA_BLOKA; j++) {
			printf("%6.2f ", *(matrix + i * 512 + j));
		}
		printf("\n");
	}
	printf("--------------------\n");
}
