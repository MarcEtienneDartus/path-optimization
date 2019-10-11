#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "trajet.h"

void StringCopy(char* mot1, char* mot2, int taille){
	for(int index = 0; index < taille; index++)
	{
		mot1[index] = mot2[index];
	}
	mot1[taille-1] = '\0';
}

noeuds* AllocationNoeud(city* depart, noeuds* parent, noeuds* frere, int distanceTotale, int dureeTotale){
	noeuds* noeud = (noeuds*) malloc(sizeof(noeuds));
	noeud->depart = depart;
	noeud->trouve = false;
	noeud->parent = parent;
	noeud->distanceTotale = distanceTotale;
	noeud->dureeTotale = dureeTotale;
	noeud->enfants = NULL;
	noeud->freres = frere;
	return noeud;
	
}

city* AllocationCity(char* depart){
	city* ville = (city*) malloc(sizeof(city));
	int taille = strlen(depart);
	char* departCopy = (char*) malloc(taille*sizeof(char));
	StringCopy(departCopy,depart,taille);
	ville->depart = departCopy;
	ville->next = NULL;
	return ville;
}

int ConvertDuree(char* duree){
	int dureeTotale = 0;
	if(duree != NULL)
	{
		dureeTotale += (int) duree[4] -'0';
		dureeTotale += ((int) duree[3] -'0')*10;
		dureeTotale += ((int) duree[1] -'0')*60;
		dureeTotale += ((int) duree[0] -'0')*600;
	}
	return dureeTotale;
}

char* ConvertDureeMinuteHeure(int duree){
	
	char* chaine=(char*)malloc(6*sizeof(char));
	if(duree != NULL)
	{
		chaine[0] = duree/600+'0';
		duree = duree%600;
		chaine[1] = duree/60+'0';
		duree = duree%60;
		chaine[2] = 'h';
		chaine[3] = duree/10+'0';
		duree = duree%10;
		chaine[4] = duree+'0';
	}
	chaine[5]='\0';
	return chaine;
}

void RechercherEtMettreConnexion(city** listeVille, connexion* connex, char* depart){
	int index = 0;
	bool estTrouve = false;
	while(!estTrouve)
	{
		if(strcmp(listeVille[index]->depart,depart)==0)
		{
			if(listeVille[index]->next != NULL)
			{
				connex->next = listeVille[index]->next;
			}
			listeVille[index]->next = connex;
			estTrouve = true;
		}
		index++;
	}
}

city* RechercherArrivee(city** listeVille, char* arrivee){
	int index = 0;
	bool estTrouve = false;
	while(!estTrouve)
	{
		if(strcmp(listeVille[index]->depart,arrivee)==0)
		{
			estTrouve = true;
		}
		else
		{
			index++;
		}
	}
	return listeVille[index];
}

connexion* AllocationConnexion( city** listeVille,char* depart, char* arrivee, char* distance, char* duree){
	connexion* connex = (connexion*) malloc(sizeof(connexion));
	connex->arrivee = RechercherArrivee(listeVille,arrivee);
	connex->distance = strtol(distance,NULL,10);
	connex->duree = ConvertDuree(duree);
	connex->next = NULL;
	RechercherEtMettreConnexion(listeVille,connex,depart);
	return connex;
}

const char* getfield(char* line, int num){
    const char* tok;
    for (tok = strtok(line, ",");
            tok && *tok;
            tok = strtok(NULL, ",\n"))
    {
        if (!--num)
            return tok;
    }
    return NULL;
}

noeuds* CreerArbre(city** listeVille, char* depart, char* arrivee,int etapeMax, listeNoeuds* trajet){
	int index = 0;
	bool estTrouve = false;
	while(!estTrouve) //pour trouver le pointeur de la ville de départ
	{
		if(strcmp(listeVille[index]->depart,depart)==0)
		{
			estTrouve = true;
		}
		else
		{
			index++;
		}
	}
	noeuds* noeud = AllocationNoeud(listeVille[index],NULL,NULL,0,0);
	CreerFeuille(noeud,0,etapeMax,arrivee,trajet);
	return noeud;
}

void CheminTrouvee(noeuds* noeud){
	if(noeud != NULL && noeud->trouve!= true)
	{
		noeud->trouve= true;
		CheminTrouvee(noeud->parent);
	}
}

listeNoeuds* AllocationListeNoeud(noeuds* noeud, listeNoeuds* next){
	listeNoeuds* listeNoeud = (listeNoeuds*) malloc(sizeof(listeNoeuds));
	listeNoeud->noeud = noeud;
	listeNoeud->next = next;
	return listeNoeud;
}

void ListeAjouteeNoeudTrajet(noeuds* noeud, listeNoeuds* trajet){
	if(trajet->noeud == NULL)
	{
		trajet->noeud = noeud;
	}
	else
	{
		trajet->next = AllocationListeNoeud(noeud,trajet->next);
	}
}

void CreerFeuille(noeuds* noeudParent, int etape, int etapeMax, char* arrivee, listeNoeuds* trajet){
	if(etape<=etapeMax)
	{
		connexion* nextConnexion = noeudParent->depart->next;
		noeuds* dernierFrere = NULL;
		while( nextConnexion != NULL){
			int distanceTotale = noeudParent->distanceTotale + nextConnexion->distance;
			int dureeTotale = noeudParent->dureeTotale + nextConnexion->duree;
			
			noeuds* tempParent = noeudParent;
			bool dejaPassee = false;
			while(tempParent != NULL && dejaPassee==false){
				if(strcmp(tempParent->depart->depart,nextConnexion->arrivee->depart)==0)
				{
					dejaPassee = true;
				}
				tempParent=tempParent->parent;
			}
			
			if(!dejaPassee)
			{
				dernierFrere = AllocationNoeud(nextConnexion->arrivee,noeudParent,dernierFrere,distanceTotale,dureeTotale);
				if(strcmp(nextConnexion->arrivee->depart,arrivee)!=0)
				{
					CreerFeuille(dernierFrere,etape+1,etapeMax,arrivee,trajet);
				}
				else
				{
					CheminTrouvee(dernierFrere);
					ListeAjouteeNoeudTrajet(dernierFrere,trajet);
				}
			}
			nextConnexion = nextConnexion->next;
		}
		noeudParent->enfants = dernierFrere;
	}
}

void LectureConnexion(char* nomFichier, city** listeVille){
	FILE * fichier = fopen ( nomFichier ,"r") ;
	if( fichier != NULL )
	{
		
		char texte[100];
		fgets( texte, 100, fichier);
		int index = 0;
		while ( fgets( texte, 100, fichier) != NULL )
		{
			char* tmp0 = strdup(texte);
			char* tmp1 = strdup(texte);
			char* tmp2 = strdup(texte);
			char* tmp3 = strdup(texte);
			connexion* connex = AllocationConnexion(listeVille,getfield(tmp0, 1),getfield(tmp1, 2),getfield(tmp2, 3),getfield(tmp3, 4));
			index++;
		}
		fclose ( fichier );
	}
	else
	{
		printf (" Probleme lors de l'ouverture du fichier %s\n", nomFichier ) ;
	}
}

noeuds* ComparerValeurTrajet(noeuds*meilleurTrajet,noeuds* trajetATester, bool enTemps, bool enDistance){
	if(trajetATester!=NULL)
	{
		if(enTemps)
		{
			if(meilleurTrajet->dureeTotale>trajetATester->dureeTotale) meilleurTrajet = trajetATester;
		}
		if(enDistance)
		{
			if(meilleurTrajet->distanceTotale>trajetATester->distanceTotale) meilleurTrajet = trajetATester;
		}
	}
	return meilleurTrajet;
}

void TrouverMeilleur(listeNoeuds* trajet, bool enTemps,bool enDistance){
	if(trajet != NULL && trajet->noeud != NULL)
	{
		noeuds* meilleurTrajet = trajet->noeud;
		trajet = trajet->next;
		while(trajet!=NULL)
		{
			meilleurTrajet = ComparerValeurTrajet(meilleurTrajet,trajet->noeud,enTemps,enDistance);
			trajet = trajet->next;
		}
		AfficherListeNoeudParent(meilleurTrajet,true);
	}
}

void LectureVille(char* nomFichier, city** listeVille){
	FILE * fichier = fopen ( nomFichier ,"r") ;
	if( fichier != NULL )
	{
		char texte[30];
		fgets( texte, 30, fichier);
		int index = 0;
		while ( fgets( texte, 30, fichier) != NULL )
		{
			listeVille[index] = AllocationCity(texte);
			
			index++;
		}
		fclose ( fichier );
	}
	else
	{
		printf (" Probleme lors de l'ouverture du fichier %s\n", nomFichier ) ;
	}
}

void SupprmierArbre (noeuds* arbre){
	if(arbre != NULL)
	{		
		SupprmierArbre(arbre->freres);
		noeuds * a_supprimer= arbre->freres;
		free(a_supprimer);
		SupprmierArbre(arbre->enfants);
		noeuds * a_supprimerbis= arbre->enfants;
		free(a_supprimerbis);
		
	}
	//else <=> arrêt de la récursivité
}

void SupprimerTrajet (listeNoeuds* trajet){
	while(trajet != NULL)
	{
		listeNoeuds* a_supprimer= trajet;
		trajet = trajet->next;
		free(a_supprimer);
	}
	
}

void SupprimerListeVille(city** listeVille, int taille){
	if(listeVille != NULL)
	{
		for(int index = 0; index < taille; index++) 
		{
			while( listeVille[index]->next != NULL)
			{
				connexion* tempConnexion = listeVille[index]->next;
				listeVille[index]->next = tempConnexion->next;
				free(tempConnexion);
			}
			printf("\n");
			free(listeVille[index]->depart);
			free(listeVille[index]);
		}
		free(listeVille);
	}
}