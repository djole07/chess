#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
	char boja;			// odredjuje koje je boje figura na datom polju
	char figura;
}Sah;

void popuni(Sah [][8]);
void stampaj(Sah [][8]);
void dodaj_figure(Sah [][8]);
void nadji_potez(char [], int *, int *, int *, int *);
int odredi_figuru(Sah [][8], int , int , int , int );
int odigraj_potez(Sah [][8], int, int, int, int);
int pawnW(Sah [][8], int , int , int , int );
int pawnB(Sah [][8], int , int , int , int );
int rook(Sah [][8], int , int , int , int );
int knight(Sah [][8], int , int , int , int );
int bishop(Sah [][8], int , int , int , int );
int queen(Sah [][8], int , int , int , int );
int kingW(Sah [][8], int , int , int , int );
int kingB(Sah [][8], int , int , int , int );
int greska();
void checkmate(Sah);

int main() {
	Sah tabla[8][8];
	char Igrac[2] = {'B', 'W'};
	char potez[6];
	int preJ = 0, preI = 0, posleJ = 0, posleI = 0, status;
	
	popuni(tabla);
	dodaj_figure(tabla);
	int i = 0;
	while(++i) {
		do {
			system("cls");
			stampaj(tabla);
			printf("Unesite potez (igra %c): ", Igrac[i%2]);
			gets(potez);
			nadji_potez(potez, &preJ, &preI, &posleJ, &posleI);
		} while(tabla[preI][preJ].boja != Igrac[i%2] && printf("To nije tvoja figura\n"));
		
			
			status = odredi_figuru(tabla, preJ, preI, posleJ, posleI);
			if(status > 0) i--;
			else if(status < 0) i = -1;		// usledio je sah-mat
			// uvodimo sledecu komandu da bismo se uverili da ako korisnik pogresno unese potez,
			// da moze ponovo da ga odigra(inace ukoliko beli pogresi potez, onda ce opet crni igrati)
	}
	
	return 0;
}

void popuni(Sah tabla[][8]) {
	for(int i = 0 ; i < 8 ; i++)
		for(int j = 0 ; j < 8 ; j++) {
			tabla[i][j].figura = '0';
			tabla[i][j].boja = '0';
		}
}
void dodaj_figure(Sah tabla [][8]) {
	int i, j;
// white
	for(i = 6, j = 0 ; j < 8 ; j++) {							// pawns
		tabla[i][j].boja 	= 'W';
		tabla[i][j].figura	= 'P';
	}	
	for(i = 7, j = 0 ; j < 8 ; j++)
		tabla[i][j].boja = 'W';
	tabla[i][0].figura = 'R' ; tabla[i][7].figura = 'R' ;		// rooks
	tabla[i][1].figura = 'N' ; tabla[i][6].figura = 'N' ;		// knights
	tabla[i][2].figura = 'B' ; tabla[i][5].figura = 'B' ;		// bishops
	tabla[i][3].figura = 'Q' ; tabla[i][4].figura = 'K' ;		// queen king
	
// black
	for(i = 1, j = 0 ; j < 8 ; j++) {							// pawns
		tabla[i][j].boja 	= 'B';
		tabla[i][j].figura	= 'p';
	}	
	for(i = 0, j = 0 ; j < 8 ; j++)
		tabla[i][j].boja = 'B';
	tabla[i][0].figura = 'r' ; tabla[i][7].figura = 'r' ;		// rooks
	tabla[i][1].figura = 'n' ; tabla[i][6].figura = 'n' ;		// knights
	tabla[i][2].figura = 'b' ; tabla[i][5].figura = 'b' ;		// bishops
	tabla[i][3].figura = 'q' ; tabla[i][4].figura = 'k' ;		// queen king
}

void stampaj(Sah tabla[][8]) {
	puts("\n");
	for(int i = 0 ; i < 8 ; i++) {
		printf("\t %d  ", 8-i);
		for(int j = 0 ; j < 8 ; j++)
			if(tabla[i][j].figura == '0') printf("- ");
			else printf("%c ", tabla[i][j].figura);
	printf("\n");
	}
	printf("\n\t    a b c d e f g h \n\n");
}
void nadji_potez(char potez[], int *preJ, int *preI, int *posleJ, int *posleI) {
	*preI = '8' - potez[1];
	*preJ = potez[0] - 'a';
	*posleI = '8' - potez[4];
	*posleJ = potez[3] - 'a';
}
int odredi_figuru(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	switch(tabla[preI][preJ].figura) {
		case 'P': pawnW(tabla, preJ, preI, posleJ, posleI)	; break;
		case 'p': pawnB(tabla, preJ, preI, posleJ, posleI)	; break;
		case 'K': kingW(tabla, preJ, preI, posleJ, posleI)	; break;
		case 'k': kingB(tabla, preJ, preI, posleJ, posleI)	; break;
		case 'R': case 'r': rook(tabla, preJ, preI, posleJ, posleI)		; break;
		case 'N': case 'n': knight(tabla, preJ, preI, posleJ, posleI)	; break;
		case 'B': case 'b': bishop(tabla, preJ, preI, posleJ, posleI)	; break;
		case 'Q': case 'q': queen(tabla, preJ, preI, posleJ, posleI)	; break;
		default : greska() 									; break;
	}
}
int pawnW(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	if((preI != 6 && (preI - posleI) >= 2) || (preI - posleI) > 2) {		// pijun moze da ide 2 koraka samo ako je na 2. odnosno 7. liniji
			return greska();
	}
	if(preJ == posleJ && tabla[preI-1][posleJ].figura == '0' && preI - posleI >= 1) {			// kretanje napred
		if(preI==6 && posleI==4 && tabla[posleI][posleJ].figura != '0') {
			return greska();
		}
		odigraj_potez(tabla, preJ, preI, posleJ, posleI);
		if(posleI == 0) {	// promocija
			printf("Da li zelite topa(R), lovca(B), konja(N) ili kraljicu(Q)? ");
			tabla[posleI][posleJ].figura = getchar();
			tabla[posleI][posleJ].boja = 'W';
			getchar();
		}			
	}
	else		// ispitamo da li mozda jede
		if(posleJ>=0 && posleJ<8 && (preJ-posleJ == 1 || preJ-posleJ == -1) && (preI-posleI == 1) && tabla[posleI][posleJ].figura != '0')
			return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
		else {
			return greska();
		}
}
int pawnB(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {

	if((preI != 1 && (posleI - preI) >= 2) || (posleI - preI) > 2) {		// pijun moze da ide 2 koraka samo ako je na 2. odnosno 7. liniji
			return greska();
	}
	if(preJ == posleJ && tabla[preI+1][posleJ].figura == '0' && posleI - preI >= 1) {			// kretanje napred
		if(preI==1 && posleI==3 && tabla[posleI][posleJ].figura != '0') {
			return greska();
		}
		odigraj_potez(tabla, preJ, preI, posleJ, posleI);
		if(posleI == 7) {	// promocija
			printf("Da li zelite topa(r), lovca(b), konja(n) ili kraljicu(q)? ");
			tabla[posleI][posleJ].figura = getchar();
			tabla[posleI][posleJ].boja = 'B';
			getchar();
		}			
	}
	else		// ispitamo da li mozda jede
		if(posleJ>=0 && posleJ<8 && (preJ-posleJ == 1 || preJ-posleJ == -1) && (posleI-preI == 1) && tabla[posleI][posleJ].figura != '0')
			return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
		else {
			return greska();
		}
}
int rook(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	if(preI == posleI) {
		if(posleJ > preJ) {			// kretanje u desno
			for(int j = preJ + 1 ; j < posleJ ; j++)
				if (tabla[posleI][j].figura != '0') {
					return greska();
					
				} 
			return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
		}
		else {		// posleJ < preJ		kretanje u levo
			 for(int j = preJ - 1 ; j > posleJ ; j--)
				if (tabla[posleI][j].figura != '0') {
					return greska();
				} 
			return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
			}
	}
	else if(preJ == posleJ) {
			if(posleI < preI) { // kreanje ka gore
				for(int i = preI - 1 ; i > posleI ; i--) 
					if(tabla[i][posleJ].figura != '0') {
						return greska();
					}
				return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
			}
			else {				// posleI > preI	kretanje na dole
				for(int i = preI + 1 ; i < posleI ; i++)
					if(tabla[i][posleJ].figura != '0') {
						return greska();
					}
				return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
			}
	}
	else greska();
}
int knight(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	if(posleJ != preJ && posleI != preI && (fabs(posleI - preI)+fabs(posleJ - preJ))==3)
		return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
	else return greska();
}
int bishop(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	int i = preI, j = preJ;
	if(posleI > preI)
		if(posleJ > preJ) {		// dole desno
			do {
				i++;
				j++;
			} while(tabla[i][j].figura == '0' && i != posleI && j != posleJ);		// ako je nakon petlje i==posleI (znaci brojaci su stigli do trazene pozicije) onda nema figura izmedju
		}
		else {			// dole levo
			do{
				i++;
				j--;
			} while(tabla[i][j].figura == '0' && i != posleI && j != posleJ);
		}
	else {				//	gore levo
		if(posleJ < preJ) {
			do {
				i--;
				j--;
			} while(tabla[i][j].figura == '0' && i != posleI && j != posleJ);
		}
		else {		//gore desno
			do {
				i--;
				j++;
			} while(tabla[i][j].figura == '0' && i != posleI && j != posleJ);
		}
	}
	if(i == posleI && j == posleJ) return odigraj_potez(tabla, preJ, preI, posleJ, posleI);
	else 
		if(tabla[preI][preJ].figura != 'Q' || tabla[preI][preJ].figura != 'q') return greska();
}
int queen(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	rook(tabla, preJ, preI, posleJ, posleI);
	bishop(tabla, preJ, preI, posleJ, posleI);	// ukoliko ne moze da ucini potez kao top ili lovac, javlja gresku
}
int kingW(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	if(preI == 7 && preJ == 4 && posleI == 7) {		// mala rokada
		if(posleJ == 7 && tabla[7][7].figura == 'R' && tabla[7][6].figura == '0' && tabla[7][5].figura == '0')	{
			tabla[7][6].figura = 'K';
			tabla[7][5].figura = 'R';
			tabla[7][7].figura = '0';
			tabla[7][4].figura = '0';
		}
		else if(posleJ == 0 && tabla[7][0].figura == 'R' && tabla[7][1].figura == '0' && tabla[7][2].figura == '0' && tabla[7][3].figura == '0') {	// velika rokada
			tabla[7][2].figura = 'K';
			tabla[7][3].figura = 'R';
			tabla[7][0].figura = '0';
			tabla[7][4].figura = '0';
		}
	}
	else if(fabs(preI - posleI) <= 1 && fabs(preJ - posleJ) <= 1)
		odigraj_potez(tabla, preJ, preI, posleJ, posleI);
	else greska();
}
int kingB(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
}
int odigraj_potez(Sah tabla[][8], int preJ, int preI, int posleJ, int posleI) {
	if(tabla[posleI][posleJ].boja == tabla[preI][preJ].boja) {
		printf("Ne mozete pojesti svoju figuru\n");
		return greska();
	}
	else {
		if(tabla[posleI][posleJ].figura == 'K' || tabla[posleI][posleJ].figura == 'k') {	// checkmate
			checkmate(tabla[posleI][posleI]);
			tabla[posleI][posleJ].figura = tabla[preI][preJ].figura;
			tabla[posleI][posleJ].boja = tabla[preI][preJ].boja;
			return -1;
		}
		tabla[posleI][posleJ].figura = tabla[preI][preJ].figura;
		tabla[posleI][posleJ].boja = tabla[preI][preJ].boja;
		
		tabla[preI][preJ].figura = '0';
		tabla[preI][preJ].boja = '0';
		printf("\t Potez odigran\n");
		return 0;
	}
}
int greska() {
	printf("Invalid move!\n");
	return 1;
}
void checkmate(Sah tabla) {
	if(tabla.boja == 'W') printf("Pobedio je crni!\n");
	else printf("Pobedio je beli\n");
	exit(0);
}
