#include <gtk/gtk.h>

enum {
    COL_NOM,
    COL_ACTION,
    NUM_COLS
};

void on_treeview_row_activated(GtkTreeView *treeview,
                               GtkTreePath *path,
                               GtkTreeViewColumn *col,
                               gpointer user_data)
{
    GtkTreeModel *model = gtk_tree_view_get_model(treeview);
    GtkTreeIter iter;
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gchar *nom;
        gchar *action;
        gtk_tree_model_get(model, &iter, COL_NOM, &nom, COL_ACTION, &action, -1);

        g_print("Action '%s' sur '%s'\n", action, nom);

        g_free(nom);
        g_free(action);
    }
}

int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkWidget *treeview;
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

    gtk_init(&argc, &argv);

    // ---- Fenêtre principale
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "TreeView avec bouton");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 200);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // ---- TreeView
    treeview = gtk_tree_view_new();
    gtk_container_add(GTK_CONTAINER(window), treeview);

    // Colonne Nom
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("Nom", renderer, "text", COL_NOM, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // Colonne Action simulée (texte cliquable)
    renderer = gtk_cell_renderer_text_new();
    g_object_set(renderer,
                 "foreground", "blue",
                 "underline", PANGO_UNDERLINE_SINGLE,
                 NULL);
    column = gtk_tree_view_column_new_with_attributes("Action", renderer, "text", COL_ACTION, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

    // ---- Données
    store = gtk_list_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, COL_NOM, "Utilisateur 1", COL_ACTION, "Modifier", -1);

    gtk_list_store_append(store, &iter);
    gtk_list_store_set(store, &iter, COL_NOM, "Utilisateur 2", COL_ACTION, "Supprimer", -1);

    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
    g_object_unref(store);

    // ---- Signal sur clic (double-clic ou Entrée)
    g_signal_connect(treeview, "row-activated", G_CALLBACK(on_treeview_row_activated), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
