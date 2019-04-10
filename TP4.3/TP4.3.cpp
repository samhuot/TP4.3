#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;


string const cli("clients.dat");
string const inv("inventaire.dat");
string const factu("factures.dat");

typedef struct facture fac;
struct facture {
	string no_cli;
	string no_prod;
	char desc[21];
	int quantite;
	float prix;
	float montant;
};
fac un;

void Nettoyer(bool, bool);
void Menu(int&);
void Achat(fac &un);
void Liste(fac &un);
void Etat(fac &un);

int main()
{
	int choix = 0;
	do {
		Menu(choix);

		switch (choix) {
		case 1:
			Achat(un);
			break;
		case 2:
			Liste(un);
			break;
		case 3:
			Etat(un);
			break;
		case 9:
			break;
		}
	} while (choix != 9);
}

void Nettoyer(bool pause, bool nettoyer)
{
	if (pause) {
		system("pause");
	}
	if (nettoyer) {
		system("CLS");
	}
}

void Menu(int &choix) {
	Nettoyer(0, 1);

	cout << "\n\nMenu principal" << endl << endl
		<< "	1 - Achat" << endl
		<< "	2 - Facture accumulees" << endl
		<< "	3 - Etats de compte" << endl
		<< "	9 - Quitter le programme" << endl << endl << endl
		<< "	Votre choix: ";
	cin >> choix;
}

void Achat(fac &un)
{
	Nettoyer(0, 1);

	string cli_achat;
	int prod_achat = 0;

	cout << "=== FACTURATION ===\n\n\n";
	while (prod_achat != 999)
	{
		cout << "Numero de produit: ";
		cin >> prod_achat;
		while (prod_achat != 999)
		{ 
			cout << "Quantite: ";
			cin >> un.quantite;
			cout << "\nNumero de client: ";
			cin >> cli_achat;
			cin.ignore();

			//PARTIE CLIENT
			fstream clientFic(cli, ios::binary | ios::in);

			int sentinelle = 0;

			string nomPrenom, nocivic, rue, ville;
			float solde=0;
			char type;
			int cod_valid=0;

			while (sentinelle != 1)
			{
				clientFic.read((char *)&un.no_cli, sizeof(un.no_cli));

				if (cli_achat == un.no_cli)
				{
					clientFic.read((char *)&nomPrenom, sizeof(nomPrenom));
					clientFic.read((char *)&solde, sizeof(solde));
					clientFic.read((char *)&nocivic, sizeof(nocivic));
					clientFic.read((char *)&rue, sizeof(rue));
					clientFic.read((char *)&ville, sizeof(ville));
					clientFic.read((char *)&type, sizeof(type));
					clientFic.read((char *)&cod_valid, sizeof(cod_valid));

					sentinelle = 1;
				}
				else
				{
					clientFic.read((char *)&nomPrenom, sizeof(nomPrenom));
					clientFic.read((char *)&solde, sizeof(solde));
					clientFic.read((char *)&nocivic, sizeof(nocivic));
					clientFic.read((char *)&rue, sizeof(rue));
					clientFic.read((char *)&ville, sizeof(ville));
					clientFic.read((char *)&type, sizeof(type));
					clientFic.read((char *)&cod_valid, sizeof(cod_valid));
				}
			}
			cout << "Nom et prenom: " << nomPrenom << endl << endl;
			clientFic.close();

			// PARTIE INVENTAIRE

			fstream invFic(inv, ios::binary || ios::in);
			int no_produit = 0, qte;
			double coutProd, prix;
			string description;

			sentinelle = 0;
			while (sentinelle != 1)
			{
				invFic.read((char *)&no_produit, sizeof(no_produit));

				if (prod_achat == no_produit)
				{
					invFic.read((char *)&description, sizeof(description));
					invFic.read((char *)&coutProd, sizeof(coutProd));
					invFic.read((char *)&prix, sizeof(prix));
					invFic.read((char *)&qte, sizeof(qte));

					un.no_prod = to_string(no_produit);
					strcpy_s(un.desc, description.c_str());
					un.prix = prix;
					un.montant = un.prix * un.quantite;
					sentinelle = 1;
				}
				else {
					invFic.read((char *)&description, sizeof(description));
					invFic.read((char *)&coutProd, sizeof(coutProd));
					invFic.read((char *)&prix, sizeof(prix));
					invFic.read((char *)&qte, sizeof(qte));
				}
			}
			invFic.close();

			ofstream facFic(factu, ios::binary | ios::app);

			facFic.write((char *)&un.no_cli, sizeof(un.no_cli));
			facFic.write((char *)&un.no_prod, sizeof(un.no_prod));
			facFic.write((char *)&un.desc, sizeof(un.desc));
			facFic.write((char *)&un.quantite, sizeof(un.quantite));
			facFic.write((char *)&un.prix, sizeof(un.prix));
			facFic.write((char *)&un.montant, sizeof(un.montant));

			cout << setw(10) << left << un.no_prod;
			cout << setw(30) << left << un.desc;
			cout << setw(20) << left << un.quantite;
			cout << setw(20) << left << setprecision(2) << fixed << un.prix; 
			cout << setprecision(2) << fixed << un.montant << endl; 

			facFic.close();
		}

	}
	Nettoyer(1, 0);

}

void Liste(fac &fac)
{
	Nettoyer(0, 1);

	ifstream facFic(factu, ios::binary | ios::in);

	cout << "=== LISTE DES FACTURES DANS LE FICHIER 'factures.dat'===\n\n";

	cout << left << setw(9) << "Cli";
	cout << left << setw(20) << "No.Prod";
	cout << left << setw(20) << "Description";
	cout << left << setw(20) << "Qtite";
	cout << left << setw(15) << "Prix";
	cout << "Montant" << endl;

	while (!facFic.eof())
	{
		facFic.read((char *)&un.no_cli, sizeof(un.no_cli));
		facFic.read((char *)&un.no_prod, sizeof(un.no_prod));
		facFic.read((char *)&un.desc, sizeof(un.desc));
		facFic.read((char *)&un.quantite, sizeof(un.quantite));
		facFic.read((char *)&un.prix, sizeof(un.prix));
		facFic.read((char *)&un.montant, sizeof(un.montant));

		if (!facFic.eof())
		{
			cout << left << setw(9) << un.no_cli;
			cout << left << setw(20) << un.no_prod;
			cout << left << setw(20) << un.desc;
			cout << left << setw(20) << un.quantite;
			cout << left << setw(15) << setprecision(2) << fixed << un.prix;
			cout << setprecision(2) << fixed << un.montant << endl;
		}
	}

	facFic.close();

	Nettoyer(1, 0);
}

void Etat(fac &fac)
{
	Nettoyer(0, 1);

	float total = 0;
	string numcli = "";

	cout << "=== ETATS DE COMPTES ===\n\n";

	ifstream facFic(factu, ios::binary | ios::in);

	while (!facFic.eof())
	{

		facFic.read((char *)&un.no_cli, sizeof(un.no_cli));
		if (numcli != un.no_cli && numcli != "")
		{
			cout << "\tClient: " << numcli << "\tCUMUL MONTANT: " << setprecision(2) << fixed << total << endl << endl;
			total = 0;
		}
		numcli = un.no_cli;
		facFic.read((char *)&un.no_prod, sizeof(un.no_prod));
		facFic.read((char *)&un.desc, sizeof(un.desc));
		facFic.read((char *)&un.quantite, sizeof(un.quantite));
		facFic.read((char *)&un.prix, sizeof(un.prix));
		facFic.read((char *)&un.montant, sizeof(un.montant));
		if (!facFic.eof())
		{
			total = total + un.montant;
			cout << "Client: " << un.no_cli << "\tMontant: " << setprecision(2) << fixed << un.montant << endl << endl;
		}

	}
	
	cout << "\tClient: " << numcli << "\tCUMUL MONTANT: " << setprecision(2) << fixed << total << endl << endl;

	Nettoyer(1, 0);

	facFic.close();

}



