#include <gtk/gtk.h>

void on_calculate(GtkWidget *widget, gpointer data) {
    GtkWidget **entries = (GtkWidget **)data;

    const char *num1_str = gtk_entry_get_text(GTK_ENTRY(entries[0]));
    const char *num2_str = gtk_entry_get_text(GTK_ENTRY(entries[1]));
    const char *op_str   = gtk_entry_get_text(GTK_ENTRY(entries[2]));
    GtkWidget *result_label = entries[3];

    double a = atof(num1_str);
    double b = atof(num2_str);
    double result = 0;

    if (strcmp(op_str, "+") == 0) result = a + b;
    else if (strcmp(op_str, "-") == 0) result = a - b;
    else if (strcmp(op_str, "*") == 0) result = a * b;
    else if (strcmp(op_str, "/") == 0) {
        if (b == 0) {
            gtk_label_set_text(GTK_LABEL(result_label), "Error: divide by zero");
            return;
        }
        result = a / b;
    }
    else {
        gtk_label_set_text(GTK_LABEL(result_label), "Invalid operator");
        return;
    }

    char buf[64];
    sprintf(buf, "Result: %.2f", result);
    gtk_label_set_text(GTK_LABEL(result_label), buf);
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);

    GtkWidget *window, *grid;
    GtkWidget *entry1, *entry2, *entry_op, *button, *label;

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GTK Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();
    entry_op = gtk_entry_new();
    label = gtk_label_new("Result: ");

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Num1:"), 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry1, 1, 0, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Num2:"), 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry2, 1, 1, 1, 1);

    gtk_grid_attach(GTK_GRID(grid), gtk_label_new("Op (+ - * /):"), 0, 2, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), entry_op, 1, 2, 1, 1);

    button = gtk_button_new_with_label("Calculate");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 2, 1);

    gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 2, 1);

    GtkWidget *entries[4] = {entry1, entry2, entry_op, label};
    g_signal_connect(button, "clicked", G_CALLBACK(on_calculate), entries);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
