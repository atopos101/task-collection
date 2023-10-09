#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>

//! fn(n, n int args...)
void leading_varg_fn(int n, ...) {
    va_list ap;
    va_start(ap, n);
    putchar('{');
    for (int i = 0; i < n; ++i) {
        int value = va_arg(ap, int);
        printf(" %d", value);
    }
    puts(" }");
    va_end(ap);
}

//! fn(hint, ..., '\0')
void trailing_varg_fn(const char* hint, ...) {
    va_list ap;
    va_start(ap, hint);
    printf("%s: ", hint);
    char ch = 0;
    do {
        ch = (char)va_arg(ap, int);
        if (ch > 32 && ch < 127) {
            printf("'%c' ", ch);
        } else {
            printf("'\\%03o'", ch);
        }
    } while (ch != 0);
    putchar('\n');
    va_end(ap);
}

//! fn(hint, [ni, ni int args]... until ni == 0)
void variant_varg_fn(const char* hint, ...) {
    va_list ap;
    va_start(ap, hint);
    printf("%s: ", hint);
    int n = va_arg(ap, int);
    while (n != 0) {
        putchar('[');
        while (n-- > 0) {
            int value = va_arg(ap, int);
            printf(" %d", value);
        }
        printf(" ]");
        n = va_arg(ap, int);
    }
    putchar('\n');
    va_end(ap);
}

void print(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}

//! expect: fn(_, int, double)
//! actual: fn(_, double)
void unsafe_vargs(int placeholder, ...) {
    va_list ap;
    va_start(ap, placeholder);
    int    i = va_arg(ap, int);
    double d = va_arg(ap, double);
    printf("%d %a\n", i, d);
}

int main(int argc, char* argv[]) {
    //! task 1
    leading_varg_fn(3, 1, 2, 3);
    trailing_varg_fn("char-sequence", 'a', '\x3f', ' ', ')', 0);
    variant_varg_fn("int-sequence", 2, 114, 514, 1, 1919810, 0);

    //! task 2
    print("%s%*s%-6s-%4d", "Hello", 1, "", "World", 233);

    //! task 3
    /// 类型及类型的大小决定了变参结构如何从栈帧上读取数据，错误地使用不同字长的类型将导致
    /// 变参结构的指针错误地偏移，其一将导致数据地错误读取，其二将会对其后所有数据的读取产
    /// 生影响另一方面，非法的取参可能导致栈结构相关的数据泄露，如栈指针或函数返回地址等，
    /// 一旦这些数据被非法利用或意外写，将对程序运行造成严重的破坏。
    double value = 0x11451419198.p+10;
    unsafe_vargs(0, value);

    return 0;
}
