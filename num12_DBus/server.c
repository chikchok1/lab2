#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SERVICE_NAME   "com.example.DBusNotifyServer"
#define OBJECT_PATH    "/com/example/DBusNotify"
#define INTERFACE_NAME "com.example.DBusNotify"
#define METHOD_NAME    "Notify"

static void die_if_error(DBusError *err, const char *msg) {
    if (dbus_error_is_set(err)) {
        fprintf(stderr, "%s: %s\n", msg, err->message);
        dbus_error_free(err);
        exit(1);
    }
}

int main(void) {
    DBusError err;
    DBusConnection *conn;
    int ret;

    dbus_error_init(&err);

    // 1) 세션 버스 접속
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    die_if_error(&err, "dbus_bus_get 실패");
    if (!conn) {
        fprintf(stderr, "세션 버스 연결 실패\n");
        return 1;
    }

    // 2) 서비스 이름 요청
    ret = dbus_bus_request_name(conn, SERVICE_NAME,
                                DBUS_NAME_FLAG_REPLACE_EXISTING, &err);
    die_if_error(&err, "dbus_bus_request_name 실패");
    if (ret != DBUS_REQUEST_NAME_REPLY_PRIMARY_OWNER) {
        fprintf(stderr, "서비스 이름 획득 실패\n");
        return 1;
    }

    printf("DBus 알림 서버 시작됨. (서비스: %s)\n", SERVICE_NAME);

    // 3) 메시지 루프
    while (1) {
        // 새 메시지 대기 (타임아웃 100ms)
        dbus_connection_read_write(conn, 100);

        DBusMessage *msg = dbus_connection_pop_message(conn);
        if (msg == NULL) {
            // 메시지 없으면 잠깐 쉼
            usleep(100000);
            continue;
        }

        // 3-1) 우리가 원하는 메서드 호출인지 확인
        if (dbus_message_is_method_call(msg, INTERFACE_NAME, METHOD_NAME)) {
            const char *text = NULL;

            DBusMessageIter args;
            dbus_message_iter_init(msg, &args);
            if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&args)) {
                dbus_message_iter_get_basic(&args, &text);
            }

            if (text) {
                printf("[알림 수신] %s\n", text);
                // 여기서 system("notify-send ...") 같은 걸 써서
                // 실제 GUI 알림으로 띄워도 됨.
            } else {
                printf("[알림 수신] (빈 문자열)\n");
            }

            // 3-2) 클라이언트에게 응답 보내기 (간단히 "OK")
            DBusMessage *reply;
            DBusMessageIter reply_args;

            reply = dbus_message_new_method_return(msg);
            dbus_message_iter_init_append(reply, &reply_args);

            const char *ok = "OK";
            if (!dbus_message_iter_append_basic(&reply_args, DBUS_TYPE_STRING, &ok)) {
                fprintf(stderr, "응답 메시지 생성 실패\n");
                exit(1);
            }

            if (!dbus_connection_send(conn, reply, NULL)) {
                fprintf(stderr, "응답 전송 실패\n");
                exit(1);
            }
            dbus_connection_flush(conn);
            dbus_message_unref(reply);
        }

        dbus_message_unref(msg);
    }

    return 0;
}
