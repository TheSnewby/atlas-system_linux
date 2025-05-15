#include "list.h"
#include "multithreading.h"
#define NB_THREADS  8

static pthread_mutex_t task_mutex;

/**
 * init_task_mutex - constructor to init task_mutex
 */
void init_task_mutex(void)
{
	if (pthread_mutex_init(&task_mutex, NULL) == -1)
	{
		perror("task mutex init failed");
		exit(1);
	}
}

/**
 * destroy_task_mutex - destructor for task_mutex
 */
void destroy_task_mutex(void)
{
	if (pthread_mutex_destroy(&task_mutex) == -1)
	{
		perror("mutex destroy failed");
		exit(1);
	}
}

/**
 * create_task - creates a task struct
 * @entry: a pointer ot the entry function of the task
 * @param: the paramter that will be passed ot the entry function
 *
 * Return: task
 */
task_t *create_task(task_entry_t entry, void *param)
{
	task_t *new_task = (task_t *)malloc(sizeof(task_t));

	new_task->entry = entry;
	new_task->param = param;
	new_task->status = PENDING;
	new_task->result = NULL;
	new_task->lock = task_mutex;

	return (new_task);
}

/**
 * destroy_task - destroys a task
 * @task: task to be destroyed
 *
 * Return: void
 */
void destroy_task(task_t *task)
{
	if (!task)
		return;

	if (task->result) /* if task is completed */
	{
		list_destroy((list_t *)task->result, free);
		free((list_t *)task->result);
	}
	free(task);

	/* possibly more */
}

/**
 * exec_tasks - Execute task list
 * @tasks: doubly linked list of tasks?
 *
 * Return: void *
 */
void *exec_tasks(list_t const *tasks)
{
	size_t i = 0;
	node_t *node = NULL;
	task_t *task = NULL;

	if (!tasks)
		return (NULL);

	for (node = tasks->head; node; node = node->next, i++)
	{
		task = node->content;

		pthread_mutex_lock(&task_mutex);
		if (task->status == PENDING)
		{
			task->status = STARTED;
			pthread_mutex_unlock(&task_mutex);
			tprintf("[%02lu] Started\n", i);
			task->result = task->entry(task->param);

			if (task->result)
				task->status = SUCCESS;
			else
				task->status = FAILURE;
			tprintf("[%02lu] %s\n", i, task->result ? "Success" : "Failure");
		}
		else
			pthread_mutex_unlock(&task_mutex);
	}
	return (NULL);
}
