#include <stdio.h>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;
typedef unsigned long long int UINT_64;
typedef unsigned int UINT_32;
typedef unsigned char UINT_8;
typedef unsigned short int UINT_16;
#define num_files 442
#define max_size 1048576
//#define max_size 1048
#define prin_max 10485760

#define titleWiki 46875
#define infoWiki  1875
#define bodyWiki  25
#define catWiki   5
#define extWiki   1

typedef struct Ranking {
	UINT_32 doc;
	UINT_64 rank;
} Ranking;

typedef struct FileWord {
	UINT_64 word;
	UINT_32 file;
	vector <Ranking> inv_index;
} FileWord;

struct CompareWord  {   
	   bool operator()(const FileWord& l, const FileWord& r) {return l.word > r.word;}
};  

bool CompareRank  (const Ranking& l, const Ranking& r) {return l.rank > r.rank;}

FILE *fp[num_files];
UINT_64 maxy[num_files];
UINT_8 buff[num_files][max_size];

UINT_64 compressit(UINT_64 val, UINT_8 * str, UINT_64 pos) {
	while(true) {
		str[pos] = (val&127)<<1;
		if (val >> 7) {
			val = val >> 7;
			str[pos++] += 1;
		} else {
			pos++;
			break;
		}
	}
	return pos;
}

UINT_64 uncopressit(UINT_64 *curr, UINT_32 currfile){
	UINT_64 ret = 0;
	UINT_64 iter = *curr,pos = 0;
	do {
		if (iter == maxy[currfile]) {
			maxy[currfile] = fread(buff[currfile],1,max_size,fp[currfile]);
			*curr = 0,iter = 0;
			printf("%llu %u\n", maxy[currfile], currfile);
			if (maxy[currfile] == 0) return 0;
		}
		UINT_64 pu = buff[currfile][iter] >> 1;
		ret += pu << pos;
		pos += 7;

	} while(buff[currfile][iter++]&1);
	*curr = iter;
	//printf("%llu\n", *curr);
	return ret;
}

FileWord heapVal(UINT_64 *curr, UINT_32 currfile) {
	UINT_64 val;
	FileWord ret;
	ret.word = uncopressit(curr, currfile);
//	printf("word = %llu maxy = %llu curr = %llu file = %u\n", ret.word, maxy[currfile], *curr, currfile);
//	printf("file = %u Word = %llu\n", currfile,ret.word);
	//printf("%u %llu\n", currfile,ret.word);
	ret.file = currfile;
	while (true) {
		val = uncopressit(curr, currfile);
		if (val == 0) break;
		Ranking r;
		r.rank = val;
		r.doc = uncopressit(curr, currfile);
		ret.inv_index.push_back(r);
	}
	//sort(ret.inv_index.begin(),ret.inv_index.end(), CompareRank);
	//printf("word = %llu, file = %u\n", ret.word,ret.file);
	//for (int i = 0; i < ret.inv_index.size(); i++) printf("%llu:%u;",ret.inv_index[i].rank, ret.inv_index[i].doc); puts("");
	return ret;
}

UINT_8 Binaryit[10485860];
int main(int argc, const char** argv)
{
	FILE *check = fopen("./check","r");
	char lol[50];
	UINT_64 pos[num_files]; 
	priority_queue<FileWord, vector<FileWord>, CompareWord>  heap;
	for (int currfile = 0; currfile < num_files; currfile++){
		fscanf(check,"%s",lol);
		char now[100];
		sprintf(now,"/media/Shared/index/%s", lol);
		//puts(now); 
		maxy[currfile] = max_size;
		pos[currfile] = max_size;
		fp[currfile] = fopen(now,"r");
		heap.push(heapVal(&pos[currfile], currfile));
	}
	UINT_64 prin_pos = 0;
	UINT_32 rankcheck[5] = {titleWiki, infoWiki, bodyWiki, catWiki, extWiki};
	FILE * fmi = fopen("/home/dilipvamsi/mainindex.txt","w");
	UINT_64 word = heap.top().word;
	//vector<Ranking> inv_index (heap.top().inv_index.begin(), heap.top().inv_index.end());
	vector<Ranking> inv_index = heap.top().inv_index;
	UINT_32 currfile = heap.top().file;
	heap.pop();
	heap.push(heapVal(&pos[currfile],currfile));
	while (!heap.empty()) {
		//printf("size = %u\n", heap.size());
		if (word == heap.top().word) {
			inv_index.insert(inv_index.end(),heap.top().inv_index.begin(), heap.top().inv_index.end());
			currfile = heap.top().file;
			heap.pop();
		} else {
			prin_pos = compressit(word, Binaryit, prin_pos);
			sort(inv_index.begin(), inv_index.end(), CompareRank);
			//printf("%llu\n",word); 
			//printf("%llu$",word);for (int i = 0; i < inv_index.size(); i++) printf("%u,",inv_index[i].doc); puts("");
			prin_pos = compressit(inv_index.size(), Binaryit, prin_pos);
			for (int ivec = 0; ivec < inv_index.size(); ivec++) {
				if (prin_pos > prin_max) {
					fwrite(Binaryit, 1, prin_pos, fmi);
					prin_pos = 0;
				}
				UINT_8  addit = 0;
				UINT_64 val = inv_index[ivec].rank;
				for (int i = 0; i < 5; i++) {
					UINT_8 ch = val / rankcheck[i];
					if (ch) addit += 1 << (4-i);
					val -= ch * rankcheck[i];
				}
				UINT_64 prin = inv_index[ivec].doc;
				prin = (prin << 5) + addit;
				prin_pos = compressit(prin, Binaryit, prin_pos);
			}
			prin_pos = compressit(0, Binaryit, prin_pos);
			//puts("");
			//exit(0);
			inv_index.clear();
			currfile = heap.top().file;
			heap.pop();
			FileWord pushit = heapVal(&pos[currfile],currfile);
			if (maxy[currfile]) heap.push(pushit);// puts("else pop");}
			if (heap.empty()) break;
			word = heap.top().word;
			inv_index = heap.top().inv_index;
			currfile = heap.top().file;
			//inv_index.insert(inv_index.end(),heap.top().inv_index.begin(), heap.top().inv_index.end());
			heap.pop();
			//printf("sz = %d\n", heap.size());
			//exit(0);
		}
		//if (pos[currfile]!=0) heap.push(heapVal(&pos[currfile],currfile));
		FileWord pushit = heapVal(&pos[currfile],currfile);
		if (maxy[currfile]) heap.push(pushit); //puts("If pop");}
	}
//	puts("out");
	if (inv_index.size()) {
		sort(inv_index.begin(), inv_index.end(), CompareRank);
		prin_pos = compressit(inv_index.size(), Binaryit, prin_pos);
		for (int ivec = 0; ivec < inv_index.size(); ivec++) {
			if (prin_pos > prin_max) {
				fwrite(Binaryit, 1, prin_pos, fmi);
				prin_pos = 0;
			}
			UINT_8  addit = 0;
			UINT_64 val = inv_index[ivec].doc;
			for (int i = 0; i < 5; i++) {
				UINT_8 ch = val / rankcheck[i];
				if (ch) addit += 1 << (4-i);
				val -= ch * rankcheck[i];
			}
			UINT_64 prin = inv_index[ivec].doc;
			prin = (prin << 5) + addit;
			prin_pos = compressit(prin, Binaryit, prin_pos);
		}
		inv_index.clear();
	}
	return 0;
}
