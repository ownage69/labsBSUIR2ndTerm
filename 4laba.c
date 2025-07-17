// Структура содержит информацию о сотрудниках фирмы: шифр отдела (число), фамилию (указатель), вложенное объединение – дату приёма на работу (строка фиксированной длины) и сумму оклада. Найти сотрудников с минимальным окладом. 
// Удалить сотрудников, принятых на работу в заданный день.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#define DATE_LEN 11  // "ДД.ММ.ГГГГ" + '\0'

union SalaryDate {
    float salary; // зп
    char data[4 + DATE_LEN]; // первые 4 байта совпадают с salary, далее строка даты
};

struct Employee {
    char* surname;
    int departmentCode;
    union SalaryDate info;
};

// Функция для безопасного ввода строки с проверкой фамилии
void getString(char** str) {
    char buffer[100];
    int valid_input = 0;

    do {
        printf("Введите фамилию сотрудника:\n");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Удаление \n

        if (buffer[0] >= 'А' && buffer[0] <= 'Я') {
            valid_input = 1;
        }
        else {
            printf("Ошибка! Фамилия должна начинаться с большой русской буквы (А-Я).\n");
        }
    } while (!valid_input);

    *str = (char*)malloc(strlen(buffer) + 1); // Выделение памяти под строку фамилии
    if (!*str) {
        printf("Ошибка памяти.\n");
        exit(1);
    }
    strcpy_s(*str, strlen(buffer) + 1, buffer);
}

// Функция для ввода целого числа с проверкой диапазона
int execute_verification(int min_limit, int max_limit) {
    char input[100];
    int number;
    char symbol;

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода! Попробуйте еще раз!\n");
            continue;
        }

        if (sscanf_s(input, "%d %c", &number, &symbol) != 1) {
            printf("Ошибка ввода! Введите целое число:\n");
            continue;
        }

        if (number > max_limit || number < min_limit) {
            printf("Ошибка ввода! Введите число из диапазона [%d, %d]:\n", min_limit, max_limit);
            continue;
        }

        return number;
    }
}

// Функция для ввода числа с плавающей запятой с проверкой диапазона
float execute_verification_float(float min_limit, float max_limit) {
    char input[100];
    float number;
    char symbol;

    while (1) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода! Попробуйте еще раз!\n");
            continue;
        }

        if (sscanf_s(input, "%f %c", &number, &symbol) != 1) {
            printf("Ошибка ввода! Введите число с плавающей запятой:\n");
            continue;
        }

        if (number > max_limit || number < min_limit) {
            printf("Ошибка ввода! Введите число из диапазона [%.2f, %.2f]:\n", min_limit, max_limit);
            continue;
        }

        return number;
    }
}

// Функция для добавления нового сотрудника в список
void addEmployee(struct Employee** list, int* size) {
    *list = realloc(*list, (*size + 1) * sizeof(struct Employee)); // Увеличение памяти под массив сотрудников
    if (!*list) {
        printf("Ошибка выделения памяти!\n");
        exit(1);
    }

    struct Employee* emp = &(*list)[*size];

    getString(&emp->surname);

    printf("Введите шифр отдела (например: 0001):\n");
    int valid_input = 0;
    char temp_code[10];
    do {
        fgets(temp_code, sizeof(temp_code), stdin);
        int len = strlen(temp_code);
        if (len > 0 && temp_code[len - 1] == '\n') {
            temp_code[len - 1] = '\0';
        }
        else {
            while (getchar() != '\n' && getchar() != EOF);
            printf("Ошибка! Введено больше 4 цифр\n");
            continue;
        }

        if (strlen(temp_code) != 4) {
            printf("Ошибка! Шифр отдела должен содержать 4 цифры.\n");
            continue;
        }

        valid_input = 1;
        for (int i = 0; i < 4; i++) {
            if (temp_code[i] < '0' || temp_code[i] > '9') {
                printf("Ошибка! Шифр отдела должен содержать только цифры.\n");
                valid_input = 0;
                break;
            }
        }
    } while (!valid_input);
    emp->departmentCode = atoi(temp_code); // Преобразование строки в число и запись в структуру

    printf("Введите оклад: ");
    emp->info.salary = execute_verification_float(0.0f, 1000000.0f);

    printf("Введите дату приёма (ДД.ММ.ГГГГ): ");
    char buffer[DATE_LEN];
    int valid_date = 0;
    do {
        fgets(buffer, DATE_LEN, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        // Проверка длины и формата введённой даты
        if (strlen(buffer) != 10 || buffer[2] != '.' || buffer[5] != '.') {
            printf("Ошибка! Дата должна быть в формате ДД.ММ.ГГГГ (например, 15.03.2025).\n");
            continue;
        }

        // Проверка, что все символы, кроме точек, — цифры
        for (int i = 0; i < 10; i++) {
            if (i != 2 && i != 5 && (buffer[i] < '0' || buffer[i] > '9')) {
                printf("Ошибка! Дата должна содержать только цифры и точки в формате ДД.ММ.ГГГГ.\n");
                valid_date = 0;
                break;
            }
            if (i == 9) valid_date = 1;
        }
        if (!valid_date) continue;

        // Извлечение дня, месяца и года
        int day, month, year;
        if (sscanf_s(buffer, "%2d.%2d.%4d", &day, &month, &year) != 3) {
            printf("Ошибка! Неверный формат даты.\n");
            valid_date = 0;
            continue;
        }

        // Проверка диапазонов
        if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
            printf("Ошибка! Некорректные значения: день (1-31), месяц (1-12), год (1900-2100).\n");
            valid_date = 0;
            continue;
        }

        // Проверка количества дней в месяце
        int days_in_month;
        if (month == 2) {
            if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                days_in_month = 29; // Високосный год
            }
            else {
                days_in_month = 28;
            }
        }
        else if (month == 4 || month == 6 || month == 9 || month == 11) {
            days_in_month = 30;
        }
        else {
            days_in_month = 31;
        }

        if (day > days_in_month) {
            printf("Ошибка! В месяце %d года %d не может быть %d дней.\n", month, year, day);
            valid_date = 0;
            continue;
        }

        valid_date = 1;
    } while (!valid_date);
    // Очистка буфера ввода после успешного ввода даты
    while (getchar() != '\n' && getchar() != EOF);
    strcpy_s(emp->info.data + 4, DATE_LEN, buffer);

    (*size)++;
}

// Функция для вывода списка всех сотрудников
void printEmployees(struct Employee* list, int size) {
    if (size == 0) {
        printf("Список пуст.\n");
        return;
    }
    printf("\nСотрудники:\n");
    for (int i = 0; i < size; i++) {
        printf("%d. %s | Шифр отдела: %d | Оклад: %.2f | Дата приёма: %s\n",
            i + 1, list[i].surname, list[i].departmentCode,
            list[i].info.salary, list[i].info.data + 4);
    }
}

// Функция для поиска сотрудников с минимальным окладом
void findMinSalary(struct Employee* list, int size) {
    if (size == 0) {
        printf("Список пуст.\n");
        return;
    }

    float min = list[0].info.salary; // Инициализация минимального оклада значением первого сотрудника
    for (int i = 1; i < size; i++) {
        if (list[i].info.salary < min) {
            min = list[i].info.salary;
        }
    }

    printf("\nСотрудники с минимальным окладом (%.2f):\n", min);
    for (int i = 0; i < size; i++) {
        if (list[i].info.salary == min) {
            printf("%s | Шифр отдела: %d | Оклад: %.2f | Дата приёма: %s\n",
                list[i].surname, list[i].departmentCode,
                list[i].info.salary, list[i].info.data + 4);
        }
    }
}

// Функция для удаления сотрудников, принятых в заданную дату
void removeByDate(struct Employee** list, int* size, const char* date) {
    int newSize = 0; 
    for (int i = 0; i < *size; i++) { 
        if (strcmp((*list)[i].info.data + 4, date) != 0) { 
            (*list)[newSize++] = (*list)[i]; 
        }
        else {
            printf("Удалён: %s | Дата приёма: %s\n", (*list)[i].surname, (*list)[i].info.data + 4);
            free((*list)[i].surname);
        }
    }
    *list = realloc(*list, newSize * sizeof(struct Employee)); 
    *size = newSize;
}

// Функция для освобождения выделенной памяти под список сотрудников
void freeEmployees(struct Employee* list, int size) {
    for (int i = 0; i < size; i++) {
        free(list[i].surname);
    }
    free(list);
}

int processMenu(struct Employee** staff, int* size) {
    printf("\nМеню:\n");
    printf("1. Добавить сотрудника\n");
    printf("2. Вывести всех сотрудников\n");
    printf("3. Найти сотрудников с минимальным окладом\n");
    printf("4. Удалить сотрудников, принятых в заданную дату\n");
    printf("5. Выход\n");
    printf("Выберите действие: ");
    int choice = execute_verification(1, 5);

    switch (choice) {
    case 1:
        addEmployee(staff, size);
        break;
    case 2:
        printEmployees(*staff, *size);
        break;
    case 3:
        findMinSalary(*staff, *size);
        break;
    case 4: {
        if (*size == 0) {
            printf("Список пуст, удаление невозможно.\n");
            break;
        }
        char date[DATE_LEN];
        int valid_date = 0;
        do {
            printf("Введите дату (ДД.ММ.ГГГГ): ");
            fgets(date, DATE_LEN, stdin);
            date[strcspn(date, "\n")] = '\0';

            // Проверка длины и формата введённой даты
            if (strlen(date) != 10 || date[2] != '.' || date[5] != '.') {
                printf("Ошибка! Дата должна быть в формате ДД.ММ.ГГГГ (например, 15.03.2025).\n");
                continue;
            }

            // Проверка, что все символы, кроме точек, — цифры
            for (int i = 0; i < 10; i++) {
                if (i != 2 && i != 5 && (date[i] < '0' || date[i] > '9')) {
                    printf("Ошибка! Дата должна содержать только цифры и точки в формате ДД.ММ.ГГГГ.\n");
                    valid_date = 0;
                    break;
                }
                if (i == 9) valid_date = 1;
            }
            if (!valid_date) continue;

            // Извлечение дня, месяца и года
            int day, month, year;
            if (sscanf_s(date, "%2d.%2d.%4d", &day, &month, &year) != 3) {
                printf("Ошибка! Неверный формат даты.\n");
                valid_date = 0;
                continue;
            }

            // Проверка диапазонов
            if (day < 1 || day > 31 || month < 1 || month > 12 || year < 1900 || year > 2100) {
                printf("Ошибка! Некорректные значения: день (1-31), месяц (1-12), год (1900-2100).\n");
                valid_date = 0;
                continue;
            }

            // Проверка количества дней в месяце
            int days_in_month;
            if (month == 2) {
                if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)) {
                    days_in_month = 29; // Високосный год
                }
                else {
                    days_in_month = 28;
                }
            }
            else if (month == 4 || month == 6 || month == 9 || month == 11) {
                days_in_month = 30;
            }
            else {
                days_in_month = 31;
            }

            if (day > days_in_month) {
                printf("Ошибка! В месяце %d года %d не может быть %d дней.\n", month, year, day);
                valid_date = 0;
                continue;
            }

            valid_date = 1;
        } while (!valid_date);
        // Очистка буфера ввода после успешного ввода даты
        while (getchar() != '\n' && getchar() != EOF);
        removeByDate(staff, size, date);
        break;
    }
    case 5:
        freeEmployees(*staff, *size);
        *staff = NULL; 
        *size = 0;     
        return 1;     
    default:
        printf("Неверный выбор. Повторите.\n");
    }
    return 0; 
}

int main() {
    system("chcp 1251");
    setlocale(LC_ALL, "RUS");
    int size = 0; 
    struct Employee* staff = NULL;

    while (!processMenu(&staff, &size)) {
    }

    return 0;
}
