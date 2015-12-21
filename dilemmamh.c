    // dilemmamh.c - versione 0.03
	
    // Copyright 2015 Sandro Bellone
	
    // This program is free software: you can redistribute it and/or modify
    // it under the terms of the GNU General Public License as published by
    // the Free Software Foundation, either version 3 of the License, or
    // (at your option) any later version.

    // This program is distributed in the hope that it will be useful,
    // but WITHOUT ANY WARRANTY; without even the implied warranty of
    // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    // GNU General Public License for more details.

    // You should have received a copy of the GNU General Public License
    // along with this program.  If not, see <http://www.gnu.org/licenses/>.
	
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define OEMRESOURCE
#include<windows.h>
#include<windowsx.h>
#include"resource.h"
#include<time.h>
#include<stdio.h>

int scelta (int);
int terza_scelta(int,int);
int escludi_vera(int,int);
BOOL CALLBACK DlgProc(HWND , UINT , WPARAM , LPARAM );
void OnDlgClose(HWND );
BOOL OnDlgInitDialog(HWND , HWND , LPARAM );
void OnDlgCommand(HWND , int , HWND , UINT );

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	srand((unsigned)time(NULL));      // sceglie il seme
	DialogBoxParam(0,(LPCTSTR)IDD_DIALOG1,0,(DLGPROC)DlgProc,0);
	printf("ciao!\n");
	return 0;
}

void OnDlgClose(HWND hWnd) {
	EndDialog(hWnd,0);
}

BOOL OnDlgInitDialog(HWND hwndDlg, HWND hwndFocus, LPARAM lParam) {
	SetDlgItemText(hwndDlg, IDC_EC_ITE,"1000");
	CheckRadioButton(hwndDlg,IDC_RB_RAND,IDC_RB_ALT,IDC_RB_RAND);
}

void OnDlgCommand(HWND hwndDlg, int wId, HWND hwndCtl, UINT wNotifica) {
	if(wNotifica==BN_CLICKED && wId == IDC_BT_AVVIA) {
		int x, y, z, c, i, j,nite;
		int numero_successi=0, numero_scambi=0, numero_successi_con_scambio=0,
		    numero_successi_senza_scambio=0;
		char buff[10];
		nite=GetDlgItemInt(hwndDlg,IDC_EC_ITE,NULL,FALSE);
		for (i=0; i<nite; i++) {
			x=scelta(3);		// ok
			y=scelta(3);		// tentativo
			z=escludi_vera(x,y);
			if(IsDlgButtonChecked(hwndDlg,IDC_RB_RAND)) {
				if(scelta(2)) c=y;
				else c=terza_scelta(y,z);
			} else if(IsDlgButtonChecked(hwndDlg,IDC_RB_SEMPRE)) c=y;
			else if (IsDlgButtonChecked(hwndDlg,IDC_RB_MAI)) c=terza_scelta(y,z);
			else if (IsDlgButtonChecked(hwndDlg,IDC_RB_ALT)) {
				if (i%2) c=y;
				else c=terza_scelta(y,z);
			}
			if (x==c) {
				numero_successi++;
				if(y!=c) {
					numero_successi_con_scambio++;
				} else numero_successi_senza_scambio++;
			}
			if (y!=c) numero_scambi++;
		}
		printf("Iterazioni: %d; scambi: %d; successi: %d,\n"
		       "di cui con scambio: %d, senza scambio: %d\n",
		       i, numero_scambi, numero_successi, numero_successi_con_scambio, numero_successi_senza_scambio);
		SetDlgItemInt(hwndDlg, IDC_EC_NITE,i,FALSE);
		SetDlgItemInt(hwndDlg, IDC_EC_NS, numero_successi, FALSE);
		SetDlgItemInt(hwndDlg, IDC_EC_NSE, numero_scambi, FALSE);
		SetDlgItemInt(hwndDlg, IDC_EC_NSS, numero_successi_con_scambio, FALSE);
	} else if(wNotifica==BN_CLICKED && wId == IDC_BT_ESCI) {
		EndDialog(hwndDlg, 0);
	} else if(wNotifica==BN_CLICKED && wId == IDC_BT_RST) {
		SetDlgItemText(hwndDlg, IDC_EC_ITE,"1000");
		CheckRadioButton(hwndDlg,IDC_RB_RAND,IDC_RB_ALT,IDC_RB_RAND);
	} else if(wNotifica==BN_CLICKED && wId == IDC_BT_ABOUT) {
		char testo[100];
		sprintf(testo,"Software di simulazione del dilemma di Monty Hall\n"
						"Sandro Bellone\nNovembre 2015");
		MessageBox(0, testo, "About", MB_OK);
	}
}

BOOL CALLBACK DlgProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch(uMsg) {
			HANDLE_MSG(hwndDlg,WM_COMMAND,OnDlgCommand);
			HANDLE_MSG(hwndDlg,WM_CLOSE,OnDlgClose);
			HANDLE_MSG(hwndDlg,WM_INITDIALOG,OnDlgInitDialog);
	}
	return 0;
}

int scelta(int i) {				// ritorna un numero intero casuale da 0 a i
	return (int)(rand()%i);
}

int terza_scelta(int x, int y) { // dati due numeri differenti da 1 a 3, ritorna il terzo
	if((x+y)==1) return 2;
	if((x+y)==2) return 1;
	return 0;
}

int escludi_vera(x,y) {			// sceglie un numero sbagliato
	int k;
	if (x==y) {
		k=scelta(2);
		if (x==0) return k+1;	// x==0; y==0;
		if (x==1) return k*2;	// x==1; y==1;
		else return k;			// x==2; y==2;
	}
	return terza_scelta(x,y);
}
