#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <list>
#include <vector>
#include <unordered_map>
#include <stdint.h>

using std::vector;
using std::list;
using std::unordered_map;

static const int MEGA = 1e6;

static bool init_called = false;
static uint32_t _max_mem_size;
static uint32_t mem_used;
static vector<int> buf_sizes({ 1, 2, 5, 10 });

struct BufInfo
{
    uint32_t size;
    bool in_use;
};

static unordered_map<void *, BufInfo> all_bufs;
static unordered_map<int, list<void *>> bufs_in_use;
static unordered_map<int, list<void *>> back_pocket;

int init(uint32_t max_mem_size)
{
    if (!init_called)
    {
        _max_mem_size = max_mem_size;
        init_called = true;

        for (int size : buf_sizes)
        {
            bufs_in_use[size] = list<void *>();
            back_pocket[size] = list<void *>();
        }

        return 0;
    } 
    else 
    {
        return -1;
    }
}

void *alloc(uint32_t buf_size)
{
    bool valid_size =
        (std::find(buf_sizes.begin(), buf_sizes.end(), buf_size)
        != buf_sizes.end());
    bool is_mem_remaining = (mem_used + buf_size) <= _max_mem_size;

    if (init_called && valid_size && is_mem_remaining)
    {
        void *buf;

        if (back_pocket[buf_size].empty())
        {
            buf = operator new(buf_size * MEGA, std::nothrow);
        }
        else
        {
            buf = back_pocket[buf_size].front();
            back_pocket[buf_size].pop_front();
        }

        if (buf)
        {
            mem_used += buf_size;
            bufs_in_use[buf_size].push_front(buf);

            if (all_bufs.count(buf) == 1)
            {
                all_bufs[buf].in_use = true;
            }
            else
            {
                all_bufs[buf] = BufInfo { buf_size, true };
            }
        }

        return buf;
    }
    else
    {
        return nullptr;
    }
}

int dealloc(void *buf)
{
    if (init_called && all_bufs.count(buf) == 1 && all_bufs[buf].in_use)
    {
        int size = all_bufs[buf].size;

        bufs_in_use[size].remove(buf);
        back_pocket[size].push_front(buf);
        all_bufs[buf].in_use = false;
        mem_used -= size;

        return 0;
    }
    else
    {
        return -1;
    }
}

int reset()
{
    if (init_called)
    {
        init_called = false;
        _max_mem_size = 0;
        mem_used = 0;
        all_bufs.clear();

        for (int size : buf_sizes)
        {
            bufs_in_use[size].clear();
            back_pocket[size].clear();
        }

        return 0;
    } 
    else 
    {
        return -1;
    }
}

void print_buf_stats()
{
    for (auto pair : bufs_in_use)
        printf("Buffer Size: %d Amount Allocated: %ld\n", pair.first, pair.second.size());

    printf("\n");
}

void print_buf_details()
{
    int num_in_use = 0;
    int num_witheld = 0;

    for (auto pair : bufs_in_use)
        num_in_use += pair.second.size();

    for (auto pair : back_pocket)
        num_witheld += pair.second.size();

    printf("Total amount of buffers: %ld\n", all_bufs.size());
    printf("Amount of buffers in use: %d\n", num_in_use);
    printf("Amount of buffers witheld: %d\n", num_witheld);
    printf("\n");
}

