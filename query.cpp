
#include <map>
#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <string.h>
#include "./include/Stopit.h"
#include "./include/porter.h"
#include <iostream>
using namespace std;

map<string, map<unsigned int, unsigned long long int> > mapstore;

bool WordPresent(string word) {
    map<string, map<unsigned int, unsigned long long int> >::const_iterator it = mapstore.find(word);
    return (it != mapstore.end());
}

bool DocPresent(unsigned int docID, string word) {
    map<unsigned int, unsigned long long int>::const_iterator it = mapstore[word].find(docID);
    return (it != mapstore[word].end());
}

int main(int argc, const char *argv[])
{
    FILE *fp = fopen(argv[1],"r");
    const char ind_tok[] = ":;-";
    const char Tok[50]=" \n0123456789'\"-^@#|+-*:/=><\t}_;.,!$&%{()[]\\";
    char str[10000];
    char query[10000];
    unsigned int DocId = 0;
    unsigned long long int num = 0;
    while (fscanf(fp,"%s", str) != EOF) {
        char *pch = strtok(str, ind_tok);
        string word(pch);
        mapstore[word] = {};
        while(true) {
            pch = strtok(NULL, ind_tok);
            if (pch == NULL) break;
            DocId = atol(pch);
            pch = strtok(NULL, ind_tok);
            num = strtoull(pch,NULL,0);
            mapstore[word][DocId] = num;
        }
    }
    int numQu;
    scanf("%d",&numQu);
    getchar();
    while(numQu--) {
        scanf("%[^\n]", query);
        getchar();
        char *ret = strtok(query, Tok);
        if (ret == NULL) {
            puts("");
            continue;
        }
        string qword = stemword(ret);
        string Writeit;
        map <unsigned int, unsigned long long int>::iterator WordIter;
        if (WordPresent(qword)) {
            for (WordIter = mapstore[qword].begin(); WordIter != mapstore[qword].end(); ++WordIter) {
                Writeit.append(to_string(WordIter->first)+',');
            }
        }
        if (Writeit.size() > 0) {
            Writeit.pop_back();
            puts(Writeit.c_str());
        } else puts("");
    }
    return 0;
}
