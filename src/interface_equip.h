#include <stdio.h>

typedef struct Reservation {
    char date[12];
    int heure_debut;
    int duree;
} Reservation;

typedef struct Equipement {
    char nom[32];
    char marque[32];
    char id[20];           
    int quantite;
    char id_centre[20];    
    char date_achat[24];
    char etat[20];         
    char utilisation[12];
    char type[12];
    float prix;            
    Reservation tab_reservation[16];
} Equipement;

int ajouter(const char * filename ,Equipement *eq);
supprimer(const char *filename, const char *id);
int rechercher(const char *filename, const char name[], Equipement result[]);
int reserver(const char * filename ,char* id , Reservation *rv);
int modifier(const char *filename, const char *id, Equipement *new_eq);
