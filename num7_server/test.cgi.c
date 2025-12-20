#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

    char *method = getenv("REQUEST_METHOD");
    printf("<h1>CGI Test OK</h1>\n");
    printf("<p>REQUEST_METHOD = %s</p>\n", method);

    if (method && strcmp(method, "POST") == 0) {
        char buf[1024];
        fgets(buf, sizeof(buf), stdin);
        printf("<p>POST_DATA = %s</p>\n", buf);
    }

    return 0;
}
