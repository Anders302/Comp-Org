#include <stdio.h>

char* learnToCopyAndPrint(const char* xOne);
void countCWLSol1(const char* x);
void countCWLSol2(const char* x);
void countCWLSol3(const char* x);
void countDWOSol1(const char* x);
void countDWOSol2(const char* x);
void countDWOSol3(const char* x);

int main () {

	char* xOne = "Hello, world.";
	char* xTwo;
	xTwo = learnToCopyAndPrint(xOne);
	
	char x1[100] = "The quick brown fox jumped over the lazy dog.";
	countCWLSol1(x1);
	countCWLSol2(x1);
	countCWLSol3(x1);

	char x2[100] = "The 25 quick brown foxes jumped over the 27 lazy dogs 17 times.";
	countDWOSol1(x2);
	countDWOSol2(x2);
	countDWOSol3(x2);

	getchar();
	getchar();

	return 0;

}

char* learnToCopyAndPrint(const char* xOne) {

	int size = 0;

	for(int i = 0; xOne[i] != '\0'; i++) {

		size ++;

	}

	char xTwo[20];

	for(int i = 0; xOne[i] != '\0'; i++) {

		xTwo[i] = xOne[i];

	}

	printf("The size is : %d\n", size);

	return xTwo;

}

void countCWLSol1(const char* x) {

	int numChar = 0, numWord = 0, numLines = 0;
	int i = 0;
	bool inWord = false;

	while(x[i] != '\0') {
		numChar++;

		if(x[i] == 'n')
			numLines++;

		if(x[i] == ' ' || x[i] == '\n' || x[i] == '\t') {
			inWord = false;
		} else if(inWord == false) {
			inWord = true;
			numWord++;
		}

		i++;
	}

	printf("The number of characters: %d, words: %d, and lines: %d\n", 
		numChar, numWord, numLines);
}

void countCWLSol2(const char* x) {

	int numChar = 0, numWord = 0, numLines = 0;
	int i = 0;
	bool inWord = false;

	while(*(x + i) != '\0') {
		numChar++;

		if(*(x + i) == 'n')
			numLines++;

		if(*(x + i) == ' ' || *(x + i) == '\n' || *(x + i) == '\t') {
			inWord = false;
		} else if(inWord == false) {
			inWord = true;
			numWord++;
		}

		i++;
	}

	printf("The number of characters: %d, words: %d, and lines: %d\n", 
		numChar, numWord, numLines);
}

void countCWLSol3(const char* x) {

	int numChar = 0, numWord = 0, numLines = 0;
	int i = 0;
	bool inWord = false;
	const char* x_p = x;

	while(*x_p != '\0') {
		numChar++;

		if(*x_p == 'n')
			numLines++;

		if(*x_p == ' ' || *x_p == '\n' || *x_p == '\t') {
			inWord = false;
		} else if(inWord == false) {
			inWord = true;
			numWord++;
		}

		x_p++;
	}

	printf("The number of characters: %d, words: %d, and lines: %d\n", 
		numChar, numWord, numLines);
}

void countDWOSol1(const char* x) {

	int i, k, nwhite, nother;
	int ndigit[10];
	k = nwhite = nother = 0;

	for (i = 0; i < 10; ++i)
		ndigit[i] = 0;

	while (x[k] != '\0') {
		if (x[k] >= '0' && x[k] <= '9')
			++ndigit[x[k]-'0'];
		else if (x[k] == ' ' || x[k] == '\n' || x[k] == '\t')
			++nwhite;
		else
			++nother;

		k++;
	}

	printf("digits =");
	for (i = 0; i < 10; ++i)
		printf(" %d", ndigit[i]);
	printf(", white space = %d, other = %d\n",
		nwhite, nother);

}

void countDWOSol2(const char* x) {

	int i, k, nwhite, nother;
	int ndigit[10];
	k = nwhite = nother = 0;

	for (i = 0; i < 10; ++i)
		*(ndigit + i) = 0;

	while (*(x + k) != '\0') {
		if (*(x + k) >= '0' && *(x + k) <= '9')
			++(* (ndigit + *(x + k) -'0'));
		else if (*(x + k) == ' ' || *(x + k) == '\n' || *(x + k) == '\t')
			++nwhite;
		else
			++nother;

		k++;
	}

	printf("digits =");
	for (i = 0; i < 10; ++i)
		printf(" %d", *(ndigit + i));
	printf(", white space = %d, other = %d\n",
		nwhite, nother);

}

void countDWOSol3(const char* x) {

	int i, k, nwhite, nother;
	int ndigit[10];
	k = nwhite = nother = 0;

	const char * x_p = x;
	int * ndigit_p = ndigit;
	int * temp_p = ndigit_p;

	for (i = 0; i < 10; i++) {
		*temp_p = 0;
		temp_p = temp_p++;
	}

	while (*x_p != '\0') {
		temp_p = ndigit_p;
		if (*x_p >= '0' && *x_p <= '9') {
			for(int i = 0; i < *x_p -'0'; i ++) {
				temp_p = temp_p ++;
			}

			(*temp_p) ++;
		}
		else if (*x_p == ' ' || *x_p == '\n' || *x_p == '\t')
			++nwhite;
		else
			++nother;

		x_p++;
		k++;
	}

	temp_p = ndigit_p;

	printf("digits =");
	for (i = 0; i < 10; ++i) {
		printf(" %d", *temp_p);
		temp_p = temp_p++;
	}

	printf(", white space = %d, other = %d\n",
		nwhite, nother);

}
