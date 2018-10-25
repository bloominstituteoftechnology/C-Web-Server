# 1 "net.c"
# 1 "<built-in>"
# 1 "<command-line>"
# 1 "net.c"
# 1 "/usr/include/stdio.h" 1 3 4
# 29 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/_ansi.h" 1 3 4
# 10 "/usr/include/_ansi.h" 3 4
# 1 "/usr/include/newlib.h" 1 3 4
# 14 "/usr/include/newlib.h" 3 4
# 1 "/usr/include/_newlib_version.h" 1 3 4
# 15 "/usr/include/newlib.h" 2 3 4
# 11 "/usr/include/_ansi.h" 2 3 4
# 1 "/usr/include/sys/config.h" 1 3 4



# 1 "/usr/include/machine/ieeefp.h" 1 3 4
# 5 "/usr/include/sys/config.h" 2 3 4
# 1 "/usr/include/sys/features.h" 1 3 4
# 6 "/usr/include/sys/config.h" 2 3 4
# 234 "/usr/include/sys/config.h" 3 4
# 1 "/usr/include/cygwin/config.h" 1 3 4
# 235 "/usr/include/sys/config.h" 2 3 4
# 12 "/usr/include/_ansi.h" 2 3 4
# 30 "/usr/include/stdio.h" 2 3 4





# 1 "/usr/include/sys/cdefs.h" 1 3 4
# 45 "/usr/include/sys/cdefs.h" 3 4
# 1 "/usr/include/machine/_default_types.h" 1 3 4
# 41 "/usr/include/machine/_default_types.h" 3 4

# 41 "/usr/include/machine/_default_types.h" 3 4
typedef signed char __int8_t;

typedef unsigned char __uint8_t;
# 55 "/usr/include/machine/_default_types.h" 3 4
typedef short int __int16_t;

typedef short unsigned int __uint16_t;
# 77 "/usr/include/machine/_default_types.h" 3 4
typedef int __int32_t;

typedef unsigned int __uint32_t;
# 103 "/usr/include/machine/_default_types.h" 3 4
typedef long int __int64_t;

typedef long unsigned int __uint64_t;
# 134 "/usr/include/machine/_default_types.h" 3 4
typedef signed char __int_least8_t;

typedef unsigned char __uint_least8_t;
# 160 "/usr/include/machine/_default_types.h" 3 4
typedef short int __int_least16_t;

typedef short unsigned int __uint_least16_t;
# 182 "/usr/include/machine/_default_types.h" 3 4
typedef int __int_least32_t;

typedef unsigned int __uint_least32_t;
# 200 "/usr/include/machine/_default_types.h" 3 4
typedef long int __int_least64_t;

typedef long unsigned int __uint_least64_t;
# 214 "/usr/include/machine/_default_types.h" 3 4
typedef long int __intmax_t;







typedef long unsigned int __uintmax_t;







typedef long int __intptr_t;

typedef long unsigned int __uintptr_t;
# 46 "/usr/include/sys/cdefs.h" 2 3 4

# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 216 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 3 4
typedef long unsigned int size_t;
# 48 "/usr/include/sys/cdefs.h" 2 3 4
# 36 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 149 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 3 4
typedef long int ptrdiff_t;
# 328 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 3 4
typedef short unsigned int wchar_t;
# 426 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 3 4
typedef struct {
  long long __max_align_ll __attribute__((__aligned__(__alignof__(long long))));
  long double __max_align_ld __attribute__((__aligned__(__alignof__(long double))));
# 437 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 3 4
} max_align_t;
# 37 "/usr/include/stdio.h" 2 3 4



# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stdarg.h" 1 3 4
# 40 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stdarg.h" 3 4
typedef __builtin_va_list __gnuc_va_list;
# 41 "/usr/include/stdio.h" 2 3 4





typedef __gnuc_va_list va_list;
# 60 "/usr/include/stdio.h" 3 4
# 1 "/usr/include/sys/reent.h" 1 3 4
# 13 "/usr/include/sys/reent.h" 3 4
# 1 "/usr/include/_ansi.h" 1 3 4
# 14 "/usr/include/sys/reent.h" 2 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 15 "/usr/include/sys/reent.h" 2 3 4
# 1 "/usr/include/sys/_types.h" 1 3 4
# 24 "/usr/include/sys/_types.h" 3 4
# 1 "/usr/include/machine/_types.h" 1 3 4
# 17 "/usr/include/machine/_types.h" 3 4
typedef __int64_t __blkcnt_t;


typedef __int32_t __blksize_t;


typedef __uint32_t __dev_t;



typedef unsigned long __fsblkcnt_t;



typedef unsigned long __fsfilcnt_t;


typedef __uint32_t __uid_t;


typedef __uint32_t __gid_t;


typedef __uint64_t __ino_t;


typedef long long __key_t;


typedef __uint16_t __sa_family_t;



typedef int __socklen_t;
# 25 "/usr/include/sys/_types.h" 2 3 4
# 1 "/usr/include/sys/lock.h" 1 3 4
# 12 "/usr/include/sys/lock.h" 3 4
typedef void *_LOCK_T;
# 42 "/usr/include/sys/lock.h" 3 4
void __cygwin_lock_init(_LOCK_T *);
void __cygwin_lock_init_recursive(_LOCK_T *);
void __cygwin_lock_fini(_LOCK_T *);
void __cygwin_lock_lock(_LOCK_T *);
int __cygwin_lock_trylock(_LOCK_T *);
void __cygwin_lock_unlock(_LOCK_T *);
# 26 "/usr/include/sys/_types.h" 2 3 4
# 44 "/usr/include/sys/_types.h" 3 4
typedef long _off_t;





typedef int __pid_t;
# 65 "/usr/include/sys/_types.h" 3 4
typedef __uint32_t __id_t;
# 88 "/usr/include/sys/_types.h" 3 4
typedef __uint32_t __mode_t;





__extension__ typedef long long _off64_t;





typedef _off_t __off_t;


typedef _off64_t __loff_t;
# 114 "/usr/include/sys/_types.h" 3 4
typedef long _fpos_t;





typedef _off64_t _fpos64_t;
# 129 "/usr/include/sys/_types.h" 3 4
typedef long unsigned int __size_t;
# 145 "/usr/include/sys/_types.h" 3 4
typedef long signed int _ssize_t;
# 156 "/usr/include/sys/_types.h" 3 4
typedef _ssize_t __ssize_t;


# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 357 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 3 4
typedef unsigned int wint_t;
# 160 "/usr/include/sys/_types.h" 2 3 4



typedef struct
{
  int __count;
  union
  {
    wint_t __wch;
    unsigned char __wchb[4];
  } __value;
} _mbstate_t;



typedef _LOCK_T _flock_t;




typedef void *_iconv_t;






typedef unsigned long __clock_t;






typedef long __time_t;





typedef unsigned long __clockid_t;


typedef unsigned long __timer_t;
# 213 "/usr/include/sys/_types.h" 3 4
typedef int __nl_item;
typedef unsigned short __nlink_t;
typedef long __suseconds_t;
typedef unsigned long __useconds_t;


typedef __builtin_va_list __va_list;
# 16 "/usr/include/sys/reent.h" 2 3 4
# 25 "/usr/include/sys/reent.h" 3 4
typedef unsigned int __ULong;
# 38 "/usr/include/sys/reent.h" 3 4
struct _reent;

struct __locale_t;






struct _Bigint
{
  struct _Bigint *_next;
  int _k, _maxwds, _sign, _wds;
  __ULong _x[1];
};


struct __tm
{
  int __tm_sec;
  int __tm_min;
  int __tm_hour;
  int __tm_mday;
  int __tm_mon;
  int __tm_year;
  int __tm_wday;
  int __tm_yday;
  int __tm_isdst;
};







struct _on_exit_args {
 void * _fnargs[32];
 void * _dso_handle[32];

 __ULong _fntypes;


 __ULong _is_cxa;
};
# 93 "/usr/include/sys/reent.h" 3 4
struct _atexit {
 struct _atexit *_next;
 int _ind;

 void (*_fns[32])(void);
        struct _on_exit_args _on_exit_args;
};
# 117 "/usr/include/sys/reent.h" 3 4
struct __sbuf {
 unsigned char *_base;
 int _size;
};
# 181 "/usr/include/sys/reent.h" 3 4
struct __sFILE {
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;






  void * _cookie;

  _ssize_t (*_read) (struct _reent *, void *,
        char *, size_t);
  _ssize_t (*_write) (struct _reent *, void *,
         const char *,
         size_t);
  _fpos_t (*_seek) (struct _reent *, void *, _fpos_t, int);
  int (*_close) (struct _reent *, void *);


  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;


  unsigned char _ubuf[3];
  unsigned char _nbuf[1];


  struct __sbuf _lb;


  int _blksize;
  _off_t _offset;


  struct _reent *_data;



  _flock_t _lock;

  _mbstate_t _mbstate;
  int _flags2;
};
# 239 "/usr/include/sys/reent.h" 3 4
struct __sFILE64 {
  unsigned char *_p;
  int _r;
  int _w;
  short _flags;
  short _file;
  struct __sbuf _bf;
  int _lbfsize;

  struct _reent *_data;


  void * _cookie;

  _ssize_t (*_read) (struct _reent *, void *,
        char *, size_t);
  _ssize_t (*_write) (struct _reent *, void *,
         const char *,
         size_t);
  _fpos_t (*_seek) (struct _reent *, void *, _fpos_t, int);
  int (*_close) (struct _reent *, void *);


  struct __sbuf _ub;
  unsigned char *_up;
  int _ur;


  unsigned char _ubuf[3];
  unsigned char _nbuf[1];


  struct __sbuf _lb;


  int _blksize;
  int _flags2;

  _off64_t _offset;
  _fpos64_t (*_seek64) (struct _reent *, void *, _fpos64_t, int);


  _flock_t _lock;

  _mbstate_t _mbstate;
};
typedef struct __sFILE64 __FILE;





struct _glue
{
  struct _glue *_next;
  int _niobs;
  __FILE *_iobs;
};
# 319 "/usr/include/sys/reent.h" 3 4
struct _rand48 {
  unsigned short _seed[3];
  unsigned short _mult[3];
  unsigned short _add;




};
# 608 "/usr/include/sys/reent.h" 3 4
struct _reent
{
  int _errno;




  __FILE *_stdin, *_stdout, *_stderr;

  int _inc;
  char _emergency[25];


  int _unspecified_locale_info;
  struct __locale_t *_locale;

  int __sdidinit;

  void (*__cleanup) (struct _reent *);


  struct _Bigint *_result;
  int _result_k;
  struct _Bigint *_p5s;
  struct _Bigint **_freelist;


  int _cvtlen;
  char *_cvtbuf;

  union
    {
      struct
        {
          unsigned int _unused_rand;
          char * _strtok_last;
          char _asctime_buf[26];
          struct __tm _localtime_buf;
          int _gamma_signgam;
          __extension__ unsigned long long _rand_next;
          struct _rand48 _r48;
          _mbstate_t _mblen_state;
          _mbstate_t _mbtowc_state;
          _mbstate_t _wctomb_state;
          char _l64a_buf[8];
          char _signal_buf[24];
          int _getdate_err;
          _mbstate_t _mbrlen_state;
          _mbstate_t _mbrtowc_state;
          _mbstate_t _mbsrtowcs_state;
          _mbstate_t _wcrtomb_state;
          _mbstate_t _wcsrtombs_state;
   int _h_errno;
        } _reent;



      struct
        {

          unsigned char * _nextf[30];
          unsigned int _nmalloc[30];
        } _unused;
    } _new;



  struct _atexit *_atexit;
  struct _atexit _atexit0;



  void (**(_sig_func))(int);




  struct _glue __sglue;

  __FILE __sf[3];

};
# 814 "/usr/include/sys/reent.h" 3 4
extern struct _reent *_impure_ptr ;
extern struct _reent *const _global_impure_ptr ;

void _reclaim_reent (struct _reent *);





  struct _reent * __getreent (void);
# 61 "/usr/include/stdio.h" 2 3 4
# 1 "/usr/include/sys/types.h" 1 3 4
# 28 "/usr/include/sys/types.h" 3 4
typedef __uint8_t u_int8_t;


typedef __uint16_t u_int16_t;


typedef __uint32_t u_int32_t;


typedef __uint64_t u_int64_t;

typedef int register_t;
# 62 "/usr/include/sys/types.h" 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 63 "/usr/include/sys/types.h" 2 3 4

# 1 "/usr/include/sys/_stdint.h" 1 3 4
# 20 "/usr/include/sys/_stdint.h" 3 4
typedef __int8_t int8_t ;



typedef __uint8_t uint8_t ;







typedef __int16_t int16_t ;



typedef __uint16_t uint16_t ;







typedef __int32_t int32_t ;



typedef __uint32_t uint32_t ;







typedef __int64_t int64_t ;



typedef __uint64_t uint64_t ;






typedef __intmax_t intmax_t;




typedef __uintmax_t uintmax_t;




typedef __intptr_t intptr_t;




typedef __uintptr_t uintptr_t;
# 65 "/usr/include/sys/types.h" 2 3 4


# 1 "/usr/include/machine/endian.h" 1 3 4





# 1 "/usr/include/machine/_endian.h" 1 3 4
# 14 "/usr/include/machine/_endian.h" 3 4
# 1 "/usr/include/bits/endian.h" 1 3 4
# 15 "/usr/include/machine/_endian.h" 2 3 4
# 23 "/usr/include/machine/_endian.h" 3 4
static __inline__ __uint32_t __ntohl(__uint32_t);
static __inline__ __uint16_t __ntohs(__uint16_t);

static __inline__ __uint32_t
__ntohl(__uint32_t _x)
{
 __asm__("bswap %0" : "=r" (_x) : "0" (_x));
 return _x;
}

static __inline__ __uint16_t
__ntohs(__uint16_t _x)
{
 __asm__("xchgb %b0,%h0"
  : "=Q" (_x)
  : "0" (_x));
 return _x;
}
# 7 "/usr/include/machine/endian.h" 2 3 4
# 68 "/usr/include/sys/types.h" 2 3 4
# 1 "/usr/include/sys/select.h" 1 3 4
# 14 "/usr/include/sys/select.h" 3 4
# 1 "/usr/include/sys/_sigset.h" 1 3 4
# 41 "/usr/include/sys/_sigset.h" 3 4
typedef unsigned long __sigset_t;
# 15 "/usr/include/sys/select.h" 2 3 4
# 1 "/usr/include/sys/_timeval.h" 1 3 4
# 35 "/usr/include/sys/_timeval.h" 3 4
typedef __suseconds_t suseconds_t;




typedef long time_t;
# 52 "/usr/include/sys/_timeval.h" 3 4
struct timeval {
 time_t tv_sec;
 suseconds_t tv_usec;
};
# 16 "/usr/include/sys/select.h" 2 3 4
# 1 "/usr/include/sys/timespec.h" 1 3 4
# 38 "/usr/include/sys/timespec.h" 3 4
# 1 "/usr/include/sys/_timespec.h" 1 3 4
# 45 "/usr/include/sys/_timespec.h" 3 4
struct timespec {
 time_t tv_sec;
 long tv_nsec;
};
# 39 "/usr/include/sys/timespec.h" 2 3 4
# 58 "/usr/include/sys/timespec.h" 3 4
struct itimerspec {
 struct timespec it_interval;
 struct timespec it_value;
};
# 17 "/usr/include/sys/select.h" 2 3 4



typedef __sigset_t sigset_t;
# 34 "/usr/include/sys/select.h" 3 4
typedef unsigned long fd_mask;







typedef struct _types_fd_set {
 fd_mask fds_bits[(((64)+(((sizeof (fd_mask) * 8))-1))/((sizeof (fd_mask) * 8)))];
} _types_fd_set;
# 60 "/usr/include/sys/select.h" 3 4


int select (int __n, _types_fd_set *__readfds, _types_fd_set *__writefds, _types_fd_set *__exceptfds, struct timeval *__timeout)
                                                   ;

int pselect (int __n, _types_fd_set *__readfds, _types_fd_set *__writefds, _types_fd_set *__exceptfds, const struct timespec *__timeout, const sigset_t *__set)

                           ;



# 69 "/usr/include/sys/types.h" 2 3 4




typedef __uint32_t in_addr_t;




typedef __uint16_t in_port_t;
# 87 "/usr/include/sys/types.h" 3 4
typedef unsigned char u_char;



typedef unsigned short u_short;



typedef unsigned int u_int;



typedef unsigned long u_long;







typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;



typedef __blkcnt_t blkcnt_t;




typedef __blksize_t blksize_t;




typedef unsigned long clock_t;
# 135 "/usr/include/sys/types.h" 3 4
typedef long daddr_t;



typedef char * caddr_t;




typedef __fsblkcnt_t fsblkcnt_t;
typedef __fsfilcnt_t fsfilcnt_t;




typedef __id_t id_t;




typedef __ino_t ino_t;
# 173 "/usr/include/sys/types.h" 3 4
typedef __off_t off_t;



typedef __dev_t dev_t;



typedef __uid_t uid_t;



typedef __gid_t gid_t;




typedef __pid_t pid_t;




typedef __key_t key_t;




typedef _ssize_t ssize_t;




typedef __mode_t mode_t;




typedef __nlink_t nlink_t;




typedef __clockid_t clockid_t;





typedef __timer_t timer_t;





typedef __useconds_t useconds_t;
# 236 "/usr/include/sys/types.h" 3 4
typedef __int64_t sbintime_t;


# 1 "/usr/include/sys/_pthreadtypes.h" 1 3 4
# 15 "/usr/include/sys/_pthreadtypes.h" 3 4
typedef struct __pthread_t {char __dummy;} *pthread_t;
typedef struct __pthread_mutex_t {char __dummy;} *pthread_mutex_t;

typedef struct __pthread_key_t {char __dummy;} *pthread_key_t;
typedef struct __pthread_attr_t {char __dummy;} *pthread_attr_t;
typedef struct __pthread_mutexattr_t {char __dummy;} *pthread_mutexattr_t;
typedef struct __pthread_condattr_t {char __dummy;} *pthread_condattr_t;
typedef struct __pthread_cond_t {char __dummy;} *pthread_cond_t;
typedef struct __pthread_barrierattr_t {char __dummy;} *pthread_barrierattr_t;
typedef struct __pthread_barrier_t {char __dummy;} *pthread_barrier_t;


typedef struct
{
  pthread_mutex_t mutex;
  int state;
}
pthread_once_t;
typedef struct __pthread_spinlock_t {char __dummy;} *pthread_spinlock_t;
typedef struct __pthread_rwlock_t {char __dummy;} *pthread_rwlock_t;
typedef struct __pthread_rwlockattr_t {char __dummy;} *pthread_rwlockattr_t;
# 240 "/usr/include/sys/types.h" 2 3 4
# 1 "/usr/include/machine/types.h" 1 3 4
# 19 "/usr/include/machine/types.h" 3 4
# 1 "/usr/include/endian.h" 1 3 4
# 38 "/usr/include/endian.h" 3 4
# 1 "/usr/include/bits/byteswap.h" 1 3 4
# 16 "/usr/include/bits/byteswap.h" 3 4
static __inline unsigned short
__bswap_16 (unsigned short __x)
{
  return (__x >> 8) | (__x << 8);
}

static __inline unsigned int
__bswap_32 (unsigned int __x)
{
  return (__bswap_16 (__x & 0xffff) << 16) | (__bswap_16 (__x >> 16));
}

static __inline unsigned long long
__bswap_64 (unsigned long long __x)
{
  return (((unsigned long long) __bswap_32 (__x & 0xffffffffull)) << 32) | (__bswap_32 (__x >> 32));
}
# 39 "/usr/include/endian.h" 2 3 4
# 20 "/usr/include/machine/types.h" 2 3 4
# 1 "/usr/include/bits/wordsize.h" 1 3 4
# 21 "/usr/include/machine/types.h" 2 3 4




typedef struct timespec timespec_t;




typedef struct timespec timestruc_t;


typedef __loff_t loff_t;
# 46 "/usr/include/machine/types.h" 3 4
struct flock {
 short l_type;
 short l_whence;
 off_t l_start;
 off_t l_len;
 pid_t l_pid;
};






typedef unsigned long vm_offset_t;




typedef unsigned long vm_size_t;




typedef void *vm_object_t;




typedef char *addr_t;





# 1 "/usr/include/sys/sysmacros.h" 1 3 4
# 12 "/usr/include/sys/sysmacros.h" 3 4
# 1 "/usr/include/sys/types.h" 1 3 4
# 13 "/usr/include/sys/sysmacros.h" 2 3 4

static __inline__ int gnu_dev_major(dev_t);
static __inline__ int gnu_dev_minor(dev_t);
static __inline__ dev_t gnu_dev_makedev(int, int);

static __inline__ int
gnu_dev_major(dev_t dev)
{
 return (int)(((dev) >> 16) & 0xffff);
}

static __inline__ int
gnu_dev_minor(dev_t dev)
{
 return (int)((dev) & 0xffff);
}

static __inline__ dev_t
gnu_dev_makedev(int maj, int min)
{
 return (((maj) << 16) | ((min) & 0xffff));
}
# 81 "/usr/include/machine/types.h" 2 3 4
# 241 "/usr/include/sys/types.h" 2 3 4
# 62 "/usr/include/stdio.h" 2 3 4




typedef __FILE FILE;




typedef _fpos64_t fpos_t;







# 1 "/usr/include/sys/stdio.h" 1 3 4
# 33 "/usr/include/sys/stdio.h" 3 4


ssize_t getline (char **, size_t *, FILE *);
ssize_t getdelim (char **, size_t *, int, FILE *);


# 80 "/usr/include/stdio.h" 2 3 4
# 181 "/usr/include/stdio.h" 3 4
char * ctermid (char *);




FILE * tmpfile (void);
char * tmpnam (char *);

char * tempnam (const char *, const char *);

int fclose (FILE *);
int fflush (FILE *);
FILE * freopen (const char *restrict, const char *restrict, FILE *restrict);
void setbuf (FILE *restrict, char *restrict);
int setvbuf (FILE *restrict, char *restrict, int, size_t);
int fprintf (FILE *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));
int fscanf (FILE *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__scanf__, 2, 3)));
int printf (const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 1, 2)));
int scanf (const char *restrict, ...)
               __attribute__ ((__format__ (__scanf__, 1, 2)));
int sscanf (const char *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__scanf__, 2, 3)));
int vfprintf (FILE *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
int vprintf (const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 1, 0)));
int vsprintf (char *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
int fgetc (FILE *);
char * fgets (char *restrict, int, FILE *restrict);
int fputc (int, FILE *);
int fputs (const char *restrict, FILE *restrict);
int getc (FILE *);
int getchar (void);
char * gets (char *);
int putc (int, FILE *);
int putchar (int);
int puts (const char *);
int ungetc (int, FILE *);
size_t fread (void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t fwrite (const void *restrict , size_t _size, size_t _n, FILE *);



int fgetpos (FILE *restrict, fpos_t *restrict);

int fseek (FILE *, long, int);



int fsetpos (FILE *, const fpos_t *);

long ftell ( FILE *);
void rewind (FILE *);
void clearerr (FILE *);
int feof (FILE *);
int ferror (FILE *);
void perror (const char *);

FILE * fopen (const char *restrict _name, const char *restrict _type);
int sprintf (char *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));
int remove (const char *);
int rename (const char *, const char *);
# 257 "/usr/include/stdio.h" 3 4
int fseeko (FILE *, off_t, int);
off_t ftello (FILE *);







int snprintf (char *restrict, size_t, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int vsnprintf (char *restrict, size_t, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int vfscanf (FILE *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 2, 0)));
int vscanf (const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 1, 0)));
int vsscanf (const char *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 2, 0)));
# 284 "/usr/include/stdio.h" 3 4
int asiprintf (char **, const char *, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));
char * asniprintf (char *, size_t *, const char *, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
char * asnprintf (char *restrict, size_t *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));

int diprintf (int, const char *, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));

int fiprintf (FILE *, const char *, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));
int fiscanf (FILE *, const char *, ...)
               __attribute__ ((__format__ (__scanf__, 2, 3)));
int iprintf (const char *, ...)
               __attribute__ ((__format__ (__printf__, 1, 2)));
int iscanf (const char *, ...)
               __attribute__ ((__format__ (__scanf__, 1, 2)));
int siprintf (char *, const char *, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));
int siscanf (const char *, const char *, ...)
               __attribute__ ((__format__ (__scanf__, 2, 3)));
int sniprintf (char *, size_t, const char *, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int vasiprintf (char **, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
char * vasniprintf (char *, size_t *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
char * vasnprintf (char *, size_t *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int vdiprintf (int, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
int vfiprintf (FILE *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
int vfiscanf (FILE *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 2, 0)));
int viprintf (const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 1, 0)));
int viscanf (const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 1, 0)));
int vsiprintf (char *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
int vsiscanf (const char *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 2, 0)));
int vsniprintf (char *, size_t, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
# 339 "/usr/include/stdio.h" 3 4
FILE * fdopen (int, const char *);

int fileno (FILE *);


int pclose (FILE *);
FILE * popen (const char *, const char *);



void setbuffer (FILE *, char *, int);
int setlinebuf (FILE *);



int getw (FILE *);
int putw (int, FILE *);


int getc_unlocked (FILE *);
int getchar_unlocked (void);
void flockfile (FILE *);
int ftrylockfile (FILE *);
void funlockfile (FILE *);
int putc_unlocked (int, FILE *);
int putchar_unlocked (int);
# 374 "/usr/include/stdio.h" 3 4
int dprintf (int, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));

FILE * fmemopen (void *restrict, size_t, const char *restrict);


FILE * open_memstream (char **, size_t *);
int vdprintf (int, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));



int renameat (int, const char *, int, const char *);

int renameat2 (int, const char *, int, const char *, unsigned int);







int _asiprintf_r (struct _reent *, char **, const char *, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
char * _asniprintf_r (struct _reent *, char *, size_t *, const char *, ...)
               __attribute__ ((__format__ (__printf__, 4, 5)));
char * _asnprintf_r (struct _reent *, char *restrict, size_t *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 4, 5)));
int _asprintf_r (struct _reent *, char **restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int _diprintf_r (struct _reent *, int, const char *, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int _dprintf_r (struct _reent *, int, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int _fclose_r (struct _reent *, FILE *);
int _fcloseall_r (struct _reent *);
FILE * _fdopen_r (struct _reent *, int, const char *);
int _fflush_r (struct _reent *, FILE *);
int _fgetc_r (struct _reent *, FILE *);
int _fgetc_unlocked_r (struct _reent *, FILE *);
char * _fgets_r (struct _reent *, char *restrict, int, FILE *restrict);
char * _fgets_unlocked_r (struct _reent *, char *restrict, int, FILE *restrict);




int _fgetpos_r (struct _reent *, FILE *, fpos_t *);
int _fsetpos_r (struct _reent *, FILE *, const fpos_t *);

int _fiprintf_r (struct _reent *, FILE *, const char *, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int _fiscanf_r (struct _reent *, FILE *, const char *, ...)
               __attribute__ ((__format__ (__scanf__, 3, 4)));
FILE * _fmemopen_r (struct _reent *, void *restrict, size_t, const char *restrict);
FILE * _fopen_r (struct _reent *, const char *restrict, const char *restrict);
FILE * _freopen_r (struct _reent *, const char *restrict, const char *restrict, FILE *restrict);
int _fprintf_r (struct _reent *, FILE *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int _fpurge_r (struct _reent *, FILE *);
int _fputc_r (struct _reent *, int, FILE *);
int _fputc_unlocked_r (struct _reent *, int, FILE *);
int _fputs_r (struct _reent *, const char *restrict, FILE *restrict);
int _fputs_unlocked_r (struct _reent *, const char *restrict, FILE *restrict);
size_t _fread_r (struct _reent *, void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t _fread_unlocked_r (struct _reent *, void *restrict, size_t _size, size_t _n, FILE *restrict);
int _fscanf_r (struct _reent *, FILE *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__scanf__, 3, 4)));
int _fseek_r (struct _reent *, FILE *, long, int);
int _fseeko_r (struct _reent *, FILE *, _off_t, int);
long _ftell_r (struct _reent *, FILE *);
_off_t _ftello_r (struct _reent *, FILE *);
void _rewind_r (struct _reent *, FILE *);
size_t _fwrite_r (struct _reent *, const void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t _fwrite_unlocked_r (struct _reent *, const void *restrict, size_t _size, size_t _n, FILE *restrict);
int _getc_r (struct _reent *, FILE *);
int _getc_unlocked_r (struct _reent *, FILE *);
int _getchar_r (struct _reent *);
int _getchar_unlocked_r (struct _reent *);
char * _gets_r (struct _reent *, char *);
int _iprintf_r (struct _reent *, const char *, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));
int _iscanf_r (struct _reent *, const char *, ...)
               __attribute__ ((__format__ (__scanf__, 2, 3)));
FILE * _open_memstream_r (struct _reent *, char **, size_t *);
void _perror_r (struct _reent *, const char *);
int _printf_r (struct _reent *, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 2, 3)));
int _putc_r (struct _reent *, int, FILE *);
int _putc_unlocked_r (struct _reent *, int, FILE *);
int _putchar_unlocked_r (struct _reent *, int);
int _putchar_r (struct _reent *, int);
int _puts_r (struct _reent *, const char *);
int _remove_r (struct _reent *, const char *);
int _rename_r (struct _reent *,
      const char *_old, const char *_new);
int _scanf_r (struct _reent *, const char *restrict, ...)
               __attribute__ ((__format__ (__scanf__, 2, 3)));
int _siprintf_r (struct _reent *, char *, const char *, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int _siscanf_r (struct _reent *, const char *, const char *, ...)
               __attribute__ ((__format__ (__scanf__, 3, 4)));
int _sniprintf_r (struct _reent *, char *, size_t, const char *, ...)
               __attribute__ ((__format__ (__printf__, 4, 5)));
int _snprintf_r (struct _reent *, char *restrict, size_t, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 4, 5)));
int _sprintf_r (struct _reent *, char *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__printf__, 3, 4)));
int _sscanf_r (struct _reent *, const char *restrict, const char *restrict, ...)
               __attribute__ ((__format__ (__scanf__, 3, 4)));
char * _tempnam_r (struct _reent *, const char *, const char *);
FILE * _tmpfile_r (struct _reent *);
char * _tmpnam_r (struct _reent *, char *);
int _ungetc_r (struct _reent *, int, FILE *);
int _vasiprintf_r (struct _reent *, char **, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
char * _vasniprintf_r (struct _reent*, char *, size_t *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 4, 0)));
char * _vasnprintf_r (struct _reent*, char *, size_t *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 4, 0)));
int _vasprintf_r (struct _reent *, char **, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int _vdiprintf_r (struct _reent *, int, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int _vdprintf_r (struct _reent *, int, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int _vfiprintf_r (struct _reent *, FILE *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int _vfiscanf_r (struct _reent *, FILE *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 3, 0)));
int _vfprintf_r (struct _reent *, FILE *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int _vfscanf_r (struct _reent *, FILE *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 3, 0)));
int _viprintf_r (struct _reent *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
int _viscanf_r (struct _reent *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 2, 0)));
int _vprintf_r (struct _reent *, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 2, 0)));
int _vscanf_r (struct _reent *, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 2, 0)));
int _vsiprintf_r (struct _reent *, char *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int _vsiscanf_r (struct _reent *, const char *, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 3, 0)));
int _vsniprintf_r (struct _reent *, char *, size_t, const char *, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 4, 0)));
int _vsnprintf_r (struct _reent *, char *restrict, size_t, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 4, 0)));
int _vsprintf_r (struct _reent *, char *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__printf__, 3, 0)));
int _vsscanf_r (struct _reent *, const char *restrict, const char *restrict, __gnuc_va_list)
               __attribute__ ((__format__ (__scanf__, 3, 0)));



int fpurge (FILE *);
ssize_t __getdelim (char **, size_t *, int, FILE *);
ssize_t __getline (char **, size_t *, FILE *);


void clearerr_unlocked (FILE *);
int feof_unlocked (FILE *);
int ferror_unlocked (FILE *);
int fileno_unlocked (FILE *);
int fflush_unlocked (FILE *);
int fgetc_unlocked (FILE *);
int fputc_unlocked (int, FILE *);
size_t fread_unlocked (void *restrict, size_t _size, size_t _n, FILE *restrict);
size_t fwrite_unlocked (const void *restrict , size_t _size, size_t _n, FILE *);
# 577 "/usr/include/stdio.h" 3 4
int __srget_r (struct _reent *, FILE *);
int __swbuf_r (struct _reent *, int, FILE *);







FILE *funopen (const void *__cookie,
  int (*__readfn)(void *__c, char *__buf,
    size_t __n),
  int (*__writefn)(void *__c, const char *__buf,
     size_t __n),
  _fpos64_t (*__seekfn)(void *__c, _fpos64_t __off, int __whence),
  int (*__closefn)(void *__c));
FILE *_funopen_r (struct _reent *, const void *__cookie,
  int (*__readfn)(void *__c, char *__buf,
    size_t __n),
  int (*__writefn)(void *__c, const char *__buf,
     size_t __n),
  _fpos64_t (*__seekfn)(void *__c, _fpos64_t __off, int __whence),
  int (*__closefn)(void *__c));
# 667 "/usr/include/stdio.h" 3 4
static __inline__ int __sgetc_r(struct _reent *__ptr, FILE *__p);

static __inline__ int __sgetc_r(struct _reent *__ptr, FILE *__p)
  {
    int __c = (--(__p)->_r < 0 ? __srget_r(__ptr, __p) : (int)(*(__p)->_p++));
    if ((__p->_flags & 0x4000) && (__c == '\r'))
      {
      int __c2 = (--(__p)->_r < 0 ? __srget_r(__ptr, __p) : (int)(*(__p)->_p++));
      if (__c2 == '\n')
        __c = __c2;
      else
        ungetc(__c2, __p);
      }
    return __c;
  }





static __inline__ int __sputc_r(struct _reent *_ptr, int _c, FILE *_p) {

 if ((_p->_flags & 0x4000) && _c == '\n')
   __sputc_r (_ptr, '\r', _p);

 if (--_p->_w >= 0 || (_p->_w >= _p->_lbfsize && (char)_c != '\n'))
  return (*_p->_p++ = _c);
 else
  return (__swbuf_r(_ptr, _c, _p));
}
# 741 "/usr/include/stdio.h" 3 4
static __inline int
_getchar_unlocked(void)
{
 struct _reent *_ptr;

 _ptr = (__getreent());
 return (__sgetc_r(_ptr, ((_ptr)->_stdin)));
}

static __inline int
_putchar_unlocked(int _c)
{
 struct _reent *_ptr;

 _ptr = (__getreent());
 return (__sputc_r(_ptr, _c, ((_ptr)->_stdout)));
}
# 797 "/usr/include/stdio.h" 3 4

# 2 "net.c" 2
# 1 "/usr/include/unistd.h" 1 3 4



# 1 "/usr/include/sys/unistd.h" 1 3 4
# 14 "/usr/include/sys/unistd.h" 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 15 "/usr/include/sys/unistd.h" 2 3 4

extern char **environ;

void _exit (int __status) __attribute__ ((__noreturn__));

int access (const char *__path, int __amode);
unsigned alarm (unsigned __secs);
int chdir (const char *__path);
int chmod (const char *__path, mode_t __mode);

int chown (const char *__path, uid_t __owner, gid_t __group);


int chroot (const char *__path);

int close (int __fildes);

size_t confstr (int __name, char *__buf, size_t __len);
# 44 "/usr/include/sys/unistd.h" 3 4
int daemon (int nochdir, int noclose);

int dup (int __fildes);
int dup2 (int __fildes, int __fildes2);
# 56 "/usr/include/sys/unistd.h" 3 4
void endusershell (void);




int execl (const char *__path, const char *, ...);
int execle (const char *__path, const char *, ...);
int execlp (const char *__file, const char *, ...);

int execlpe (const char *__file, const char *, ...);

int execv (const char *__path, char * const __argv[]);
int execve (const char *__path, char * const __argv[], char * const __envp[]);
int execvp (const char *__file, char * const __argv[]);




int faccessat (int __dirfd, const char *__path, int __mode, int __flags);


int fchdir (int __fildes);


int fchmod (int __fildes, mode_t __mode);



int fchown (int __fildes, uid_t __owner, gid_t __group);



int fchownat (int __dirfd, const char *__path, uid_t __owner, gid_t __group, int __flags);


int fexecve (int __fd, char * const __argv[], char * const __envp[]);

pid_t fork (void);
long fpathconf (int __fd, int __name);
int fsync (int __fd);

int fdatasync (int __fd);




char * getcwd (char *__buf, size_t __size);

int getdomainname (char *__name, size_t __len);


int getentropy (void *, size_t);


gid_t getegid (void);
uid_t geteuid (void);
gid_t getgid (void);

int getgroups (int __gidsetsize, gid_t __grouplist[]);

long gethostid (void);

char * getlogin (void);

int getlogin_r (char *name, size_t namesize) ;


char * getpass (const char *__prompt);
int getpagesize (void);


int getpeereid (int, uid_t *, gid_t *);


pid_t getpgid (pid_t);

pid_t getpgrp (void);
pid_t getpid (void);
pid_t getppid (void);

pid_t getsid (pid_t);


uid_t getuid (void);


char * getusershell (void);


char * getwd (char *__buf);


int iruserok (unsigned long raddr, int superuser, const char *ruser, const char *luser);

int isatty (int __fildes);

int issetugid (void);



int lchown (const char *__path, uid_t __owner, gid_t __group);


int link (const char *__path1, const char *__path2);

int linkat (int __dirfd1, const char *__path1, int __dirfd2, const char *__path2, int __flags);


int nice (int __nice_value);


off_t lseek (int __fildes, off_t __offset, int __whence);






int lockf (int __fd, int __cmd, off_t __len);

long pathconf (const char *__path, int __name);
int pause (void);

int pthread_atfork (void (*)(void), void (*)(void), void (*)(void));

int pipe (int __fildes[2]);




ssize_t pread (int __fd, void *__buf, size_t __nbytes, off_t __offset);
ssize_t pwrite (int __fd, const void *__buf, size_t __nbytes, off_t __offset);

_ssize_t read (int __fd, void *__buf, size_t __nbyte);

int rresvport (int *__alport);
int revoke (char *__path);

int rmdir (const char *__path);

int ruserok (const char *rhost, int superuser, const char *ruser, const char *luser);


void * sbrk (ptrdiff_t __incr);



int setegid (gid_t __gid);
int seteuid (uid_t __uid);

int setgid (gid_t __gid);


int setgroups (int ngroups, const gid_t *grouplist);


int sethostname (const char *, size_t);

int setpgid (pid_t __pid, pid_t __pgid);

int setpgrp (void);


int setregid (gid_t __rgid, gid_t __egid);
int setreuid (uid_t __ruid, uid_t __euid);

pid_t setsid (void);

int setuid (uid_t __uid);


void setusershell (void);

unsigned sleep (unsigned int __seconds);



long sysconf (int __name);
pid_t tcgetpgrp (int __fildes);
int tcsetpgrp (int __fildes, pid_t __pgrp_id);
char * ttyname (int __fildes);
int ttyname_r (int, char *, size_t);
int unlink (const char *__path);

int usleep (useconds_t __useconds);


int vhangup (void);

_ssize_t write (int __fd, const void *__buf, size_t __nbyte);



# 1 "/usr/include/getopt.h" 1 3 4
# 45 "/usr/include/getopt.h" 3 4
extern int __attribute__((dllimport)) opterr;
extern int __attribute__((dllimport)) optind;
extern int __attribute__((dllimport)) optopt;
extern int __attribute__((dllimport)) optreset;
extern char __attribute__((dllimport)) *optarg;


int getopt (int, char * const *, const char *);
# 250 "/usr/include/sys/unistd.h" 2 3 4
# 259 "/usr/include/sys/unistd.h" 3 4
pid_t vfork (void);
# 283 "/usr/include/sys/unistd.h" 3 4
int ftruncate (int __fd, off_t __length);


int truncate (const char *, off_t __length);




int getdtablesize (void);


useconds_t ualarm (useconds_t __useconds, useconds_t __interval);





 int gethostname (char *__name, size_t __len);




int setdtablesize (int);



void sync (void);



ssize_t readlink (const char *restrict __path,
                          char *restrict __buf, size_t __buflen);
int symlink (const char *__name1, const char *__name2);


ssize_t readlinkat (int __dirfd1, const char *restrict __path,
                            char *restrict __buf, size_t __buflen);
int symlinkat (const char *, int, const char *);
int unlinkat (int, const char *, int);
# 5 "/usr/include/unistd.h" 2 3 4
# 3 "net.c" 2
# 1 "/usr/include/string.h" 1 3 4
# 17 "/usr/include/string.h" 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 18 "/usr/include/string.h" 2 3 4


# 1 "/usr/include/xlocale.h" 1 3 4
# 9 "/usr/include/xlocale.h" 3 4
struct __locale_t;
typedef struct __locale_t *locale_t;
# 21 "/usr/include/string.h" 2 3 4



# 1 "/usr/include/strings.h" 1 3 4
# 44 "/usr/include/strings.h" 3 4


int bcmp(const void *, const void *, size_t) __attribute__((__pure__));
void bcopy(const void *, void *, size_t);
void bzero(void *, size_t);


void explicit_bzero(void *, size_t);


int ffs(int) __attribute__((__const__));


int ffsl(long) __attribute__((__const__));
int ffsll(long long) __attribute__((__const__));
int fls(int) __attribute__((__const__));
int flsl(long) __attribute__((__const__));
int flsll(long long) __attribute__((__const__));


char *index(const char *, int) __attribute__((__pure__));
char *rindex(const char *, int) __attribute__((__pure__));

int strcasecmp(const char *, const char *) __attribute__((__pure__));
int strncasecmp(const char *, const char *, size_t) __attribute__((__pure__));


int strcasecmp_l (const char *, const char *, locale_t);
int strncasecmp_l (const char *, const char *, size_t, locale_t);


# 25 "/usr/include/string.h" 2 3 4




void * memchr (const void *, int, size_t);
int memcmp (const void *, const void *, size_t);
void * memcpy (void *restrict, const void *restrict, size_t);
void * memmove (void *, const void *, size_t);
void * memset (void *, int, size_t);
char *strcat (char *restrict, const char *restrict);
char *strchr (const char *, int);
int strcmp (const char *, const char *);
int strcoll (const char *, const char *);
char *strcpy (char *restrict, const char *restrict);
size_t strcspn (const char *, const char *);
char *strerror (int);
size_t strlen (const char *);
char *strncat (char *restrict, const char *restrict, size_t);
int strncmp (const char *, const char *, size_t);
char *strncpy (char *restrict, const char *restrict, size_t);
char *strpbrk (const char *, const char *);
char *strrchr (const char *, int);
size_t strspn (const char *, const char *);
char *strstr (const char *, const char *);

char *strtok (char *restrict, const char *restrict);

size_t strxfrm (char *restrict, const char *restrict, size_t);


int strcoll_l (const char *, const char *, locale_t);
char *strerror_l (int, locale_t);
size_t strxfrm_l (char *restrict, const char *restrict, size_t, locale_t);


char *strtok_r (char *restrict, const char *restrict, char **restrict);


int timingsafe_bcmp (const void *, const void *, size_t);
int timingsafe_memcmp (const void *, const void *, size_t);


void * memccpy (void *restrict, const void *restrict, int, size_t);
# 76 "/usr/include/string.h" 3 4
char *stpcpy (char *restrict, const char *restrict);
char *stpncpy (char *restrict, const char *restrict, size_t);






char *strdup (const char *);

char *_strdup_r (struct _reent *, const char *);

char *strndup (const char *, size_t);

char *_strndup_r (struct _reent *, const char *, size_t);
# 100 "/usr/include/string.h" 3 4
int strerror_r (int, char *, size_t)

             __asm__ ("" "__xpg_strerror_r")

  ;







char * _strerror_r (struct _reent *, int, int, int *);


size_t strlcat (char *, const char *, size_t);
size_t strlcpy (char *, const char *, size_t);


size_t strnlen (const char *, size_t);


char *strsep (char **, const char *);


char *strnstr(const char *, const char *, size_t) __attribute__((__pure__));



char *strlwr (char *);
char *strupr (char *);



char *strsignal (int __signo);



int strtosigno (const char *__name);
# 175 "/usr/include/string.h" 3 4
# 1 "/usr/include/sys/string.h" 1 3 4
# 176 "/usr/include/string.h" 2 3 4


# 4 "net.c" 2

# 1 "/usr/include/sys/socket.h" 1 3 4
# 12 "/usr/include/sys/socket.h" 3 4
# 1 "/usr/include/features.h" 1 3 4
# 13 "/usr/include/sys/socket.h" 2 3 4
# 1 "/usr/include/cygwin/socket.h" 1 3 4
# 20 "/usr/include/cygwin/socket.h" 3 4
typedef __socklen_t socklen_t;



typedef __sa_family_t sa_family_t;


struct sockaddr {
  sa_family_t sa_family;
  char sa_data[14];
};
# 39 "/usr/include/cygwin/socket.h" 3 4
struct sockaddr_storage {
  sa_family_t ss_family;
  char _ss_pad1[((sizeof (int64_t)) - sizeof (sa_family_t))];
  int64_t __ss_align;
  char _ss_pad2[(128 - (sizeof (sa_family_t) + ((sizeof (int64_t)) - sizeof (sa_family_t)) + (sizeof (int64_t))))];
};


# 1 "/usr/include/asm/socket.h" 1 3 4
# 12 "/usr/include/asm/socket.h" 3 4
# 1 "/usr/include/cygwin/if.h" 1 3 4
# 17 "/usr/include/cygwin/if.h" 3 4
# 1 "/usr/include/cygwin/socket.h" 1 3 4
# 18 "/usr/include/cygwin/if.h" 2 3 4
# 32 "/usr/include/cygwin/if.h" 3 4
struct if_nameindex {
  unsigned if_index;
  char *if_name;
};






struct ifreq_frndlyname {
  int ifrf_len;
  char ifrf_friendlyname[260];
};
# 57 "/usr/include/cygwin/if.h" 3 4
struct ifreq {
  union {
    char ifrn_name[44];
  } ifr_ifrn;

  union {
    struct sockaddr ifru_addr;
    struct sockaddr ifru_broadaddr;
    struct sockaddr ifru_dstaddr;
    struct sockaddr ifru_netmask;
    struct sockaddr ifru_hwaddr;
    int ifru_flags;
    int ifru_metric;
    int ifru_mtu;
    int ifru_ifindex;

    void *ifru_data;

    char __ifru_pad[28];
  } ifr_ifru;
};
# 99 "/usr/include/cygwin/if.h" 3 4
struct ifconf
{
  int ifc_len;
  union
  {
    caddr_t ifcu_buf;
    struct ifreq *ifcu_req;
  } ifc_ifcu;
};





extern unsigned if_nametoindex (const char *);
extern char *if_indextoname (unsigned, char *);
extern struct if_nameindex *if_nameindex (void);
extern void if_freenameindex (struct if_nameindex *);
# 13 "/usr/include/asm/socket.h" 2 3 4
# 48 "/usr/include/cygwin/socket.h" 2 3 4
# 1 "/usr/include/cygwin/sockios.h" 1 3 4
# 49 "/usr/include/cygwin/socket.h" 2 3 4
# 1 "/usr/include/sys/uio.h" 1 3 4
# 13 "/usr/include/sys/uio.h" 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 14 "/usr/include/sys/uio.h" 2 3 4











struct iovec
{
  void *iov_base;
  size_t iov_len;
};

extern ssize_t readv (int filedes, const struct iovec *vector, int count);
extern ssize_t writev (int filedes, const struct iovec *vector, int count);


# 50 "/usr/include/cygwin/socket.h" 2 3 4
# 1 "/usr/include/cygwin/_ucred.h" 1 3 4
# 14 "/usr/include/cygwin/_ucred.h" 3 4
struct ucred {
  pid_t pid;
  uid_t uid;
  gid_t gid;
};
# 51 "/usr/include/cygwin/socket.h" 2 3 4

struct linger {
  unsigned short l_onoff;
  unsigned short l_linger;
};

struct msghdr
{
  void * msg_name;
  socklen_t msg_namelen;
  struct iovec * msg_iov;
  int msg_iovlen;
  void * msg_control;
  socklen_t msg_controllen;
  int msg_flags;
};

struct cmsghdr
{



  size_t cmsg_len;
  int cmsg_level;
  int cmsg_type;
};
# 130 "/usr/include/cygwin/socket.h" 3 4
# 1 "/usr/include/cygwin/_socketflags.h" 1 3 4
# 131 "/usr/include/cygwin/socket.h" 2 3 4
# 14 "/usr/include/sys/socket.h" 2 3 4







  int accept (int, struct sockaddr *__peer, socklen_t *);
  int accept4 (int, struct sockaddr *__peer, socklen_t *, int flags);
  int bind (int, const struct sockaddr *__my_addr, socklen_t __addrlen);
  int connect (int, const struct sockaddr *, socklen_t);
  int getpeername (int, struct sockaddr *__peer, socklen_t *);
  int getsockname (int, struct sockaddr *__addr, socklen_t *);
  int listen (int, int __n);
  ssize_t recv (int, void *__buff, size_t __len, int __flags);
  ssize_t recvfrom (int, void *__buff, size_t __len, int __flags,
      struct sockaddr *__from, socklen_t *__fromlen);
  ssize_t recvmsg(int s, struct msghdr *msg, int flags);
  ssize_t send (int, const void *__buff, size_t __len, int __flags);
  ssize_t sendmsg(int s, const struct msghdr *msg, int flags);
  ssize_t sendto (int, const void *, size_t __len, int __flags,
    const struct sockaddr *__to, socklen_t __tolen);
  int setsockopt (int __s, int __level, int __optname, const void *optval,
    socklen_t __optlen);
  int getsockopt (int __s, int __level, int __optname, void *__optval,
    socklen_t *__optlen);
  int shutdown (int, int);
  int socket (int __family, int __type, int __protocol);
  int sockatmark (int __fd);
  int socketpair (int __domain, int __type, int __protocol, int __fds[2]);

  struct servent *getservbyname (const char *__name, const char *__proto);
# 6 "net.c" 2
# 1 "/usr/include/netinet/in.h" 1 3 4
# 12 "/usr/include/netinet/in.h" 3 4
# 1 "/usr/include/cygwin/in.h" 1 3 4
# 37 "/usr/include/cygwin/in.h" 3 4
enum
{
  IPPROTO_IP = 0,
  IPPROTO_HOPOPTS = 0,
  IPPROTO_ICMP = 1,
  IPPROTO_IGMP = 2,
  IPPROTO_IPIP = 4,
  IPPROTO_TCP = 6,
  IPPROTO_EGP = 8,
  IPPROTO_PUP = 12,
  IPPROTO_UDP = 17,
  IPPROTO_IDP = 22,
  IPPROTO_IPV6 = 41,
  IPPROTO_ROUTING = 43,
  IPPROTO_FRAGMENT = 44,
  IPPROTO_ESP = 50,
  IPPROTO_AH = 51,
  IPPROTO_ICMPV6 = 58,
  IPPROTO_NONE = 59,
  IPPROTO_DSTOPTS = 60,
  IPPROTO_RAW = 255,
  IPPROTO_MAX
};
# 83 "/usr/include/cygwin/in.h" 3 4
enum
{
  IPPORT_ECHO = 7,
  IPPORT_DISCARD = 9,
  IPPORT_SYSTAT = 11,
  IPPORT_DAYTIME = 13,
  IPPORT_NETSTAT = 15,
  IPPORT_FTP = 21,
  IPPORT_TELNET = 23,
  IPPORT_SMTP = 25,
  IPPORT_TIMESERVER = 37,
  IPPORT_NAMESERVER = 42,
  IPPORT_WHOIS = 43,
  IPPORT_MTP = 57,

  IPPORT_TFTP = 69,
  IPPORT_RJE = 77,
  IPPORT_FINGER = 79,
  IPPORT_TTYLINK = 87,
  IPPORT_SUPDUP = 95,


  IPPORT_EXECSERVER = 512,
  IPPORT_LOGINSERVER = 513,
  IPPORT_CMDSERVER = 514,
  IPPORT_EFSSERVER = 520,


  IPPORT_BIFFUDP = 512,
  IPPORT_WHOSERVER = 513,
  IPPORT_ROUTESERVER = 520,


  IPPORT_RESERVED = 1024,


  IPPORT_USERRESERVED = 5000
};




struct in_addr
{
  in_addr_t s_addr;
};





struct ip_mreq
{
  struct in_addr imr_multiaddr;
  struct in_addr imr_interface;
};

struct ip_mreq_source
{
  struct in_addr imr_multiaddr;
  struct in_addr imr_sourceaddr;
  struct in_addr imr_interface;
};

struct ip_msfilter
{
  struct in_addr imsf_multiaddr;
  struct in_addr imsf_interface;
  uint32_t imsf_fmode;
  uint32_t imsf_numsrc;
  struct in_addr imsf_slist[1];
};





struct in_pktinfo
{
  struct in_addr ipi_addr;
  uint32_t ipi_ifindex;
};



struct group_req
{
  uint32_t gr_interface;
  struct sockaddr_storage gr_group;
};

struct group_source_req
{
  uint32_t gsr_interface;
  struct sockaddr_storage gsr_group;
  struct sockaddr_storage gsr_source;
};

struct group_filter
{
  uint32_t gf_interface;
  struct sockaddr_storage gf_group;
  uint32_t gf_fmode;
  uint32_t gf_numsrc;
  struct sockaddr_storage gf_slist[1];
};







struct sockaddr_in
{
  sa_family_t sin_family;
  in_port_t sin_port;
  struct in_addr sin_addr;


  unsigned char __pad[16 - sizeof(short int)
   - sizeof(unsigned short int) - sizeof(struct in_addr)];
};
# 263 "/usr/include/cygwin/in.h" 3 4
# 1 "/usr/include/asm/byteorder.h" 1 3 4
# 12 "/usr/include/asm/byteorder.h" 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stdint.h" 1 3 4
# 9 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stdint.h" 3 4
# 1 "/usr/include/stdint.h" 1 3 4
# 13 "/usr/include/stdint.h" 3 4
# 1 "/usr/include/sys/_intsup.h" 1 3 4
# 35 "/usr/include/sys/_intsup.h" 3 4
       
       
       
       
       
       
       
# 187 "/usr/include/sys/_intsup.h" 3 4
       
       
       
       
       
       
       
# 14 "/usr/include/stdint.h" 2 3 4







typedef __int_least8_t int_least8_t;
typedef __uint_least8_t uint_least8_t;




typedef __int_least16_t int_least16_t;
typedef __uint_least16_t uint_least16_t;




typedef __int_least32_t int_least32_t;
typedef __uint_least32_t uint_least32_t;




typedef __int_least64_t int_least64_t;
typedef __uint_least64_t uint_least64_t;
# 51 "/usr/include/stdint.h" 3 4
  typedef signed char int_fast8_t;
  typedef unsigned char uint_fast8_t;
# 61 "/usr/include/stdint.h" 3 4
  typedef long int int_fast16_t;
  typedef long unsigned int uint_fast16_t;
# 71 "/usr/include/stdint.h" 3 4
  typedef long int int_fast32_t;
  typedef long unsigned int uint_fast32_t;
# 81 "/usr/include/stdint.h" 3 4
  typedef long int int_fast64_t;
  typedef long unsigned int uint_fast64_t;
# 10 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stdint.h" 2 3 4
# 13 "/usr/include/asm/byteorder.h" 2 3 4
# 23 "/usr/include/asm/byteorder.h" 3 4
extern uint32_t ntohl(uint32_t);
extern uint16_t ntohs(uint16_t);
extern uint32_t htonl(uint32_t);
extern uint16_t htons(uint16_t);
# 264 "/usr/include/cygwin/in.h" 2 3 4
# 274 "/usr/include/cygwin/in.h" 3 4
# 1 "/usr/include/cygwin/in6.h" 1 3 4
# 73 "/usr/include/cygwin/in6.h" 3 4
struct in6_addr
{
  union
    {
      uint8_t __s6_addr[16];
      uint16_t __s6_addr16[8];
      uint32_t __s6_addr32[4];
    } __u6;



};

struct ipv6_mreq
{
  struct in6_addr ipv6mr_multiaddr;
  uint32_t ipv6mr_interface;
};

struct ipv6_rt_hdr {
  uint8_t nexthdr;
  uint8_t hdrlen;
  uint8_t type;
  uint8_t segments_left;

};

struct in6_pktinfo
{
  struct in6_addr ipi6_addr;
  uint32_t ipi6_ifindex;
};





struct sockaddr_in6
{
  sa_family_t sin6_family;
  in_port_t sin6_port;
  uint32_t sin6_flowinfo;
  struct in6_addr sin6_addr;
  uint32_t sin6_scope_id;
};




extern const struct in6_addr in6addr_any;
extern const struct in6_addr in6addr_loopback;
# 275 "/usr/include/cygwin/in.h" 2 3 4
# 13 "/usr/include/netinet/in.h" 2 3 4







extern int bindresvport (int, struct sockaddr_in *);
extern int bindresvport_sa (int, struct sockaddr *);
# 7 "net.c" 2
# 1 "/usr/include/netdb.h" 1 3 4
# 66 "/usr/include/netdb.h" 3 4
# 1 "/usr/include/inttypes.h" 1 3 4
# 22 "/usr/include/inttypes.h" 3 4
# 1 "/usr/lib/gcc/x86_64-pc-cygwin/7.3.0/include/stddef.h" 1 3 4
# 23 "/usr/include/inttypes.h" 2 3 4
# 312 "/usr/include/inttypes.h" 3 4
typedef struct {
  intmax_t quot;
  intmax_t rem;
} imaxdiv_t;

struct _reent;





extern intmax_t imaxabs(intmax_t j);
extern imaxdiv_t imaxdiv(intmax_t numer, intmax_t denomer);
extern intmax_t strtoimax(const char *restrict, char **restrict, int);
extern intmax_t _strtoimax_r(struct _reent *, const char *restrict, char **restrict, int);
extern uintmax_t strtoumax(const char *restrict, char **restrict, int);
extern uintmax_t _strtoumax_r(struct _reent *, const char *restrict, char **restrict, int);
extern intmax_t wcstoimax(const wchar_t *restrict, wchar_t **restrict, int);
extern intmax_t _wcstoimax_r(struct _reent *, const wchar_t *restrict, wchar_t **restrict, int);
extern uintmax_t wcstoumax(const wchar_t *restrict, wchar_t **restrict, int);
extern uintmax_t _wcstoumax_r(struct _reent *, const wchar_t *restrict, wchar_t **restrict, int);


extern intmax_t strtoimax_l(const char *restrict, char **_restrict, int, locale_t);
extern uintmax_t strtoumax_l(const char *restrict, char **_restrict, int, locale_t);
extern intmax_t wcstoimax_l(const wchar_t *restrict, wchar_t **_restrict, int, locale_t);
extern uintmax_t wcstoumax_l(const wchar_t *restrict, wchar_t **_restrict, int, locale_t);
# 67 "/usr/include/netdb.h" 2 3 4
# 79 "/usr/include/netdb.h" 3 4
struct hostent {
 const char *h_name;
 char **h_aliases;
 short h_addrtype;
 short h_length;
 char **h_addr_list;

};






struct netent {
 char *n_name;
 char **n_aliases;
 short n_addrtype;
 uint32_t n_net;
};

struct servent {
 char *s_name;
 char **s_aliases;
 short s_port;
 char *s_proto;
};

struct protoent
{
  char *p_name;
  char **p_aliases;
  short p_proto;
};

struct rpcent {
 char *r_name;
 char **r_aliases;
 int r_number;
};


struct addrinfo {
  int ai_flags;
  int ai_family;
  int ai_socktype;
  int ai_protocol;
  socklen_t ai_addrlen;
  char *ai_canonname;
  struct sockaddr *ai_addr;
  struct addrinfo *ai_next;
};
# 143 "/usr/include/netdb.h" 3 4
extern __attribute__((dllimport)) int h_errno;
# 226 "/usr/include/netdb.h" 3 4
void endhostent (void);
void endnetent (void);
void endprotoent (void);
void endservent (void);
void endrpcent (void);
struct hostent *gethostbyaddr (const void *, socklen_t, int);
struct hostent *gethostbyname (const char *);

struct hostent *gethostbyname2 (const char *, int);

struct hostent *gethostent (void);
struct netent *getnetbyaddr (uint32_t, int);
struct netent *getnetbyname (const char *);
struct netent *getnetent (void);
struct protoent *getprotobyname (const char *);
struct protoent *getprotobynumber (int);
struct protoent *getprotoent (void);
struct servent *getservbyname (const char *, const char *);
struct servent *getservbyport (int, const char *);
struct servent *getservent (void);
struct rpcent *getrpcent (void);
struct rpcent *getrpcbyname (const char *);
struct rpcent *getrpcbynumber (int);

const char *hstrerror (int);
void herror (const char *);

void sethostent (int);
void setnetent (int);
void setprotoent (int);
void setservent (int);
void setrpcent (int);

void freeaddrinfo (struct addrinfo *);
const char *gai_strerror (int);
int getaddrinfo (const char *, const char *,
        const struct addrinfo *, struct addrinfo **);
int getnameinfo (const struct sockaddr *, socklen_t, char *,
        socklen_t, char *, socklen_t, int);



int rcmd (char **, uint16_t, const char *, const char *,
        const char *, int *);
int rcmd_af (char **, uint16_t, const char *, const char *,
    const char *, int *, int);
int rexec (char **, uint16_t rport, char *, char *, char *, int *);
int rresvport (int *);
int rresvport_af (int *, int);
int iruserok (unsigned long, int, const char *, const char *);
int iruserok_sa (const void *, int, int, const char *,
        const char *);
int ruserok (const char *, int, const char *, const char *);
# 8 "net.c" 2
# 1 "/usr/include/arpa/inet.h" 1 3 4
# 20 "/usr/include/arpa/inet.h" 3 4
in_addr_t inet_addr (const char *);
int inet_aton (const char *, struct in_addr *);
in_addr_t inet_lnaof (struct in_addr);
struct in_addr inet_makeaddr (unsigned long , unsigned long);
in_addr_t inet_netof (struct in_addr);
in_addr_t inet_network (const char *);
char *inet_ntoa (struct in_addr);
int inet_pton (int, const char *, void *);
const char *inet_ntop (int, const void *, char *, socklen_t);
# 9 "net.c" 2
# 1 "net.h" 1




# 4 "net.h"
void *get_in_addr(struct sockaddr *sa);
int get_listener_socket(char *port);
# 10 "net.c" 2
# 18 "net.c"
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == 
# 20 "net.c" 3 4
                        2
# 20 "net.c"
                               ) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}






int get_listener_socket(char *port)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int yes = 1;
    int rv;






    memset(&hints, 0, sizeof hints);
    hints.ai_family = 
# 45 "net.c" 3 4
                     0
# 45 "net.c"
                              ;
    hints.ai_socktype = 
# 46 "net.c" 3 4
                       1
# 46 "net.c"
                                  ;
    hints.ai_flags = 
# 47 "net.c" 3 4
                    0x1
# 47 "net.c"
                              ;

    if ((rv = getaddrinfo(
# 49 "net.c" 3 4
                         ((void *)0)
# 49 "net.c"
                             , port, &hints, &servinfo)) != 0) {
        fprintf(
# 50 "net.c" 3 4
               ((__getreent())->_stderr)
# 50 "net.c"
                     , "getaddrinfo: %s\n", gai_strerror(rv));
        return -1;
    }




    for(p = servinfo; p != 
# 57 "net.c" 3 4
                          ((void *)0)
# 57 "net.c"
                              ; p = p->ai_next) {


        if ((sockfd = socket(p->ai_family, p->ai_socktype,
            p->ai_protocol)) == -1) {

            continue;
        }



        if (setsockopt(sockfd, 
# 68 "net.c" 3 4
                              0xffff
# 68 "net.c"
                                        , 
# 68 "net.c" 3 4
                                          0x0004
# 68 "net.c"
                                                      , &yes,
            sizeof(int)) == -1) {
            perror("setsockopt");
            close(sockfd);
            freeaddrinfo(servinfo);
            return -2;
        }




        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);

            continue;
        }


        break;
    }

    freeaddrinfo(servinfo);



    if (p == 
# 93 "net.c" 3 4
            ((void *)0)
# 93 "net.c"
                ) {
        fprintf(
# 94 "net.c" 3 4
               ((__getreent())->_stderr)
# 94 "net.c"
                     , "webserver: failed to find local address\n");
        return -3;
    }



    if (listen(sockfd, 10) == -1) {

        close(sockfd);
        return -4;
    }

    return sockfd;
}
