#include <stdio.h>
#include <math.h>
#include <string.h>
#define PI 3.141592653
#define VELICINA_BLOKA 8

float F(int u, int v, short *matrix, int width, int iBlokova, int jBlokova);
void ispisBlokaFloat(float *matrix);
void ispisBlokaUChar(unsigned char *matrix);
void ispisBlokaShort(short *matrix, int rb, int sirinaUBlokovima, int width, FILE *out);

int main(int argc, char *argv[])
{

	unsigned char buff[255], buffPixel[3], buffPixel2[4], newLineChar, testc;
	unsigned char r, g, b;


	FILE *fp;
	fp = fopen(argv[1], "rb");

	fscanf(fp, "%s", buff);

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

	fclose(fp);

	return 0;
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
