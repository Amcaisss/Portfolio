import db
from flask import Flask, render_template, request, redirect, url_for, session, flash

app = Flask(__name__)

# Clé secrète utilisée pour chiffrer les cookies de session
app.secret_key = b'd2b01c987b6f7f0d5896aae06c4f318c9772d6651abff24aec19297cdf5eb199'



#page de connexion
@app.route('/connexion', methods=['GET', 'POST'])
def connexion():
    if request.method == 'POST':
        user = request.form.get('username')
        mdp = request.form.get('password')

        #si un champ n'est pas rempli
        if not user or not mdp:
            return render_template('connexion.html', error="Veuillez entrer un nom d'utilisateur et un mot de passe.")

        with db.connect() as conn:
            with conn.cursor() as cur:
                #trouver l'utilisateur
                cur.execute("SELECT idU FROM utilisateur WHERE pseudo=%s AND motdepasse=%s", (user, mdp))
                idU = cur.fetchone()

        if idU:
            # Stocker les informations utilisateur 
            session['user_id'] = idU[0]
            session['username'] = user
            return redirect(url_for('profil', idU=idU[0]))
        else:
            return render_template('connexion.html', error="Nom d'utilisateur ou mot de passe incorrect.")
    return render_template('connexion.html')



#page d'inscription
@app.route('/inscription', methods=['GET', 'POST'])
def inscription():
    if request.method == 'POST':
        pseudo = request.form.get('username')
        mdp = request.form.get('password')
        mail = request.form.get('mail')

        if not pseudo or not mdp or not mail:
            return render_template('inscription.html', error="Tous les champs sont obligatoires.")

        
        with db.connect() as conn:
            with conn.cursor() as cur:
                cur.execute("SELECT idU FROM utilisateur WHERE pseudo=%s", (pseudo,))
                exist = cur.fetchone()
                
                #verification du pseudo
                if exist:
                    return render_template('inscription.html', error="Nom d'utilisateur déjà pris.")

                #creation de l'utilisateur
                cur.execute(
                    "INSERT INTO utilisateur (pseudo, mail, motdepasse, dateArriver) VALUES (%s, %s, %s, CURRENT_DATE)",
                    (pseudo, mail, mdp))
                conn.commit()

                #trouver le nouvelle idU
                cur.execute("SELECT idU FROM utilisateur WHERE pseudo=%s", (pseudo,))
                idU = cur.fetchone()
            session['user_id'] = idU
            session['username'] = pseudo
            return redirect(url_for('profil', idU=idU))
    return render_template('inscription.html')




#page d'accueil
@app.route('/')
def TheWave():
    user_id = session.get('user_id')
    username = session.get('username')
    return render_template('theWave.html', user_id=user_id, username=username)




#profil de la personne connecter 
@app.route('/profil/<int:idU>')
def profil(idU):
    #verification de l'utilisateur
    if 'user_id' not in session or session['user_id'] != idU:
        return redirect(url_for('connexion'))

    with db.connect() as conn:
        with conn.cursor() as cur:
            #infromation de l'utilisateur 
            cur.execute("SELECT idU, pseudo, mail, dateArriver FROM utilisateur WHERE idU=%s", (idU,))
            utilisateur = cur.fetchone()

            #ses playlists
            cur.execute("SELECT idP, titre, description, prive FROM playlist WHERE idU=%s", (idU,))
            playlists = cur.fetchall()

            #ses trois musique préféré 
            cur.execute("""SELECT ecoute.idM, titre FROM ecoute NATURAL JOIN musique GROUP BY idM,titre,idU 
                        HAVING idu=%s ORDER BY count(ecoute.idM) DESC LIMIT 3 """,(idU,))
            ecoute=cur.fetchall()

            #ses dernière ecoute (historique)
            cur.execute("""SELECT ecoute.idE, ecoute.idU, musique.idM, musique.titre, musique.duree
                        FROM ecoute JOIN musique ON ecoute.idM = musique.idM JOIN album ON musique.idA = album.idA JOIN groupe ON album.idG = groupe.idG
                        WHERE ecoute.idU = %s ORDER BY ecoute.idE DESC""",(idU,))
            dernierE=cur.fetchall()

            #actualite de ses groupes suivis
            cur.execute("""SELECT groupe.idG,groupe.nom, album.titre, album.dateParution, idA FROM groupe NATURAL JOIN suitGroupe NATURAL JOIN album
                WHERE suitGroupe.idU = %s AND album.dateParution = ( SELECT MAX(album.dateParution) FROM album WHERE album.idG = groupe.idG
                )ORDER BY album.dateParution DESC""",(idU,))    
            actualG=cur.fetchall()

            #actualite de ses utilisateurs suivis
            cur.execute("""SELECT abonnement.idAbonne, utilisateur.pseudo , playlist.idP, playlist.titre, playlist.description
                    FROM utilisateur JOIN abonnement on utilisateur.idU=abonnement.idAbonne JOIN playlist ON abonnement.idAbonne = playlist.idU WHERE abonnement.idU = %s
                    AND playlist.idP = (SELECT MAX(playlist.idP) FROM playlist WHERE playlist.idU = abonnement.idAbonne AND playlist.prive = false
                    )AND playlist.prive = false""",(idU,))    
            actualP=cur.fetchall()
    if utilisateur:
        return render_template('profil.html', utilisateur=utilisateur, playlists=playlists,ecoute=ecoute,dernierE=dernierE, actualG=actualG,actualP=actualP)
    return redirect(url_for('connexion', error="Profil introuvable."))
   



#suggestion par rapport au profil connecter 
@app.route('/suggestion')
def suggestion():
    #trouver idU
    pseudo=session.get('user_id')
    if 'user_id' not in session :
        return redirect(url_for('connexion'))
    
    with db.connect() as conn:
        with conn.cursor() as cur:
            #les albums que l'utilisateur n'a pas ecouté de ses groupes préféré
            cur.execute("""SELECT album.idA, idG, groupe.nom, album.titre FROM groupe NATURAL JOIN album LEFT JOIN ecoute ON album.idA=ecoute.idA
                            GROUP BY album.idA,idG HAVING count(ecoute.idA)=0 and idG IN (SELECT idG FROM ecoute NATURAL JOIN album 
                            GROUP BY idG,idU  HAVING idU=%s ORDER BY count(ecoute.idU))""",(pseudo,))
            nvA=cur.fetchall()

            #les musiques qu'il n'a pas écouté dans les albums que l'utilisateur connait
            cur.execute("""SELECT DISTINCT m.idM, m.titre FROM musique m JOIN album a ON m.idA = a.idA WHERE (
                            a.idA IN (
                                SELECT e1.idA FROM ecoute e1 WHERE e1.idU = %s 
                            )
                            OR m.idA IN (
                                SELECT a2.idA FROM cree c JOIN musique m2 ON c.idM = m2.idM JOIN album a2 ON m2.idA = a2.idA
                                WHERE c.id IN (
                                    SELECT c2.id FROM cree c2 WHERE c2.idM IN (
                                        SELECT e3.idM FROM ecoute e3 WHERE e3.idU = %s
                                    )
                                )
                            )
                        )
                        AND m.idM NOT IN ( SELECT e4.idM FROM ecoute e4 WHERE e4.idU = %s  
                        )""",(pseudo,pseudo,pseudo,))
            nvM=cur.fetchall()

            #les playlist qui peuvent correspondre au gout de l'utilisateur
            cur.execute("""SELECT p.idP, p.titre, p.description
                FROM playlist p JOIN contient c ON p.idP = c.idP LEFT JOIN ecoute e ON c.idM = e.idM AND e.idU = %s
                WHERE p.prive = FALSE GROUP BY p.idP, p.titre, p.description HAVING COUNT(e.idM) >= COUNT(c.idM) / 2
                ORDER BY COUNT(e.idM) DESC;
                """,(pseudo,))
            playlistE=cur.fetchall()

            #les musiques qui des artistes préfére qu'il n'a pas encore écouté
            cur.execute("""SELECT DISTINCT m.idM, m.titre, a.nom AS artiste
                    FROM musique m JOIN cree c ON m.idM = c.idM JOIN artiste a ON c.id = a.id
                    WHERE c.id IN (
                        SELECT c1.id FROM cree c1 JOIN ecoute e ON c1.idM = e.idM WHERE e.idU = %s
                        GROUP BY c1.id ORDER BY COUNT(e.idM) DESC LIMIT 5 
                    )
                    AND m.idM NOT IN (
                        SELECT e1.idM FROM ecoute e1 WHERE e1.idU = %s
                    )
                    ORDER BY a.nom, m.titre""",(pseudo,pseudo,))
            musique=cur.fetchall()

            #les groupes que suivent les abonnee de l'utilisateur qui ont les même gout
            cur.execute("""SELECT g.idG, g.nom, g.genre, COUNT(sg2.idU) AS nb_suiveurs
                FROM groupe g JOIN suitGroupe sg2 ON g.idG = sg2.idG WHERE sg2.idU IN (
                    SELECT ab.idAbonne FROM abonnement ab WHERE ab.idU = %s 
                    AND ab.idAbonne IN (
                        SELECT sg1.idU FROM suitGroupe sg1 WHERE sg1.idG IN (
                            SELECT sg3.idG FROM suitGroupe sg3 WHERE sg3.idU = %s 
                        )
                    )
                )
                AND g.idG NOT IN (
                    SELECT sg4.idG FROM suitGroupe sg4 WHERE sg4.idU = %s
                )
                GROUP BY g.idG, g.nom, g.genre ORDER BY nb_suiveurs DESC, g.nom""",(pseudo,pseudo,pseudo,))
            grp=cur.fetchall()


    return render_template('suggestion.html',nvA=nvA, nvM=nvM, play=playlistE,musique=musique,grp=grp)




#toute les musiques, les 3 musiques les plus écouté, les groupe les plus connu, les nouveau album et les groupes 
@app.route('/musique')
def musique():
    #recuperer l'idU
    pseudo=session.get('user_id')
    with db.connect() as conn:
        if conn is not None:
            with conn.cursor() as cur:
                #toutes les musiques
                cur.execute("""SELECT idM, titre FROM musique 
                            ORDER BY titre,idM""")
                musiques=cur.fetchall()
     
                #tout les groupes
                cur.execute("""SELECT idG, nom FROM groupe """)
                groupes=cur.fetchall() 

                #les musiques les plus ecoutés
                cur.execute("""SELECT idM,titre FROM musique NATURAL JOIN nbecouteUnique 
                            GROUP BY idM,titre order by count(nbE) LIMIT 3""")
                ecouteM=cur.fetchall() 

                #les groupes les plus suivies
                cur.execute("""SELECT groupe.idG, groupe.nom, COUNT(DISTINCT suit.idU) AS nb_suivis 
                            FROM  groupe JOIN appartient ON groupe.idG = appartient.idG JOIN suit  ON appartient.id = suit.id
                            GROUP BY groupe.idG, groupe.nom ORDER BY nb_suivis DESC""")
                connuG=cur.fetchall() 

                #les derniers album sortie
                cur.execute("""SELECT idA,titre FROM album ORDER BY dateParution DESC LIMIT 2""")
                nvA=cur.fetchall()
           
    return render_template("musique.html", musiques=musiques, groupes=groupes,ecouteM=ecouteM,connu=connuG,nv=nvA)




#pages avec tout les utilisateurs
@app.route('/utilisateur')
def utili(): 
    with db.connect() as conn:
        with conn.cursor() as cur:
            #table avec les utilisateurs dans la base de données
            cur.execute("SELECT idU, pseudo, mail, dateArriver FROM utilisateur")
            utilisateur = cur.fetchall()

    return render_template("utilisateur.html", utilisateur=utilisateur)
 





#Les pages de description

#description des groupes
@app.route('/groupe/<int:idg>')
def groupe(idg):
    with db.connect() as conn:
        if conn is not None:
            with conn.cursor() as cur:
                #les informations importante du groupe
                cur.execute("SELECT idG, nom, nationalite, dateG, genre FROM groupe WHERE idG = %s", (idg,))
                groupe = cur.fetchone()
    
                #les artistes du groupe
                cur.execute("""
                    SELECT depart, arriver, role, artiste.id, artiste.nom, artiste.prenom
                    FROM appartient
                    NATURAL JOIN artiste
                    WHERE appartient.idG = %s AND depart IS NULL
                """, (idg,))
                artistes = cur.fetchall()

                #les artistes ayant quitté le groupe
                cur.execute("""
                    SELECT depart, arriver, role, artiste.id, artiste.nom, artiste.prenom
                    FROM appartient
                    NATURAL JOIN artiste
                    WHERE appartient.idG = %s AND depart IS NOT NULL
                """, (idg,))
                artisteout = cur.fetchall()
                     
                #les albums du groupe     
                cur.execute("""SELECT titre, idA, dateParution, description
                            FROM groupe NATURAL JOIN album WHERE idG = %s""", (idg,))
                album = cur.fetchall()   

                #le nombre de personne qui suivent le groupe
                cur.execute("""SELECT COUNT(DISTINCT suit.idU) AS nb_suivants
                        FROM suit JOIN artiste  ON suit.id = artiste.id JOIN appartient ON appartient.id = artiste.id
                        JOIN groupe  ON appartient.idG = groupe.idG WHERE groupe.idG = %s""", (idg,))
                nb = cur.fetchone()  

    return render_template("groupes.html", groupe=groupe, artistes=artistes, artisteout=artisteout, albums=album, nb=nb)




#description des albums
@app.route('/album/<int:ida>')
def album(ida):
    with db.connect() as conn:
        if conn is not None:
            with conn.cursor() as cur:
                #les informations importante de l'album
                cur.execute("SELECT idA,titre, description, dateParution, image_path FROM album WHERE idA= %s", (ida,))
                album = cur.fetchone()

                #les informations importante du groupe ayant cree l'album
                cur.execute("""
                    SELECT idG, nom, nationalite, dateG, genre
                    FROM album NATURAL JOIN groupe
                    WHERE album.idA = %s
                """, (ida,))
                groupe = cur.fetchall()
                     
                #les musiques dans l'album
                cur.execute("""SELECT  musique.idM,musique.titre, musique.duree, album.idA
                            FROM album JOIN musique ON album.idA = musique.idA JOIN groupe ON album.idG = groupe.idG
                            WHERE album.idA = %s
                            ORDER BY titre, idM """, (ida,))
                musiques = cur.fetchall()   

    return render_template("album.html", album=album, musiques=musiques, groupe=groupe)


    


#description des playlists
@app.route('/playlist/<int:idp>')
def playlist(idp):
    user_id = session.get('user_id')
    username = session.get('username')
    with db.connect() as conn:
        if conn is not None:
            with conn.cursor() as cur:
                #les informations importante de la playlist
                cur.execute("SELECT * FROM playlist WHERE idP = %s", (idp,))
                playlist = cur.fetchone()

                #les musiques contenu dans la playlist
                cur.execute("""
                    SELECT musique.titre, musique.duree, musique.idM
                    FROM musique
                    JOIN contient ON musique.idM = contient.idM
                    WHERE contient.idP = %s""", (idp,))
                musiques = cur.fetchall()
        else:
            playlist = None
            musiques = []
    return render_template("playlist.html", playlist=playlist, musiques=musiques, user_id=user_id, username=username)




#page sur un utilisateur precis
@app.route('/pageutilisateur/<int:idu>')
def user(idu): 
    user_id = session.get('user_id')
    username = session.get('username')
    with db.connect() as conn:
        with conn.cursor() as cur:
            #les informations importante de l'utilisateur
            cur.execute("SELECT idU, pseudo, mail, dateArriver FROM utilisateur WHERE idU=%s", (idu,))
            utilisateur = cur.fetchone()

            #les playlists cree par cette utilisateur
            cur.execute("SELECT idP, titre, description, prive FROM playlist WHERE prive=false AND idU=%s", (idu,))
            playlists = cur.fetchall()

    return render_template("pageUtilisateur.html", utilisateur=utilisateur,playlists=playlists)


#les informations des artistes
@app.route('/artiste/<int:id>')
def artiste(id):
    with db.connect() as conn:
        if conn is not None:
            with conn.cursor() as cur:
                #les informations importante de l'artiste
                cur.execute("""
                    SELECT id, nom, prenom, nationalite, naissance, mort
                    FROM artiste
                    WHERE id = %s
                """, (id,))
                artiste = cur.fetchone()

                #les groupes auquel il appartient
                cur.execute("""
                    SELECT groupe.idG, groupe.nom AS groupe_nom, groupe.dateG, appartient.arriver, appartient.depart, 
                           groupe.genre, appartient.role
                    FROM groupe
                    JOIN appartient ON groupe.idG = appartient.idG
                    WHERE appartient.id = %s
                """, (id,))
                groupes = cur.fetchall()

                #les musiques auquel il a participé
                cur.execute("""
                    SELECT musique.idM, musique.titre, musique.duree
                    FROM musique
                    JOIN cree ON musique.idM = cree.idM
                    WHERE cree.id = %s
                    ORDER BY musique.titre
                """, (id,))
                musiques = cur.fetchall()

        else:
            artiste = None
            groupes = []
            musiques = []

    return render_template("artiste.html", artiste=artiste, groupes=groupes, musiques=musiques)



#les information sur une musique
@app.route('/musiqueInfo/<int:idM>')
def musique_info(idM):
    with db.connect() as conn:
        if conn is not None:
            with conn.cursor() as cur:
                #les informations importante
                cur.execute("SELECT * FROM musique WHERE idM = %s", (idM,))
                musique = cur.fetchone()

                #les artistes qui ont participé
                cur.execute("""
                    SELECT artiste.id, artiste.nom, artiste.prenom
                    FROM artiste
                    JOIN cree ON artiste.id = cree.id
                    WHERE cree.idM = %s""", (idM,))
                artiste = cur.fetchall()
                
                #l'album auquel il appartient
                cur.execute("SELECT album.idA, album.titre FROM musique JOIN album ON musique.idA=album.idA WHERE idM = %s", (idM,))
                album= cur.fetchone()
        else:
            musique = None
            artiste = []
    return render_template("musique_info.html", musique=musique, artistes=artiste, album=album)






#pour les ecoutes

#ecoutes de musique
@app.route('/ecouter/<int:idM>')
def ecouter_musique(idM):
    if 'user_id' not in session:
        return redirect(url_for('connexion')) 
    #recuperer idu
    user_id = session.get('user_id')
    if not user_id:
        return redirect(url_for('connexion'))

    with db.connect() as conn:
        with conn.cursor() as cur:
            #insertion de l'ecoute dans la table ecoute
            cur.execute("INSERT INTO ecoute (idU, idM) VALUES (%s, %s)", (user_id, idM))
            conn.commit()

    return redirect(url_for('musique_info', idM=idM))




#ecoute de playlist
@app.route('/ecouter_playlist/<int:idp>', methods=['GET'])
def ecouter_playlist(idp):
    if 'user_id' not in session:
        return redirect(url_for('connexion')) 
    #recupere idU
    user_id = session.get('user_id')
    
    with db.connect() as conn:
        with conn.cursor() as cur:
            #prendre toutes les musiques d'une playlist
            cur.execute("""
                SELECT idM FROM musique NATURAL JOIN contient NATURAL JOIN playlist
                WHERE idP = %s
            """, (idp,))
            musiques = cur.fetchall()
            
            #inserer toute les musiques dans ecoutePLay
            for musique in musiques:
                cur.execute("""
                    INSERT INTO ecoutePlay (idU, idP, idM)
                    VALUES (%s, %s, %s)
                """, (user_id, idp, musique[0]))

                #inserer toute les musiques dans ecoute
                cur.execute("""
                        INSERT INTO ecoute (idU, idM)
                        VALUES (%s, %s)
                    """, (user_id, musique[0]))
                
            conn.commit()
    return redirect(url_for('playlist', idp=idp))




# ecoute d'album
@app.route('/ecouter_album/<int:ida>', methods=['GET'])
def ecouter_album(ida):
    #recuperer idU
    user_id = session.get('user_id')
    if not user_id:
        return redirect(url_for('connexion'))

    with db.connect() as conn:
        with conn.cursor() as cur:
            #prendre toutes les musiques d'un album
            cur.execute("""
                SELECT idM FROM musique WHERE idA = %s
            """, (ida,))
            musiques = cur.fetchall()

            #inserer toute les musiques dans ecoute
            for musique in musiques:
                cur.execute("""
                    INSERT INTO ecoute (idU, idM) VALUES (%s, %s)
                """, (user_id, musique))
            conn.commit()
    
    return redirect(url_for('album', ida=ida))
  
  




#creation, modification et recherche

#cree une playlist
@app.route('/creer_playlist', methods=['GET', 'POST'])
def cree_playlist():
    #recuperer idU
    user_id = session.get('user_id')
    
    with db.connect() as conn:
        with conn.cursor() as cur:
            #acces a toute les musiques disponible
            cur.execute("SELECT idM, titre FROM musique ORDER BY titre")
            musiques = cur.fetchall()
    
    if request.method == 'POST':
        titre = request.form.get('titre')
        description = request.form.get('description')
        prive = bool(request.form.get('prive'))
        selection = request.form.getlist('musiques') 

        if not titre:
            return render_template("creePlay.html", musiques=musiques)
 
        with db.connect() as conn:
            with conn.cursor() as cur:
                #inserer toute les informations entré dans playlist
                cur.execute("""
                    INSERT INTO playlist (titre, description, prive, idU) VALUES (%s, %s, %s, %s) RETURNING idP
                """, (titre, description, prive, user_id))
                playlist_id = cur.fetchone()

                #inserer toute les musique dans la playlist
                for idM in selection:
                    cur.execute("INSERT INTO contient (idM, idP) VALUES (%s, %s)", (idM, playlist_id))
                conn.commit()
        return redirect(url_for('profil', idU=user_id))
 
    return render_template("creePlay.html", musiques=musiques)




#modifier une playlist
@app.route('/edit_playlist/<int:idp>', methods=['GET', 'POST'])
def edit_playlist(idp):
    #recuperer idU
    user_id = session.get('user_id')

    with db.connect() as conn:
        with conn.cursor() as cur:
            #informations sur la playlist 
            cur.execute("SELECT titre, description, prive FROM playlist WHERE idP = %s AND idU = %s", (idp, user_id))
            playlist = cur.fetchone()

            #les musiques de la playlist
            cur.execute("""
                SELECT musique.idM, musique.titre 
                FROM musique
                JOIN contient ON musique.idM = contient.idM
                WHERE contient.idP = %s
            """, (idp,))
            playlist_musiques = cur.fetchall()

            #toutes les musiques 
            cur.execute("SELECT idM, titre FROM musique ORDER BY titre")
            musiques = cur.fetchall()


    if request.method == 'POST':
        titre = request.form.get('titre')
        description = request.form.get('description')
        prive = bool(request.form.get('prive'))
        selection = request.form.getlist('musiques')


        with db.connect() as conn:
            with conn.cursor() as cur:
                #modification de la playlist
                cur.execute("""
                    UPDATE playlist SET titre = %s, description = %s, prive = %s WHERE idP = %s AND idU = %s
                """, (titre, description, prive, idp, user_id))

                #enlever toutes les musiques
                cur.execute("DELETE FROM contient WHERE idP = %s", (idp,))

                #remettre les musiques choisit 
                for idM in selection:
                    cur.execute("INSERT INTO contient (idM, idP) VALUES (%s, %s)", (idM, idp))
                conn.commit()

        return redirect(url_for('profil', idU=user_id))
    return render_template("editer.html", playlist=playlist, playlist_musiques=playlist_musiques, musiques=musiques)



#recherche 
@app.route("/recherche", methods=['GET', 'POST'])
def recherche():
    #recuperer informations de l'utilisateur 
    user_id = session.get('user_id')
    username = session.get('username')

    choix1 = request.form.get('choix1')  
    choix2 = request.form.get('choix2') 
    options = [] 

    if request.method == 'POST':
        if choix1 and not choix2: 
            with db.connect() as conn:
                with conn.cursor() as cur:
                    #regarder ce qu'il recherche et donner toute les informations
                    if choix1 == 'musique':
                        cur.execute("SELECT idM, titre FROM musique ORDER BY titre")
                    elif choix1 == 'groupe':
                        cur.execute("SELECT idG, nom FROM groupe ORDER BY nom")
                    elif choix1 == 'album':
                        cur.execute("SELECT idA, titre FROM album ORDER BY titre")
                    elif choix1 == 'utilisateur':
                        cur.execute("SELECT idU, pseudo FROM utilisateur")
                    elif choix1 == 'artiste':
                        cur.execute("SELECT id, nom FROM artiste ")
                    elif choix1 == 'playlist':
                        cur.execute("SELECT idP, titre FROM playlist ")
                    options = cur.fetchall()

        elif choix1 and choix2: 
            #renvoyer vers la page choisis
            if choix1 == 'musique':
                return redirect(url_for('musique_info', idM=choix2))
            elif choix1 == 'groupe':
                return redirect(url_for('groupe', idg=choix2))
            elif choix1 == 'album':
                return redirect(url_for('album', ida=choix2)) 
            elif choix1 == 'utilisateur':
                return redirect(url_for('user', idu=choix2)) 
            elif choix1 == 'artiste':
                return redirect(url_for('artiste', id=choix2)) 
            elif choix1 == 'playlist':
                return redirect(url_for('playlist', idp=choix2)) 
        else:
            return render_template("recherche.html",choix1=choix1,choix2=choix2,options=options,user_id=user_id, username=username)
    return render_template("recherche.html",choix1=choix1,choix2=choix2,options=options,user_id=user_id,username=username )





#suivit

#suivit d'artiste
@app.route('/suivre/<int:ida>', methods=['POST'])
def suivre(ida):
    #recuperer idU
    user_id = session.get('user_id')  
    if not user_id:
        return redirect(url_for('connexion')) 

    with db.connect() as conn:
        with conn.cursor() as cur:
            #verifier que il ne le suit pas deja
            cur.execute("SELECT suit.idU FROM suit WHERE idU = %s AND id = %s", (user_id, ida))
            suivre= cur.fetchone()

            #inserer le suivit
            if not suivre: 
                cur.execute("INSERT INTO suit (idU, id) VALUES (%s, %s)", (user_id, ida))
                conn.commit()
    return redirect(url_for('artiste',id=ida)) 




#suivit d'utilisateur
@app.route('/suivreUtilisateur/<int:idu>', methods=['POST'])
def suivreU(idu):
    #recuperer idU
    user_id = session.get('user_id')  
    if not user_id:
        return redirect(url_for('connexion')) 

    with db.connect() as conn:
        with conn.cursor() as cur:
            #verifier que il ne le suit pas deja
            cur.execute("SELECT abonnement.idU FROM abonnement WHERE idU = %s AND idAbonne = %s", (user_id, idu))
            suivre = cur.fetchone()

            #inserer le suivit
            if not suivre: 
                cur.execute("INSERT INTO abonnement (idU, idAbonne) VALUES (%s, %s)", (user_id, idu))
                conn.commit()
    return redirect(url_for('user',idu=idu)) 




#suivre un groupe
@app.route('/suivreGroupe/<int:idg>', methods=['POST'])
def suivreG(idg):
    #recuperer idU
    user_id = session.get('user_id')  
    if not user_id:
        return redirect(url_for('connexion')) 

    with db.connect() as conn:
        with conn.cursor() as cur:
            #verifier que il ne le suit pas deja
            cur.execute("SELECT suitGroupe.idU FROM suitGroupe WHERE idU = %s AND idG = %s", (user_id, idg))
            suivre = cur.fetchone()

            #inserer le suivit
            if not suivre: 
                cur.execute("INSERT INTO suitGroupe (idU, idG) VALUES (%s, %s)", (user_id, idg))
                conn.commit()
    return redirect(url_for('groupe',idg=idg)) 





#se deconnecter
@app.route('/logout')
def logout():
    session.clear() 
    return redirect(url_for('TheWave'))



if __name__ == '__main__':
    app.run(debug=True)
