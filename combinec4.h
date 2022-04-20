//---------------------------------------------------------------------------

#ifndef combinec4H
#define combinec4H
//---------------------------------------------------------------------------
#endif

class combinec4
{
	private: char idFeux[4];
			 char idMoteur[4];
			 char donneFaux[17];
			 int regime;
			 int vitesse;
			 unsigned int chaineHexaVersint(char*chaineHexa,int NbCar);

	public: combinec4();
			 void EnregistrerDonnee(char*id,char*donnee);
			 void CalculerRegime();
			 void CalculerVitesse();
			 int Vitesse();
			 int Regime();
			 char*DonneeCAN(char*id);


};
