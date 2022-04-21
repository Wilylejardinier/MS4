//---------------------------------------------------------------------------

#pragma hdrstop
#include <string.h>
#include "combinec4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
// definition du constructeur
combinec4::combinec4()
{
	strcpy(idFeux,"128");
	strcpy(idMoteur,"0B6");
}
void combinec4::EnregistrerDonnee(char*id,char*donnee)
  {
   if (!strcmp (idFeux,id))


  }

void combinec4::CalculerRegime()
  {

  }

void combinec4::CalculerVitesse()
  {

  }

int combinec4::Vitesse()
  {

  }

int combinec4::Regime()
  {

  }

char*combinec4::DonneeCAN(char*id)
  {

  }
