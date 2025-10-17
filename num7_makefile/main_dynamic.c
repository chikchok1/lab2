#include <stdio.h>
#include <dlfcn.h>

int main() {
    void *handle;
    int (*add)(int,int);
    int (*sub)(int,int);
    int (*mul)(int,int);
    double (*divi)(int,int);

    handle = dlopen("./libarith.so", RTLD_LAZY);
    if (!handle) {
        printf("Failed to load library\n");
        return 1;
    }

    add  = dlsym(handle, "add");
    sub  = dlsym(handle, "subtract");
    mul  = dlsym(handle, "multiply");
    divi = dlsym(handle, "divide");

    int a = 10, b = 2;
    printf("Add: %d\n", add(a,b));
    printf("Subtract: %d\n", sub(a,b));
    printf("Multiply: %d\n", mul(a,b));
    printf("Divide: %.2f\n", divi(a,b));

    dlclose(handle);
    return 0;
}

