#include <stdint.h>

/**
 * Sets maximum memory amount to be used by the buffer pool. Must be called
 * before calling any of the other functions. Should only be called once.
 * 
 * @param max_mem_size Maximum memory size in MB to be used.
 * @return 0 if successful -1 if failed
 */
int init(uint32_t max_mem_size);

/**
 * Allocated a buffer of buf_size if buf_size is valid. Fails if
 * buf_size does not match predetermined buffer sizes, if total memory
 * allocated exceeds max_mem_size, if memory cannot be allocated, or if init()
 * was not called.
 * 
 * @param buf_size Desired size of buffer in MB.
 * @return A pointer to the allocated buffer if successful or nullptr if
 *     allocation fails.
 */
void *alloc(uint32_t buf_size);

/**
 * Deallocates given buffer making it available for allocation. Fails if buf 
 * does not match allocated memory or if init() was not called.
 * 
 * @param buf Buffer to be deallocated.
 * @return 0 if successful -1 if failed.
 */
int dealloc(void *buf);

/**
 * Resets the buffer pool by flushing the contents of any buffer in use.
 * 
 * @return 0 if successful -1 if failed.
 */
int reset();

/**
 * 
 */
void print_buf_stats();

/**
 * 
 */
void print_buf_details();