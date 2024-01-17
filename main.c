#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <math.h>

void printScr(double** A, int h, int l) {
	for (int i=0; i<h; i++) {
		for (int j=0; j<l; j++) {
			if (A[i][j] <= -3.5) printf(".");
			else if (A[i][j] <= -3.0) printf(":");
			else if (A[i][j] <= -2.5) printf("-");
			else if (A[i][j] <= -2.0) printf("~");
			else if (A[i][j] <= -1.5) printf("_");
			else if (A[i][j] <= -1.0) printf("*");
			else if (A[i][j] <= -0.5) printf("?");
			else if (A[i][j] <= 0) printf("|");
			else if (A[i][j] <= 0.5) printf("/");
			else if (A[i][j] <= 1.5) printf("[");
			else if (A[i][j] <= 2.0) printf("#");
			else if (A[i][j] <= 2.5) printf("W");
			else if (A[i][j] <= 3.0) printf("&");
			else if (A[i][j] <= 3.5) printf("%%");
			else printf("@");
			
		}
		printf("\n");
	}
}

void maths(double** A, int h, int l, float scale, int t, float speed) {
	for (int i=0; i<h; i++) {
		for (int j=0; j<l; j++) {
			A[i][j] = 2*sin(scale*0.5*i + t*speed) * sin(scale*j + t*speed);
			A[i][j] += sin(scale*1.5*i + t*speed) * sin(scale*2*j + t*speed);
			A[i][j] += 0.5*sin(scale*4.5*i + t*speed) * sin(scale*4*j + t*speed);
			A[i][j] += 0.25*sin(scale*13.5*i + t*speed) * sin(scale*8*j + t*speed);
			A[i][j] += 0.125*sin(scale*40.5*i + t*speed) * sin(scale*16*j + t*speed);
			A[i][j] += 0.0625*sin(scale*131.5*i + t*speed) * sin(scale*32*j + t*speed);
		}
	}
}

int input() {
	fd_set readfs;
	int res, key;
	struct timeval timeout;
	FD_SET(0, &readfs);
	timeout.tv_usec = 10;
	timeout.tv_sec = 0;
	res = select(1, &readfs, NULL, NULL, &timeout);
	if (res) {
		key = getchar();
		if (key == 'q') return 1;
	}
	return 0;
	
}

int main(void) {
	int h = 50;
	int l = 100;
	double** a = (double**)malloc(h*sizeof(double*));
	for (int i=0; i<h; i++) {
		a[i] = (double*)malloc(l*sizeof(double));
	}
	
	struct timeval t, t1;
	for (int i=0;;i++) {
		gettimeofday(&t, NULL);
		maths(a, h, l, 0.075, i, 0.01); 
		printScr(a, h, l);
		usleep(1000);
		gettimeofday(&t1, NULL);
		printf("elapsed usec: %d\n,", t1.tv_usec - t.tv_usec);
		int diff = 1000000 - (t1.tv_usec - t.tv_usec);
		diff = diff / 60;
		usleep(diff);
		printf("\e[1;1H\e[2J");
		if (input() == 1) break;
	}


	for (int i=0; i<h; i++) {
		free(a[i]);
	}
	free(a);
	return 0;
}
