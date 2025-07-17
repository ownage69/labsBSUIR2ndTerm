#include <stdio.h>      
#include <stdlib.h>     
#include <limits.h>     
#include <locale.h>     

typedef struct StackNode {
    int data;                   // Число
    struct StackNode* next;     // Указатель на следующий узел
} StackNode;

StackNode* createNode(int data);                        
void push(StackNode** top, int data);                   
int pop(StackNode** top);                               
int peek(StackNode* top);                             
void printStack(StackNode* top);                        
void freeStack(StackNode* top);                         
int getValidIntegerInput();                             
void mainMenu();                                        
void stackSubMenu(StackNode** stack, int ascending);    
void insertSorted(StackNode** top, int data, int ascending);
StackNode* mergeStacks(StackNode** s1, StackNode** s2);


int main() {
    setlocale(LC_ALL, "RUS");                           
    StackNode* stack1 = NULL;                           // Первый стек (по убыванию)
    StackNode* stack2 = NULL;                           // Второй стек (по возрастанию)
    StackNode* stack3 = NULL;                           // Итоговый стек (по убыванию)
    int choice;                                         

    while (1) {                                        
        mainMenu();                                     
        choice = getValidIntegerInput();                

        switch (choice) {                               
        case 1:                                         
            stackSubMenu(&stack1, 0);                  
            break;
        case 2:                                         
            stackSubMenu(&stack2, 1);                   
            break;
        case 3:                                         
            freeStack(stack3);                          
            stack3 = mergeStacks(&stack1, &stack2);     
            printf("Исходные стеки:\n");              
            printStack(stack1);                         
            printStack(stack2);                         
            printf("Третий стек (по убыванию):\n");     
            printStack(stack3);                         
            break;
        case 4:                                       
            freeStack(stack1);                          
            freeStack(stack2);                          
            freeStack(stack3);                        
            printf("Выход.\n");                         
            return 0;                                   
        default:                                        
            printf("Неверный выбор\n");
        }
    }
}

void mainMenu() {
    printf("\n//Главное меню//\n");                 
    printf("1. Работа с первым стеком (по убыванию)\n"); 
    printf("2. Работа со вторым стеком (по возрастанию)\n"); 
    printf("3. Создать третий стек (по убыванию)\n");   
    printf("4. Выход\n");                               
    printf("Выберите действие: ");                      
}

void stackSubMenu(StackNode** stack, int ascending) {
    int choice, value;                                 
    while (1) {                                         
        printf("\n//Меню работы со стеком (%s)//\n", ascending ? "по возрастанию" : "по убыванию"); // истина : ложь
        printf("1. Добавить элемент\n");                
        printf("2. Удалить элемент\n");                 
        printf("3. Посмотреть верхний элемент\n");       
        printf("4. Вывести стек\n");                    
        printf("5. Назад\n");                           
        printf("Выберите действие: ");                  
        choice = getValidIntegerInput();                

        switch (choice) {                               
        case 1:                                         
            printf("Введите число: ");                 
            value = getValidIntegerInput();             
            if (*stack) {                               
                if ((ascending && peek(*stack) >= value) || (!ascending && peek(*stack) <= value)) {
                    push(stack, value); // если не нарушает сортировку
                }
                else {
                    printf("Ошибка! Стек должен заполняться по %s", ascending ? "возрастанию\n" : "убыванию\n"); 
                }
            }
            else {
                push(stack, value); // Добавление элемента в пустой стек
            }
            printStack(*stack);                         
            break;
        case 2:                                         
            if (*stack) {                               
                printf("Удалённый элемент: %d\n", pop(stack)); // Удаление и вывод элемента
                printStack(*stack);                     
            }
            else {
                printf("Стек пуст\n");                  
            }
            break;
        case 3:                                         
            if (*stack) {                               
                printf("Верхний элемент: %d\n", peek(*stack)); 
            }
            else {
                printf("Стек пуст\n");                  
            }
            break;
        case 4:                                       
            printStack(*stack);                         
            break;
        case 5:                                         
            return;                                     
        default:                                        
            printf("Неверный выбор\n");
        }
    }
}

// Создание нового узла
StackNode* createNode(int data) {
    StackNode* node = (StackNode*)malloc(sizeof(StackNode)); 
    if (!node) exit(1);                                     
    node->data = data; // значение узла
    node->next = NULL;                                      
    return node;                                           
}

// Добавление элемента в стек (push)
void push(StackNode** top, int data) {
    StackNode* node = createNode(data);                     
    node->next = *top; // указатель на текущую вершину 
    *top = node;                                            
}

// Удаление элемента из стека (pop)
int pop(StackNode** top) {
    if (!*top) return INT_MIN;                             
    StackNode* temp = *top;  // указатель на вершину
    int data = temp->data; // значение вершины                         
    *top = (*top)->next; // указатель на новую вершину                                 
    free(temp);                                            
    return data;                                            
}

// Просмотр верхнего элемента (peek)
int peek(StackNode* top) {
    if (top) {                                             
        return top->data;                                  
    }
    else {
        return INT_MIN;                                  
    }
}

// Вывод стека
void printStack(StackNode* top) {
    if (!top) {                                            
        printf("Стек пуст\n");
        return;                                            
    }
    printf("Стек: ");                                      
    while (top) {                                           
        printf("%d", top->data); // Вывод значения узла
        if (top->next) {                                   
            printf(" -> ");                                 
        }
        top = top->next;                                  
    }
    printf("\n");                                       
}

// Освобождение памяти стека
void freeStack(StackNode* top) {
    while (top) {                                           
        StackNode* temp = top; // Сохранение текущего узла
        top = top->next;  // Переход к следующему узлу
        free(temp);                                       
    }
}

// Вставка элемента
void insertSorted(StackNode** top, int data, int ascending) {
    StackNode* tempStack = NULL; // Временный стек для сортировки
    int val;                                        

    // Извлечение элементов, нарушающих порядок
    while (*top && ((ascending && peek(*top) < data) || (!ascending && peek(*top) > data))) {
        val = pop(top); // Удаление элемента
        push(&tempStack, val); // Сохранение во временный стек
    }

    push(top, data); // Добавление нового элемента

    // Восстановление извлечённых элементов
    while (tempStack) {
        val = pop(&tempStack); // Извлечение элемента
        push(top, val); // Добавление в основной стек
    }
}

// Объединение двух стеков в третий (по убыванию)
StackNode* mergeStacks(StackNode** s1, StackNode** s2) {
    StackNode* copy1 = NULL;                                
    StackNode* copy2 = NULL;                                
    StackNode* temp = NULL;                                 
    StackNode* merged = NULL;                            

    // Копирование первого стека (s1) в copy1
    while (*s1) {
        int val = pop(s1);                         
        push(&temp, val);                                   // Сохранение во временный стек
    }
    while (temp) {
        int val = pop(&temp);                               
        push(s1, val);                                      // Восстановление оригинала
        push(&copy1, val);                                  // Добавление в копию
    }

    // Копирование второго стека (s2) в copy2
    while (*s2) {
        int val = pop(s2);                                  
        push(&temp, val);                                   // Сохранение во временный стек
    }
    while (temp) {
        int val = pop(&temp);                            
        push(s2, val);                                      // Восстановление оригинала
        push(&copy2, val);                                  // Добавление в копию
    }

    // Объединение копий в третий стек по убыванию
    while (copy1) {
        int val = pop(&copy1);                              // Удаление элемента из copy1
        insertSorted(&merged, val, 0);                      // Вставка с сортировкой по убыванию
    }
    while (copy2) {
        int val = pop(&copy2);                              // Удаление элемента из copy2
        insertSorted(&merged, val, 0);                      // Вставка с сортировкой по убыванию
    }

    return merged;                                         
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
