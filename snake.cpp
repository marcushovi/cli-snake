#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>


#define maxRiadkov 20
#define maxStlpcov 40

int  i, j, hraciePole[maxRiadkov][maxStlpcov], x, y, Gy, hlava, chvost, hra, jedlo, jedloX, jedloY, var, smer, skore, HighScore, rychlost;

FILE *f;

void start(){
	
	// naskenovanie najvacsieho skore
    f=fopen("highscore.txt","r");
    fscanf(f,"%d",&HighScore);
    fclose(f);
    
    // vyynulovanie pola
    for(i=0;i<maxRiadkov;i++){
        for(j=0;j<maxStlpcov;j++){
            hraciePole[i][j]=0;
        }
    }
    
//    00000000
//    76543200
//    0000-100
    
    // nastavime pociatocno poziciu hada
    x=maxRiadkov/2;
    y=maxStlpcov/2;
    Gy = y;
    
    hlava = 5;
    chvost =1;
    hra = 0,
    jedlo = 0,
    
    // smer doprava
    smer='d',
    
    skore=0,
    rychlost = 99;

    for(i=0;i<hlava;i++){
        Gy++;
        hraciePole[x][Gy-hlava] = i+1;
    }
}


int getch_noblock(){
	
	// ak stlacil klavesnicu vrat stlaceny znak inak -1 
    if(_kbhit())
        return _getch();
    else
        return -1;
}

void GameOver(){
	
    printf("\a");
    Sleep(1500);
    system("Cls");

    if(skore>HighScore){
    	
        printf("  New HighScore %d!!!!!!\n\n",skore);
        system("pause");
        
        f=fopen("highscore.txt","w");
        fprintf(f,"%d",skore);
        fclose(f);
    }

    system("Cls");
    printf("\n\n         GAME OVER !!!!!!\n");
    printf("             Score : %d \n\n",skore);
    printf("             Press ENTER to play again or ESC to exit ... \n");


    while(1){
        var = getch_noblock();
        if(var == 13){
            hra = 0;
            start();
            break;
        }else if(var == 27){
            hra = 1;
            break;
        }
    }
    
    system("Cls");
}

void vypis(){
	
	// prvy riadok vo vypise 
    for(i=0;i<=maxStlpcov+1;i++){
    	
    	
        if(i==0){
        	
        	// lavy horny roh 
            printf("%c",201); 
            
        }else if(i==maxStlpcov+1){
        	
        	// pravy horny roh
            printf("%c",187); 
            
        }else{
        	
        	// vodorovna horna dojciara
            printf("%c",205); 
        }
    }
    
    // vypis skore jedloX najvyssieho skore
    printf("   Current Score: %d  HighScore: %d",skore,HighScore);
    printf("\n");

    for(i=0;i<maxRiadkov;i++){
    	
    	// lava vertikalna dvojciara
        printf("%c",186); 

        for(j=0;j<maxStlpcov;j++){
		
			// ak hodnota prvku je nula vypis medzeru        	
            if(hraciePole[i][j]==0) printf(" "); 
            
            // ak hodnota prvku je vacsia ako nula (cize telo hada) jedloX zaroven sa nerovna hlave tak vypis znak tela
            if(hraciePole[i][j]>0 && hraciePole[i][j]!=hlava) printf("%c",9633);  
            
            
            // znak hlavy
            if(hraciePole[i][j]==hlava) printf("%c",178); 
            
            // ak sa rovna hodnota prvok pola -1 (to znamena jedlo) tak vypis znak jedla
            if(hraciePole[i][j]==-1) printf("%c",15);
            
            // prava vertikalna dvojciara
            if(j==maxStlpcov-1) printf("%c\n",186); 
        }
    }

    for(i=0;i<=maxStlpcov+1;i++){
        if(i==0){
            printf("%c",200); // lavy dolny roh
        }else if(i==maxStlpcov+1){
            printf("%c",188); // pravy dolny roh
        }else{
            printf("%c",205); // dolna vodorovna dvojciara
        }
    }
}
void ResetScreenPosition(){
	
	// vymaz co je v konzole
	
    HANDLE hOut;
    COORD Position;
    hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    Position.X = 0;
    Position.Y = 0;
    SetConsoleCursorPosition(hOut,Position);
}


void Random(){
	
    srand(time(0));
    
    // vygenerovanie navych suradnic pre jedlo
    jedloX = 1+ rand() % 18;
    jedloY = 1 + rand() % 38;

	// ak jedlo sa rovna 0 a zaroven  na poli nie je jedlo
    if(jedlo == 0 && hraciePole[jedloX][jedloY]==0){
    	
    	// nastavime poziciu jedla
        hraciePole[jedloX][jedloY]= -1;
        jedlo = 1;
        
        // takzvane zrychlovanie pohybu hada po kazdom zjedeni jedla
        if(rychlost>10 && skore!=0) 
		rychlost = rychlost - 5;
        
    }
}


void pohyb(){
	
	// ulozenie smeru zadaneho z klavesnice
    var = getch_noblock();
    // ak by bol zapnuty Capslock tak zmensi velke pismeno na male
    var = tolower(var);
    
    // skontrolujeme ci zadal z klavesnice w, a, s, d  a zaroven ci plati ze nezadal s hned w alebo a hned po d alebo naopak 
    // jednoduche nemoze ist dolava ak ide doprava alebo ist hore ked ide dole
    
    if( ((var=='d'||var=='a') || (var=='w'||var=='s') ) && ( abs(smer-var) > 5 ) ) 
		smer = var;
    
    // doprava
    if(smer =='d'){
    	
    	// posunieme suradnicu hada doprava
        y++;
        
        // ak je na konci pri pravej stene tak ho presun na lavu stranu
        if(y==maxStlpcov-1) y = 0;
        
        // ak narazil do chvosta tak koniec hry
        if(hraciePole[x][y]!=0 && hraciePole[x][y]!=-1) GameOver();
        
        // ak ziedol jedlo, zvys skore, zmensi chvost
        if(hraciePole[x][y]==-1){
            jedlo = 0;
            skore+=5;
            chvost -= 1;
        }

        hlava++;
        hraciePole[x][y] = hlava;
    }
    
    // dolava
    if(smer =='a'){
        y--;
        if(y==0) y = maxStlpcov-1;
        
        if(hraciePole[x][y]!=0 && hraciePole[x][y]!=-1) GameOver();
        
        if(hraciePole[x][y]==-1){
            jedlo = 0;
            skore+=5;
            chvost -= 1;
        }
        hlava++;
        hraciePole[x][y] = hlava;
    }
    
    // hore
    if(smer =='w'){
        x--;
        if(x==-1) x = maxRiadkov-1;
        
        if(hraciePole[x][y]!=0 && hraciePole[x][y]!=-1) GameOver();
        
        if(hraciePole[x][y]==-1){
            jedlo = 0;
            skore+=5;
            chvost -= 1;
        }
        hlava++;
        hraciePole[x][y] = hlava;
    }
    
    // dole
    if(smer =='s'){
        x++;
        
        if(x==maxRiadkov-1) x = 0;
        
        if(hraciePole[x][y]!=0 && hraciePole[x][y]!=-1) GameOver();
        
        if(hraciePole[x][y]==-1){
            jedlo = 0;
            skore+=5;
            chvost -= 1;
        }
        hlava++;
        hraciePole[x][y] = hlava;
    }
}

void vymazChvost(){
	
    for(i=0;i<maxRiadkov;i++){
    	
        for(j=0;j<maxStlpcov;j++){
        	
			// ak sa prvok pola rovna poslednej casti tela hada tak ju vymaz        	
            if(hraciePole[i][j]==chvost){
                hraciePole[i][j] = 0;
            }
        }
    }
    
    chvost++;
}


int main(){
    start();

    while(hra == 0){
        vypis();
        ResetScreenPosition();
        Random();
        pohyb();
        vymazChvost();
        Sleep(rychlost);
    }
}
