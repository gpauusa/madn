#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include "madn_threading.h"
#include "madn_socket_helper.h"

void* pt_nbf_thread(void* en)
{
    MADN_INSTANCE* env = (MADN_INSTANCE*) en;

    while(1)
    {
        nb_thread(env);
        //sleep(1); //sleep 2 secs
        usleep(env->globals->SNB_THREAD_INTERVAL);
        //sched_yield();
    }

    //never reaches here
    pthread_exit(NULL);
}

void* pt_bcn_thread(void* en)
{
    MADN_INSTANCE* env = (MADN_INSTANCE*) en;

    while(1)
    {
        beacon_thread(env);
        //sleep(1); //sleep 2 secs
        usleep(env->globals->BCN_THREAD_INTERVAL);
        //sched_yield();
    }

    //never reaches here
    pthread_exit(NULL);
}

void create_threads(MADN_INSTANCE* env)
{
    //this implementation spawns snb thread as a pthread as we need time
    //guarantee and runs the rest of the threads in a simple loop so as to
    //avoid locking issues

    pthread_t threads[2];
    
    int rc = pthread_create(&threads[0], NULL, pt_nbf_thread, (void *) env);
    if (rc)
    {
        printf("Error spawning thread. Exiting.\n");
        exit(-1);
    }
    rc = pthread_create(&threads[1], NULL, pt_bcn_thread, (void *) env);
    if (rc)
    {
        printf("Error spawning thread. Exiting.\n");
        exit(-1);
    }


    while (1)
    {
        receive_thread(env);

        leech_thread(env);
        forward_thread(env);
        service_thread(env);
    }

    //never reaches here
    pthread_exit(NULL);

}
