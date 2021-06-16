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
static unordered_map<int, list<void *>> allocated_bufs;
static unordered_map<int, list<void *>> back_pocket;

int init(uint32_t max_mem_size)
{
    if (!init_called)
    {
        _max_mem_size = max_mem_size;
        init_called = true;

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

        if (back_pocket.count(buf_size) == 0)
        {
            buf = operator new(buf_size * MEGA, std::nothrow);

            if (allocated_bufs.count(buf_size) == 0)
            {
                allocated_bufs.insert( {{buf_size, list<void *>( {buf} )}} );
            }
            else
            {
                allocated_bufs[buf_size].push_front(buf);
            }
        }
        else if (back_pocket[buf_size].empty())
        {
            buf = operator new(buf_size * MEGA, std::nothrow);
            allocated_bufs[buf_size].push_front(buf);
        }
        else
        {
            buf = back_pocket[buf_size].front();
            back_pocket[buf_size].pop_front();
            allocated_bufs[buf_size].push_front(buf);
        }

        if (buf != nullptr)
            mem_used += buf_size;

        return buf;
    }
    else
    {
        return nullptr;
    }
}

int dealloc(void *buf)
{
    return -1;
}

int reset()
{
    if (init_called)
    {
        init_called = false;
        _max_mem_size = 0;
        allocated_bufs.erase(allocated_bufs.begin(), allocated_bufs.end());
        back_pocket.erase(back_pocket.begin(), back_pocket.end());

        return 0;
    } 
    else 
    {
        return -1;
    }
}

void print_buf_stats()
{
    for (auto pair : allocated_bufs)
        printf("Buffer Size: %d Amount Allocated: %ld\n", pair.first, pair.second.size());
}

void print_buf_details()
{

}

