# Image de base
FROM gcc:latest

# Créer dossier de travail
WORKDIR /app

# Copier les fichiers du projet
COPY . .

# Compiler le serveur
RUN make clean && make server

# Exposer le port
EXPOSE 8080

# Commande de lancement
CMD ["./server"]
