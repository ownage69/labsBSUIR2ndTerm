// Создать очередь для символов. Максимальный размер очереди вводится с экрана. Создать функции для ввода и вывода элементов очереди. Вводить символы с экрана в очередь. В случае совпадения вводимого символа с последним элементом очереди выводить первый элемент очереди

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

// Структура узла 
typedef struct Node {
    char data;          
    struct Node* next;  
} Node;

// Структура очереди
typedef struct {
    Node* front;        
    Node* back;         
    int size;           
    int capacity;      
} CharQueue;

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

// Добавление элемента (**enqueue**)
void enqueue(CharQueue* q, char value) {
    if (isFull(q)) {
        return; 
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Ошибка выделения памяти.\n");
        exit(1); 
    }

    newNode->data = value;  
    newNode->next = NULL;   

    if (q->back != NULL) 
        q->back->next = newNode; // новый элемент - в конец очереди 
    else
        q->front = newNode; // пустая очередь - узел - первый и последний элемент     

    q->back = newNode; 
    q->size++;
}

// Удаление символа (**dequeue**)
char dequeue(CharQueue* q) {
    if (isEmpty(q)) {
        printf("Очередь пуста!\n");
        return '\0'; 
    }

    Node* temp = q->front; 
    char value = temp->data; // Сохраняем указатель и значение первого элемента

    q->front = q->front->next;  // Сдвиг начала очереди
    if (q->front == NULL)
        q->back = NULL; // Пустая очередь -> обнуление back 

    free(temp);                 
    q->size--;                  
    return value; 
}

// Просмотр первого элемента (**front**)
char front(const CharQueue* q) {
    if (isEmpty(q)) {
        printf("Очередь пуста!\n");
        return '\0'; 
    }
    return q->front->data; 
}

// Просмотр последнего элемента (**back**)
char back(const CharQueue* q) {
    if (isEmpty(q)) {
        printf("Очередь пуста!\n");
        return '\0'; 
    }
    return q->back->data; 
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


void freeQueue(CharQueue* q) {
    while (!isEmpty(q)) {
        dequeue(q); 
    }
}

int getValidNaturalInput() {
    int num;
    char buffer[100], symbol;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue; 
        if (sscanf_s(buffer, "%d %c", &num, &symbol, 1) == 1 && num > 0) return num;
        printf("Ошибка! Введите корректное целое число: ");
    }
}

char getCharacterValue() {
    char input[100];
    char ch;
    while (1) {
        printf("Введите символ (один): ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            if (sscanf_s(input, "%c", &ch, 1) == 1 && input[1] == '\n' && input[0] != '\n') {
                return ch; 
            }
        }
        printf("Некорректное значение. Попробуйте ещё раз!\n");
    }
}

void displayMenu() {
    printf("\nМеню:\n");
    printf("1. Изменить размер очереди\n");
    printf("2. Ввести символы\n");
    printf("3. Вывести очередь\n");
    printf("4. Показать первый элемент очереди\n");
    printf("5. Показать последний элемент очереди\n");
    printf("6. Выход\n");
    printf("Выберите действие: ");
}


void resizeQueue(CharQueue* q) {
    int newCapacity;
    printf("Введите новый максимальный размер очереди: ");
    newCapacity = getValidNaturalInput(); 

    if (newCapacity < q->size) {
        printf("Новый размер меньше текущего. Удаляем лишние элементы...\n");
        while (q->size > newCapacity) {
            char removed = dequeue(q); 
            printf("Удалён элемент: %c\n", removed);
        }
    }

    q->capacity = newCapacity; 
    printf("Новый размер установлен: %d\n", q->capacity);
    printQueue(q); 
}

// Условие задачи
void inputElements(CharQueue* q) {
    while (!isEmpty(q))
        dequeue(q); // очистка перед новым вводом

    printf("Введите символы (ввод остановится при совпадении с последним элементом):\n");
    while (1) {
        char ch = getCharacterValue(); 
        if (ch == '\n') continue; 
        if (!isEmpty(q) && ch == back(q)) {
            printf("Совпадение с последним элементом! Первый элемент: %c\n", front(q));
            break; 
        }
        enqueue(q, ch); 
        printQueue(q); 
        if (isFull(q)) {
            printf("Очередь переполнена. Ввод окончен.\n");
            break; 
        }
    }
}

int main() {
    setlocale(LC_ALL, "RUS");
    CharQueue queue;
    int choice;

    printf("Введите максимальный размер очереди: ");
    int capacity = getValidNaturalInput();
    initQueue(&queue, capacity);

    while (1) {
        displayMenu();
        choice = getValidNaturalInput();
        switch (choice) {
        case 1: resizeQueue(&queue); break;
        case 2: inputElements(&queue); break;
        case 3: printQueue(&queue); break;
        case 4:
            if (!isEmpty(&queue))
                printf("Первый элемент: %c\n", front(&queue));
            else
                printf("Очередь пуста!\n");
            break;
        case 5:
            if (!isEmpty(&queue))
                printf("Последний элемент: %c\n", back(&queue));
            else
                printf("Очередь пуста!\n");
            break;
        case 6:
            freeQueue(&queue);
            return 0;
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
}
