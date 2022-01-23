#pragma once
#define __LOG_FATAL    (1)
#define __LOG_ERR      (2)
#define __LOG_WARN     (3)
#define __LOG_INFO     (4)
#define __LOG_DBG      (5)

#define LOG(level,level_name, ...) do {  \
                            if (level <= debug_level) { \
                                fprintf(dbgstream,"%s %s:%d:", level_name, __FILE__, __LINE__); \
                                fprintf(dbgstream, __VA_ARGS__); \
                                fprintf(dbgstream, "\n"); \
                                fflush(dbgstream); \
                            } \
                        } while (0)

#define LOG_FATAL(...) LOG(__LOG_FATAL,"FATAL", __VA_ARGS__)
#define LOG_ERR(...) LOG(__LOG_ERR, "ERR", __VA_ARGS__)
#define LOG_WARN(...) LOG(__LOG_WARN, "WARN", __VA_ARGS__)
#define LOG_INFO(...) LOG(__LOG_INFO, "INFO", __VA_ARGS__)
#define LOG_DBG(...) LOG(__LOG_DBG, "DBG", __VA_ARGS__)
extern FILE *dbgstream;
extern int  debug_level;
