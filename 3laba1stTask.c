// Найти и вывести 2 старших бита беззнакового числа

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

enum {
    max_limit = 4294967295, // 32 бита
    min_limit = 0,          
    rand_max_limit = 1000,  
    rand_min_limit = 0      
};

typedef struct {
    unsigned int high_bit : 1;
    unsigned int low_bit : 1;
} TwoBits;

unsigned int execute_verification(unsigned int min_limit, unsigned int max_limit) {
    char input[100]; 
    unsigned int number; 
    char symbol; 

    while (1) {
        
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Ошибка ввода! Попробуйте ещё раз:\n");
            continue;
        }

        if (sscanf_s(input, "%u %c", &number, &symbol) != 1) {
            printf("Ошибка ввода! Введите целое число:\n");
            continue;
        }

       
        if (number < min_limit || number > max_limit) {
            printf("Ошибка! Введите число в диапазоне [%u, %u]:\n", min_limit, max_limit);
            continue;
        }
        return number; 
    }
}

unsigned int identify_user_choice() {
    char input[256];
    unsigned int number;
    printf("Введите целое число самостоятельно - 1. Выбрать случайное - любая клавиша:\n");
    fgets(input, sizeof(input), stdin);

    if (input[0] == '1') { 
        printf("Введите число:\n");
        number = execute_verification(min_limit, max_limit);
    }
    else { // Иначе генерируем случайное число
        number = rand_min_limit + rand() % (rand_max_limit - rand_min_limit + 1); 
        printf("Случайное число: %u\n", number);
    }

    return number;
}
// Функция для вывода числа в двоичном виде 
void print_binary_short(unsigned int number) {
    if (number == 0) {
        printf("Число в двоичном коде: 0\n");
        return;
    }
   int total_bits = 0; // биты
    unsigned int temp = number;

    while (temp) {
        temp >>= 1; // Сдвиг вправо 
        total_bits++;
    }
    printf("Число в двоичном коде: ");
    for (int i = total_bits - 1; i >= 0; i--) {
        printf("%u", (number >> i) & 1); 
    }
    printf("\n");
}
// Функция для поиска и вывода двух старших бит числа
void print_highest_two_bits(unsigned int number) {
    if (number == 0) {
        printf("Число равно 0, старшие биты: 0 0\n");
        return;
    }
    int total_bits = 0;
    unsigned int temp = number;
    while (temp) {
        temp >>= 1;
       total_bits++;
    }
    if (total_bits == 1) {
        printf("Только один старший бит: %u\n", (number >> 0) & 1);
        return;
    }
    unsigned int top_two_bits = number >> (total_bits - 2); // Старшие -> младшие
    top_two_bits &= 3; // оставляет 2 бита 
    TwoBits bits;
    bits.high_bit = (top_two_bits >> 1) & 1;
    bits.low_bit = top_two_bits & 1;
    printf("Старшие биты : %u %u\n", bits.high_bit, bits.low_bit);

}

int main() {
    setlocale(LC_ALL, "RUS"); 
    srand(time(NULL)); 
    unsigned int user_number = identify_user_choice(); 
    print_binary_short(user_number);
    print_highest_two_bits(user_number); 
    return 0;
}
