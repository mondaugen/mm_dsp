#ifndef TEST_H
#define TEST_H 

#define SHOW_TEST_START(msg) \
   printf("Doing test: %s... ",msg) 

#define SHOW_TEST_RESULT(res) \
    if (res) { \
        printf("passed\n"); \
    } else { \
        printf("failed\n"); \
    }

#define FABS(x) (((x) < 0) ? (-(x)) : (x)) 

#endif /* TEST_H */
