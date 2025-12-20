#include <gtk/gtk.h>

// 1. 버튼 클릭 시 호출될 콜백 함수 (이벤트 핸들러)
static void on_button_clicked(GtkWidget *widget, gpointer data) {
    g_print("버튼이 클릭되었습니다! (터미널 확인)\n");
}

int main(int argc, char *argv[]) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *box; // 위젯을 담을 컨테이너

    // 2. GTK 초기화 (필수)
    gtk_init(&argc, &argv);

    // 3. 윈도우 생성 및 설정
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "우분투 GUI 연습");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    // 윈도우의 'X' 버튼을 누르면 프로그램이 종료되도록 연결
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // 4. 레이아웃 박스 생성 (수직 정렬)
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    // 5. 버튼 생성 및 이벤트 연결
    button = gtk_button_new_with_label("클릭해보세요");
    // "clicked" 시그널이 발생하면 on_button_clicked 함수 실행
    g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);

    // 박스에 버튼 추가
    gtk_box_pack_start(GTK_BOX(box), button, TRUE, TRUE, 0);

    // 6. 모든 위젯 화면에 표시
    gtk_widget_show_all(window);

    // 7. 메인 이벤트 루프 실행 (대기 상태)
    gtk_main();

    return 0;
}
