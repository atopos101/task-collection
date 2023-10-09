#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

struct T {
    void foo();
    void bar() const;
};

struct G {
    double value;
};

int callback(int x) {
    printf("get %d\n", x);
    return x * x;
}

void f(int type, void *value, ...) {
    switch (type) {
        case 0: {
            //! value is of type void*
            printf("0x%p\n", value);
        } break;
        case 1: {
            //! value is of type float
            printf("%a\n", (float)(uint32_t)(intptr_t)value);
        } break;
        case 2: {
            //! value is of type G* const
            printf("%a\n", ((G *const)value)->value);
        } break;
        case 3: {
            //! value is of type int(*)(int)
            va_list ap;
            va_start(ap, value);
            int param  = va_arg(ap, int);
            int retval = ((int (*)(int))value)(param);
            printf("f(%d) -> %d\n", param, retval);
            va_end(ap);
        } break;
        default: {
            puts("unknown type");
        } break;
    }
}

int main(int argc, char *argv[]) {
    void    *_1 = nullptr;
    int      _2 = 0;
    T *const _3 = nullptr;
    const T *_4 = nullptr;
    float    _5 = 0x114514.p0;
    G        _6 = {0x11451419198.p10};

    union cast_non_const {
        decltype(&T::foo) src;
        void             *ptr;
    };

    union cast_const {
        decltype(&T::bar) src;
        void             *ptr;
    };

    //! task 1
    /// rules:
    /// 1. void* -> any pointer type
    /// 2. non-const type -> const type

    _1 = (void *)int{};     //<! [safe] int -> void*
    _2 = (int)(intptr_t)_1; //<! [unsafe] void* -> int
    _1 = (void *)_3;        //<! [safe] T*const -> void*
    _1 = (void *)_4;        //<! [unsafe] const T* -> void*
    _1 = static_cast<void *>(const_cast<T *>(_4));

    //! task 2
    using nested_param_func  = int (*)(int (*)());
    using nested_retval_func = int (*())();

    //! task 3
    /// 非类成员指针直接强转即可

    //! task 4
    f(0, (void *)0x0baadf00d);
    f(1, (void *)(intptr_t) * (uint32_t *)&_5);
    f(2, &_6);
    f(3, (void *)callback, 9);
    f(-1, nullptr);

    //! task 5
    /// &T::foo -> void(T::*)()
    /// &T::bar -> void(T::*)() const
    /// 可以进行不安全的互转
    /// 1. reinterpret_cast<decltype(&T::foo)>(&T::bar);
    /// 2. reinterpret_cast<decltype(&T::bar)>(&T::foo);

    _1 = cast_non_const{.src = &T::foo}.ptr;
    _1 = cast_const{.src = &T::bar}.ptr;

    return 0;
}
