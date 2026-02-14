def lire_logs(fichier) :
    logs = []
    
    with open(fichier, "r") as f :
        for ligne in f :
            date, heure, adresse_ip, port, protocole, statut = ligne.strip().split(";")
            
            logs.append({
                "date" : date,
                "heure" : heure,
                "adresse_ip" : adresse_ip,
                "port" : port,
                "protocole" : protocole,
                "statut" : statut
            })

    return logs


def calculer_statistiques(logs) :
    nbre_total = len(logs)
    nbre_succes = 0
    nbre_echec = 0

    ports = {}
    adresses_ip = {}
    suspects = {}

    for log in logs :
        adresse_ip = log["adresse_ip"]
        port = log["port"]
        statut = log["statut"]

        if statut == "SUCCES" :
            nbre_succes += 1
        else :
            nbre_echec += 1

        ports[port] = ports.get(port, 0) + 1
        adresses_ip[adresse_ip] = adresses_ip.get(adresse_ip, 0) + 1

        if statut == "ECHEC" :
            if adresse_ip not in suspects :
                suspects[adresse_ip] = {}
            suspects[adresse_ip][port] = suspects[adresse_ip].get(port, 0) + 1

    max_ports = max(ports.values())
    ports_plus = [p for p, v in ports.items() if v == max_ports]

    max_addresses_ip = max(adresses_ip.values())
    adresses_ip_plus = [a for a, v in adresses_ip.items() if v == max_addresses_ip]

    ip_suspects = detecter_suspect(suspects)

    top_ports = sorted(ports.items(), key= lambda x : x[1], reverse = True)[:3]

    results = {
        "nbre_total" : nbre_total,
        "nbre_succes" : nbre_succes,
        "nbre_echec" : nbre_echec,
        "port_plus" : ports_plus,
        "adresse_ip_plus" : adresses_ip_plus,
        "ip_suspects" : ip_suspects,
        "top_ports" : top_ports
    }

    afficher_statistiques(results)
    return results


def detecter_suspect(list_suspects) :
    ip_suspects = []
    for ip in list_suspects :
        for port in list_suspects[ip] :
            if list_suspects[ip][port] > 5 :
                ip_suspects.append((ip, port, list_suspects[ip][port]))

    return ip_suspects


def afficher_statistiques(statistiques) :
    print("\n================================")
    print("STATISTIQUES")
    print("================================\n")

    print("1. Nombre total deconnexions : ")
    print("  - ", statistiques["nbre_total"], "\n")

    print("2. Nombre total de succes : ")
    print("  - ", statistiques["nbre_succes"], "\n")

    print("3. Nombre total d echecs : ")
    print("  - ", statistiques["nbre_echec"], "\n")

    print("4. Port le plus utilise : ")
    for port in statistiques["port_plus"]:
        print("  - ", port)
    print("")

    print("5. Adresse IP la plus active : ")
    for adresse_ip in statistiques["adresse_ip_plus"]:
        print("  - ", adresse_ip)
    print("")
    print("================================\n")


def generer_rapport(statistiques) :
    with open("rapport_analyse.txt", "w") as f :

        f.write("RAPPORT D'ANALYSE DES LOGS \n")
        f.write("=========================== \n\n\n")

        f.write(f"1. Nombre total déconnexions\n")
        f.write(f"----------------------------\n")
        f.write(f" - {statistiques['nbre_total']} \n\n")

        f.write(f"2. Nombre total de succès\n")
        f.write(f"-------------------------\n")
        f.write(f" - {statistiques['nbre_succes']} \n\n")

        f.write(f"3. Nombre total d'échecs\n")
        f.write(f"------------------------\n")
        f.write(f" - {statistiques['nbre_echec']} \n\n")

        f.write(f"4. Port le plus utilisé\n")
        f.write(f"-----------------------\n")
        for port in statistiques['port_plus'] :
           f.write(f" - {port} \n")

        f.write(f"\n")

        f.write(f"5. Adresse IP la plus active\n")
        f.write(f"----------------------------\n")
        for address_ip in statistiques['adresse_ip_plus']:
            f.write(f" - {address_ip} \n")

        f.write(f"\n")

        f.write(f"6. Adresses IP suspectes\n")
        f.write(f"------------------------\n")
        for adresse_ip in statistiques['ip_suspects']:
            f.write(f" - Adresse IP : {adresse_ip[0]}, Port : {adresse_ip[1]}, Tentatives : {adresse_ip[2]} \n")

        f.write(f"\n")

        f.write(f"7. Top trois des ports les plus utilisés\n")
        f.write(f"----------------------------------------\n")
        for port in statistiques['top_ports']:
            f.write(f" - Port : {port[0]}, Connexions : {port[1]} \n")

        f.write(f"\n")


def main() :
    logs = lire_logs("network_log.txt")
    statistiques = calculer_statistiques(logs)
    generer_rapport(statistiques)


if __name__ == "__main__" :
    main()