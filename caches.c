#include <stdio.h>
#include <math.h>

void nWaySetAssociative(int sizeOfCacheInBytes, int cacheLinesPerSet, int bytesPerCacheLine, int * test_list, int sizeOfTestList);
int isHitOrMiss(int cacheLinesPerSet, int tag, int setNumber);
void resetCache();

struct row {
	int valid;
	int tag;
	unsigned int timestamp;
};

static unsigned int timestamp = 0;
struct row cache[5000];

int main (int argc, char * argv[]) {

	int test_list[] = { 0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60, 64, 68, 72, 
		76, 80, 0, 4, 8, 12, 16, 71, 3, 41, 81, 39, 38, 71, 15, 39, 11, 51, 57, 41 };

	// Cannot pass arrays by reference, so size is lost.
	int sizeOfTestList = sizeof(test_list)/sizeof(int);

	// Direct-mapped
	nWaySetAssociative(128, 1, 8, test_list, sizeOfTestList);

	resetCache();
	printf("\n");

	// 2-way set associative
	nWaySetAssociative(64, 2, 8, test_list, sizeOfTestList);

	resetCache();
	printf("\n");

	// Direct-mapped
	nWaySetAssociative(128, 1, 16, test_list, sizeOfTestList);

	resetCache();
	printf("\n");

	// Fully-associative
	nWaySetAssociative(64, (64 / 8), 8, test_list, sizeOfTestList);
	
	getchar();
	getchar();
	getchar();

}

void nWaySetAssociative(int sizeOfCacheInBytes, int cacheLinesPerSet, int bytesPerCacheLine, int * test_list, int sizeOfTestList) {

	printf("cacheLinesPerSet: %d \n", cacheLinesPerSet);

	int cacheLines = sizeOfCacheInBytes / bytesPerCacheLine;
	int numberOfSets = cacheLines / cacheLinesPerSet;

	printf("cacheLines: %d \n", cacheLines);
	printf("numberOfSets: %d \n", numberOfSets);

	int bitsInOffSet = log10((float)cacheLines) / log10(2.0);

	printf("bitsInOffSet: %d \n\n", bitsInOffSet);

	int i = 0;
	for(i = 0; i < sizeOfTestList; i++ ) {

		int tagSetNum = test_list[i] / ((int) pow ((double) 2.0, (double) bitsInOffSet));

		int tag = tagSetNum / cacheLines;
		int setNumber = tagSetNum % cacheLines;

		int result = isHitOrMiss(cacheLinesPerSet, tag, setNumber);
		printf("The number %d: setNumber = %d : tag = %d : %s \n", 
			test_list[i], setNumber, tag, result ? "HIT" : "MISS");

		setNumber = 0;
		tag = 0;

	}

}

int isHitOrMiss(int cacheLinesPerSet, int tag, int setNumber) {
	int rowIdx = 0;

	// See if a valid cacheline with that tag exists
	for ( rowIdx = cacheLinesPerSet*setNumber; rowIdx < cacheLinesPerSet*setNumber + cacheLinesPerSet; rowIdx++ ) {
		if ( cache[rowIdx].valid && cache[rowIdx].tag == tag ) {
			return 1;
		}
	}

	// Else search for cache line with valid field == 0 (false), if found add the
	// new cacheline there.
	for ( rowIdx = cacheLinesPerSet*setNumber; rowIdx < cacheLinesPerSet*setNumber + cacheLinesPerSet; rowIdx++ ) {
		if (!cache[rowIdx].valid) {
			cache[rowIdx].valid = 1;
			cache[rowIdx].tag = tag;
			cache[rowIdx].timestamp = timestamp;
			timestamp++;
			printf("TIMESTAMP : %u \n", cache[rowIdx].timestamp);
			return 0;
		}
	}

	// Find the cacheline with the lowest timestamp in the set
	unsigned int lowest = 4294967295; // Could not use UINT_MAX in C
	for ( rowIdx = cacheLinesPerSet*setNumber; rowIdx < cacheLinesPerSet*setNumber + cacheLinesPerSet; rowIdx++ ) {
		unsigned int temp = cache[rowIdx].timestamp;
		if (temp < lowest) {
			lowest = temp;
		}
	}

	// Evict the cacheline with the lowest timestamp in the set and add
	// the new cacheline.
	for ( rowIdx = cacheLinesPerSet*setNumber; rowIdx < cacheLinesPerSet*setNumber + cacheLinesPerSet; rowIdx++ ) {
		if (cache[rowIdx].timestamp == lowest) {
			cache[rowIdx].valid = 1;
			cache[rowIdx].tag = tag;
			cache[rowIdx].timestamp = timestamp;
			timestamp++;

		}
	}

	return 0;

}

void resetCache() {

	// Set every cacheline to invalid, it does not matter what the other content is
	int i = 0;
	for(i = 0; i < sizeof(cache)/sizeof(struct row); i++) {
		cache[i].valid = 0;
	}
}
