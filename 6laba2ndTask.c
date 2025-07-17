// Создать дек для символов. Максимальный размер дека вводится с экрана. Создать функции для ввода и вывода элементов дека. Добавлять символы в дек поочередно справа и слева. В случае совпадения добавляемого символа с элементом на другом конце дека выводить его на экран.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

// Структура узла 
typedef struct Node {
    char data;          
    struct Node* next;  
    struct Node* prev;  
} Node;

// Структура дека 
typedef struct {
    Node* front;       
    Node* back;         
    int size;           
    int capacity;       
} CharDeque;

void initDeque(CharDeque* dq, int capacity) {
    dq->front = NULL;       
    dq->back = NULL;        
    dq->size = 0;          
    dq->capacity = capacity; 
}

int isEmpty(CharDeque* dq) {
    return dq->size == 0;  
}

int isFull(CharDeque* dq) {
    return dq->size >= dq->capacity; 
}

// Добавление символа в начало (**pushFront**)
void pushFront(CharDeque* dq, char value) {
    if (isFull(dq)) {
        return; 
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Ошибка выделения памяти.\n");
        exit(1); 
    }

    newNode->data = value;  
    newNode->prev = NULL;   // слева - ничего
    newNode->next = dq->front; // справа - элемент который раньше был первым 

    if (dq->front != NULL)
        dq->front->prev = newNode; // у старого узла слева новый 
    else
        dq->back = newNode;       // новый узел — первый и последний

    dq->front = newNode;       
    dq->size++;               
}

// Добавление символа в конец (**pushBack**)
void pushBack(CharDeque* dq, char value) {
    if (isFull(dq)) {
        return; 
    }

    Node* newNode = (Node*)malloc(sizeof(Node)); 
    if (!newNode) {
        printf("Ошибка выделения памяти.\n");
        exit(1); 
    }

    newNode->data = value;  
    newNode->next = NULL;   // справа - ничего
    newNode->prev = dq->back; // слева - элемент который раньше был последним

    if (dq->back != NULL)
        dq->back->next = newNode;  // у старого узла справа новый
    else
        dq->front = newNode;      // новый узел — первый и последний

    dq->back = newNode;         
    dq->size++;                 
}

// Удаление элемента из начала (**popFront**)
char popFront(CharDeque* dq) {
    if (isEmpty(dq)) {
        printf("Дек пуст!\n");
        return '\0'; 
    }

    Node* temp = dq->front;     
    char value = temp->data;    // Сохраняем символ, который мы собираемся удалить

    dq->front = dq->front->next; // Переносим начало дека на следующий элемент
    if (dq->front != NULL)
        dq->front->prev = NULL; // у нового первого убираем связь с прошлым
    else
        dq->back = NULL;        

    free(temp);               
    dq->size--;                
    return value;               
}

// Удаление эелмента из конца (**popBack**)
char popBack(CharDeque* dq) {
    if (isEmpty(dq)) {
        printf("Дек пуст!\n");
        return '\0'; 
    }

    Node* temp = dq->back;     
    char value = temp->data;    // Сохраняем символ, который мы собираемся удалить

    dq->back = dq->back->prev;  // Переносим "конец" на предыдущий элемент
    if (dq->back != NULL)
        dq->back->next = NULL;  // у нового последнего обнуляем связь с удалённым
    else
        dq->front = NULL;       

    free(temp);                
    dq->size--;                 
    return value;               
}

char front(const CharDeque* dq) {
    if (isEmpty(dq)) {
        printf("Дек пуст!\n");
        return '\0'; 
    }
    return dq->front->data; 
}

char back(const CharDeque* dq) {
    if (isEmpty(dq)) {
        printf("Дек пуст!\n");
        return '\0'; 
    }
    return dq->back->data; 
}

void freeDeque(CharDeque* dq) {
    while (!isEmpty(dq)) {
        popFront(dq); 
    }
}

void printDeque(CharDeque* dq) {
    if (isEmpty(dq)) {
        printf("Дек пуст!\n");
        return;
    }

    printf("Дек: ");
    Node* current = dq->front;
    while (current != NULL) {
        printf("%c", current->data);
        if (current->next != NULL) printf(" <-> ");
        current = current->next;
    }
    printf("\n");
}

int getValidNaturalInput() {
    int num;
    char buffer[100], symbol;
    while (1) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue; 
        if (sscanf_s(buffer, "%d %c", &num, &symbol, 1) == 1 && num > 0)
            return num; 
        printf("Ошибка! Введите корректное натуральное число: ");
    }
}

char getCharacterValue() {
    char input[100];
    while (1) {
        printf("Введите символ (один): ");
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        if (strlen(input) == 2 && input[1] == '\n') return input[0]; 
        printf("Некорректный ввод. Попробуйте снова.\n");
    }
}

void displayMenu() {
    printf("\nМеню:\n");
    printf("1. Изменить размер деки\n");
    printf("2. Ввести символы (поочередно слева и справа)\n");
    printf("3. Вывести дек\n");
    printf("4. Выход\n");
    printf("Выберите действие: ");
}

void resizeDeque(CharDeque* dq) {
    int newCapacity;
    printf("Введите новый максимальный размер деки: ");
    newCapacity = getValidNaturalInput(); 

    if (newCapacity < dq->size) {
        printf("Новый размер меньше текущего. Удаляем лишние элементы с начала...\n");
        while (dq->size > newCapacity) {
            char removed = popFront(dq); 
            printf("Удалён элемент: %c\n", removed);
        }
    }

    dq->capacity = newCapacity; 
    printf("Новый размер установлен: %d\n", dq->capacity);
    printDeque(dq); 
}

void inputElements(CharDeque* dq) {
    while (!isEmpty(dq))
        popFront(dq); // Очищаем деку перед вводом

    printf("Введите символы (ввод остановится при совпадении с элементом на другом конце):\n");
    int toggle = 0; // 0 — в начало, 1 — в конец 

    while (!isFull(dq)) {
        char ch = getCharacterValue(); 
        if (ch == '\n') continue;

        if (!isEmpty(dq)) {
            if (toggle == 0 && ch == back(dq)) {
                printf("Совпадение с последним элементом! Элемент на другом конце: %c\n", back(dq));
                break; 
            }
            else if (toggle == 1 && ch == front(dq)) {
                printf("Совпадение с первым элементом! Элемент на другом конце: %c\n", front(dq));
                break; 
            }
        }

        if (toggle == 0)
            pushFront(dq, ch); 
        else
            pushBack(dq, ch);  

        printDeque(dq); 
        toggle = 1 - toggle; // Переключаем сторону добавления
    }

    if (isFull(dq))
        printf("Дек переполнен. Ввод окончен.\n"); 
}


int main() {
    setlocale(LC_ALL, "RUS"); 
    CharDeque deque;
    int choice;

    printf("Введите максимальный размер деки: ");
    int capacity = getValidNaturalInput();
    initDeque(&deque, capacity); 

    while (1) {
        displayMenu();
        choice = getValidNaturalInput(); 
        switch (choice) {
        case 1:
            resizeDeque(&deque); 
            break;
        case 2:
            inputElements(&deque); 
            break;
        case 3:
            printDeque(&deque); 
            break;
        case 4:
            freeDeque(&deque); 
            return 0; 
        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }
    }
}
