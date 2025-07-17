// Создать стек для целых чисел. Максимальный размер стека вводится с экрана. Найти сумму четных элементов стека.

#include <stdio.h>      
#include <stdlib.h>     
#include <locale.h>     
#include <limits.h>     

typedef struct StackNode {
    int data; // число 
    struct StackNode* next; // указатель на след. узел
} StackNode;

// Новый узел стека
StackNode* createNode(int data) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode)); 
    if (!node) {                                            
        printf("Ошибка выделения памяти\n");
        exit(1);                                            
    }
    node->data = data; // значение узла
    node->next = NULL;                                      
    return node; // созданный узел
}

// Добавление элемента в стек (push)
void push(StackNode** top, int* size, int max_size, int data) {
    if (*size >= max_size) {                                
        printf("Стек переполнен\n");
        return;                                             
    }
    StackNode* node = createNode(data);                     
    node->next = *top; // указатель на текущую вершину                                  
    *top = node; // веришна - созданный узел                                  
    (*size)++;                                             
}

// Удаление элемента из стека (pop)
int pop(StackNode** top, int* size) {
    if (*size == 0) {                                       
        printf("Стек пуст\n");
        exit(1);                                            
    }
    StackNode* temp = *top; // указатель на текущую вершину                               
    int data = temp->data;  // значение вершины
    *top = temp->next;  // обновление вершины                                   
    free(temp);                                             
    (*size)--;                                              
    return data;                                          
}

// Просмотр верхнего элемента (peek)
int peek(StackNode* top) {
    if (top) {                                            
        return top->data; // верхний элемент
    }
    else {
        return INT_MIN;                               
    }
}

// Обработка просмотра верхнего элемента
void handlePeek(StackNode* top) {
    int temp = peek(top); // Получение верхнего элемента           
    if (temp == INT_MIN) {                            
        printf("Стек пуст\n");
    }
    else {
        printf("Верхний элемент: %d\n", temp);              
    }
}

// Вывод стека
void printStack(StackNode* top) {
    if (!top) {                                             
        printf("Стек пуст\n");
        return;                                             
    }
    printf("Стек: ");                                       
    StackNode* current = top; // указатель для прохода по стеку                               
    while (current) {                                       
        printf("%d", current->data);                        
        if (current->next) printf(" -> ");                   
        current = current->next;                           
    }
    printf("\n");                                           
}

// Освобождение памяти стека
void freeStack(StackNode* top) {
    StackNode* current = top; // Указатель для прохода по стеку
    while (current) {                                     
        StackNode* temp = current;                          
        current = current->next;                            
        free(temp);                                         
    }
}

// Изменение размера стека
StackNode* resizeStack(StackNode* top, int* size, int* max_size) {
    printf("Введите новый размер стека: ");                  
    int newSize = getValidIntegerInput();                   

    if (newSize < *size) {                                  
        int removed = *size - newSize;                      
        printf("Удаление %d верхних элемента(ов)...\n", removed); 
        for (int i = 0; i < removed; i++) {                 
            pop(&top, size);                                
        }
    }

    *max_size = newSize;                                    
    printf("Новый максимальный размер стека: %d\n", *max_size); 
    return top;                                           
}

// Нахождение суммы чётных элементов
int sumEven(StackNode** top, int* size, int max_size) {
    StackNode* tempTop = NULL; // Временный стек для извлечения из основного и сохранения                         
    int tempSize = 0; // его размер
    int sum = 0;                                         
    int val;                                                
    int originalSize = *size;                              
    int evenCount = 0; // четные элементы
    int* evenNumbers = (int*)malloc(originalSize * sizeof(int)); 
    if (!evenNumbers) {                                    
        printf("Ошибка выделения памяти\n");
        exit(1);                                           
    }

    for (int i = 0; i < originalSize; i++) {
        val = pop(top, size); // Удаление элемента с вершины
        if (val % 2 == 0) {                                 
            sum += val;                                    
            evenNumbers[evenCount++] = val; // Сохранение чётного числа в массив
        }
        push(&tempTop, &tempSize, max_size, val); // Сохранение элемента во временный стек
    }

    // Восстановление исходного стека
    for (int i = 0; i < originalSize; i++) {
        val = pop(&tempTop, &tempSize); // Извлечение элемента из временного стека
        push(top, size, max_size, val); // Добавление элемента в исходный стек
    }

    if (evenCount == 0) {                                   
        printf("Чётных элементов нет\n");
    }
    else {
        printf("Чётные элементы: ");                        
        for (int i = 0; i < evenCount; i++) {               
            printf("%d", evenNumbers[i]);                 
            if (i < evenCount - 1) printf("+");            
        }
        printf("=%d\n", sum);                              
    }

    free(evenNumbers);                                      
    freeStack(tempTop);                                     
    return sum;                                            
}


int getValidIntegerInput() {
    int num;
    char symbol;
    while (1) {
        char input[100];

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода! Повторите ввод: \n");
            continue;
        }

        if (sscanf_s(input, "%d %c", &num, &symbol) != 1) {
            printf("Ошибка ввода! Введите целое число: \n");
            continue;
        }
        return num;
    }
}

int getValidNaturalInput() {
    int num;                                                
    char buffer[100];                                      
    char symbol;                                          
    while (1) {                                            
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) continue; 
        if (sscanf_s(buffer, "%d %c", &num, 1, &symbol, 1) == 1 && num > 0) return num; 
        printf("Ошибка! Введите корректное натуральное число: "); 
    }
}

void displayMenu() {
    printf("\nМеню:\n");                                  
    printf("1. Изменить размер стека\n");                    
    printf("2. Добавить элемент\n");                       
    printf("3. Удалить элемент\n");                        
    printf("4. Посмотреть верхний элемент\n");               
    printf("5. Вывести стек\n");                           
    printf("6. Найти сумму чётных элементов\n");             
    printf("7. Выход\n");                                  
    printf("Выберите действие: ");                        
}

int main() {
    setlocale(LC_ALL, "RUS");                               
    StackNode* top = NULL;                                  
    int stackSize = 0;                                     
    int max_size = 0;                                       
    int choice, value;                                      

    printf("Введите максимальный размер стека: ");
    max_size = getValidIntegerInput();                      

    while (1) {                                          
        displayMenu();                                      
        choice = getValidIntegerInput();                    

        switch (choice) {                                  
        case 1:                                            
            top = resizeStack(top, &stackSize, &max_size);             
            break;
        case 2:                                             
            printf("Введите элемент для добавления в стек: ");
            value = getValidIntegerInput();                
            push(&top, &stackSize, max_size, value);        
            printStack(top);                               
            break;
        case 3:                                             
            if (stackSize == 0) {                           
                printf("Стек пуст\n");
            }
            else {
                value = pop(&top, &stackSize);              
                printf("Удалённый элемент: %d\n", value);   
                printStack(top);                            
            }
            break;
        case 4:                                            
            handlePeek(top);                                
            break;
        case 5:                                            
            printStack(top);                               
            break;
        case 6:                                             
            if (stackSize == 0) {                           
                printf("Стек пуст!\n");
            }
            else {
                int sum = sumEven(&top, &stackSize, max_size); 
            }
            break;
        case 7:                                             
            freeStack(top);                                                             
            return 0;                                       
        default:                                            
            printf("Ошибка! Попробуйте снова.\n");
        }
    }
}
