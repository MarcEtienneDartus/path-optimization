#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "trajet.h"

void AfficherCity(city** listeVille, int taille){
	if(listeVille != NULL)
	{
		for(int index = 0; index < taille; index++) 
		{
			printf("%s", listeVille[index]->depart);
			connexion* tempConnexion = listeVille[index]->next;
			while( tempConnexion != NULL)
			{
				printf("\n");
				printf("	%s",tempConnexion->arrivee->depart);
				printf(" %d",tempConnexion->distance);
				printf(" %d",tempConnexion->duree);
				tempConnexion = tempConnexion->next;
			}
			printf("\n");
		}
	}
}

void AffichageNoeud(noeuds* noeud){
	printf("noeud : %p",noeud);
	printf(" %s",noeud->depart->depart);
	printf(" %d",noeud->distanceTotale);
	printf(" %d",noeud->dureeTotale);
	printf(" %d",noeud->trouve);
	printf(" %p",noeud->parent);
	printf(" %p",noeud->freres);
	printf(" %p\n",noeud->enfants);
}

void AfficherListeNoeudParent(noeuds* noeud, bool first){
	if(noeud!=NULL)
	{
		AfficherListeNoeudParent(noeud->parent,false);
		if(first)
		{
		 printf("%s",noeud->depart->depart);
		 printf(" avec une duree de %s et une distance de :%d",ConvertDureeMinuteHeure(noeud->dureeTotale),noeud->distanceTotale);
		}
		else
		{
			printf("%s->",noeud->depart->depart);
		}
	}
}

void AfficherListeNoeud(listeNoeuds* listeNoeud){
	while(listeNoeud!=NULL)
	{
		AfficherListeNoeudParent(listeNoeud->noeud,true);
		printf("\n");
		listeNoeud = listeNoeud->next;
	}
}

int DemanderVille(char* depart, char* arrivee){
	printf("Ecrivez la ville de depart : ");
	scanf("%s",depart);
	printf("Ecrivez la ville de d'arrivee : ");
	scanf("%s",arrivee);
	printf("\n");
	int profondeur = 0;
	printf("Quelle profondeur de recherche voulez vous ?");
	scanf("%d",&profondeur);
	return profondeur;
}

void AffichageArbre(noeuds* noeud, int etape){
	for(int i=0; i<etape;i++ )
	{
		printf("	");
	}
	printf("%s",noeud->depart->depart);
	printf(" %d",noeud->distanceTotale);
	printf(" %d",noeud->dureeTotale);
	noeud->trouve ? printf(" true") : printf(" false");
	printf("\n");
	if(noeud->enfants!=NULL)
	{
		AffichageArbre(noeud->enfants,etape+1);
	}
	if(noeud->freres!=NULL)
	{
		AffichageArbre(noeud->freres,etape);
	}
}

void AffichageArbreTrouvee(noeuds* noeud, int etape){
	if(noeud->trouve)
	{
		for(int i=0; i<etape;i++ )
		{
			printf("	");
		}
		printf("%s\n",noeud->depart->depart);
	}
	if(noeud->enfants!=NULL)
	{
		if(noeud->trouve)
		{
			AffichageArbreTrouvee(noeud->enfants,etape+1);
		}
	}
	if(noeud->freres!=NULL)
	{
		AffichageArbreTrouvee(noeud->freres,etape);
	}
}

void Menu(){
	int nbVille = 23;
	city** listeVille = (city**) malloc(nbVille*sizeof(city*));
	LectureVille("villes.csv",listeVille);
	LectureConnexion("connexions.csv",listeVille);
	
	char depart[30];
	char arrivee[30];
	int profondeur = DemanderVille(depart,arrivee);
	listeNoeuds* trajet = AllocationListeNoeud(NULL,NULL);
	noeuds* arbre  = CreerArbre(listeVille,depart,arrivee,profondeur,trajet);
	
	int choixFonction = 0;
	do
	{
		printf("\n");
		printf("#1 Afficher un trajet trouve\n");
		printf("#2 Afficher tous les trajets trouves\n");
		printf("#3 Afficher le trajet le plus court\n");
		printf("#4 Afficher le trajet le plus rapide\n");
		printf("#5 Afficher l arbre des trajets\n");
		printf("#6 Quitter\n");
		printf("Votre choix : ");
		scanf("%d",&choixFonction);
		
		printf("\n");
		
		switch(choixFonction)
		{
			case 1:
				AfficherListeNoeudParent(trajet->noeud,true);
				break;
			case 2:
				AfficherListeNoeud(trajet);
				break;
			case 3:
				TrouverMeilleur(trajet,false,true);
				break;
			case 4:
				TrouverMeilleur(trajet,true,false);
				break;
			case 5:
				AffichageArbreTrouvee(arbre,0);
				break;
		}
		
	}while(choixFonction != 6);
	SupprimerTrajet(trajet);
	SupprmierArbre(arbre);
	SupprimerListeVille(listeVille,nbVille);
}


int main(int argc, char **argv)
{
	Menu();
	return 0;
}

