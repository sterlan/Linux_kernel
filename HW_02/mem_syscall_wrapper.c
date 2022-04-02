#include <sys/syscall.h>
#include <unistd.h>
#include "mem_syscall_wrapper.h"

long slob_get_total_alloc_syscall(void) {

	return(syscall(__NR_slob_get_total_alloc_mem));

}

long slob_get_total_free_syscall(void) {

	return (syscall(__NR_slob_get_total_free_mem));

}
