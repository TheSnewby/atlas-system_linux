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
		list_destroy(task->result, free);
		free(task->result);
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
	size_t i;
	node_t *tmp_node = NULL;
	task_t *tmp_task = NULL;
	char task_result[10];

	if (!tasks)
		return (NULL);

	for (tmp_node = tasks->head; i < tasks->size; tmp_node = tmp_node->next, i++)
	{
		tmp_task = tmp_node->content;

		pthread_mutex_lock(&task_mutex);
		if (tmp_task->status == PENDING)
		{
			tmp_task->status = STARTED;
			tprintf("[%lu] Started\n", i);
			tmp_task->result = tmp_task->entry(tmp_task->param);

			if (tmp_task->result)
				tmp_task->status = SUCCESS;
			else
				tmp_task->status = FAILURE;
			sprintf(task_result, "%s\n", tmp_task->result ? "Success" : "Failure");
			tprintf("%lu %s\n", i, task_result);
		}
		else
			pthread_mutex_unlock(&task_mutex);
	}
	return (NULL);
}





/**
 * print_task_result - Print the result of a task
 *
 * @task: Pointer to the task to print the result of
 */
void print_task_result(task_t *task)
{

    list_t *factors = (list_t *)task->result;
    node_t *factor;

    printf("%s =", (char *)task->param);
    for (factor = factors->head; factor; factor = factor->next)
    {
        unsigned long n = *((unsigned long *)factor->content);

        printf("%s %lu", factor->prev ? " *" : "", n);
    }
    printf("\n");
}

/**
 * main - Entry point
 *
 * @ac: Arguments count
 * @av: Arguments vector
 *
 * Return: EXIT_SUCCESS upon success, EXIT_FAILURE otherwise
 */
int main(int ac, char **av)
{
    list_t tasks;
    pthread_t threads[NB_THREADS];
    size_t i;

    if (ac < 2)
    {
        fprintf(stderr, "Usage: %s n\n", av[0]);
        return (EXIT_FAILURE);
    }

    if (!list_init(&tasks))
        return (EXIT_FAILURE);

    for (i = 1; i < (size_t)ac; i++)
        list_add(&tasks,
            create_task((task_entry_t)prime_factors, av[i]));

    printf("Executing %lu tasks on %u threads\n", tasks.size, NB_THREADS);

    for (i = 0; i < NB_THREADS; i++)
        pthread_create(&threads[i], NULL,
            (void *(*) (void *))exec_tasks, &tasks);
    for (i = 0; i < NB_THREADS; i++)
        pthread_join(threads[i], NULL);

    list_each(&tasks, (node_func_t)print_task_result);

    list_destroy(&tasks, (node_func_t)destroy_task);

    return (EXIT_SUCCESS);
}