#include "/home/msmariano/OpenWrt-SDK/package/CtrlRemServer/include/evento.h"


HANDLE * CreateEvent(char * str)
{

	HANDLE * ret = malloc(sizeof(HANDLE));
	sem_t sema;
	sem_init(&sema, 0, 1);
	ret->sema = sema;
	ret->val =1;
	ret->str = str;
	ret->bGo = true;
	return ret;
}
int WaitForEvent(HANDLE * id,int time)
{

	int ret = -1;
	while(true)
	{
		sem_wait(&id->sema);
		if(id->val == 0 || time == 0)
		{
			if(time == 0)
			{
				ret = 0;
			}
			else
				ret = 1;
			sem_post(&id->sema);
			break;
		}
		sem_post(&id->sema);
		if(time > -1)
		{
			time--;
			sleep(1);
		}
		usleep(1000);
	}

	return ret;
}

void SetEvent(HANDLE *  id)
{
	sem_wait(&id->sema);

	id->val = 0;

	sem_post(&id->sema);
}
