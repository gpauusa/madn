#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include "madn_threading.h"
#include "madn_socket_helper.h"
#include "madn_register_local.h"

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

void testfunc1(MADN_DATAID id)
{
    printf("Recd data!!!\n");
}

void* pt_test_thread(void* en)
{
    MADN_INSTANCE* env = (MADN_INSTANCE*) en;

    //while(1)
    //{
        sleep(5); //sleep 2 secs
        //sched_yield();
        MADN_DATAID id;
        id.chunk = 5;
        id.stripe = 15;
        id.stripe = 35;

        madn_register_receiver(env, id, 5, &testfunc1);
        sleep(10);
        madn_register_receiver(env, id, 5, &testfunc1);
        printf("Second leech\n");


    //}

    pthread_exit(NULL);
}

void* pt_test1_thread(void* en)
{
    MADN_INSTANCE* env = (MADN_INSTANCE*) en;

    //while(1)
    //{
        sleep(2); //sleep 2 secs
        //sched_yield();
        MADN_DATAID id;
        id.chunk = 5;
        id.stripe = 15;
        id.stripe = 35;

        madn_register_server(env, id);

    //}

    //never reaches here
    pthread_exit(NULL);
}

//void* pt_local_req_thread(void* en)
//{
//    MADN_INSTANCE* env = (MADN_INSTANCE*) en;
//    while(1)
//    {
//        local_req_thread(env);
//        sleep(5);
//    }
//
//}

void create_threads(MADN_INSTANCE* env)
{
    //this implementation spawns snb thread as a pthread as we need time
    //guarantee and runs the rest of the threads in a simple loop so as to
    //avoid locking issues

    pthread_t threads[3];
    
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
    //rc = pthread_create(&threads[2], NULL, pt_local_req_thread, (void *) env);
    //if (rc)
    //{
    //    printf("Error spawning thread. Exiting.\n");
    //    exit(-1);
    //}
    if (env->globals->NODE_ID == 5)
    {
        rc = pthread_create(&threads[2], NULL, pt_test_thread, (void *) env);
        if (rc)
        {
            printf("Error spawning thread. Exiting.\n");
            exit(-1);
        }
    }
    if (env->globals->NODE_ID == 29)
    {
        rc = pthread_create(&threads[2], NULL, pt_test1_thread, (void *) env);
        if (rc)
        {
            printf("Error spawning thread. Exiting.\n");
            exit(-1);
        }
    }

    while (1)
    {
        receive_thread(env);

        leech_thread(env);
        forward_thread(env);
        service_thread(env);
        usleep(50);
    }

    //never reaches here
    pthread_exit(NULL);

}
