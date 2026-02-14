#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 256
#define MAX_IPS 200
#define MAX_PORT 65536

typedef struct {
    char date[20];
    char heure[20];
    char ip[20];
    int port;
    char protocole[10];
    char statut[10];
} Log;

typedef struct {
    int total;
    int succes;
    int echec;
    int port_plus[MAX_PORT];
    int nb_ports_plus;
    char ip_plus[MAX_IPS][20];
    int nb_ips_plus;
} Statistique;

typedef struct
{
    int port;
    int connexions;
} PortStat;

Log* lire_logs(const char *nomFichier, int *count)
{
    FILE *fichier = fopen(nomFichier, "r");

    if (!fichier)
    {
        printf("Erreur ouverture de fichier.\n");
        return NULL;
    }

    Log *logs = NULL;
    int capacite = 0;
    *count = 0;
    char ligne[MAX_LINE];

    while (fgets(ligne, sizeof(ligne), fichier))
    {
        if (*count >= capacite)
        {
            capacite = capacite == 0 ? 10 : capacite * 2;
            logs = realloc(logs, capacite * sizeof(Log));
            if (!logs)
            {
                printf("Erreur de memoire.\n");
                fclose(fichier);
                return NULL;
            }
        }

        char *valeur = strtok(ligne, ";");
        strcpy(logs[*count].date, valeur);

        valeur = strtok(NULL, ";");
        strcpy(logs[*count].heure, valeur);

        valeur = strtok(NULL, ";");
        strcpy(logs[*count].ip, valeur);

        valeur = strtok(NULL, ";");
        logs[*count].port = atoi(valeur);

        valeur = strtok(NULL, ";");
        strcpy(logs[*count].protocole, valeur);

        valeur = strtok(NULL, ";");
        strcpy(logs[*count].statut, valeur);
        logs[*count].statut[strcspn(logs[*count].statut, "\n")] = 0;

        (*count)++;
    }

    fclose(fichier);
    return logs;
}

void calculer_statistiques(Log *logs, int total, Statistique *stats, int port_counts[MAX_PORT],
                    char ips[MAX_IPS][20], int ip_counts[MAX_IPS], int *ip_total, int **suspect_counts)
{
    stats->total = total;
    stats->succes = 0;
    stats->echec = 0;
    *ip_total = 0;

    for (int i = 0; i < total; i++)
    {
        if (strcmp(logs[i].statut, "SUCCES") == 0)
        {
            stats->succes ++;
        }
        else
        {
            stats->echec ++;
        }

        port_counts[logs[i].port]++;

        int ip_index = -1;
        for (int j = 0; j < *ip_total; j++)
        {
            if (strcmp(ips[j], logs[i].ip) == 0)
            {
                ip_index = j;
                break;
            }
        }

        if (ip_index == -1)
        {
            strcpy(ips[*ip_total], logs[i].ip);
            ip_index = *ip_total;
            (*ip_total)++;
        }

        ip_counts[ip_index]++;

        if (strcmp(logs[i].statut, "ECHEC") == 0)
        {
            suspect_counts[ip_index][logs[i].port]++;
        }
    }

    int max_count = 0;
    stats->nb_ports_plus = 0;
    for (int i = 0; i < MAX_PORT; i++)
    {
        if (port_counts[i] > max_count)
        {
            max_count = port_counts[i];
        }
    }

    for (int i = 0; i < MAX_PORT; i++)
    {
        if (port_counts[i] == max_count && max_count > 0)
        {
            stats->port_plus[stats->nb_ports_plus] = i;
            stats->nb_ports_plus ++;
        }
    }

    int max_ip = 0;
    stats->nb_ips_plus = 0;

    for (int i = 0; i < *ip_total; i++)
    {
        if (ip_counts[i] > max_ip)
        {
            max_ip = ip_counts[i];
        }
    }

    for (int i = 0; i < *ip_total; i++)
    {
        if (ip_counts[i] == max_ip && max_ip > 0)
        {
            strcpy(stats->ip_plus[stats->nb_ips_plus], ips[i]);
            stats->nb_ips_plus ++;
        }
    }
}


void detecter_suspect(FILE *rapport, char ips[MAX_IPS][20], int ip_total, int **suspect_counts)
{
    for (int i = 0; i < ip_total; i++)
    {
        for (int p = 0; p < MAX_PORT; p++)
        {
            if (suspect_counts[i][p] > 5)
            {
                fprintf(rapport, " - Adresse IP : %s, Port : %d, Tentatives : %d\n",
                        ips[i], p, suspect_counts[i][p]);
            }
        }
    }
}

void top_trois_ports(FILE *rapport, int port_counts[MAX_PORT])
{
    PortStat ports[MAX_PORT];
    int nb_ports = 0;

    for (int i =0; i < MAX_PORT; i++)
    {
        if (port_counts[i] > 0)
        {
            ports[nb_ports].port = i;
            ports[nb_ports].connexions = port_counts[i];
            nb_ports++;
        }
    }

    for (int i = 0; i < nb_ports - 1; i++)
    {
        for (int j = i + 1; j < nb_ports; j++)
        {
            if (ports[j].connexions > ports[i].connexions)
            {
                PortStat tmp = ports[i];
                ports[i] = ports[j];
                ports[j] = tmp;
            }
        }
    }

    int top = nb_ports < 3 ? nb_ports : 3;
    for (int i = 0; i < top; i++)
    {
        fprintf(rapport, " - Port : %d, Connexions : %d \n", ports[i].port, ports[i].connexions);
    }
}

void generer_rapport(Statistique stats, char ips[MAX_IPS][20], int ip_total, int **suspect_counts, int port_counts[MAX_PORT])
{
    FILE *rapport = fopen("rapport_analyse.txt", "w");
    if (!rapport)
    {
        printf("Erreur creation rapport. \n");
        return ;
    }

    fprintf(rapport, "RAPPORT D'ANALYSE DES LOGS \n");
    fprintf(rapport, "=========================== \n\n\n");

    fprintf(rapport, "1. Nombre total déconnexions\n");
    fprintf(rapport, "----------------------------\n");
    fprintf(rapport, " - %d \n\n", stats.total);

    fprintf(rapport, "2. Nombre total de succès\n");
    fprintf(rapport, "-------------------------\n");
    fprintf(rapport, " - %d \n\n", stats.succes);

    fprintf(rapport, "3. Nombre total d'échecs\n");
    fprintf(rapport, "------------------------\n");
    fprintf(rapport, " - %d \n\n", stats.echec);

    fprintf(rapport, "4. Port le plus utilisé\n");
    fprintf(rapport, "-----------------------\n");
    for (int i = 0; i < stats.nb_ports_plus; i++)
    {
        fprintf(rapport, " - %d \n", stats.port_plus[i]);
    }
    fprintf(rapport, "\n");

    fprintf(rapport, "5. Adresse IP la plus active\n");
    fprintf(rapport, "----------------------------\n");
    for (int i = 0; i < stats.nb_ips_plus; i++)
    {
        fprintf(rapport, " - %s \n", stats.ip_plus[i]);
    }
    fprintf(rapport, "\n");

    fprintf(rapport, "6. Adresses IP suspectes\n");
    fprintf(rapport, "------------------------\n");
    detecter_suspect(rapport, ips, ip_total, suspect_counts);
    fprintf(rapport, "\n");

    fprintf(rapport, "7. Top trois des ports les plus utilisés\n");
    fprintf(rapport, "----------------------------------------\n");
    top_trois_ports(rapport, port_counts);
    fprintf(rapport, "\n");

    fclose(rapport);
}


int main()
{
    int total_logs = 0;
    Log *logs = lire_logs("network_log.txt", &total_logs);
    if (!logs)
    {
        return 1;
    }

    int port_counts[MAX_PORT] = {0};
    int ip_counts[MAX_IPS] = {0};
    int ip_total = 0;
    char ips[MAX_IPS][20];

    int **suspect_counts = malloc(MAX_IPS * sizeof(int *));
    for (int i = 0; i < MAX_IPS; i++)
    {
        suspect_counts[i] = calloc(MAX_PORT, sizeof(int));
    }

    Statistique stats;
    calculer_statistiques(logs, total_logs, &stats, port_counts, ips, ip_counts, &ip_total, suspect_counts);

    printf("\n================================");
    printf("\nSTATISTIQUES\n");
    printf("================================\n");

    printf("1. Nombre total deconnexions : \n");
    printf("  - %d \n\n", stats.total);

    printf("2. Nombre total de succes : \n");
    printf("  - %d \n\n", stats.succes);

    printf("3. Nombre total d echecs : \n");
    printf("  - %d \n\n", stats.echec);

    printf("4. Port le plus utilise : \n");
    for (int i = 0; i < stats.nb_ports_plus; i++)
    {
        printf("  - %d \n", stats.port_plus[i]);
    }
    printf("\n");

    printf("5. Adresse IP la plus active : \n");
    for (int i = 0; i < stats.nb_ips_plus; i++)
    {
        printf("  - %s \n", stats.ip_plus[i]);
    }
    printf("\n");
    printf("================================\n");


    generer_rapport(stats, ips, ip_total, suspect_counts, port_counts);


    for (int i = 0; i < MAX_IPS; i++)
    {
        free(suspect_counts[i]);
    }
    free(logs);
    return 0;
}