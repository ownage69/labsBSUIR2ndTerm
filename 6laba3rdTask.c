// Найдите среди трех (4, 5) очередей две одинаковые. Решение в программе оформляйте через подпрограммы.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

typedef struct Node {
    char data;          
    struct Node* next;  
} Node;


typedef struct {
    Node* front;        
    Node* back;         
    int size;           
    int capacity;      
} CharQueue;

void initQueue(CharQueue* q, int capacity);
int isEmpty(CharQueue* q);
int isFull(CharQueue* q);
char front(const CharQueue* q);
char back(const CharQueue* q);
void enqueue(CharQueue* q, char value);
char dequeue(CharQueue* q);
void printQueue(CharQueue* q);
void freeQueue(CharQueue* q);
int getValidNaturalInput();
char queue_getCharacterValue();
void displayMenu();
void resizeQueue(CharQueue* q);
void inputIntoQueue(CharQueue* q, int queueIndex);
void printAllQueues(CharQueue* queues, int count);
int areQueuesEqual(CharQueue* q1, CharQueue* q2);
void compareAllQueues(CharQueue* queues, int count);
CharQueue* addNewQueue(CharQueue* queues, int* count);

void initQueue(CharQueue* q, int capacity) {
    q->front = NULL;
    q->back = NULL;
    q->size = 0;
    q->capacity = capacity;
}

int isEmpty(CharQueue* q) {
    return q->size == 0;
}

int isFull(CharQueue* q) {
    return q->size >= q->capacity;
}

char front(const CharQueue* q) {
    if (isEmpty(q)) {
        printf("Очередь пуста!\n");
        return '\0'; 
    }
    return q->front->data; 
}

char back(const CharQueue* q) {
    if (isEmpty(q)) {
        printf("Очередь пуста!\n");
        return '\0'; 
    }
    return q->back->data;
}

void enqueue(CharQueue* q, char value) {
    if (isFull(q)) {
        printf("Очередь переполнена!\n");
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node)); 
    newNode->data = value;
    newNode->next = NULL;

    if (q->back != NULL)
        q->back->next = newNode; // новый элемент - в конец очереди 
    else
        q->front = newNode; // пустая очередь - узел - первый и последний элемент     

    q->back = newNode; 
    q->size++;
}

char dequeue(CharQueue* q) {
    if (isEmpty(q)) {
        printf("Очередь пуста!\n");
        return '\0';
    }

    Node* temp = q->front;
    char value = temp->data; // Сохраняем указатель и значение первого элемента

    q->front = q->front->next;  // Сдвиг начала очереди
    if (q->front == NULL)
        q->back = NULL;

    free(temp);
    q->size--;
    return value;
}

void freeQueue(CharQueue* q) {
    while (!isEmpty(q)) {
        dequeue(q); 
    }
}

void printQueue(const CharQueue* q) {
    if (isEmpty((CharQueue*)q)) {
        printf("Очередь пуста!\n");
        return;
    }

    printf("Очередь: ");
    Node* current = q->front;
    while (current != NULL) {
        printf("%c", current->data);
        if (current->next != NULL)
            printf(" <- ");
        current = current->next;
    }
    printf("\n");
}

void displayMenu() {
    printf("\n---> МЕНЮ <---\n");
    printf("1. Изменить размер очереди\n");
    printf("2. Ввести элементы в очередь\n");
    printf("3. Вывести все очереди\n");
    printf("4. Найти одинаковые очереди\n");
    printf("5. Добавить новую очередь\n");
    printf("6. Выход\n");
    printf("Выберите действие: ");
}

void resizeQueue(CharQueue* q) {
    printf("Введите новый размер очереди: ");
    int newCap = getValidNaturalInput();

    while (q->size > newCap) {
        printf("Удалён элемент: %c\n", dequeue(q)); 
    }

    q->capacity = newCap; 
    printf("Новый размер установлен: %d\n", newCap);
}

// Условие 1 задания
void inputIntoQueue(CharQueue* q, int queueIndex) {
    freeQueue(q); // Очищаем очередь перед вводом

    printf("\nОчередь #%d (ввод останавливается при совпадении с последним элементом):\n", queueIndex);
    while (!isFull(q)) {
        printf("Введите символ (один): ");
        char ch = queue_getCharacterValue(); 
        if (!isEmpty(q) && ch == back(q)) { 
            printf("Совпадение с последним элементом! Первый элемент: %c\n", front(q));
            break;
        }
        enqueue(q, ch); 
        printf("Очередь: ");
        printQueue(q); 
        printf("\n");
    }

    if (isFull(q)) {
        printf("Очередь заполнена до максимального размера.\n");
    }
}

void printAllQueues(CharQueue* queues, int count) {
    if (count == 0) {
        printf("Очередей пока нет.\n");
        return;
    }

    for (int i = 0; i < count; ++i) {
        printf("Очередь #%d: ", i + 1);
        printQueue(&queues[i]); 
        printf(" [размер: %d/%d]\n", queues[i].size, queues[i].capacity);
    }
}

int areQueuesEqual(CharQueue* q1, CharQueue* q2) {
    if (q1->size != q2->size) return 0; // размеры отличаются - не равны

    // Указатели на начало
    Node* n1 = q1->front; 
    Node* n2 = q2->front;

    while (n1 && n2) {
        if (n1->data != n2->data)
            return 0;
        n1 = n1->next;
        n2 = n2->next;
    }

    return 1;
}

void compareAllQueues(CharQueue* queues, int count) {
    int found = 0;
        for (int i = 0; i < count - 1; ++i) { 
            for (int j = i + 1; j < count; ++j) {
            if (areQueuesEqual(&queues[i], &queues[j])) {
                printf("Очереди #%d и #%d одинаковы.\n", i + 1, j + 1);
                found = 1;
            }
        }
    }
    if (!found) {
        printf("Одинаковых очередей не найдено.\n");
    }
}

// Добавляет новую очередь в массив очередей
CharQueue* addNewQueue(CharQueue* queues, int* count) {
    if (*count >= 5) { 
        printf("Достигнуто максимальное количество очередей (5).\n");
        return queues;
    }

    printf("Введите размер новой очереди #%d: ", *count + 1);
    int capacity = getValidNaturalInput(); 

    CharQueue* newQueues = realloc(queues, (*count + 1) * sizeof(CharQueue));
    if (!newQueues) {
        printf("Ошибка выделения памяти!\n");
        return queues;
    }

    initQueue(&newQueues[*count], capacity);
    (*count)++; 
    printf("Добавлена новая очередь #%d с размером %d\n", *count, capacity);
    return newQueues;
}

int getValidNaturalInput() {
    int num;
    char buffer[100], symbol;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue;
        if (sscanf_s(buffer, "%d %c", &num, &symbol, sizeof(char)) == 1 && num > 0)
            return num; 
        printf("Ошибка! Введите корректное положительное число: ");
    }
}

char queue_getCharacterValue() {
    char input[100];
    while (1) {
        fgets(input, sizeof(input), stdin);
        if (strlen(input) == 2 && input[1] == '\n') {
            return input[0]; 
        }
        printf("Некорректный ввод. Введите один символ.\n");
        printf("Введите символ (один): ");
    }
}


int main() {
    setlocale(LC_ALL, "RUS"); 
    int choice;
    int queueCount = 0; 
    CharQueue* queues = NULL; 

    while (1) {
        displayMenu(); 
        choice = getValidNaturalInput(); 
        switch (choice) {
        case 1:
            if (queueCount == 0) {
                printf("Нет очередей для изменения.\n");
                break;
            }
            printf("Выберите номер очереди (1 - %d): ", queueCount);
            int queueIndex = getValidNaturalInput();
            if (queueIndex < 1 || queueIndex > queueCount) {
                printf("Неверный номер очереди.\n");
                break;
            }
            resizeQueue(&queues[queueIndex - 1]); 
            break;
        case 2:
            if (queueCount == 0) {
                printf("Нет очередей для ввода данных.\n");
                break;
            }
            printf("Выберите номер очереди (1 - %d): ", queueCount);
            queueIndex = getValidNaturalInput(); 
            if (queueIndex < 1 || queueIndex > queueCount) {
                printf("Неверный номер очереди.\n");
                break;
            }
            inputIntoQueue(&queues[queueIndex - 1], queueIndex);
            break;
        case 3:
            printAllQueues(queues, queueCount); 
            break;
        case 4:
            printAllQueues(queues, queueCount); 
            compareAllQueues(queues, queueCount); 
            break;
        case 5:
            queues = addNewQueue(queues, &queueCount); 
            break;
        case 6:
            for (int i = 0; i < queueCount; ++i)
                freeQueue(&queues[i]); 
            free(queues); 
            return 0;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
}
