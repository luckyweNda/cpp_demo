#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

void *Thread(void* args);

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, Thread, NULL);
    cout << "Enter a number." << endl;
    int i = 1;
    
    while(true)
    {
        cin >> i;
        
        if(i == 0)
        {
            break;
        }

        pthread_create(&thread, NULL, Thread, (void*)&i);
    }

    pthread_exit(NULL);

    return 0;
}

void *Thread(void* args)
{
    if(args != NULL)
    {
        int i = *((int*)args);
        cout << "You have input a number: " << i << endl;
    }
    else
    {
        for(;;)
        {
            cout << "Sleep(4)" << endl;
            sleep(4);            
        }
    }

    return 0;
}