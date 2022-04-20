#pragma hdrstop
#pragma argsused
#include"SNClientTCP.h"
#include<iostream>
#ifdef _WIN32
#include <tchar.h>
#else
  typedef char _TCHAR;
  #define _tmain main
#endif
#include <stdio.h>
using namespace std;
int main ()
{
	SNClientTCP  monClient; //creation d'un objet monClient de la classe

	char trameCAN[50];
	char idCAN[10];

	while (true)
	{
	std::cout<<"saisir l'identifiant CAN:";
	std::cin>> idCAN;
	monClient.SeConnecterAUnServeur("172.20.15.54",4111);
	monClient.Envoyer(idCAN,3);
	monClient.Recevoir(trameCAN,50);
	std::cout<<trameCAN<<endl;
	}
}


