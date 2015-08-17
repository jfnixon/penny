#ifndef __dbg_h__
#define __dbg_h__

#ifdef NDEBUG
#define log_debug(M, ...)
#else
#define log_debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define errno_msg() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, errno_msg(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, errno_msg(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define log_check(A, M, ...) do { if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; } } while (0)

#define log_check_debug(A, M, ...) do { if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; } } while (0)

#endif
