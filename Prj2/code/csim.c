/*
 * Name: DouYiming
 * ID: 519021910366
 */
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "cachelab.h"

const int m = 64;

typedef struct Arg
{
    int s; // Number of set index bits
    int E; // number of lines per set
    int b; // Number of block bits
    FILE *f; // valgrind trace to replay
} Arg;

typedef struct summary
{
    int hits;
    int misses;
    int evictions;
} summary;

typedef struct Line
{
    int valid;
    int tag;
    int time_stamp;
} Line;

typedef struct Set
{
    Line* lines;
} Set;

Arg arg_parser(int argc, char ** argv)
{
    Arg arg;
    if(getopt(argc, argv, "s:E:b:t:") == 's')
        arg.s = atoi(optarg);
    if(getopt(argc, argv, "s:E:b:t:") == 'E')
        arg.E = atoi(optarg);
    if(getopt(argc, argv, "s:E:b:t:") == 'b')
        arg.b = atoi(optarg);
    if(getopt(argc, argv, "s:E:b:t:") == 't')
        arg.f = fopen(optarg, "r");
    return arg;
}

Set* init_cache(Arg arg)
{
    int set_num = 1 << arg.s;
    Set* caches=(Set*)malloc(set_num * sizeof(Set));
    for(int i = 0;i < set_num; ++i)
    {
        caches[i].lines = (Line*)malloc(arg.E * sizeof(Line));
        for(int j = 0; j < arg.E; ++j)
        {
            caches[i].lines[j].valid = 0;
            caches[i].lines[j].time_stamp = 0;
        }
    }
    return caches;
}

summary simulator(Arg arg, Set* caches)
{
    // information to display
    summary ans;
    ans.hits = 0;
    ans.misses = 0;
    ans.evictions = 0;
    // compute tag bits
    int t = m - arg.b - arg.s;
    // simulator time
    int cur_time = 0;
    // information of each instruction
    char op;
    long addr;
    int len;
    int cycle_time;

    // read from file
    while(!feof(arg.f))
    {
        if (fscanf(arg.f, " %c %lx,%d", &op, &addr, &len) != 3)
            continue;

        if (op == 'L' || op == 'S')
            cycle_time = 1;
        else if (op == 'M')
            cycle_time = 2;
        else
            continue;
        while(cycle_time--)
        {
            ++cur_time;
            int set_num = (addr >> arg.b) & ((1 << arg.s) - 1);
            int tag = (addr >> (arg.s + arg.b)) & ((1 << t) - 1);
            int is_hit = 0;
            int invalid_line = -1;
            int LRU_line = -1;
            int min_time = 0x7fffffff;
            // search each way
            for(int i = 0;i < arg.E; ++i)
            {
                Line* line = &caches[set_num].lines[i];
                if(line->valid && line->tag == tag) // hit
                {
                    is_hit = 1;
                    line->time_stamp = cur_time;
                    break;
                }
                else if(!line->valid)
                {
                    invalid_line = i;
                    break;
                }
                else if(line->valid && line->time_stamp<min_time)
                {
                    min_time = line->time_stamp;
                    LRU_line = i;
                }
            }
            if(is_hit)//hit
                ++ans.hits;
            else
            {
                ++ans.misses;
                if(invalid_line != -1)//miss
                {
                    Line*l=&caches[set_num].lines[invalid_line];
                    l->tag = tag;
                    l->time_stamp = cur_time;
                    l->valid = 1;
                }
                else//evict
                {
                    ++ans.evictions;
                    Line*l=&caches[set_num].lines[LRU_line];
                    l->tag = tag;
                    l->time_stamp = cur_time;
                }
            }
        }
    }
    return ans;
}

void terminate(Arg arg, Set* caches)
{
    fclose(arg.f);
    for (int i = 0; i < (1 << arg.s); ++i)
        free(caches[i].lines);
    free(caches);
}

int main(int argc, char **argv)
{
    // arg parsing
    Arg arg = arg_parser(argc, argv);
    // cache initialization
    Set* caches = init_cache(arg);
    // simulation
    summary s = simulator(arg, caches);
    // ending
    terminate(arg,caches);
    // print ans
    printSummary(s.hits, s.misses, s.evictions);
    return 0;
}
