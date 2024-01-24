#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <math.h>

void printScr(double** A, int h, int l) {
	for (int i=0; i<h; i++) {
		for (int j=0; j<l; j++) {
			if (A[i][j] <= -3.5) printf(".");
			else if (A[i][j] <= -3.0) printf("-");
			else if (A[i][j] <= -2.5) printf("~");
			else if (A[i][j] <= -2.0) printf(":");
			else if (A[i][j] <= -1.5) printf("_");
			else if (A[i][j] <= -1.0) printf("*");
			else if (A[i][j] <= -0.5) printf("?");
			else if (A[i][j] <= 0) printf("|");
			else if (A[i][j] <= 0.5) printf("/");
			else if (A[i][j] <= 1.0) printf("[");
			else if (A[i][j] <= 1.5) printf("%%");
			else if (A[i][j] <= 2.0) printf("O");
			else if (A[i][j] <= 2.5) printf("W");
			else if (A[i][j] <= 3.0) printf("&");
			else if (A[i][j] <= 3.5) printf("#");
			else printf("@");
			
		}
		printf("\n");
	}
}

void maths(double** A, int h, int l, float scale, int t, float speed) {
	for (int i=0; i<h; i++) {
		for (int j=0; j<l; j++) {
			A[i][j] = 2*sin(scale*0.5*i + t*speed) * sin(scale*j + t*speed*4);
			A[i][j] += sin(scale*1.5*i + t*speed) * sin(scale*2*j + t*speed*4);
			A[i][j] += 0.5*sin(scale*4.5*i + t*speed) * sin(scale*4*j + t*speed*4);
			A[i][j] += 0.25*sin(scale*13.5*i + t*speed) * sin(scale*8*j + t*speed*4);
			A[i][j] += 0.125*sin(scale*40.5*i + t*speed) * sin(scale*16*j + t*speed*4);
			A[i][j] += 0.0625*sin(scale*131.5*i + t*speed) * sin(scale*32*j + t*speed*4);
		}
	}
}

int input() {
	// used to capture live user input
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
	// get width & height
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	int h = w.ws_row-1;
	int l = w.ws_col;
	// create array of width + height
	double** a = (double**)malloc(h*sizeof(double*));
	for (int i=0; i<h; i++) {
		a[i] = (double*)malloc(l*sizeof(double));
	}
	

	// these are used for capping fps at 60 as well as displaying current framerate
	struct timeval t, t1, f1, f2;
	int framerate = 0;
	int framecount = 0;
	gettimeofday(&f1, NULL);
	for (int i=0;;i++) {
		framecount++;
		gettimeofday(&t, NULL);
		maths(a, h, l, 0.03, i, 0.005); 
		printScr(a, h, l);
		usleep(1000);
		gettimeofday(&f2, NULL);
		if (f2.tv_sec - f1.tv_sec >= 1) {
			framerate = framecount;
			framecount = 0;
			gettimeofday(&f1, NULL);
		}
		printf("framerate: %d\n", framerate);
		gettimeofday(&t1, NULL);
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
