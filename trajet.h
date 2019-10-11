struct _city;

typedef struct _connexion {
	struct _city* arrivee;
	int distance;
	int duree;
	struct _connexion* next;
} connexion;

typedef struct _city {
	char* depart;
	connexion* next;
} city;

typedef struct _noeuds{
	struct _noeuds* parent;
	struct _noeuds* enfants;
	struct _noeuds* freres;
	city* depart;
	int distanceTotale;
	int dureeTotale;
	bool trouve;
} noeuds;

typedef struct _listeNoeuds{
	noeuds* noeud;
	struct _listeNoeuds* next;
} listeNoeuds;

//FONCTIONS

//Ouvre le fichier ville et crée un tableau de villes
void LectureVille(char* nomFichier, city** listeVille);

//copie un tableau de caractere dans un autre tableau
void StringCopy(char* mot1, char* mot2, int taille);

//alloue au noeuds les différents parametres de la structure avec les infos rentrer en parametre 
noeuds* AllocationNoeud(city* depart, noeuds* parent, noeuds* frere, int distanceTotale, int dureeTotale);

//alloue à la city les differents parametre de la structure donner en parametre 
city* AllocationCity(char* depart);

//Converti la durée du fichier CSV sous forme h:min en minute
int ConvertDuree(char* duree);

//trouve l index du tableau des villes où on met les connexions et met 
void RechercherEtMettreConnexion(city** listeVille, connexion* connex, char* depart);

//donne le contenu du tableau des villes quand on recherche le nom de ma ville d arrivée rentré en parametre 
city* RechercherArrivee(city** listeVille, char* arrivee);

//alloue à la connexion les differents parametre de la structure donner en parametre
connexion* AllocationConnexion( city** listeVille,char* depart, char* arrivee, char* distance, char* duree);

//Recupere les info du fichier CSV
const char* getfield(char* line, int num);

//creer l arbre 
noeuds* CreerArbre(city** listeVille, char* depart, char* arrivee,int etapeMax, listeNoeuds* trajet);


//met le boolean a true pour tous les noeuds du trajet lorsque la ville d'arrivée est trouvé
void CheminTrouvee(noeuds* noeud);

//Alloue les valeurs du noeud rentré en parametre à la liste chainée  
listeNoeuds* AllocationListeNoeud(noeuds* noeud, listeNoeuds* next);

//ajoute à la liste chainé de noeud , un noeud final dont le trajet est vraie 
void ListeAjouteeNoeudTrajet(noeuds* noeud, listeNoeuds* trajet);

//Creer une feuille de l'arbre si letape max est pas depassé
void CreerFeuille(noeuds* noeudParent, int etape, int etapeMax, char* arrivee, listeNoeuds* trajet);

//lis le fichier et appelle la fonction AllocationConnexion
void LectureConnexion(char* nomFichier, city** listeVille);

//prend en parametre le trajet a tester et le compare avec le meilleur 
//Si le trajet a tester est un meilleur trajet, remplace le meilleur et le renvoi
noeuds* ComparerValeurTrajet(noeuds*meilleurTrajet,noeuds* trajetATester, bool enTemps, bool enDistance);

//Parcours les trajets vrais et appel la fonction compare (soit au niveau du temps soit de la distance)
//Affiche le meilleur trajet  
void TrouverMeilleur(listeNoeuds* trajet, bool enTemps,bool enDistance);

//COnverti les minutes en un char h:min
char* ConvertDureeMinuteHeure(int duree);

//free les infos de l arbre a la fin de l utilisation 
void SupprmierArbre (noeuds* arbre);

//free les infos de la liste des noeuds 
void SupprimerTrajet (listeNoeuds* trajet);

//free les infos des CSV
void SupprimerListeVille(city** listeVille, int taille);