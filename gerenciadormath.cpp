#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100
#define TASK_NAME_LEN 256
#define DATE_LEN 11 
#define TIME_LEN 6  
#define FILE_NAME "tasks.txt"

typedef struct {
    char name[TASK_NAME_LEN];
    char due_date[DATE_LEN];
    int is_completed;
} Task;

typedef struct {
    char name[TASK_NAME_LEN];
    char due_time[TIME_LEN];
    int is_completed;
    int interval_days; 
    int specific_days[7]; 
} DailyTask;

void display_time() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    printf("\nData e Hora Atual: %02d-%02d-%04d %02d:%02d:%02d\n",
           t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
           t->tm_hour, t->tm_min, t->tm_sec);
}

void add_task(Task tasks[], int *task_count) {
    if (*task_count >= MAX_TASKS) {
        printf("\nLimite maximo de tarefas atingido.\n");
        return;
    }
    printf("\nDigite o nome da tarefa: ");
    getchar(); 
    fgets(tasks[*task_count].name, TASK_NAME_LEN, stdin);
    tasks[*task_count].name[strcspn(tasks[*task_count].name, "\n")] = 0;

    printf("\nDigite a data de vencimento (dd-mm-aaaa): ");
    fgets(tasks[*task_count].due_date, DATE_LEN, stdin);
    tasks[*task_count].due_date[strcspn(tasks[*task_count].due_date, "\n")] = 0;

    tasks[*task_count].is_completed = 0;
    (*task_count)++;
    printf("Tarefa adicionada com sucesso.\n");
}

void add_daily_task(DailyTask daily_tasks[], int *daily_task_count) {
    if (*daily_task_count >= MAX_TASKS) {
        printf("Limite maximo de tarefas diarias atingido.\n");
        return;
    }
    printf("\nDigite o nome da tarefa diaria: ");
    getchar(); 
    fgets(daily_tasks[*daily_task_count].name, TASK_NAME_LEN, stdin);
    daily_tasks[*daily_task_count].name[strcspn(daily_tasks[*daily_task_count].name, "\n")] = 0;

    printf("\nDigite o horario limite para conclusao (hh:mm): ");
    fgets(daily_tasks[*daily_task_count].due_time, TIME_LEN, stdin);
    daily_tasks[*daily_task_count].due_time[strcspn(daily_tasks[*daily_task_count].due_time, "\n")] = 0;

    printf("\nDeseja ser lembrado em intervalos de dias ou em dias especificos da semana?");
    printf("\n1. Intervalos de dias");
    printf("\n2. Dias especificos da semana");
    int reminder_option;
    scanf("%d", &reminder_option);

    if (reminder_option == 1) {
        printf("\nDigite o intervalo em dias: ");
        scanf("%d", &daily_tasks[*daily_task_count].interval_days);
        for (int i = 0; i < 7; i++) {
            daily_tasks[*daily_task_count].specific_days[i] = 0;
        }
    } else if (reminder_option == 2) {
        daily_tasks[*daily_task_count].interval_days = 0;
        printf("\nDigite 1 para lembrar e 0 para nao lembrar nos seguintes dias:");
        printf(" Domingo: ");
        scanf("%d", &daily_tasks[*daily_task_count].specific_days[0]);
        printf("Segunda-feira: ");
        scanf("%d", &daily_tasks[*daily_task_count].specific_days[1]);
        printf("Terça-feira: ");
        scanf("%d", &daily_tasks[*daily_task_count].specific_days[2]);
        printf("Quarta-feira: ");
        scanf("%d", &daily_tasks[*daily_task_count].specific_days[3]);
        printf("Quinta-feira: ");
        scanf("%d", &daily_tasks[*daily_task_count].specific_days[4]);
        printf("Sexta-feira: ");
        scanf("%d", &daily_tasks[*daily_task_count].specific_days[5]);
        printf("Sabado: ");
        scanf("%d", &daily_tasks[*daily_task_count].specific_days[6]);
    } else {
        printf("\nOpcao invalida. Nao sera configurado lembrete especifico.");
        daily_tasks[*daily_task_count].interval_days = 0;
        for (int i = 0; i < 7; i++) {
            daily_tasks[*daily_task_count].specific_days[i] = 0;
        }
    }

    daily_tasks[*daily_task_count].is_completed = 0;
    (*daily_task_count)++;
    printf("\nTarefa diaria adicionada com sucesso.");
}

void list_tasks(Task tasks[], int task_count) {
    if (task_count == 0) {
        printf("\nNenhuma tarefa encontrada.");
        return;
    }
    for (int i = 0; i < task_count; i++) {
        printf("\n%d. %s [%s] - Vencimento: %s", i + 1, tasks[i].name,
               tasks[i].is_completed ? "Concluida" : "Pendente",
               tasks[i].due_date);
    }
}

void list_daily_tasks(DailyTask daily_tasks[], int daily_task_count) {
    if (daily_task_count == 0) {
        printf("\nNenhuma tarefa diaria encontrada.");
        return;
    }
    for (int i = 0; i < daily_task_count; i++) {
        printf("\n%d. %s [%s] - Hora Limite: %s", i + 1, daily_tasks[i].name,
               daily_tasks[i].is_completed ? "Concluida" : "Pendente",
               daily_tasks[i].due_time);
    }
}

void remove_task(Task tasks[], int *task_count) {
    if (*task_count == 0) {
        printf("Nenhuma tarefa disponivel para remocao.\n");
        return;
    }

    printf("Escolha o numero da tarefa a ser removida: ");
    int task_number;
    scanf("%d", &task_number);
    if (task_number < 1 || task_number > *task_count) {
        printf("Numero de tarefa invalido.\n");
        return;
    }

    for (int i = task_number - 1; i < *task_count - 1; i++) {
        strcpy(tasks[i].name, tasks[i + 1].name);
        strcpy(tasks[i].due_date, tasks[i + 1].due_date);
                tasks[i].is_completed = tasks[i + 1].is_completed;
    }

    (*task_count)--;
    printf("Tarefa removida com sucesso.\n");
}

void remove_daily_task(DailyTask daily_tasks[], int *daily_task_count) {
    if (*daily_task_count == 0) {
        printf("Nenhuma tarefa diaria disponivel para remocao.\n");
        return;
    }

    printf("Escolha o numero da tarefa diaria a ser removida: ");
    int task_number;
    scanf("%d", &task_number);
    if (task_number < 1 || task_number > *daily_task_count) {
        printf("Numero de tarefa diaria invalido.\n");
        return;
    }

    for (int i = task_number - 1; i < *daily_task_count - 1; i++) {
        strcpy(daily_tasks[i].name, daily_tasks[i + 1].name);
        strcpy(daily_tasks[i].due_time, daily_tasks[i + 1].due_time);
        daily_tasks[i].is_completed = daily_tasks[i + 1].is_completed;
        daily_tasks[i].interval_days = daily_tasks[i + 1].interval_days;
        for (int j = 0; j < 7; j++) {
            daily_tasks[i].specific_days[j] = daily_tasks[i + 1].specific_days[j];
        }
    }

    (*daily_task_count)--;
    printf("Tarefa diaria removida com	 sucesso.\n");
}

void complete_task(Task tasks[], int task_count) {
    if (task_count == 0) {
        printf("Nenhuma tarefa disponivel para conclusao.\n");
        return;
    }

    printf("Escolha o numero da tarefa a ser concluida: ");
    int task_number;
    scanf("%d", &task_number);
    if (task_number < 1 || task_number > task_count) {
        printf("Numero de tarefa invalido.\n");
        return;
    }

    tasks[task_number - 1].is_completed = 1;
    printf("Tarefa concluida com sucesso.\n");
}

void complete_daily_task(DailyTask daily_tasks[], int daily_task_count) {
    if (daily_task_count == 0) {
        printf("Nenhuma tarefa diaria disponivel para conclusao.\n");
        return;
    }

    printf("Escolha o numero da tarefa diaria a ser concluida: ");
    int task_number;
    scanf("%d", &task_number);
    if (task_number < 1 || task_number > daily_task_count) {
        printf("Numero de tarefa diaria invalido.\n");
        return;
    }

    daily_tasks[task_number - 1].is_completed = 1;
    printf("Tarefa diaria concluida com sucesso.\n");
}

void task_report(Task tasks[], int task_count) {
    if (task_count == 0) {
        printf("Nenhuma tarefa disponivel para relatorio.\n");
        return;
    }

    printf("\nRelatorio de Tarefas:\n");
    printf("------------------------------\n");
    printf("Tarefas Pendentes:\n");
    for (int i = 0; i < task_count; i++) {
        if (!tasks[i].is_completed)
            printf("%s - Vencimento: %s\n", tasks[i].name, tasks[i].due_date);
    }

    printf("\nTarefas Concluidas:\n");
    for (int i = 0; i < task_count; i++) {
        if (tasks[i].is_completed)
            printf("%s - Vencimento: %s\n", tasks[i].name, tasks[i].due_date);
    }
}

void daily_task_report(DailyTask daily_tasks[], int daily_task_count) {
    if (daily_task_count == 0) {
        printf("Nenhuma tarefa diaria disponivel para relatorio.\n");
        return;
    }

    printf("\nRelatorio de Tarefas Diarias:\n");
    printf("------------------------------\n");
    printf("Tarefas Pendentes:\n");
    for (int i = 0; i < daily_task_count; i++) {
        if (!daily_tasks[i].is_completed)
            printf("%s - Hora Limite: %s\n", daily_tasks[i].name, daily_tasks[i].due_time);
    }

    printf("\nTarefas Concluidas:\n");
    for (int i = 0; i < daily_task_count; i++) {
        if (daily_tasks[i].is_completed)
            printf("%s - Hora Limite: %s\n", daily_tasks[i].name, daily_tasks[i].due_time);
    }
}

int main() {
    Task tasks[MAX_TASKS];
    DailyTask daily_tasks[MAX_TASKS];
    int task_count = 0;
    int daily_task_count = 0;
    int choice;

    while (1) {
        printf("\n");
        printf("1. Adicionar Tarefa\n");
        printf("2. Adicionar Tarefa Diaria\n");
        printf("3. Listar Tarefas\n");
        printf("4. Listar Tarefas Diarias\n");
        printf("5. Remover Tarefa\n");
        printf("6. Remover Tarefa Diaria\n");
        printf("7. Concluir Tarefa\n");
        printf("8. Concluir Tarefa Diaria\n");
        printf("9. Relatorio de Tarefas\n");
        printf("10. Relatorio de Tarefas Diarias\n");
        printf("11. Sair\n");
        printf("\nEscolha uma opcao: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                add_task(tasks, &task_count);
                break;
            case 2:
                add_daily_task(daily_tasks, &daily_task_count);
                break;
            case 3:
                list_tasks(tasks, task_count);
                break;
            case 4:
                list_daily_tasks(daily_tasks, daily_task_count);
                break;
            case 5:
                remove_task(tasks, &task_count);
                break;
            case 6:
                remove_daily_task(daily_tasks, &daily_task_count);
                break;
            case 7:
                complete_task(tasks, task_count);
                break;
            case 8:
                complete_daily_task(daily_tasks, daily_task_count);
                break;
            case 9:
                task_report(tasks, task_count);
                break;
            case 10:
                daily_task_report(daily_tasks, daily_task_count);
                break;
            case 11:
                printf("Saindo...\n");
                exit(0);
            default:
                printf("Opcao invalida.\n");
        }
    }

    return 0;
}

            


