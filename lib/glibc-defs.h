#ifndef QSORT_GLIBC_DEFS_H
#define QSORT_GLIBC_DEFS_H

#ifndef atomic_write_barrier
# define atomic_write_barrier() __asm ("" ::: "memory")
#endif
#ifndef __set_errno
# define __set_errno(Val) errno = (Val)
#endif
#ifndef __sysconf
# define __sysconf sysconf
#endif
#ifndef __alloca
# define __alloca alloca
#endif
typedef int (*__compar_fn_t)(const void*, const void*);
typedef int (*__compar_d_fn_t)(const void*, const void*, void* dummy);
void * __mempcpy(void* dstpp, const void* srcpp, size_t len);
void glibc_quicksort (void *const pbase, size_t total_elems, size_t size,
	    __compar_d_fn_t cmp, void *arg);
#define libc_hidden_def(X)
#define weak_alias(X, Y)

#endif
