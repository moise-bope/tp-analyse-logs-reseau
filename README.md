## **Projet Analyse des Logs Réseau**



1. ###### **Description**



Ce projet permet d’analyser les logs réseau pour extraire des statistiques importantes à partir d'un fichier des logs (network_log.txt):



* Nombre total de déconnexions
* Nombre de succès et d’échecs
* Ports les plus utilisés
* Adresses IP les plus actives
* Détection d’adresses IP suspectes (plus de 5 échecs sur un même port)
* Génération d’un rapport : rapport_analyse.txt



Le programme est écrit en C et peut être complété par un script Python pour visualiser ou traiter les résultats.







###### **2. Structure du dépôt**



tp-analyse-logs-reseau



&nbsp;	── network_log.txt

&nbsp;	── analyse_logs.c

&nbsp;	── analyse_logs.py

&nbsp;	── rapport_analyse.txt

&nbsp;	── README.md

###### 



###### **3. Compilation et exécution du programme C**



* **Compiler le programme C**

&nbsp;	Dans le dossier du projet, ouvrir l'invite de commande et executer : gcc analyse_logs.c -o analyse_logs

&nbsp;	Cela génère un exécutable **analyse_logs.exe**



* **Exécuter le programme**
* 
**&nbsp;**	Executer l'exécutable : **analyse_logs.exe**

&nbsp;	Le programme lira **network_log.txt**, calculera les statistiques et générera le fichier **rapport_analyse.txt**.





###### **4. Exécution du script Python**



	Dans le dossier du projet, ouvrir l'invite de commande et executer : **python analyse_logs.py**

&nbsp;	Comme le programme C, ce script va lire le **network_log.txt**, calculera les statistiques et générera le fichier **rapport_analyse.txt**.





###### **5. Exemple d’utilisation**



Avec le fichier network\_logs.txt, voici les résultats affichés à l'écran par le programme c et le script python



+---------------------------------------------------------------------------------+

================================

STATISTIQUES

================================

1\. Nombre total deconnexions :

&nbsp; - 84



2\. Nombre total de succes :

&nbsp; - 59



3\. Nombre total d echecs :

&nbsp; - 25



4\. Port le plus utilise :

&nbsp; - 22



5\. Adresse IP la plus active :

&nbsp; - 192.168.1.10

&nbsp; - 192.168.1.20

&nbsp; - 192.168.1.40



================================

+---------------------------------------------------------------------------------+



Et voici le rapport : rapport\_analyse.txt

+---------------------------------------------------------------------------------+



RAPPORT D'ANALYSE DES LOGS 

=========================== 





1\. Nombre total déconnexions

----------------------------

&nbsp;- 84 



2\. Nombre total de succès

-------------------------

&nbsp;- 59 



3\. Nombre total d'échecs

------------------------

&nbsp;- 25 



4\. Port le plus utilisé

-----------------------

&nbsp;- 22 



5\. Adresse IP la plus active

----------------------------

&nbsp;- 192.168.1.10 

&nbsp;- 192.168.1.20 

&nbsp;- 192.168.1.40 



6\. Adresses IP suspectes

------------------------

&nbsp;- Adresse IP : 192.168.1.10, Port : 22, Tentatives : 6

&nbsp;- Adresse IP : 192.168.1.20, Port : 21, Tentatives : 6

&nbsp;- Adresse IP : 192.168.1.40, Port : 22, Tentatives : 6



7\. Top trois des ports les plus utilisés

----------------------------------------

&nbsp;- Port : 22, Connexions : 22 

&nbsp;- Port : 443, Connexions : 11 

&nbsp;- Port : 80, Connexions : 10 

+---------------------------------------------------------------------------------+





###### **6. Lien du dépôt**



https://github.com/moise-bope/tp-analyse-logs-reseau







###### **MOISE BOPE KWETE**

###### **MASTER - 1 TAR**

###### **18bk035**







