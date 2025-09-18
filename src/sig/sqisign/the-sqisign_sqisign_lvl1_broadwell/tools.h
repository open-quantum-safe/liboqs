
#ifndef TOOLS_H
#define TOOLS_H

#include <time.h>

// Debug printing:
// https://stackoverflow.com/questions/1644868/define-macro-for-debug-printing-in-c
#ifndef NDEBUG
#define DEBUG_PRINT 1
#else
#define DEBUG_PRINT 0
#endif

#ifndef __FILE_NAME__
#define __FILE_NAME__ "NA"
#endif

#ifndef __LINE__
#define __LINE__ 0
#endif

#ifndef __func__
#define __func__ "NA"
#endif

#define debug_print(fmt)                                                                           \
    do {                                                                                           \
        if (DEBUG_PRINT)                                                                           \
            printf("warning: %s, file %s, line %d, function %s().\n",                              \
                   fmt,                                                                            \
                   __FILE_NAME__,                                                                  \
                   __LINE__,                                                                       \
                   __func__);                                                                      \
    } while (0)


clock_t tic(void);
float tac(void);                             /* time in ms since last tic */
float TAC(const char *str);                  /* same, but prints it with label 'str' */
float toc(const clock_t t);                  /* time in ms since t */
float TOC(const clock_t t, const char *str); /* same, but prints it with label 'str' */
float TOC_clock(const clock_t t, const char *str);

clock_t dclock(const clock_t t); // return the clock cycle diff between now and t
float clock_to_time(const clock_t t,
                    const char *str); // convert the number of clock cycles t to time
float clock_print(const clock_t t, const char *str);
#endif
