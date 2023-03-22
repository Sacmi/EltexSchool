#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "reader.h"

// Аналог макроса todo!() из Rust
#define TODO assert(0 && "Not implemented")

#define STATS_FILE "stats.bin"
#define SWAP_RATIO 0.65
#define K_COEFFICIENT 30
#define INTERVAL_COEFFICIENT 25
#define MIN_BET_COEFFICIENT 1.01

#define USER_NAME_MIN_LENGTH 2
#define USER_NAME_MAX_LENGTH 16

#define DEPOSIT_MODE "deposit"
#define PLAY_MODE "play"
#define STATS_MODE "stats"
#define WITHDRAW_MODE "withdraw"

#define MIN_BET 10
#define MAX_BET 1000

#define MIN_DEPOSIT 100
#define MIN_WITHDRAW 100

typedef struct {
    char username[USER_NAME_MAX_LENGTH];
    unsigned int winCount;
    unsigned int lossCount;
    double balance;
} UserStats;

void loadUserStatsFromFile(const char *filename, UserStats *userStats) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        fprintf(stderr, "Error: Не удалось открыть файл %s", filename);
        exit(1);
    }

    size_t read = fread(userStats, sizeof(UserStats), 1, file);

    fclose(file);

    if (read != 1) {
        fprintf(stderr, "Error: Не удалось прочитать файл %s", filename);
        exit(1);
    }
}

void dumpUserStatsToFile(const char *filename, const UserStats *userStats) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error: Не удалось открыть файл %s", filename);
        exit(1);
    }

    size_t written = fwrite(userStats, sizeof(UserStats), 1, file);
    fclose(file);

    if (written != 1) {
        fprintf(stderr, "Error: Не удалось записать файл %s", filename);
        exit(1);
    }
}

void showUserStats(const UserStats *userStats) {
    printf("Статистика пользователя %s:\n", userStats->username);
    printf("Баланс: %.2lf\n", userStats->balance);
    printf("Побед: %u\n", userStats->winCount);
    printf("Поражений: %u\n", userStats->lossCount);
    if (userStats->lossCount != 0) {
        printf("Процент побед: %.2lf\n", (double) userStats->winCount / userStats->lossCount);
    }
}

void showUsage(const char *programName) {
    printf("Использование: ./%s <режим>\n", programName);
    printf("Режимы:\n");
    printf("- %s - пополнить баланс\n", DEPOSIT_MODE);
    printf("- %s - сыграть в игру\n", PLAY_MODE);
    printf("- %s - показать статистику\n", STATS_MODE);
    printf("- %s - вывести баланс\n", WITHDRAW_MODE);
}

int isFileExist(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0;
    }

    fclose(file);
    return 1;
}

double calculateCoefficient(int tryCount, int interval) {
    double calculatedCoefficient = (K_COEFFICIENT * ((double) interval / INTERVAL_COEFFICIENT)) / tryCount;

    if (calculatedCoefficient > MIN_BET_COEFFICIENT) {
        return calculatedCoefficient;
    } else {
        return MIN_BET_COEFFICIENT;
    }
}

void initializeUserStats(UserStats *userStats) {
    size_t length;
    char *buf = calloc(USER_NAME_MAX_LENGTH + 1, sizeof(char));

    do {
        printf("Введите имя пользователя (лимит: от %d до %d):\n", USER_NAME_MIN_LENGTH, USER_NAME_MAX_LENGTH);
        fgets(buf, USER_NAME_MAX_LENGTH + 1, stdin);

        length = strlen(buf);
        if (length < USER_NAME_MIN_LENGTH || length > USER_NAME_MAX_LENGTH) {
            printf("Ошибка: Некорректная длина имени пользователя. Попробуйте еще раз:\n");
        }
    } while (length < USER_NAME_MIN_LENGTH || length > USER_NAME_MAX_LENGTH);

    buf[length - 1] = '\0';
    strcpy(userStats->username, buf);

    free(buf);

    userStats->winCount = 0;
    userStats->lossCount = 0;
    userStats->balance = 100;
}

void deposit(UserStats *userStats) {
    printf("Пополнение баланса\n");
    printf("Ваш баланс: %f\n", userStats->balance);
    printf("Минимальная сумма пополнения: %d\n", MIN_DEPOSIT);
    printf("Курс обмена: %d у.е. = %f к.\n", 1, SWAP_RATIO);
    printf("\n");

    double deposit;

    while (1) {
        deposit = readDouble("Введите сумму пополнения:");

        if (deposit < MIN_DEPOSIT) {
            printf("Ошибка: Сумма пополнения должна быть больше %d. Попробуйте еще раз.\n", MIN_DEPOSIT);
        } else {
            break;
        }
    }

    userStats->balance += deposit * SWAP_RATIO;
    printf("Баланс успешно пополнен!\n");
    printf("Ваш баланс: %f\n", userStats->balance);
}

int randomInt(int from, int to) {
    return rand() % (to - from + 1) + from;
}

void play(UserStats *userStats) {
    double bet;
    int from, to, tryCount;

    printf("Игра: Угадай число\n");
    printf("Лимит ставки: от %d до %d\n", MIN_BET, MAX_BET);
    printf("\n");

    start_game:
    printf("Ваш баланс: %f\n", userStats->balance);
    printf("\n");

    do {
        bet = readDouble("Введите ставку:");

        if (bet < MIN_BET || bet > MAX_BET) {
            printf("Ошибка: Ставка должна быть от %d до %d. Попробуйте еще раз:\n", MIN_BET, MAX_BET);
        }
    } while (bet < MIN_BET || bet > MAX_BET);

    if (bet > userStats->balance) {
        printf("Ошибка: Недостаточно средств для совершения ставки.\n");
        return;
    }

    from = readInt("Введите начало диапазона:");

    do {
        to = readInt("Введите конец диапазона:");

        if (to < from) {
            printf("Ошибка: Конец диапазона должен быть больше начала. Попробуйте еще раз:\n");
        }
    } while (to < from);

    do {
        tryCount = readInt("Введите количество попыток:");

        if (tryCount < 1) {
            printf("Ошибка: Количество попыток должно быть больше 0. Попробуйте еще раз:\n");
        }
    } while (tryCount < 1);

    double coefficient = calculateCoefficient(tryCount, to - from);
    printf("Коэффициент: %f\n", coefficient);

    userStats->balance -= bet;

    int number = randomInt(from, to);
    int guess;

    for (size_t i = 0; i < tryCount; ++i) {
        printf("Попытка %zu из %d\n", i + 1, tryCount);
        guess = readInt("Введите число:");

        if (guess == number) {
            printf("Вы угадали!\n");
            userStats->balance += bet * coefficient;
            userStats->winCount++;
            break;
        } else if (guess < number) {
            printf("Загаданное число больше.\n");
        } else {
            printf("Загаданное число меньше.\n");
        }
    }

    if (guess != number) {
        printf("Вы проиграли. Загаданное число: %d\n", number);
        userStats->lossCount++;
    }

    printf("Повторить игру? (y/n)\n");
    char answer;
    do {
        scanf("%c", &answer);
    } while (answer != 'y' && answer != 'n');

    if (answer == 'y') {
        goto start_game;
    }

    printf("Спасибо за игру!\n");
}

void withdraw(UserStats *userStats) {
    printf("Вывод средств\n");
    printf("Ваш баланс в кредитах: %f\n", userStats->balance);
    printf("Ваш баланс в у.е.: %f\n", userStats->balance / SWAP_RATIO);
    printf("Минимальная сумма вывода: %d\n", MIN_WITHDRAW);
    printf("Курс обмена: %d у.е. = %f к.\n", 1, SWAP_RATIO);
    printf("\n");

    double withdraw;

    while (1) {
        withdraw = readDouble("Введите сумму вывода (в у.е.):");

        if (withdraw < MIN_WITHDRAW) {
            printf("Ошибка: Сумма вывода должна быть больше %d. Попробуйте еще раз.\n", MIN_WITHDRAW);
        } else if (withdraw > userStats->balance) {
            printf("Ошибка: Недостаточно средств для вывода. Попробуйте еще раз.\n");
        } else {
            break;
        }
    }

    userStats->balance -= withdraw * SWAP_RATIO;
    printf("Средства успешно выведены!\n");
    printf("Ваш баланс в кредитах: %f\n", userStats->balance);
    printf("Ваш баланс в у.е.: %f\n", userStats->balance / SWAP_RATIO);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        showUsage(argv[0]);
        return 1;
    }

    srand(time(NULL));

    UserStats userStats;

    if (!isFileExist(STATS_FILE)) {
        initializeUserStats(&userStats);
    } else {
        loadUserStatsFromFile(STATS_FILE, &userStats);
    }

    const char *mode = argv[1];

    if (!strcmp(mode, DEPOSIT_MODE)) {
        deposit(&userStats);
    } else if (!strcmp(mode, PLAY_MODE)) {
        play(&userStats);
    } else if (!strcmp(mode, STATS_MODE)) {
        showUserStats(&userStats);
    } else if (!strcmp(mode, WITHDRAW_MODE)) {
        withdraw(&userStats);
    } else {
        fprintf(stderr, "Error: Неизвестный режим `%s`)\n", mode);

        showUsage(argv[0]);
        return 1;
    }

    dumpUserStatsToFile(STATS_FILE, &userStats);

    return 0;
}
