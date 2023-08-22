#include "number.h"
#include <cstring>

uint2022_t from_uint(uint32_t i) {
    uint2022_t ans;
    for (size_t j = 0; j < ans.SIZE; j++) {
        ans.mass[j] = 0;
    }
    if (i >= ans.MODUL) {
        ans.count = 2;
        ans.mass[0] = i % ans.MODUL;
        ans.mass[1] = i / ans.MODUL;
    } else {
        ans.count = 1;
        ans.mass[0] = i;
    }

    return ans;
}

uint2022_t from_string(const char* buff) {
    uint2022_t ans;
    size_t len_str = strlen(buff);
    if (len_str > ans.SIZE * ans.BUFFSIZE) {
        std::cerr << "Overflow";
        exit(1);
    }
    for (size_t i = 0; i < ans.SIZE; i++) {
        ans.mass[i] = 0;
    }
    char chank[ans.BUFFSIZE];
    for (size_t j = 0; j < ans.BUFFSIZE; j++) {
        chank[j] = '0';
    }
    int k = ans.BUFFSIZE - 1;
    for (int i = len_str - 1; i >= 0; i--) {
        chank[k] = buff[i];
        k--;
        if (i == 0) {
            while (k != -1) {
                chank[k] = '0';
                k--;
            }
        }
        if (k == -1) {
            ans.mass[ans.count] = static_cast<uint32_t>(atoi(chank));
            ans.count++;
            k = ans.BUFFSIZE - 1;
            for (size_t j = 0; j < ans.BUFFSIZE; j++) {
                chank[j] = '0';
            }
        }
    }

    return ans;
}

uint2022_t operator+(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    if (lhs.count > ans.SIZE) {
        std::cerr << "Overflow";
        exit(1);
    }
    // хранит переход через разряд
    size_t n = 0;
    for (size_t i = 0; i < ans.SIZE; i++) {
        ans.mass[i] = 0;
    }
    for (size_t i = 0; i < std::max(lhs.count, rhs.count); i++) {
        // можно не обрабатывать переполнение uint32_t, так как temporary<2**32 всегда
        size_t temporary = lhs.mass[i] + rhs.mass[i] + n;
        ans.mass[i] = temporary % ans.MODUL;
        n = temporary / ans.MODUL;
        ans.count++;
    }
    if (n != 0) {
        if (ans.count >= ans.SIZE) {
            std::cerr << "Overflow";
            exit(1);
        }
        ans.mass[ans.count] = n;
        ans.count++;
    }

    return ans;
}

uint2022_t operator-(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs < rhs) {
        std::cerr << "Overflow";
        exit(1);
    }
    uint2022_t ans;
    for (size_t i = 0; i < ans.SIZE; i++) {
        ans.mass[i] = 0;
    }
    // хранит сколько занимали у текущего разряда
    size_t n = 0;
    for (size_t i = 0; i < lhs.count; i++) {
        if ((lhs.mass[i] - n) < rhs.mass[i]) {
            ans.mass[i] = lhs.mass[i] + ans.MODUL - rhs.mass[i] - n;
            ans.count++;
            n = 1;
        } else {
            ans.mass[i] = lhs.mass[i] - rhs.mass[i] - n;
            ans.count++;
            n = 0;
        }
    }
    while (ans.mass[ans.count - 1] == 0) {
        ans.count--;
    }

    return ans;
}

uint2022_t operator*(const uint2022_t& lhs, const uint2022_t& rhs) {
    uint2022_t ans;
    // максимально при умножении может получиться lhs.count+rhs.count
    // минимальное кол-во чисел в полученном числе(это заметно из столбика).
    ans.count = lhs.count + rhs.count - 1;
    if (ans.count > ans.SIZE) {
        std::cerr << "Overflow";
        exit(1);
    }
    // массив доп.сумм при сложении частичных произведений
    size_t add_sum[ans.SIZE + 1];
    for (size_t i = 0; i < ans.SIZE; i++) {
        ans.mass[i] = 0;
        add_sum[i] = 0;
    }
    // сделаем лишние, чтобы проверить переполнение
    add_sum[ans.SIZE] = 0;
    for (size_t i = 0; i < rhs.count; i++) {
        // доп.сумма для произведения
        size_t n = 0;
        for (size_t j = 0; (j < lhs.count || n != 0); j++) {
            // переменная, хранящая значение частичных произведений(допускается превышение MODUL)
            size_t temporary;
            // переменная, хранящая значение суммы в каком то разряде(допускается превышение MODUL)
            size_t temporary2;
            if (j >= lhs.count) {
                temporary = n;
                n = 0;
                if ((i + j) >= ans.SIZE) {
                    std::cerr << "Overflow";
                    exit(1);
                }
                // каст к от uint32_t к size_t происходит чтобы избежать переполнения uint32_t
                temporary2 = static_cast<size_t>(ans.mass[i + j]) + temporary + add_sum[i + j];
                add_sum[i + j] = 0;
                ans.mass[i + j] = temporary2 % ans.MODUL;
                add_sum[i + j + 1] = add_sum[i + j + 1] + temporary2 / ans.MODUL;

            } else {
                temporary = static_cast<size_t>(lhs.mass[j]) * static_cast<size_t>(rhs.mass[i]) + n;
                n = temporary / ans.MODUL;
                temporary = temporary % ans.MODUL;
                temporary2 = static_cast<size_t>(ans.mass[i + j]) + temporary + add_sum[i + j];
                add_sum[i + j] = 0;
                ans.mass[i + j] = temporary2 % ans.MODUL;
                add_sum[i + j + 1] = add_sum[i + j + 1] + temporary2 / ans.MODUL;
            }
        }
    }
    if (add_sum[ans.SIZE] != 0) {
        std::cerr << "Overflow";
        exit(1);
    }
    if (ans.mass[ans.count] != 0) {
        ans.count++;
    }

    return ans;
}

// деление на короткое
uint2022_t operator/(const uint2022_t& lhs, const uint32_t rhs) {
    if (rhs == 0) {
        std::cerr << "Division by zero";
        exit(1);
    }
    uint2022_t ans;
    for (size_t i = 0; i < ans.SIZE; i++) {
        ans.mass[i] = 0;
    }
    // максимально в числе может быть count=lhs.count-1
    // максимально в числе может быть count=lhs.count (в случае когда старший разряд >= rhs)
    ans.count = lhs.count;
    // остаток от деления более старшего разряда
    size_t r;
    if (lhs.mass[lhs.count - 1] < rhs) {
        ans.count--;
        r = lhs.mass[lhs.count - 1];
    } else {
        ans.mass[lhs.count - 1] = lhs.mass[lhs.count - 1] / rhs;
        r = lhs.mass[lhs.count - 1] % rhs;
    }
    size_t temporary = ans.MODUL * r;
    for (int i = lhs.count - 2; i >= 0; i--) {
        temporary = temporary + static_cast<size_t>(lhs.mass[i]);
        ans.mass[i] = temporary / rhs;
        r = temporary % rhs;
        temporary = ans.MODUL * r;
    }

    return ans;
}

uint2022_t operator/(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (rhs == from_uint(0)) {
        std::cerr << "Division by zero";
        exit(1);

    }
    uint2022_t ans;
    for (size_t i = 0; i < ans.SIZE; i++) {
        ans.mass[i] = 0;
    }
    // следует  из умножения
    // минимальное кол-во чисел lhs.count-rhs.count(когда rhs*100...0(lhs.count-rhs.count нулей)уже больше lhs)
    // максимальное кол-во чисел: lhs.count-rhs.count+1
    ans.count = lhs.count - rhs.count + 1;
    ans.mass[ans.count - 1] = 1;
    if (lhs < ans * rhs) {
        ans.mass[ans.count - 1] = 0;
        ans.count--;
    }
    ans.mass[ans.count - 1] = 0;

    uint2022_t lhs_copy = lhs;
    uint2022_t rhs_copy = rhs;
    // переменная,показывающая насколько сейчас домножено rhs_copy
    uint2022_t x = from_uint(1);
    uint2022_t k = rhs_copy * from_uint(10);
    // примерный алгоритм:домножаем rhs_copy на 10, чтобы не превысило lhs_copy, и вычитаем пока это возможно.
    // потом уменьшаем rhs_copy и x
    while (lhs_copy >= k) {
        rhs_copy = k;
        k = k * from_uint(10);
        x = x * from_uint(10);
    }
    while (lhs_copy >= rhs) {
        while (lhs_copy >= rhs_copy) {
            lhs_copy = lhs_copy - rhs_copy;
            ans = ans + x;
        }
        x = x / 10;
        rhs_copy = rhs_copy / 10;
    }

    return ans;
}

bool operator==(const uint2022_t& lhs, const uint2022_t& rhs) {
    for (size_t i = 0; i < lhs.SIZE; i++) {
        if (lhs.mass[i] != rhs.mass[i]) {
            return false;
        }
    }

    return true;
}

bool operator!=(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs == rhs);
}

bool operator>=(const uint2022_t& lhs, const uint2022_t& rhs) {
    if (lhs.count < rhs.count) {
        return false;
    } else if (lhs.count > rhs.count) {
        return true;
    }
    for (int i = lhs.count - 1; i >= 0; i--) {
        uint32_t chank1 = lhs.mass[i];
        uint32_t chank2 = rhs.mass[i];
        if (chank1 < chank2) {
            return false;
        } else if (chank1 > chank2) {
            return true;
        }
    }

    return true;
}

bool operator<(const uint2022_t& lhs, const uint2022_t& rhs) {
    return !(lhs >= rhs);
}

std::ostream &operator<<(std::ostream& stream, const uint2022_t& value) {
    // если число пустое, выводится ноль
    if (value.count == 0) {
        printf("%d", 0);
    }
    for (int i = (value.count - 1); i >= 0; i--) {
        // обработка ведущих нулей
        if (i == value.count - 1) {
            printf("%d", value.mass[i]);
        } else {
            printf("%09d", value.mass[i]);
        }
    }

    return stream;
}
