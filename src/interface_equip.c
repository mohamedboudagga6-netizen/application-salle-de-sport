#include <string.h>
#include <stdlib.h>
#include "interface_equip.h"


int ajouter(const char *filename, Equipement *eq) 
{
    FILE *f = fopen(filename, "a");
    if (f != NULL)
    {
        fprintf(f, "%s|%s|%s|%d|%s|%s|%s|%s|%s|%.2f",
                eq->nom,
                eq->marque,
                eq->id,
                eq->quantite,
                eq->id_centre,
                eq->date_achat,
                eq->etat,
                eq->utilisation,
                eq->type,
                eq->prix);
        
        for (int i = 0; i < 16; i++) {
            fprintf(f, "|%s|%d|%d", eq->tab_reservation[i].date, 
                   eq->tab_reservation[i].heure_debut, 
                    eq->tab_reservation[i].duree);
        }
        
        fprintf(f, "\n");
        fclose(f);
        return 1;
    }
    return 0;
}

int reserver(const char *filename, char *name, Reservation *rv) {     

    printf("DEBUG: Ouverture du fichier %s ...\n", filename);

    FILE *f = fopen(filename, "r");     
    FILE *temp = fopen("temp.txt", "w");     

    if (f == NULL || temp == NULL) {         
        printf("ERREUR: Impossible d'ouvrir le fichier !\n");
        return 0;     
    }          

    Equipement e;     
    int trouver = 0;          

    printf("DEBUG: Début de lecture du fichier...\n");

    while (1) {
        int ret = fscanf(f, "%31[^|]|%31[^|]|%19[^|]|%d|%19[^|]|%23[^|]|%19[^|]|%11[^|]|%11[^|]|%f",
                         e.nom, e.marque, e.id, &e.quantite, e.id_centre,
                         e.date_achat, e.etat, e.utilisation, e.type, &e.prix);

        if (ret == EOF) {
            printf("DEBUG: Fin de fichier détectée.\n");
            break;
        }

        // Debug affichage des champs lus
        printf("\n-------------------------------------------\n");
        printf("DEBUG: Equipement lu depuis le fichier:\n");
        printf("  Nom          = %s\n", e.nom);
        printf("  Marque       = %s\n", e.marque);
        printf("  ID           = %s\n", e.id);
        printf("  Quantité     = %d\n", e.quantite);
        printf("  ID Centre    = %s\n", e.id_centre);
        printf("  Date Achat   = %s\n", e.date_achat);
        printf("  État         = %s\n", e.etat);
        printf("  Utilisation  = %s\n", e.utilisation);
        printf("  Type         = %s\n", e.type);
        printf("  Prix         = %f\n", e.prix);

        if (ret != 10) {
            printf("DEBUG: ERREUR: fscanf n'a pas lu 10 éléments. ret=%d\n", ret);
            // Skip this malformed line
            char buffer[2048];
            fgets(buffer, sizeof(buffer), f);
            continue;
        }

        for (int i = 0; i < 16; i++) {
            int ret2 = fscanf(f, "|%11[^|]|%d|%d",
                e.tab_reservation[i].date,
                &e.tab_reservation[i].heure_debut,
                &e.tab_reservation[i].duree);

            if (ret2 != 3) {
                printf("DEBUG: fscanf RESERVATION interrompu à l'index %d (ret2=%d)\n", i, ret2);
                break;
            }

            printf("  Reservation[%d]: date=%s, heure_debut=%d, duree=%d\n",
                   i,
                   e.tab_reservation[i].date,
                   e.tab_reservation[i].heure_debut,
                   e.tab_reservation[i].duree);
        }

        // Skip newline
        fscanf(f, "\n");

        // Vérification nom
        if (strcmp(e.nom, name) == 0) {
            printf("DEBUG: >>> Equipement correspondant trouvé ! (nom=%s)\n", name);
            trouver = 1;

            int added = 0;

            for (int i = 0; i < 16; i++) {
                printf("DEBUG: Vérification du slot reservation[%d] (date=%s)\n",
                       i, e.tab_reservation[i].date);

                // Case vide
                if (strlen(e.tab_reservation[i].date) == 0 ||
                    strcmp(e.tab_reservation[i].date, "0") == 0) {

                    printf("DEBUG: SLOT VIDE trouvé: ajout de la nouvelle réservation.\n");

                    strcpy(e.tab_reservation[i].date, rv->date);
                    e.tab_reservation[i].heure_debut = rv->heure_debut;
                    e.tab_reservation[i].duree = rv->duree;

                    added = 1;
                    break;
                }
            }

            if (!added) {
                printf("ERREUR: Aucun slot de réservation libre.\n");
                fclose(f);
                fclose(temp);
                remove("temp.txt");
                return 0;
            }
        }

        // Écriture dans temp
        fprintf(temp, "%s|%s|%s|%d|%s|%s|%s|%s|%s|%.2f",
                e.nom, e.marque, e.id, e.quantite, e.id_centre,
                e.date_achat, e.etat, e.utilisation, e.type, e.prix);

        for (int i = 0; i < 16; i++) {
            fprintf(temp, "|%s|%d|%d",
                e.tab_reservation[i].date,
                e.tab_reservation[i].heure_debut,
                e.tab_reservation[i].duree);
        }

        fprintf(temp, "\n");
    }
    
    fclose(f);
    fclose(temp);
    remove(filename);
    rename("temp.txt", filename);
    return trouver;
}

int rechercher(const char *filename, const char name[], Equipement result[]) {
    FILE *f;
    Equipement e;
    int count = 0;
    
    printf("DEBUG: Recherche d'équipements avec le nom: '%s'\n", name);
    
    f = fopen(filename, "r");
    if (f == NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    
    // Read all equipment from file
    while (1) {
        int ret = fscanf(f, "%31[^|]|%31[^|]|%19[^|]|%d|%19[^|]|%23[^|]|%19[^|]|%11[^|]|%11[^|]|%f",
                         e.nom, e.marque, e.id, &e.quantite, e.id_centre,
                         e.date_achat, e.etat, e.utilisation, e.type, &e.prix);
        
        if (ret == EOF) {
            break;
        }
        
        if (ret != 10) {
            // Skip malformed lines
            char buffer[2048];
            fgets(buffer, sizeof(buffer), f);
            continue;
        }
        
        // Read the reservation data
        for (int i = 0; i < 16; i++) {
            int ret2 = fscanf(f, "|%11[^|]|%d|%d",
                e.tab_reservation[i].date,
                &e.tab_reservation[i].heure_debut,
                &e.tab_reservation[i].duree);
            
            if (ret2 != 3) {
                // Initialize empty reservation
                strcpy(e.tab_reservation[i].date, "0");
                e.tab_reservation[i].heure_debut = 0;
                e.tab_reservation[i].duree = 0;
            }
        }
        fscanf(f, "\n");
        
        // Check if the name contains the search string (case insensitive)
        char nom_lower[32], search_lower[32];
        int i;
        
        // Convert equipment name to lowercase
        for (i = 0; e.nom[i] && i < 31; i++) {
            nom_lower[i] = tolower(e.nom[i]);
        }
        nom_lower[i] = '\0';
        
        // Convert search string to lowercase
        for (i = 0; name[i] && i < 31; i++) {
            search_lower[i] = tolower(name[i]);
        }
        search_lower[i] = '\0';
        
        // If name contains search string, add to results
        if (strstr(nom_lower, search_lower) != NULL) {
            // Copy the equipment to result array
            strcpy(result[count].nom, e.nom);
            strcpy(result[count].marque, e.marque);
            strcpy(result[count].id, e.id);
            result[count].quantite = e.quantite;
            strcpy(result[count].id_centre, e.id_centre);
            strcpy(result[count].date_achat, e.date_achat);
            strcpy(result[count].etat, e.etat);
            strcpy(result[count].utilisation, e.utilisation);
            strcpy(result[count].type, e.type);
            result[count].prix = e.prix;
            
            // Copy reservations
            for (int j = 0; j < 16; j++) {
                strcpy(result[count].tab_reservation[j].date, e.tab_reservation[j].date);
                result[count].tab_reservation[j].heure_debut = e.tab_reservation[j].heure_debut;
                result[count].tab_reservation[j].duree = e.tab_reservation[j].duree;
            }
            
            printf("DEBUG: Équipement trouvé: %s %s (ID: %s)\n", 
                   result[count].nom, result[count].marque, result[count].id);
            
            count++;
            
            // Safety check to prevent buffer overflow
            if (count >= 100) {
                printf("AVERTISSEMENT: Limite de 100 résultats atteinte\n");
                break;
            }
        }
    }
    
    fclose(f);
    
    printf("DEBUG: Recherche terminée. %d équipement(s) trouvé(s)\n", count);
    return count;
}

int supprimer(const char *filename, const char *id) {
    FILE *f;
    FILE *temp;
    Equipement e;
    int found = 0;
    
    printf("DEBUG: Suppression de l'équipement avec ID: '%s'\n", id);
    
    f = fopen(filename, "r");
    if (f == NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    
    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("ERREUR: Impossible de créer le fichier temporaire\n");
        fclose(f);
        return 0;
    }
    
    // Read all equipment from file
    while (1) {
        int ret = fscanf(f, "%31[^|]|%31[^|]|%19[^|]|%d|%19[^|]|%23[^|]|%19[^|]|%11[^|]|%11[^|]|%f",
                         e.nom, e.marque, e.id, &e.quantite, e.id_centre,
                         e.date_achat, e.etat, e.utilisation, e.type, &e.prix);
        
        if (ret == EOF) {
            break;
        }
        
        if (ret != 10) {
            // Skip malformed lines
            char buffer[2048];
            fgets(buffer, sizeof(buffer), f);
            continue;
        }
        
        // Read the reservation data
        for (int i = 0; i < 16; i++) {
            int ret2 = fscanf(f, "|%11[^|]|%d|%d",
                e.tab_reservation[i].date,
                &e.tab_reservation[i].heure_debut,
                &e.tab_reservation[i].duree);
            
            if (ret2 != 3) {
                strcpy(e.tab_reservation[i].date, "0");
                e.tab_reservation[i].heure_debut = 0;
                e.tab_reservation[i].duree = 0;
            }
        }
        fscanf(f, "\n");
        
        // Check if this is the equipment to delete
        if (strcmp(e.id, id) == 0) {
            printf("DEBUG: Équipement trouvé et sera supprimé: %s %s (ID: %s)\n", 
                   e.nom, e.marque, e.id);
            found = 1;
            // Don't write this equipment to temp file (skip it)
            continue;
        }
        
        // Write this equipment to temp file (keep it)
        fprintf(temp, "%s|%s|%s|%d|%s|%s|%s|%s|%s|%.2f",
                e.nom, e.marque, e.id, e.quantite, e.id_centre,
                e.date_achat, e.etat, e.utilisation, e.type, e.prix);
        
        for (int i = 0; i < 16; i++) {
            fprintf(temp, "|%s|%d|%d",
                e.tab_reservation[i].date,
                e.tab_reservation[i].heure_debut,
                e.tab_reservation[i].duree);
        }
        
        fprintf(temp, "\n");
    }
    
    fclose(f);
    fclose(temp);
    
    // Replace original file with temp file
    if (found) {
        remove(filename);
        rename("temp.txt", filename);
        printf("DEBUG: Équipement supprimé avec succès\n");
    } else {
        remove("temp.txt");
        printf("DEBUG: Équipement non trouvé\n");
    }
    
    return found;
}

int modifier(const char *filename, const char *id, Equipement *new_eq) {
    FILE *f;
    FILE *temp;
    Equipement e;
    int found = 0;
    
    printf("DEBUG: Modification de l'équipement avec ID: '%s'\n", id);
    
    f = fopen(filename, "r");
    if (f == NULL) {
        printf("ERREUR: Impossible d'ouvrir le fichier %s\n", filename);
        return 0;
    }
    
    temp = fopen("temp.txt", "w");
    if (temp == NULL) {
        printf("ERREUR: Impossible de créer le fichier temporaire\n");
        fclose(f);
        return 0;
    }
    
    // Read all equipment from file
    while (1) {
        int ret = fscanf(f, "%31[^|]|%31[^|]|%19[^|]|%d|%19[^|]|%23[^|]|%19[^|]|%11[^|]|%11[^|]|%f",
                         e.nom, e.marque, e.id, &e.quantite, e.id_centre,
                         e.date_achat, e.etat, e.utilisation, e.type, &e.prix);
        
        if (ret == EOF) {
            break;
        }
        
        if (ret != 10) {
            // Skip malformed lines
            char buffer[2048];
            fgets(buffer, sizeof(buffer), f);
            continue;
        }
        
        // Read the reservation data
        for (int i = 0; i < 16; i++) {
            int ret2 = fscanf(f, "|%11[^|]|%d|%d",
                e.tab_reservation[i].date,
                &e.tab_reservation[i].heure_debut,
                &e.tab_reservation[i].duree);
            
            if (ret2 != 3) {
                strcpy(e.tab_reservation[i].date, "0");
                e.tab_reservation[i].heure_debut = 0;
                e.tab_reservation[i].duree = 0;
            }
        }
        fscanf(f, "\n");
        
        // Check if this is the equipment to modify
        if (strcmp(e.id, id) == 0) {
            printf("DEBUG: Équipement trouvé et sera modifié: %s %s (ID: %s)\n", 
                   e.nom, e.marque, e.id);
            found = 1;
            
            // Write the modified equipment to temp file
            fprintf(temp, "%s|%s|%s|%d|%s|%s|%s|%s|%s|%.2f",
                    new_eq->nom, new_eq->marque, new_eq->id, new_eq->quantite, 
                    new_eq->id_centre, new_eq->date_achat, new_eq->etat, 
                    new_eq->utilisation, new_eq->type, new_eq->prix);
            
            // Keep the old reservations
            for (int i = 0; i < 16; i++) {
                fprintf(temp, "|%s|%d|%d",
                    e.tab_reservation[i].date,
                    e.tab_reservation[i].heure_debut,
                    e.tab_reservation[i].duree);
            }
            
            fprintf(temp, "\n");
        } else {
            // Write this equipment unchanged to temp file
            fprintf(temp, "%s|%s|%s|%d|%s|%s|%s|%s|%s|%.2f",
                    e.nom, e.marque, e.id, e.quantite, e.id_centre,
                    e.date_achat, e.etat, e.utilisation, e.type, e.prix);
            
            for (int i = 0; i < 16; i++) {
                fprintf(temp, "|%s|%d|%d",
                    e.tab_reservation[i].date,
                    e.tab_reservation[i].heure_debut,
                    e.tab_reservation[i].duree);
            }
            
            fprintf(temp, "\n");
        }
    }
    
    fclose(f);
    fclose(temp);
    
    // Replace original file with temp file
    if (found) {
        remove(filename);
        rename("temp.txt", filename);
        printf("DEBUG: Équipement modifié avec succès\n");
    } else {
        remove("temp.txt");
        printf("DEBUG: Équipement non trouvé\n");
    }
    
    return found;
}
