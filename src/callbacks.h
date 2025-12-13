#include <gtk/gtk.h>

void setup_treeview_columns(GtkWidget *treeview);
void afficher_equipements(GtkWidget *treeview, const char *filename);
void on_buttonEnregistrer_clicked(GtkButton *button,gpointer user_data);

void
on_sreserve_buttonPrecedent_clicked    (GtkButton       *button,
                                        gpointer         user_data);

void
on_sreserve_buttonEnregistrer_clicked  (GtkButton       *button,
                                        gpointer         user_data);

void
on_sreserve_buttonQuitter_clicked      (GtkButton       *button,
                                        gpointer         user_data);

void
on_smodifbuttonPrecedent_clicked       (GtkButton       *button,
                                        gpointer         user_data);

void
on_smodifbuttonQuitter_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_swarnbuttonCancel_clicked           (GtkButton       *button,
                                        gpointer         user_data);

void
on_Swarnbuttonok_clicked               (GtkButton       *button,
                                        gpointer         user_data);

void
on_scherbuttonChercher_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_scherbuttonPrecedent_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_scherbuttonQuitter_clicked          (GtkButton       *button,
                                        gpointer         user_data);

void
on_scherbuttonModifier_clicked         (GtkButton       *button,
                                        gpointer         user_data);

void
on_scherbuttonSupprimer_clicked        (GtkButton       *button,
                                        gpointer         user_data);

void
on_smodifbuttonEnregistrer_clicked     (GtkButton       *button,
                                        gpointer         user_data);
