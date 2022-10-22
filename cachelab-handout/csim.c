#include "cachelab.h"
#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

typedef struct cache_line
{
    int valid;
    int tag;
    int timestamp;
} Cache_line;


typedef struct cache {
    int S;
    int E;
    int B;
    Cache_line **line;
} Cache;


Cache *cache = NULL;
char t[1000];
int verbose = 0;
int hit_count = 0, miss_count = 0, eviction_count = 0;

void Init_Cache(int s, int E, int b) {
    int S = 1 << s;
    int B = 1 << b;
    cache = (Cache *)malloc(sizeof(Cache));
    cache -> S = S;
    cache -> E = E;
    cache -> B = B;
    cache -> line = (Cache_line **)malloc(sizeof(Cache_line *) * S);
    for (int i = 0; i < S; i++)
    {
        cache -> line[i] = (Cache_line *)malloc(sizeof(Cache_line) * E);
        for (int j = 0; j < E; j++)
        {
            cache -> line[i][j].valid = 0;
            cache -> line[i][j].timestamp = 0;
            cache -> line[i][j].tag = -1;
        }
    }
}

void free_Cache() {
    int S = cache -> S;
    for (int i = 0; i < S; ++i)
    {
        free(cache -> line[i]);
    }
    free(cache -> line);
    free(cache);
}


int get_row(int index, int tag) {
    for (int i = 0; i < cache -> E; ++i)
    {
        if (cache -> line[index][i].valid && tag == cache -> line[index][i].tag) {
            return i;
        }
    }
    return -1;
}

int find_LRU(int index) {
    int max_stamp = 0;
    int choosen_row = 0;
    for (int i = 0; i < cache -> E; ++i)
    {
        if (cache -> line[index][i].timestamp > max_stamp)
        {
            max_stamp = cache -> line[index][i].timestamp;
            choosen_row = i;
        }
    }
    return choosen_row;
}

int is_full(int index) {
    for (int i = 0; i < cache -> E; ++i)
    {  
        if (cache -> line[index][i].valid == 0)
        {
            return i;
        }
    }
    return -1;
}

void update(int row, int index, int tag) {
    cache -> line[index][row].tag = tag;
    cache -> line[index][row].valid = 1;

    for (int i = 0; i < cache -> E; ++i)
    {
        if (cache -> line[index][i].valid == 1)
        {
            cache -> line[index][i].timestamp++;
        }
    }
    cache -> line[index][row].timestamp = 0;
}



void update_info(int tag, int index) {
    int row = get_row(index, tag);
    // 如果命中
    if (row == -1)
    {
        miss_count++;
        if (verbose)
            {
                printf("miss");
            }
        int i = is_full(index);
        // 没命中，但cache中有空闲行，无需替换，只要把需要的行放到空闲行即可
        if(i == -1) {
            eviction_count++;
            if (verbose) {
                printf("eviction");
            }
            i = find_LRU(index);
        }
        update(i, index, tag);
        
    }
    // 否则没命中
    else {
        hit_count++;
        if(verbose) {
            printf("hit");
        }
        update(row, index, tag);
    }
}




void get_trace(int s, int E, int b) {
    FILE * pFile;
    pFile = fopen(t, "r");
    // S 7ff000398,8
    char identifier;
    unsigned address;
    int size;

    if (pFile == NULL)
    {
        exit(-1);
    }
    // M 0421c7f0,4
    while (fscanf(pFile, "%c %x, %d", &identifier, &address, &size) > 0) {
        // 标记位和组序号
        int tag = address >> (s + b);
        int index = (address >> b) & ((unsigned)(-1) >> (sizeof(unsigned) * 8 - s));
        switch(identifier) {
            case 'L': //  data load
                update_info(tag, index);
                break;
            case 'M': // data modify (i.e., a data load followed by a data store)
                update_info(tag, index);
            case 'S': // data store
                update_info(tag, index);
                break;
        }
    }
    fclose(pFile);
}

void print_help() {
    printf("Usage: ./csim-ref [-hv] -s <s> -E <E> -b <b> -t <tracefile>\n"
           "-h: Optional help flag that prints usage info\n"
           "-v: Optional verbose flag that displays trace info\n"
           "-s <s>: Number of set index bits (S = 2s is the number of sets)\n"
           "-E <E>: Associativity (number of lines per set)\n"
           "-b <b>: Number of block bits (B = 2b is the block size)\n"
           "-t <tracefile>: Name of the valgrind trace to replay\n"
           "examples:\n"
           " linux> ./csim-ref -s 4 -E 1 -b 4 -t traces/yi.trace\n"
           " ./csim-ref -v -s 4 -E 1 -b 4 -t traces/yi.trace");
}

int main(int argc, char* argv[]) {
    char opt;
    int s, E, b;

    while (-1 != (opt = getopt(argc, argv, "hvs:E:b:t:")))
    {
        switch (opt)
        {
        case 'h':
            print_help();
            exit(0);
        case 'v':
            verbose = 1;
            break;
        case 's':
            s = atoi(optarg);
            break;
        case 'E':
            E = atoi(optarg);
            break;
        case 'b':
            b = atoi(optarg);
            break;
        case 't':
            strcpy(t, optarg);
            break;
        default:
            print_help();
            exit(-1);
        }
    }
    Init_Cache(s, E, b);
    get_trace(s, E, b);
    free_Cache();
    printSummary(hit_count, miss_count, eviction_count);
    return 0;
}