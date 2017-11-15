#ifndef __BUSY_THREADED__
#define __BUSY_THREADED__

#include <pthread.h>

struct thread_arg
{
    void* (*task)(void*);
    void* data;
};

pthread_mutex_t busyvar_mutex;

class busy_threaded
{
    pthread_t* active_thread;

protected:
    busy_threaded() {
        //once
        pthread_mutex_init(&busyvar_mutex, NULL);
        active_thread = NULL;
    }
    ~busy_threaded() {
        pthread_mutex_destroy(&busyvar_mutex);
    }

    static int is_busy;
    void thread_spawn(bool insist, void* task(void*), void* data)
    {
        printf("check lock...");
        if (pthread_mutex_trylock(&busyvar_mutex)) {
            printf("busy\n");
            if (!is_busy) {
                printf("but not busy\n");
                //pthread_detach(*active_thread);
            } else if (insist) {
                printf("killing render thread\n");
                pthread_cancel(*active_thread);
            } else {
                printf("already rendering\n");
                return;
            }
            delete active_thread;
        }
        is_busy = 1;
        active_thread = new pthread_t;
        thread_arg* arg = new thread_arg;
        arg->task = task;
        arg->data = data;
        pthread_create(active_thread, NULL, busy_wrapper, arg);
        thread_unlock();
    }

    static void* busy_wrapper(void* arg)
    {
    thread_lock();
    printf("rendering\n");
        thread_arg* t_arg = (thread_arg *)arg;

        t_arg->task(t_arg->data);

        delete t_arg;

    printf("done rendering\n");
    is_busy = 0;
        thread_unlock();
        pthread_exit(0);
    }

    static void thread_lock() {
        pthread_mutex_lock(&busyvar_mutex);
    }

    static void thread_unlock() {
        pthread_mutex_unlock(&busyvar_mutex);
    }
};

int busy_threaded::is_busy = 0;

#endif
