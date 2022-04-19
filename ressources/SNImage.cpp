#include"SNImage.h"
SNImage::SNImage()
{   signature=0;largeur=0;hauteur=0;tailleFichier=0;debutImage=0;tailleImage=0;
	tailleEntete=0;format=0;resoLarg=0;resoHaut=0;dimensionMax=0;
}
SNImage::~SNImage()
{   //libération de la mémoire
	if(dimensionMax)
	{	for(int i=0;i<dimensionMax;i++)
			delete []image[i];
		delete []image;
	}
}
void SNImage::Charger(string nomFichier)
{	ifstream entree;
	long i,j;char k; long h;
	unsigned long dimensionMaxPrecedente=dimensionMax;
	entree.open(nomFichier.c_str(),fstream::binary);
	//lecture entête image:
	entree.read((char*)&signature,2*sizeof(char));//signature (BM)=424D
	entree.read((char*)&tailleFichier,sizeof(int));
	entree.read((char*)&zero,sizeof(int));
	entree.read((char*)&debutImage,sizeof(int));
	entree.read((char*)&tailleEntete,sizeof(int));
	entree.read((char*)&largeur,sizeof(int));
	entree.read((char*)&hauteur,sizeof(int));
	entree.read((char*)&format,sizeof(int));
	entree.read((char*)&zero,sizeof(int));
	entree.read((char*)&tailleImage,sizeof(int));
	entree.read((char*)&resoLarg,sizeof(int));
	entree.read((char*)&resoHaut,sizeof(int));
	entree.read((char*)&zero,sizeof(int));
	entree.read((char*)&zero,sizeof(int));
	//calcul de k
	k=(4-(largeur*3)%4)%4;
	//remplissage tableau 2 dimensions
	i=hauteur-1;j=0;//i indice des lignes
	if(signature!=0x4D42) cout<<"Vous devez choisir un bitmap .bmp";
	else   if(format!=0x180001) cout<<"Vous devez choisir un bitmap 24 bits";
	else
	{   //libération éventuelle de la mémoire occupée par l'ancienne image
		if(dimensionMaxPrecedente)
		{	for(h=0;h<dimensionMaxPrecedente;h++)
				delete []image[h];
			delete []image;
		}
		//détermination de la dimension maximum (hauteur ou largeur ?)
		if(hauteur>largeur) dimensionMax=hauteur;
		else dimensionMax=largeur;
		//réservation de la mémoire
		image=new Pixel*[dimensionMax];//i
		for(h=0;h<dimensionMax;h++)
			image[h]=new Pixel[dimensionMax];//j
		while(!entree.eof())
		{	entree.read((char*)&image[i][j].bleu,sizeof(char));
			entree.read((char*)&image[i][j].vert,sizeof(char));
			entree.read((char*)&image[i][j].rouge,sizeof(char));
			j++;
			if(j==largeur)
			{  	entree.seekg(k,ios_base::cur);
				j=0;i--;
			}
		}
	}
	entree.close();
}
void SNImage::Sauvegarder(string nomFichier)
{   ofstream sortie; int i,j; char k,z=0,n;
	sortie.open(nomFichier.c_str(),fstream::binary);
	k=(4-(largeur*3)%4)%4;
	sortie.write((char*)&signature,2*sizeof(char));
	sortie.write((char*)&tailleFichier,sizeof(int));
	sortie.write((char*)&zero,sizeof(int));
	sortie.write((char*)&debutImage,sizeof(int));
	sortie.write((char*)&tailleEntete,sizeof(int));
	sortie.write((char*)&largeur,sizeof(int));
	sortie.write((char*)&hauteur,sizeof(int));
	sortie.write((char*)&format,sizeof(int));
	sortie.write((char*)&zero,sizeof(int));
	sortie.write((char*)&tailleImage,sizeof(int));
	sortie.write((char*)&resoLarg,sizeof(int));
	sortie.write((char*)&resoHaut,sizeof(int));
	sortie.write((char*)&zero,sizeof(int));
	sortie.write((char*)&zero,sizeof(int));
	for(i=hauteur-1;i>=0;i--)
	{	for(j=0;j<largeur;j++)
		{   sortie.write((char*)&image[i][j].bleu,sizeof(char));
			sortie.write((char*)&image[i][j].vert,sizeof(char));
			sortie.write((char*)&image[i][j].rouge,sizeof(char));
		}
		//pixels de remplissage
		for(n=0;n<k;n++)
			sortie.write((char*)&z,sizeof(char));
	}
	 sortie.close();
}
void SNImage::Tourner90Droite()
{	int i,j;
	Pixel **temp;
	temp=new Pixel*[dimensionMax];//la hauteur de temp est la largeur de image
	for(i=0;i<dimensionMax;i++)
		temp[i]=new Pixel[dimensionMax];
	unsigned long htemp;
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur;j++)
		{	temp[j][(hauteur-1)-i]=image[i][j];
		}
	}
	htemp=hauteur;hauteur=largeur;largeur=htemp;
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur;j++)
		{	image[i][j]=temp[i][j];
		}
	}
	for(i=0;i<dimensionMax;i++)
		delete []temp[i];
	delete []temp;
}
void SNImage::Tourner180()
{   Tourner90Droite();
	Tourner90Droite();
}
void SNImage::Tourner90Gauche()
{   Tourner90Droite();
	Tourner180();
}
void SNImage::Negatif()
{	int i,j;
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur;j++)
		{	image[i][j].bleu=~(image[i][j].bleu);
			image[i][j].vert=~(image[i][j].vert);
			image[i][j].rouge=~(image[i][j].rouge);
		}
	}
}
void SNImage::RetournerHorizontal()
{	int i,j;
	Pixel temp;
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur/2;j++)
		{	temp=image[i][(largeur-1)-j];image[i][(largeur-1)-j]=image[i][j];image[i][j]=temp;
		}
	}
}
void SNImage::RetournerVertical()
{	int i,j;
	Pixel temp;
	for(i=0;i<hauteur/2;i++)
	{	for(j=0;j<largeur;j++)
		{	temp=image[(hauteur-1)-i][j];image[(hauteur-1)-i][j]=image[i][j];image[i][j]=temp;
		}
	}
}
void SNImage::NiveauDeGris()
{	int i,j;
	unsigned char rouge,vert,bleu,gris;
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur;j++)
		{	bleu=image[i][j].bleu;
			vert=image[i][j].vert;
			rouge=image[i][j].rouge;
			image[i][j].bleu=(bleu+vert+rouge)/3;
			image[i][j].vert=(bleu+vert+rouge)/3;
			image[i][j].rouge=(bleu+vert+rouge)/3;
		}
	}
}
void SNImage::Eclaircir(int niveau)
{	int i,j;
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur;j++)
		{	if((int)(image[i][j].bleu)+niveau<256) image[i][j].bleu=image[i][j].bleu+niveau; else image[i][j].bleu=255;
			if((int)(image[i][j].vert)+niveau<256) image[i][j].vert=image[i][j].vert+niveau; else image[i][j].vert=255;
			if((int)(image[i][j].rouge)+niveau<256) image[i][j].rouge=image[i][j].rouge+niveau; else image[i][j].rouge=255;
		}
	}
}
void SNImage::Assombrir(int niveau)
{	int i,j;
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur;j++)
		{	if((int)(image[i][j].bleu)-niveau>=0) image[i][j].bleu=image[i][j].bleu-niveau; else image[i][j].bleu=0;
			if((int)(image[i][j].vert)-niveau>=0) image[i][j].vert=image[i][j].vert-niveau; else image[i][j].vert=0;
			if((int)(image[i][j].rouge)-niveau>=0) image[i][j].rouge=image[i][j].rouge-niveau; else image[i][j].rouge=0;
		}
	}
}
void SNImage::SeuillerNoirBlanc(int niveau)
{	int i,j;
	NiveauDeGris();
	for(i=0;i<hauteur;i++)
	{	for(j=0;j<largeur;j++)
		{	if(image[i][j].bleu<niveau)
			{	image[i][j].bleu=0; image[i][j].vert=0; image[i][j].rouge=0;
			}
			else
			{	image[i][j].bleu=255; image[i][j].vert=255; image[i][j].rouge=255;
			}
		}
	}
}
void SNImage::Colorier(Coordonnee coord,int l,int h,Pixel couleur)
{   if((coord.ligne+h<=hauteur)&&(coord.colonne+l<=largeur))
	for(int i=coord.ligne;i<coord.ligne+h;i++)
	{	for(int j=coord.colonne;j<coord.colonne+l;j++)
		{	if(i>=0 && i<hauteur && j>=0 && j<largeur)
			{	image[i][j].bleu=couleur.bleu;
				image[i][j].vert=couleur.vert;
				image[i][j].rouge=couleur.rouge;
			}
		}
	}

}
void SNImage::Detourer(int largeurGauche,int largeurDroite,int largeurHaut,int largeurBas,Pixel couleur)
{   Coordonnee coor;
	coor.ligne=0;coor.colonne=0;
	Colorier(coor,largeurGauche,hauteur,couleur);
	Colorier(coor,largeur,largeurHaut,couleur);
	coor.ligne=0;coor.colonne=largeur-largeurDroite;
	Colorier(coor,largeurDroite,hauteur,couleur);
	coor.ligne=hauteur-largeurBas;coor.colonne=0;
	Colorier(coor,largeur,largeurBas,couleur);

}
void SNImage::Recadrer(Coordonnee coord,int l,int h)
{   if((coord.ligne+h<=hauteur)&&(coord.colonne+l<=largeur))
	{   for(int i=0;i<h;i++)
		{	for(int j=0;j<l;j++)
			{	image[i][j]=image[i+coord.ligne][j+coord.colonne];
			}
		}
		largeur=l;hauteur=h;
		int k=(4-(largeur*3)%4)%4;
		tailleImage=(largeur+k)*(hauteur)*3;
		tailleFichier=tailleImage+debutImage;
	}
}
Coordonnee SNImage::RechercherZone(int l,int h,Pixel couleur)
{   Coordonnee coord={-1,-1};
	int i,j,m,n;
	bool zoneTrouve;
	for(int i=0;i<hauteur;i++)
	{	for(int j=0;j<largeur;j++)
		{   if(image[i][j].bleu==couleur.bleu && image[i][j].vert==couleur.vert && image[i][j].rouge==couleur.rouge)
			{   zoneTrouve=true;
				for(int m=0;m<h;m++)
				{   for(int n=0;n<l;n++)
					{	if(image[i+m][j+n].bleu!=couleur.bleu || image[i+m][j+n].vert!=couleur.vert || image[i+m][j+n].rouge!=couleur.rouge)
							zoneTrouve=false;
					}
				}
				if(zoneTrouve)
				{   coord.ligne=i;
					coord.colonne=j;
					i=hauteur;
					j=largeur;
				}
			}
		}
	}
	return coord;
}

void SNImage::DessinerCroix(Coordonnee coord,int taille,int epaisseur,Pixel couleur)
{       Coordonnee debut;
		debut.ligne=coord.ligne-taille/2;debut.colonne=coord.colonne-epaisseur/2;
		if(debut.ligne<0) debut.ligne=0;
		if(debut.colonne<0) debut.colonne=0;
		Colorier(debut,epaisseur,taille,couleur);
		debut.ligne=coord.ligne-epaisseur/2;debut.colonne=coord.colonne-taille/2;
		if(debut.ligne<0) debut.ligne=0;
		if(debut.colonne<0) debut.colonne=0;
		Colorier(debut,taille,epaisseur,couleur);
}
void SNImage::DessinerCarre(Coordonnee coord,int taille,int epaisseur,Pixel couleur)
{       Coordonnee debut=coord;
		debut.ligne-=epaisseur/2;
		debut.colonne-=epaisseur/2;
		Colorier(debut,epaisseur,taille,couleur);//f ou e
		Colorier(debut,taille,epaisseur,couleur);//a
		debut.colonne+=taille;
		Colorier(debut,epaisseur,taille,couleur);//b ou c
		debut=coord;
		debut.ligne-=epaisseur/2;
		debut.colonne-=epaisseur/2;
		debut.ligne+=taille;
		Colorier(debut,taille+epaisseur,epaisseur,couleur);//g ou d
}
void SNImage::Dessiner11Segments(Coordonnee coord,int taille,int epaisseur,Pixel couleur,string message)
{       int ligneDepart=coord.ligne;
		for(int i=0;i<message.length();i++)
		{       coord.ligne=ligneDepart;
				switch (message.c_str()[i])
				{       case '¤'://tous les segments
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case '0':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case '1':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								break;
						case '2':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '3':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '4':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '5':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '6':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '7':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case '8':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '9':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'A':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'a':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;
						case 'B':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'b':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'C':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;
						case 'c':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'D':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'd':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'E':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'e':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;
						case 'F':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'f':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								break;
						case 'G':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'g':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								break;
						case 'H':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'h':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'I':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'l');
								break;
						case 'i':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								break;
						case 'J':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;
						case 'j':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case 'K':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'k':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'L':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'l':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;

						case 'M':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'm':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'N':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'n':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								break;
						case 'O':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'o':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'P':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'p':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case 'Q':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'q':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'R':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'r':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'S':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 's':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'T':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'l');
								break;
						case 't':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'U':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'u':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								break;
						case 'V':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								break;
						case 'v':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case 'W':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'w':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'X':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'x':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case 'Y':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case 'y':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								break;
						case 'Z':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case 'z':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
						case '-':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '\\':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'h');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'k');
								break;
						case '/':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'i');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'j');
								break;
				}
				coord.colonne+=2*(taille/2);
				coord.ligne=ligneDepart;
		}
}
void SNImage::Dessiner7Segments(Coordonnee coord,int taille,int epaisseur,Pixel couleur,string message)
{       int ligneDepart=coord.ligne;
		for(int i=0;i<message.length();i++)
		{       coord.ligne=ligneDepart;
				switch (message.c_str()[i])
				{       case '0':
						case 'D':
						case 'O':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
                                ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case '1':
						case 'I':
						case 'l':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								break;
						case '2':
						case 'Z':
						case 'z':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
                                ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
                                break;
						case '3':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '4':
						case 'Y':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '5':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
                        case '6':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
                                ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
                                break;
                        case '7':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
                                ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								break;
						case '8':
						case 'B':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '9':
						case 'g':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
												ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'A':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'b':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'C':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;
						case 'c':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'd':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'E':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'e':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'F':
						case 'f':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'G':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'H':
						case 'h':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'i':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								break;
						case 'J':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
						case 'j':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;
						case 'k':
						case 'K':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'L':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'M':
						case 'm':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'N':
						case 'n':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;

						case 'o':
						case 'a':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'P':
						case 'p':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'Q':
						case 'q':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'R':
						case 'r':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'S':
						case 's':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'T':
						case 't':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case 'U':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'u':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								break;
						case 'V':
						case 'W':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								break;
						case 'v':
						case 'w':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'e');
								break;
						case 'x':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								break;
						case 'X':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'a');
								break;
						case 'y':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'b');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'c');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'d');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'f');
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
						case '-':
								ChangerCouleurSegment(coord,taille,epaisseur,couleur,'g');
								break;
				}
				coord.colonne+=2*(taille/2);
				coord.ligne=ligneDepart;
		}
}

void SNImage::ChangerCouleurSegment(Coordonnee coord,int taille,int epaisseur,Pixel couleur,char segment)//segment='a' 'b' 'c' 'd' 'e' 'f'
{       Coordonnee debut,fin;
		Pixel c1={255,0,0},c2={0,0,255};
		Pixel c={0,0,255};   Pixel cc={255,0,0}; Pixel cv={0,255,0}; Pixel cx={255,0,255};
		int demiepaisseur,demitaille,quarttaille;
		demiepaisseur=epaisseur/2.0;
		demitaille=Arrondi((float)taille/2.0);
		quarttaille=Arrondi((float)taille/4.0);
		switch (segment)
		{       case 'a':
						debut=coord; debut.ligne-=demiepaisseur; debut.colonne-=demiepaisseur; Colorier(debut,demitaille+epaisseur,epaisseur,couleur); break;
				case 'b':
						debut=coord; debut.ligne-=demiepaisseur; debut.colonne-=demiepaisseur; debut.colonne+=demitaille; Colorier(debut,epaisseur,demitaille+epaisseur,couleur); break;
				case 'f':
						debut=coord; debut.ligne-=demiepaisseur; debut.colonne-=demiepaisseur; Colorier(debut,epaisseur,demitaille+epaisseur,couleur); break;
				case 'g':
						debut=coord; debut.ligne-=demiepaisseur; debut.colonne-=demiepaisseur; debut.ligne+=demitaille; Colorier(debut,demitaille+epaisseur,epaisseur,couleur); break;
				case 'c':
						debut=coord; debut.ligne-=demiepaisseur-demitaille; debut.colonne-=demiepaisseur; debut.colonne+=demitaille; Colorier(debut,epaisseur,demitaille+epaisseur,couleur); break;
				case 'e':
						debut=coord; debut.ligne-=demiepaisseur-demitaille; debut.colonne-=demiepaisseur; Colorier(debut,epaisseur,demitaille+epaisseur,couleur); break;
				case 'd':
						debut=coord; debut.ligne-=demiepaisseur-demitaille; debut.colonne-=demiepaisseur; debut.ligne+=demitaille; Colorier(debut,demitaille+epaisseur,epaisseur,couleur); break;
				case 'h':    //10h
						debut=coord; fin=coord;
						debut.colonne=debut.colonne+quarttaille;        //-demiepaisseur
						debut.ligne=debut.ligne+demitaille;
						fin.colonne=fin.colonne;						//-demiepaisseur
						fin.ligne=fin.ligne;
						//DessinerCroix(debut,30,1,c1);
						//DessinerCroix(fin,30,1,c2);
						TracerDiagonale(debut,fin,epaisseur,couleur);
						break;
				case 'k':    //4h   k
						debut=coord; fin=coord;
						debut.colonne=debut.colonne+quarttaille;        //-demiepaisseur
						debut.ligne=debut.ligne+demitaille;
						fin.colonne+=demitaille;						//-demiepaisseur
						fin.ligne+=taille;
						//DessinerCroix(debut,30,1,cx);
						//DessinerCroix(fin,30,1,c2);
						TracerDiagonale(debut,fin,epaisseur,couleur);
						break;
				case 'i':    //2h  i
						debut=coord; fin=coord;
						debut.colonne=debut.colonne+quarttaille;        //-demiepaisseur
						debut.ligne=debut.ligne+demitaille;
						fin.colonne+=demitaille;						//-demiepaisseur
						fin.ligne=fin.ligne;
						//DessinerCroix(debut,30,1,c1);
						//DessinerCroix(fin,30,1,c2);
						TracerDiagonale(debut,fin,epaisseur,couleur);
						break;
				case 'j':    //8h   j
						debut=coord; fin=coord;
						debut.colonne=debut.colonne+quarttaille;        //-demiepaisseur
						debut.ligne=debut.ligne+demitaille;
						fin.colonne=fin.colonne;						//-demiepaisseur
						fin.ligne+=taille;
						//DessinerCroix(debut,30,1,c1);
						//DessinerCroix(fin,30,1,c2);
						TracerDiagonale(debut,fin,epaisseur,couleur);
						break;
				case 'l':    //12h+6h
						debut=coord; debut.ligne-=demiepaisseur; debut.colonne-=demiepaisseur; debut.colonne+=quarttaille;
						Colorier(debut,epaisseur,taille+epaisseur,couleur); break;
		}
}
void SNImage::TracerDiagonaleUnPixel(Coordonnee debut,Coordonnee fin, Pixel couleur)
{//   Pixel r={0,0,255},b={255,0,0},v={0,255,0},bl={255,255,255},n={0,0,0};
	if(debut.ligne>fin.ligne)
	{   Coordonnee temp=fin;
		fin=debut;
		debut=temp;
	}
//	DessinerCroix(debut,5,1,r);    DessinerCroix(fin,5,1,v);
	float h=abs(debut.ligne-fin.ligne);
	float l=abs(debut.colonne-fin.colonne);
	//cout<<"h="<<h<<" l="<<l<<endl;
	if(l==0) Colorier(debut,1,h,couleur);
	else
	{	if(h==0)
		{	if(debut.colonne<=fin.colonne) Colorier(debut,l,1,couleur);
			else
			{   Coordonnee d;d.ligne=debut.ligne;d.colonne=debut.colonne-l;
				Colorier(d,l,1,couleur);
			}
		}
		else
		{       if(debut.colonne<=fin.colonne)
				{   if (l>=h)
					{   float i = debut.ligne;
						float r = l/h;
						for (int j = debut.colonne; j <= fin.colonne; j++)
						{   //if(i<=fin.ligne)
							if(i>=0 && i<hauteur && j>=0 && j<largeur)
								image[(int)i][j] = couleur;
							i+=1/r;
						}
					}
					else
					{   float j = debut.colonne;
						float r = h/l;
						for (int i = debut.ligne; i <= fin.ligne; i++)
						{   //if(j<=fin.colonne)
							if(i>=0 && i<hauteur && j>=0 && j<largeur)
								image[i][(int)j] = couleur;
							j+=1/r;
						}
					}
				}
				else
				{   if (l>=h)
					{   float i = debut.ligne;
						float r = l/h;
						for (int j = debut.colonne; j >= fin.colonne; j--)
						{   //if(i<=fin.ligne)
							if(i>=0 && i<hauteur && j>=0 && j<largeur)
								image[(int)i][j] = couleur;
							i+=1/r;
						}
					}
					else
					{   float j = debut.colonne;
						float r = h/l;
						for (int i = debut.ligne; i <= fin.ligne; i++)
						{   //if(j>=fin.colonne)
							if(i>=0 && i<hauteur && j>=0 && j<largeur)
								image[i][(int)j] = couleur;
							j-=1/r;
						}
					}
				}
		}
	}
}
void SNImage::TracerDiagonale(Coordonnee debut,Coordonnee fin, int epaisseur, Pixel couleur)
{	Coordonnee CDebut=debut,CFin=fin;
	int h=abs(CDebut.ligne-CFin.ligne);
	int l=abs(CDebut.colonne-CFin.colonne);
	if(l>h)
	{	CDebut.ligne-=epaisseur/2;
		CFin.ligne-=epaisseur/2;
		for(int k=0;k<epaisseur;k++)
		{	TracerDiagonaleUnPixel(CDebut,CFin,couleur);
			CDebut.ligne++;
			CFin.ligne++;
		}
	}
	else
	{   CDebut.colonne-=epaisseur/2;
		CFin.colonne-=epaisseur/2;
		for(int k=0;k<epaisseur;k++)
		{	TracerDiagonaleUnPixel(CDebut,CFin,couleur);
			CDebut.colonne++;
			CFin.colonne++;
		}
	}
}
void SNImage::TracerCercleUnPixel(Coordonnee centre, int rayon, Pixel couleur)
{   for(int angle=0; angle<3600; angle++)
	{	int i=rayon*sin(angle*M_PI/1800)+centre.ligne;
		int j=rayon*cos(angle*M_PI/1800)+centre.colonne;
		if(i>=0 && i<hauteur && j>=0 && j<largeur) image[i][j]=couleur;
	}
}
void SNImage::TracerCercle(Coordonnee centre, int rayon, int epaisseur, Pixel couleur)
{   int Rmax=rayon+epaisseur/2;
	for(int k=0;k<epaisseur;k++)
	{	TracerCercleUnPixel(centre, Rmax--, couleur);
	}
}
void SNImage::AfficherImageEnCaracteres(int niveau)
{	SeuillerNoirBlanc(niveau);
	for(int i=0;i<hauteur;i+=2)
	{	for(int j=0;j<largeur;j++)
		{	if(image[i][j].bleu==0)
				cout<<'X';
			else	cout<<' ';
		}
		cout<<endl;
	}
}
void SNImage::AfficherImageEnCaracteres4Seuils()
{   NiveauDeGris();
	for(int i=0;i<hauteur;i+=2)
	{	for(int j=0;j<largeur;j++)
		{	if(image[i][j].bleu<64)
				cout<<'X';
			else
			if(image[i][j].bleu<128)
				cout<<'x';
			else
			if(image[i][j].bleu<196)
				cout<<'.';
			else
				cout<<' ';
		}
			cout<<endl;
	}
}
void SNImage::AfficherImageEnCaracteres10caracteres(char caracteres[10])  //0 pour le noir 10 pour le blanc : "#X*+x-:,. "
{   NiveauDeGris();
	for(int i=0;i<hauteur;i+=2)
	{	for(int j=0;j<largeur;j++)
		{	if(image[i][j].bleu<25)
				cout<<caracteres[0];
			else
			if(image[i][j].bleu<50)
				cout<<caracteres[1];
			else
			if(image[i][j].bleu<75)
				cout<<caracteres[2];
			else
			if(image[i][j].bleu<100)
				cout<<caracteres[3];
			else
			if(image[i][j].bleu<125)
				cout<<caracteres[4];
			else
			if(image[i][j].bleu<150)
				cout<<caracteres[5];
			else
			if(image[i][j].bleu<175)
				cout<<caracteres[6];
			else
			if(image[i][j].bleu<200)
				cout<<caracteres[7];
			else
			if(image[i][j].bleu<225)
				cout<<caracteres[8];
			else
			cout<<caracteres[9];
		}
		cout<<endl;
	}
}
void SNImage::Histogrammes(/*bool horsZero*/)  //nécessite histogrammeBleu.bmp histogrammeVert.bmp histogrammeRouge.bmp 256x256pixels
{   unsigned int histo[3][256]; //[0][]=bleu [1][]=vert [2][]=rouge
	int amplitudeMax[3]={0,0,0};
	for(int c=0;c<3;c++) for(int i=0;i<256;i++) histo[c][i]=0;
	for(int i=0;i<hauteur;i++)
	{	for(int j=0;j<largeur;j++)
		{   histo[0][image[i][j].bleu]++;
			if(histo[0][image[i][j].bleu]>amplitudeMax[0]) amplitudeMax[0]=histo[0][image[i][j].bleu];
			histo[1][image[i][j].vert]++;
			if(histo[1][image[i][j].vert]>amplitudeMax[1]) amplitudeMax[1]=histo[1][image[i][j].vert];
			histo[2][image[i][j].rouge]++;
			if(histo[2][image[i][j].rouge]>amplitudeMax[2]) amplitudeMax[2]=histo[2][image[i][j].rouge];
		}
	}
	//for(int i=0;i<256;i++) cout<<"["<<i<<"]="<<histo[i]<<" ";
	char compo[3][6]; strcpy(compo[0],"Bleu");strcpy(compo[1],"Vert");strcpy(compo[2],"Rouge");
	Pixel blanc={255,255,255}, couleur[3]={255,0,0,0,255,0,0,0,255};
	for(int composante=0; composante<3;composante++)
	{   SNImage histogramme;
		char fichier[30]="histogramme";
		strcat(fichier,compo[composante]);
		strcat(fichier,".bmp");
		histogramme.Charger(fichier);
		// effacement
		for(int i=0;i<histogramme.hauteur;i++)
			for(int j=0;j<histogramme.largeur;j++)
			   histogramme.image[i][j]=blanc;
		//dessin histo
		for(int i=0/*horsZero*/;i<256;i++)
		{   float h=(float)histo[composante][i]*256.0/(float)amplitudeMax[composante];
			for(int a=0;a<(int)h;a++)
				histogramme.image[histogramme.hauteur-1-a][i]=couleur[composante];
		}
		histogramme.Sauvegarder(fichier);
	}
};
void SNImage::DissimulerTexte(char texte[])
{   //'a' : 011 000 01
	int i=0,j=0;
	for(int k=0;k<=strlen(texte);k++)      //marqueur=zéro final
	{   //cout<<hex<<(int)image[i][j].bleu<<(int)image[i][j].vert<<(int)image[i][j].rouge<<"+ lettre ="<<(int)texte[k]<<endl;
		image[i][j].bleu=(image[i][j].bleu&0xF8)|((texte[k]>>5)&0x07);   	//011
		image[i][j].vert=(image[i][j].vert&0xF8)|((texte[k]>>2)&0x07);   	//011 000 & 00000111
		image[i][j].rouge=(image[i][j].rouge&0xFC)|(texte[k]&0x03);
		//cout<<" "<<(int)image[i][j].bleu<<" "<<(int)image[i][j].vert<<" "<<(int)image[i][j].rouge<<endl;
		j++;
		if(j==largeur) {i++;j=0; if(i==hauteur) k=strlen((char*)texte);}
	}
}
string SNImage::ExtraireTexte()
{   unsigned char texte[5000];
	string stexte;
	int k=0;
	for(int i=0;i<hauteur;i++)
	{	for(int j=0;j<largeur;j++)
		{      texte[k]=((image[i][j].bleu&0x07)<<5)|((image[i][j].vert&0x07)<<2)|(image[i][j].rouge&0x03);
			   if(texte[k]==0) {i=hauteur;j=largeur;}      //sortie si 0 final
			   k++;
		}
	}
	stexte=(char*)texte;
	return stexte;
}
int SNImage::SeuillerNoirBlancMoyen()
{   long long moy=0;
	NiveauDeGris();
	for(int i=0;i<hauteur;i++)
	for(int j=0;j<largeur;j++)                                                   {   moy+=image[i][j].bleu;
	}
	moy/=(hauteur*largeur);
	SeuillerNoirBlanc(moy);
	return moy;
}
bool SNImage::SeuillagePourcentNoir(int pourcent)// return true si seuillage possible
{   Pixel **temp;
	temp=new Pixel*[dimensionMax];
	for(int i=0;i<dimensionMax;i++)
		temp[i]=new Pixel[dimensionMax];
	long long nbPix=hauteur*largeur,nb;
	bool seuilOK=false;
	nbPix=nbPix*pourcent/100.0;
	for(int i=0;i<hauteur;i++) for(int j=0;j<largeur;j++) temp[i][j]=image[i][j];//sauvegarde image de départ
	for(int s=0;s<256;s++)
	{   nb=0;
		SeuillerNoirBlanc(s);
		for(int i=0;i<hauteur;i++)
			for(int j=0;j<largeur;j++)
			{   if(image[i][j].bleu==0)nb++;
			}
		if(nb>=nbPix) {s=256;seuilOK=true;} //sortie
		//cout<<nb<<"/"<<nbPix<<" retour="<<seuilOK<<endl;
		if(!seuilOK) for(int i=0;i<hauteur;i++) for(int j=0;j<largeur;j++) image[i][j]=temp[i][j];//annulation traitement
	}
	for(int i=0;i<dimensionMax;i++)
		delete []temp[i];
	delete []temp;
	return seuilOK;
}
void SNImage::AfficherTexteAvecImage(string texte, int pourcent)
{   SeuillagePourcentNoir(pourcent);
	int t=0;
	for(int i=0;i<hauteur;i+=2)
	{	for(int j=0;j<largeur;j++)
		{   if(!image[i][j].bleu)
			{	cout<<texte[t++];
				if(t==texte.length()) t=0;
			}
			else cout<<' ';
		}
		cout<<endl;
	}
}
void SNImage::AfficherTexteAvecImageFichier(char nomFichierTxt[20], int pourcent)
{   string texte="";
	ifstream f;
	char c;
	f.open(nomFichierTxt);
	while(!f.eof())
	{   c=f.get();
		if(c=='\n') texte+=' ';
		else if(c!='\r') texte+=c;
		f.peek();
	}
	for(int i=0;i<texte.length();i++)
	{	int t;
		switch(texte[i])
		{	case 'é': case 'è': case 'ê': case 'ë':
				texte[i]='e'; break;
			case 'à': case 'â': case 'ä':
				texte[i]='a'; break;
			case 'û': case 'ù': case 'ü':
				texte[i]='u'; break;
			case 'ô': case 'ö':
				texte[i]='o'; break;
			case 'ï': case 'î':
				texte[i]='i'; break;
			case 'ç':
				texte[i]='c'; break;
			case 'ÿ':
				texte[i]='y'; break;
			case 'É': case 'È': case 'Ê': case 'Ë':
				texte[i]='E'; break;
			case 'À': case 'Â': case 'Ä':
				texte[i]='A'; break;
			case 'Û': case 'Ù': case 'Ü':
				texte[i]='U'; break;
			case 'Ô': case 'Ö':
				texte[i]='O'; break;
			case 'Ï': case 'Î':
				texte[i]='I'; break;
			case 'Ç':
				texte[i]='C'; break;
			case 'Ÿ':
				texte[i]='Y'; break;
			case 'æ':
				texte[i]='a';
				t=texte.length();
				for(int j=t;j>i+1;j--) texte[j]=texte[j-1];
				texte[i+1]='e'; texte[t+1]=0; break;
			case 'œ':
				texte[i]='o';
				t=texte.length();
				for(int j=t;j>i+1;j--) texte[j]=texte[j-1];
				texte[i+1]='e'; texte[t+1]=0; break;
			case 'Æ':
				texte[i]='A';
				t=texte.length();
				for(int j=t;j>i+1;j--) texte[j]=texte[j-1];
				texte[i+1]='E'; texte[t+1]=0; break;
			case 'Œ':
				texte[i]='O';
				t=texte.length();
				for(int j=t;j>i+1;j--) texte[j]=texte[j-1];
				texte[i+1]='E'; texte[t+1]=0; break;
		}
	}
	f.close();
	AfficherTexteAvecImage(texte, pourcent);
}
void SNImage::DessinerCroixEtCoordonnees(Coordonnee coord,int tailleCroix,int epCroix,int tailleTexte,int epTexte,Pixel couleur,bool droite)
{	char texte[50];
	DessinerCroix(coord,tailleCroix,epCroix,couleur);
	sprintf(texte,"%d-%d",coord.ligne,coord.colonne);
	Coordonnee ctexte;
	int decalage=0;
	if(!droite)  decalage=-tailleTexte*strlen(texte);
	ctexte.ligne=coord.ligne-tailleCroix-tailleTexte/2;
	ctexte.colonne=coord.colonne+decalage;
	Dessiner36Segments(ctexte,tailleTexte,epTexte,couleur,texte);
}
void SNImage::Dessiner36Segments(Coordonnee coord,int taille,int epaisseur,Pixel couleur,string message)
//11segments : [0]=gauchehaut puis [1]=droitehaut puis [2]=gauchebas puis [3]=droitebas
{       taille=Arrondi((float)taille/2.0);
		Coordonnee coord2=coord,coord3=coord,coord4=coord;
		coord2.colonne+=Arrondi((float)taille/2.0); //coord2.colonne+=1;
		coord3.ligne+=taille;    if(taille%2)coord3.ligne++;
		coord4.ligne+=taille;    if(taille%2)coord4.ligne++;
		coord4.colonne+=Arrondi((float)taille/2.0); //coord4.colonne+=1;
		char segments[4][45]={"","","",""};
		bool espace;
		for(int i=0;i<message.length();i++)
		{       espace=false;
				switch (message.c_str()[i])
				{       case '¤':
								strcpy(segments[0],"abcdefghijk");	strcpy(segments[1],"abcdefghijk");	strcpy(segments[2],"abcdefghijk");	strcpy(segments[3],"abcdefghijk");
								break;
						case '@':
								strcpy(segments[0],"aefbc");	strcpy(segments[1],"abcd");	strcpy(segments[2],"efd");	strcpy(segments[3],"dc");
								break;
						case '0':
								strcpy(segments[0],"aef");			strcpy(segments[1],"abcij");		strcpy(segments[2],"defij");		strcpy(segments[3],"bcd");
								break;
						case '1':
								strcpy(segments[0],"ijbc");         strcpy(segments[1],"");             strcpy(segments[2],"bc");           strcpy(segments[3],"");
								break;
						case '2':
								strcpy(segments[0],"ad");            strcpy(segments[1],"abcd");         strcpy(segments[2],"efd");         strcpy(segments[3],"d");
								break;
						case '3':
								strcpy(segments[0],"a");            strcpy(segments[1],"abc");          strcpy(segments[2],"ad");           strcpy(segments[3],"abcd");
								break;
						case '4':
								strcpy(segments[0],"fed");          strcpy(segments[1],"bcd");           strcpy(segments[2],"");             strcpy(segments[3],"bc");
								break;
						case '5':
								strcpy(segments[0],"aefd");         strcpy(segments[1],"ad");           strcpy(segments[2],"d");            strcpy(segments[3],"dbc");
								break;
						case '6':
								strcpy(segments[0],"aefd");         strcpy(segments[1],"ad");           strcpy(segments[2],"def");          strcpy(segments[3],"dbc");
								break;
						case '7':
								strcpy(segments[0],"a");            strcpy(segments[1],"abc");          strcpy(segments[2],"");             strcpy(segments[3],"bc");
								break;
						case '8':
								strcpy(segments[0],"afed");         strcpy(segments[1],"abcd");         strcpy(segments[2],"def");          strcpy(segments[3],"bcd");
								break;
						case '9':
								strcpy(segments[0],"afed");         strcpy(segments[1],"abcd");         strcpy(segments[2],"d");            strcpy(segments[3],"bcd");
								break;
						case '[':
								strcpy(segments[0],"afe");         strcpy(segments[1],"");         strcpy(segments[2],"efd");            strcpy(segments[3],"");
								break;
						case ']':
								strcpy(segments[0],"");         strcpy(segments[1],"abc");         strcpy(segments[2],"");            strcpy(segments[3],"bcd");
								break;
						case '(':
								strcpy(segments[0],"ij");         strcpy(segments[1],"");         strcpy(segments[2],"hk");            strcpy(segments[3],"");
								break;
						case ')':
								strcpy(segments[0],"");         strcpy(segments[1],"hk");         strcpy(segments[2],"");            strcpy(segments[3],"ij");
								break;
						case ':':
								strcpy(segments[0],"b");         strcpy(segments[1],"");         strcpy(segments[2],"c");            strcpy(segments[3],"");
								break;
						case '.':
								strcpy(segments[0],"");         strcpy(segments[1],"");         strcpy(segments[2],"c");            strcpy(segments[3],"");
								break;
						case ',':
								strcpy(segments[0],"");         strcpy(segments[1],"");         strcpy(segments[2],"");            strcpy(segments[3],"j");
								break;
						case '=':
								strcpy(segments[0],"g");         strcpy(segments[1],"g");         strcpy(segments[2],"g");            strcpy(segments[3],"g");
								break;
						case '\\':
								strcpy(segments[0],"hk");         strcpy(segments[1],"");         strcpy(segments[2],"");            strcpy(segments[3],"hk");
								break;
						case '/':
								strcpy(segments[0],"");         strcpy(segments[1],"ij");         strcpy(segments[2],"ij");            strcpy(segments[3],"");
								break;
						case '\'':
								strcpy(segments[0],"");         strcpy(segments[1],"i");         strcpy(segments[2],"");            strcpy(segments[3],"");
								break;
						case '\"':
								strcpy(segments[0],"ij");         strcpy(segments[1],"ij");         strcpy(segments[2],"");            strcpy(segments[3],"");
								break;
						case '+':
								strcpy(segments[0],"cd");         strcpy(segments[1],"d");         strcpy(segments[2],"b");            strcpy(segments[3],"");
								break;
						case '-':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"");            strcpy(segments[3],"");
								break;
						case '|':
								strcpy(segments[0],"bc");         strcpy(segments[1],"");         strcpy(segments[2],"bc");            strcpy(segments[3],"");
								break;
						case 'A':
								strcpy(segments[0],"aefd");         strcpy(segments[1],"abcd");         strcpy(segments[2],"ef");            strcpy(segments[3],"bc");
								break;
						case 'B':
								strcpy(segments[0],"afed");         strcpy(segments[1],"hjd");         strcpy(segments[2],"efd");            strcpy(segments[3],"bcd");
								break;
						case 'C':
								strcpy(segments[0],"ij");         strcpy(segments[1],"a");         strcpy(segments[2],"hk");            strcpy(segments[3],"d");
								break;
						case 'D':
								strcpy(segments[0],"afe");         strcpy(segments[1],"hk");         strcpy(segments[2],"def");            strcpy(segments[3],"ji");
								break;
						case 'E':
								strcpy(segments[0],"afed");         strcpy(segments[1],"a");         strcpy(segments[2],"efd");            strcpy(segments[3],"d");
								break;
						case 'F':
								strcpy(segments[0],"afed");         strcpy(segments[1],"a");         strcpy(segments[2],"ef");            strcpy(segments[3],"");
								break;
						case 'G':
								strcpy(segments[0],"aef");         strcpy(segments[1],"ad");         strcpy(segments[2],"efd");            strcpy(segments[3],"bcd");
								break;
						case 'H':
								strcpy(segments[0],"fed");         strcpy(segments[1],"bcd");         strcpy(segments[2],"ef");            strcpy(segments[3],"bc");
								break;
						case 'I':
								strcpy(segments[0],"abc");         strcpy(segments[1],"a");         strcpy(segments[2],"dbc");            strcpy(segments[3],"d");
								break;
						case 'J':
								strcpy(segments[0],"abc");         strcpy(segments[1],"a");             strcpy(segments[2],"edbc");            strcpy(segments[3],"");
								break;
						case 'K':
								strcpy(segments[0],"fed");         strcpy(segments[1],"ji");         strcpy(segments[2],"fe");            strcpy(segments[3],"hk");
								break;
						case 'L':
								strcpy(segments[0],"fe");         strcpy(segments[1],"");         strcpy(segments[2],"efd");            strcpy(segments[3],"d");
								break;
						case 'M':
								strcpy(segments[0],"fehk");         strcpy(segments[1],"ijbc");         strcpy(segments[2],"ef");            strcpy(segments[3],"bc");
								break;
						case 'N':
								strcpy(segments[0],"hkfe");         strcpy(segments[1],"bc");         strcpy(segments[2],"ef");            strcpy(segments[3],"hkbc");
								break;
						case 'O':
								strcpy(segments[0],"afe");         strcpy(segments[1],"abc");         strcpy(segments[2],"efd");            strcpy(segments[3],"bcd");
								break;
						case 'P':
								strcpy(segments[0],"afed");         strcpy(segments[1],"abcd");         strcpy(segments[2],"ef");            strcpy(segments[3],"");
								break;
						case 'Q':
								strcpy(segments[0],"afe");         strcpy(segments[1],"abc");         strcpy(segments[2],"efd");            strcpy(segments[3],"bcdhk");
								break;
						case 'R':
								strcpy(segments[0],"afed");         strcpy(segments[1],"abcd");         strcpy(segments[2],"ef");            strcpy(segments[3],"hk");
								break;
						case 'S':
								strcpy(segments[0],"ijd");         strcpy(segments[1],"ad");         strcpy(segments[2],"d");            strcpy(segments[3],"ij");
								break;
						case 'T':
								strcpy(segments[0],"abc");         strcpy(segments[1],"a");         strcpy(segments[2],"bc");            strcpy(segments[3],"");
								break;
						case 'U':
								strcpy(segments[0],"fe");         strcpy(segments[1],"bc");         strcpy(segments[2],"fed");            strcpy(segments[3],"bcd");
								break;
						case 'V':
								strcpy(segments[0],"fe");         strcpy(segments[1],"bc");         strcpy(segments[2],"hk");            strcpy(segments[3],"ij");
								break;
						case 'W':
								strcpy(segments[0],"fe");         strcpy(segments[1],"bc");         strcpy(segments[2],"efij");            strcpy(segments[3],"bchk");
								break;
						case 'X':
								strcpy(segments[0],"hk");         strcpy(segments[1],"ij");         strcpy(segments[2],"ij");            strcpy(segments[3],"hk");
								break;
						case 'Y':
								strcpy(segments[0],"hk");         strcpy(segments[1],"ij");         strcpy(segments[2],"bc");            strcpy(segments[3],"");
								break;
						case 'Z':
								strcpy(segments[0],"a");         strcpy(segments[1],"aij");         strcpy(segments[2],"ijd");            strcpy(segments[3],"d");
								break;
						case 'à':
								strcpy(segments[0],"d");         strcpy(segments[1],"dh");         strcpy(segments[2],"edg");            strcpy(segments[3],"gdbc");
								break;
						case 'a': case 'â':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"edg");            strcpy(segments[3],"gdbc");
								break;
						case 'b':
								strcpy(segments[0],"fed");         strcpy(segments[1],"d");         strcpy(segments[2],"efd");            strcpy(segments[3],"bcd");
								break;
						case 'c':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"efd");            strcpy(segments[3],"d");
								break;
						case 'd':
								strcpy(segments[0],"d");         strcpy(segments[1],"dbc");         strcpy(segments[2],"efd");            strcpy(segments[3],"bcd");
								break;
						case 'é':
								strcpy(segments[0],"di");         strcpy(segments[1],"d");         strcpy(segments[2],"efdg");            strcpy(segments[3],"dbg");
								break;
						case 'è':
								strcpy(segments[0],"d");         strcpy(segments[1],"dh");         strcpy(segments[2],"efdg");            strcpy(segments[3],"dbg");
								break;
						case 'ê':
								strcpy(segments[0],"di");         strcpy(segments[1],"dh");         strcpy(segments[2],"efdg");            strcpy(segments[3],"dbg");
								break;
						case 'e': case 'ë':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"efdg");            strcpy(segments[3],"dbg");
								break;
						case 'f':
								strcpy(segments[0],"ged");         strcpy(segments[1],"g");         strcpy(segments[2],"ef");            strcpy(segments[3],"");
								break;
						case 'g':
								strcpy(segments[0],"egd");         strcpy(segments[1],"gcd");         strcpy(segments[2],"ed");            strcpy(segments[3],"bcd");
								break;
						case 'h':
								strcpy(segments[0],"fed");         strcpy(segments[1],"d");         strcpy(segments[2],"ef");            strcpy(segments[3],"bc");
								break;
						case 'i': case 'ï': case 'î':
								strcpy(segments[0],"b");         strcpy(segments[1],"");         strcpy(segments[2],"bc");            strcpy(segments[3],"");
								break;
						case 'j':
								strcpy(segments[0],"");         strcpy(segments[1],"gc");             strcpy(segments[2],"k");            strcpy(segments[3],"bcd");
								break;
						case 'k':
								strcpy(segments[0],"fed");         strcpy(segments[1],"d");         strcpy(segments[2],"fe");            strcpy(segments[3],"hk");
								break;
						case 'l':
								strcpy(segments[0],"bc");         strcpy(segments[1],"");         strcpy(segments[2],"bc");            strcpy(segments[3],"d");
								break;
						case 'm':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"efbc");            strcpy(segments[3],"bc");
								break;
						case 'n':
								strcpy(segments[0],"d");         strcpy(segments[1],"");         strcpy(segments[2],"efbc");            strcpy(segments[3],"bcd");
								break;
						case 'o':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"efd");            strcpy(segments[3],"bcd");
								break;
						case 'p':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"efg");            strcpy(segments[3],"gb");
								break;
						case 'q':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"fg");            strcpy(segments[3],"bcg");
								break;
						case 'r':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"ef");            strcpy(segments[3],"");
								break;
						case 's':
								strcpy(segments[0],"d");         strcpy(segments[1],"d");         strcpy(segments[2],"fgd");            strcpy(segments[3],"gcd");
								break;
						case 't':
								strcpy(segments[0],"ed");         strcpy(segments[1],"");         strcpy(segments[2],"efd");            strcpy(segments[3],"d");
								break;
						case 'u':
								strcpy(segments[0],"");         strcpy(segments[1],"");         strcpy(segments[2],"fed");            strcpy(segments[3],"bcd");
								break;
						case 'v':
								strcpy(segments[0],"");         strcpy(segments[1],"");         strcpy(segments[2],"hk");            strcpy(segments[3],"ij");
								break;
						case 'w':
								strcpy(segments[0],"");         strcpy(segments[1],"");         strcpy(segments[2],"efij");            strcpy(segments[3],"bchk");
								break;
						case 'x':
								strcpy(segments[0],"");         strcpy(segments[1],"");         strcpy(segments[2],"hkij");            strcpy(segments[3],"");
								break;
						case 'y':
								strcpy(segments[0],"");         strcpy(segments[1],"");         strcpy(segments[2],"hij");            strcpy(segments[3],"");
								break;
						case 'z':
								strcpy(segments[0],"d");         strcpy(segments[1],"");         strcpy(segments[2],"ijd");            strcpy(segments[3],"");
								break;
						default : espace=true;
				}
				if(!espace)
				{   for(int n=0;n<strlen(segments[0]);n++)
						ChangerCouleurSegment(coord,taille,epaisseur,couleur,segments[0][n]);
					for(int n=0;n<strlen(segments[1]);n++)
						ChangerCouleurSegment(coord2,taille,epaisseur,couleur,segments[1][n]);
					for(int n=0;n<strlen(segments[2]);n++)
						ChangerCouleurSegment(coord3,taille,epaisseur,couleur,segments[2][n]);
					for(int n=0;n<strlen(segments[3]);n++)
						ChangerCouleurSegment(coord4,taille,epaisseur,couleur,segments[3][n]);
				}
				coord.colonne+=Arrondi(4*((float)taille/2));
				coord2.colonne+=Arrondi(4*((float)taille/2));
				coord3.colonne+=Arrondi(4*((float)taille/2));
				coord4.colonne+=Arrondi(4*((float)taille/2));
		}
}

