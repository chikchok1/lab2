#include <dbus/dbus.h>
#include <stdio.h>
#include <stdlib.h>
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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "사용법: %s \"보낼_알림_메시지\"\n", argv[0]);
        return 1;
    }

    const char *msg_text = argv[1];

    DBusError err;
    DBusConnection *conn;
    DBusMessage *msg, *reply;
    DBusMessageIter args;
    dbus_uint32_t serial = 0;

    dbus_error_init(&err);

    // 1) 세션 버스 접속
    conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
    die_if_error(&err, "dbus_bus_get 실패");
    if (!conn) {
        fprintf(stderr, "세션 버스 연결 실패\n");
        return 1;
    }

    // 2) 메서드 호출 메시지 생성
    msg = dbus_message_new_method_call(
        SERVICE_NAME,   // 대상 서비스 이름
        OBJECT_PATH,    // 오브젝트 경로
        INTERFACE_NAME, // 인터페이스
        METHOD_NAME     // 메서드 이름
    );

    if (!msg) {
        fprintf(stderr, "메시지 생성 실패\n");
        return 1;
    }

    // 3) 인자(알림 문자열) 추가
    dbus_message_iter_init_append(msg, &args);
    if (!dbus_message_iter_append_basic(&args, DBUS_TYPE_STRING, &msg_text)) {
        fprintf(stderr, "인자 추가 실패\n");
        exit(1);
    }

    // 4) 서버에 보내고 응답 기다리기
    reply = dbus_connection_send_with_reply_and_block(
        conn, msg, -1, &err);
    dbus_message_unref(msg);  // 보낸 뒤 해제
    die_if_error(&err, "메서드 호출 실패");

    // 5) 응답 문자열 받기
    const char *reply_text = NULL;
    if (reply) {
        DBusMessageIter rargs;
        dbus_message_iter_init(reply, &rargs);
        if (DBUS_TYPE_STRING == dbus_message_iter_get_arg_type(&rargs)) {
            dbus_message_iter_get_basic(&rargs, &reply_text);
        }
        dbus_message_unref(reply);
    }

    if (reply_text)
        printf("서버 응답: %s\n", reply_text);
    else
        printf("서버 응답 없음\n");

    return 0;
}
