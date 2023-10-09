#include <stdio.h>

/// 利用对称性枚举参数个数
/// NOTE: `, ## __VA_ARGS__` 的作用为强制插入第一个空宏参数
/// NOTE: 以下宏在参数小于等于 3 个时有效
/// NOTE: C++ 中模板参指定可能导致推断异常，如 nargs(T<1, 2>::value) 将给出错误结果 2
#define narg_impl(_3, _2, _1, _0, n, ...) (n)
#define nargs(...) narg_impl(, ##__VA_ARGS__, 3, 2, 1, 0)

/// 根据参数个数分发方法
#define nargs_unsafe(...) narg_unsafe_impl(, ##__VA_ARGS__, 3, 2, 1, 0)
#define narg_unsafe_impl(_3, _2, _1, _0, n, ...) n
#define var_fn_call(...) var_fn_call_impl(nargs_unsafe(__VA_ARGS__), __VA_ARGS__)
#define var_fn_call_impl(n, ...) dispatch(n, ## __VA_ARGS__)
#define dispatch(n, ...) dispatch_##n(__VA_ARGS__)
#define dispatch_0() puts("0")
#define dispatch_1(a) printf("%c\n", (a))
#define dispatch_2(a, b) printf("%d\n", (a) * (b))
#define dispatch_3(a, b, c) printf("%d | %d | %d\n", (a), (b), (c))

int main(int argc, char *argv[]) {
    //! task 1
    /// 当前行 __LINE__
    /// 当前文件绝对路径 __FILE__
    /// 当前文件名 __FILE_NAME__
    /// 当前函数名 __FUNCTION__
    /// 计数器 __COUNTER__
    /// 当前日期 __DATE_
    /// 当前时间 __TIME_
    /// 当前时间戳 __TIMESTAMP__
    /// 当前文件的包含深度 __INCLUDE_LEVEL__
    /// 当前 C++ 版本 __cplusplus
    /// GCC 版本号 __GNUC__
    /// MSVC 版本号 _MSC_VER
    /// Windows 平台宏定义 WIN32
    /// Linux 平台宏定义 __linux__
    /// IOS/Mac 平台宏定义 __APPLE__

    //! task 2
    /// 优先级 # > ## > 宏参数

    //! task 3
    printf("%d\n", nargs());
    printf("%d\n", nargs('a'));
    printf("%d\n", nargs('a', 'b'));
    printf("%d\n", nargs('a', 'b', 'c'));
    printf("%d\n", nargs('a', 'b', 'c', 233));
    printf("%d\n", nargs('a', 'b', 'c', 233, 'f'));

    //! task 4
    var_fn_call();
    var_fn_call('X');
    var_fn_call(3, 4);
    var_fn_call(1, 1, 4);

    return 0;
}
