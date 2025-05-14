#include "list.h"
#include "multithreading.h"
#define NB_THREADS  8

task_t *create_task(task_entry_t entry, void *param);
void destroy_task(task_t *task);
void *exec_tasks(list_t const *tasks);

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
	// new_task->result = 
	// new_task->lock = 

	return (new_task);
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