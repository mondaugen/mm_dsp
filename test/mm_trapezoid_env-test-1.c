#include <stdio.h> 
#include "mm_trapezoid_env.h"

int main(int argc, char **argv)
{
    int i;
    MMSample inc = 0.4;
    MMSample attackTime = 2.0;
    MMSample decayTime = 3.0;
    MMSample sustainTime = 1.0;
    MMTrapezoidEnv env;
    MMTrapezoidEnv_init(&env,-1,1,attackTime,decayTime);
    
    printf("%f\n",MMEnvelope_getCurValue(&env));
    MMEnvelope_incTime(&env,inc);
    printf("%f\n",MMEnvelope_getCurValue(&env));
    printf("State: %d\n", env.state);
    MMEnvelope_startAttack(&env);
    for (i = 0; i < (int)(attackTime / inc) + 1; i++) {
        MMEnvelope_incTime(&env,inc);
        printf("State: %d\n", env.state);
        printf("%f\n",MMEnvelope_getCurValue(&env));
    }
    printf("State: %d\n", env.state);
    for (i = 0; i < (int)(sustainTime / inc) + 1; i++) {
        MMEnvelope_incTime(&env,inc);
        printf("State: %d\n", env.state);
        printf("%f\n",MMEnvelope_getCurValue(&env));
    }
    MMEnvelope_startRelease(&env);
    for (i = 0; i < (int)(decayTime / inc) + 1; i++) {
        MMEnvelope_incTime(&env,inc);
        printf("State: %d\n", env.state);
        printf("%f\n",MMEnvelope_getCurValue(&env));
    }
    printf("State: %d\n", env.state);
    MMEnvelope_reset(&env);
    printf("State: %d\n", env.state);
    printf("%f\n",MMEnvelope_getCurValue(&env));
    MMEnvelope_incTime(&env,inc);
    printf("%f\n",MMEnvelope_getCurValue(&env));
    printf("State: %d\n", env.state);
    MMEnvelope_startAttack(&env);
    for (i = 0; i < (int)(attackTime / inc) + 1; i++) {
        MMEnvelope_incTime(&env,inc);
        printf("State: %d\n", env.state);
        printf("%f\n",MMEnvelope_getCurValue(&env));
    }
    printf("State: %d\n", env.state);
    for (i = 0; i < (int)(sustainTime / inc) + 1; i++) {
        MMEnvelope_incTime(&env,inc);
        printf("State: %d\n", env.state);
        printf("%f\n",MMEnvelope_getCurValue(&env));
    }
    MMEnvelope_startRelease(&env);
    for (i = 0; i < (int)(decayTime / inc) + 1; i++) {
        MMEnvelope_incTime(&env,inc);
        printf("State: %d\n", env.state);
        printf("%f\n",MMEnvelope_getCurValue(&env));
    }
    printf("State: %d\n", env.state);
    return(0);
}

    
