// С клавиатуры заполнить файл целыми числами. 
// В некоторых видах спортивных состязаний (например, в фигурном катании) выступление каждого спортсмена независимо оценивается несколькими судьями, затем из всей совокупности оценок удаляются наиболее высокая и наиболее низкая, 
// а для оставшихся оценок вычисляется среднее арифметическое, которое и идет в зачет спортсмену. Если наиболее высокую оценку выставило несколько судей, то из совокупности оценок удаляется только одна такая оценка; 
// аналогично поступают и с наиболее низкими оценками. Известны оценки одного из участников соревнований. Определить балл спортсмена. Доп массивов не использовать
// Поменять значения минимальных элементов на максимальные, а максимальных - на минимальные

#include <stdio.h>   
#include <stdlib.h>  
#include <locale.h>  

int propriety(int min, int max);
void fillFile(const char* filename);
void swapMinMax(const char* filename);
void printFile(const char* filename);
void calculateScore(const char* filename);

int propriety(int min, int max) {
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

        if (num < min || num > max) { 
            printf("Ошибка ввода! Введите число от %d до %d: ", min, max);
            continue;
        }

        return num;     }
}

void fillFile(const char* filename) {
    FILE* file = fopen(filename, "wb"); 
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }
    printf("Введите количество элементов: ");
    int count = propriety(1, 999);
    printf("Введите %d целых чисел:\n", count);
    for (int i = 0; i < count; i++) {
        int num = propriety(-999, 999); 
        fwrite(&num, sizeof(int), 1, file); 
    }
    fclose(file);
    printf("Содержимое файла после записи:\n");
    printFile(filename); 
}

void swapMinMax(const char* filename) {
    FILE* file = fopen(filename, "rb+"); 
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file); // Размер файла
    int count = fileSize / sizeof(int); // кол-во чисел
    if (count == 0) { 
        printf("Файл пуст.\n");
        fclose(file);
        return;
    }
    int* numbers = (int*)malloc(count * sizeof(int));    if (numbers == NULL) {
        printf("Ошибка выделения памяти.\n");
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_SET);
    fread(numbers, sizeof(int), count, file); 
    int min = numbers[0], max = numbers[0
    for (int i = 1; i < count; i++) { 
        if (numbers[i] < min) min = numbers[i];
        if (numbers[i] > max) max = numbers[i];
    }

    printf("Минимальное число: %d\nМаксимальное число: %d\n",  min, max);
    for (int i = 0; i < count; i++) {         if (numbers[i] == min) numbers[i] = max;
        else if (numbers[i] == max) numbers[i] = min;
    }
    fseek(file, 0, SEEK_SET); 
    fwrite(numbers, sizeof(int), count, file); 
    fclose(file); 
    free(numbers); 
    printf("Содержимое файла после обработки:\n");
    printFile(filename); 
}

void calculateScore(const char* filename) {
    FILE* file = fopen(filename, "rb");     if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }
    fseek(file, 0, SEEK_END); 
    long fileSize = ftell(file);
    int count = fileSize / sizeof(int); 
    if (count < 3) {
        printf("Недостаточно данных для вычисления среднего балла.\n\n");
        fclose(file);
        return;
    }
    int* numbers = (int*)malloc(count * sizeof(int));
    if (numbers == NULL) {
        printf("Ошибка выделения памяти.\n");
        fclose(file);
        return;
    }
    fseek(file, 0, SEEK_SET);    fread(numbers, sizeof(int), count, file); 
    for (int i = 0; i < count; i++) {
        if (numbers[i] < 0 || numbers[i] > 10) {
            printf("Обнаружено некорректное число: %d. Операция прервана.\n", numbers[i]);
            fclose(file);
            free(numbers);
            return;
        }
    }
    fclose(file);
    int min = numbers[0], max = numbers[0], minIndex = 0, maxIndex = 0    for (int i = 1; i < count; i++) {
        if (numbers[i] < min) { 
            min = numbers[i];
            minIndex = i; 
        }
        if (numbers[i] > max) { 
            max = numbers[i];
            maxIndex = i; 
        }
    }
    int removedCount = 0; 
    int sum = 0, newCount = 0;
    for (int i = 0; i < count; i++) {
        if ((i == minIndex || i == maxIndex) && removedCount < 2) {
            removedCount++; 
            printf("Удалено число: %d\n", numbers[i]);
        }
        else {
            sum += numbers[i]; // Добавляем число в сумму, если оно не удалено
            newCount++; // Увеличиваем счетчик оставшихся чисел
        }
    }
    printf("Оставшиеся элементы: ");
    for (int i = 0; i < count; i++) {
        if (i == minIndex || i == maxIndex) continue;  Пропускаем удаленные элементы
        printf("%d ", numbers[i]);
    }
    printf("\n");
    printf("Сумма оставшихся элементов = %d, количество оставшихся элементов = %d\n", sum, newCount);
    if (newCount > 0) {
        printf("Средний балл = %.2f\n", (double)sum / newCount);
    }
    free(numbers); 
}

void printFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return;
    }
    int num;
    while (fread(&num, sizeof(int), 1, file)) { 
        printf("%d ", num);
    }
    printf("\n");
    fclose(file); 
}


int main(int count, char* path[]) {
    setlocale(LC_ALL, "RUS");
    if (count < 2) {
        printf("Ошибка! Введите имя файла после %s (Например file.txt)\n", path[0]); 
        return 1;
    }
    const char* filename = path[1]; 
    int choice;
    while (1) {
        printf("Меню:\n");
        printf("1. Заполнить файл целыми числами\n");
        printf("2. Вычислить средний балл спортсмена\n");
        printf("3. Заменить минимальные элементы на максимальные и наоборот\n");
        printf("9. Выход\n");
        printf("Выберите действие: ");
        choice = propriety(1, 9);
        switch (choice) {
        case 1:
            fillFile(filename);
            break;
        case 2:
            calculateScore(filename);
            break;
        case 3:
            swapMinMax(filename);
            break;
        case 9:
            return 0; 
        default:
            printf("Некорректный выбор. Попробуйте снова.\n");
        }
    }
}
