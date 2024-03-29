/* ALUNOS */
/* GUSTAVO HENRIQUE ZENI - RA: 1985370 */
/* IANCA POLIZELO - RA: 1985388 */
/* VITOR GABRIEL ANDRADE TEIXEIRA - RA: 1655655 */

#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

// Um objeto representando um ponto
typedef struct
{
    double x, y;
} Ponto;

//Um objeto representando a dist�ncia entre dois pontos
typedef struct
{
    Ponto p1, p2;
    double d;
}Dist;

int compararX(const void* a, const void* b) {
    Ponto *p1 = (Ponto *) a;
    Ponto *p2 = (Ponto *) b;
    if(p1->x < p2->x) return(-1);
    if(p1->x <= p2->x+0.0000001 && p1->y <= p2->y) {
     return (-1);
    }
    else return(1);
}

int compararY(const void* a, const void* b) {
    Ponto *p1 = (Ponto *) a;
    Ponto *p2 = (Ponto *) b;
    if(p1->y < p2->y) return(-1);
    if(p1->y <= p2->y+0.0000001 && p1->x <= p2->x) {
     return (-1);
    }
    else return(1);
}

//Fun��o que calcula a dist�ncia eucliana entre dois pontos
Dist distance(Ponto p1, Ponto p2)
{
    Dist d;
    d.d = sqrt(pow((p1.x - p2.x),2)+pow((p1.y - p2.y),2));
    d.p1 = p1;
    d.p2 = p2;
    return(d);
}

//Fun��o que calcula a menor dist�ncia entre um array de pontos
Dist closest(Ponto X[], Ponto Y[], int tamanho)
{
    //Vari�veis:
    Dist d; //� a dist�ncia que ser� retornado
    Dist d_esq; //Dist�ncia do lado esquerdo da divis�o
    Dist d_dir; //Dist�ncia do lado direito da divis�o

    Ponto mid; //Vari�vel que vai receber o ponto do meio onde faz a divis�o pra direita e esquerda

    int meio = tamanho/2; //Metade do vetor

    //Caso base:
    //Se tiver 2 pontos, apenas retorna a distancia entre eles
    if(tamanho == 2)
    {
        d = distance(X[0],X[1]);
        return(d);
    }
    //Se tiver 3 pontos, calcula a distancia entre os 3 e retorna a menor distancia
    if(tamanho == 3)
    {
        Dist d1 = distance(X[0],X[1]);
        Dist d2 = distance(X[0],X[2]);
        Dist d3 = distance(X[1],X[2]);

        if(d1.d<d2.d && d1.d<d3.d) {return(d1);}//quando d1 menor

        if(d2.d<d1.d && d2.d<d3.d) {return(d2);}//quando d2 menor

        //Se chegou at� aqui � porque d3 � menor
        return(d3);
    }

    //Divis�o:
    Ponto* X_esq = (Ponto*)malloc((meio)*sizeof(Ponto));
    Ponto* X_dir = (Ponto*)malloc((tamanho-meio)*sizeof(Ponto));

    Ponto* Y_esq = (Ponto*)malloc((meio)*sizeof(Ponto));
    Ponto* Y_dir = (Ponto*)malloc((tamanho-meio)*sizeof(Ponto));

    mid = X[meio-1];

    for(int i=0; i<meio; i++) {X_esq[i] = X[i];}
    for(int i=meio; i<tamanho;i++){X_dir[i-meio] = X[i];}

    int tam_esq = 0;
    int tam_dir = 0;

    for (int i=0; i<tamanho; i++)
    {
        if (Y[i].x < mid.x-0.0000001)
            Y_esq[tam_esq++] = Y[i];
        else if (Y[i].x<=mid.x+0.0000001 && Y[i].y<mid.y+0.0000001)
            Y_esq[tam_esq++] = Y[i];
        else
            Y_dir[tam_dir++] = Y[i];
    }

    d_esq = closest(X_esq, Y_esq, tam_esq);//Lado esquerdo
    d_dir = closest(X_dir, Y_dir, tam_dir);//Lado direito

    //Pega menor dist�ncia entre os dois
    if(d_esq.d < d_dir.d)
            d = d_esq;
    else
            d = d_dir;

    //Combina:
    Ponto* S = (Ponto*)malloc((tamanho)*sizeof(Ponto)); //Vetor do retangulo
    int s_size = 0;

    for(int i=0; i<tamanho; i++)
    {
        if((Y[i].x >= mid.x-d.d)&&(Y[i].x <=  mid.x+d.d))
        {
            S[s_size] = Y[i];
            s_size++;
        }
    }

    for(int i=0; i < s_size; i++)
    {
        for(int j=i+1; j < s_size && ((S[j].y - S[i].y) < d.d);j++)
        {
            if ((distance(S[i],S[j])).d < d.d)
            {
                d = (distance(S[i],S[j]));
            }
        }
    }

    free(X_esq);
    free(X_dir);
    free(Y_esq);
    free(Y_dir);
    free(S);

    return (d);
}


int main (int argc, char *argv[]) {

    if (argc != 2) {
        printf("Erro ao ler o arquivo\n");
        return 0;
    }

    //Start no tempo de execu��o
    clock_t start_closest, end_closest;
    float cpu_time_clo;

    //Inicia o tempo de execu��o
    start_closest = clock();

    Dist menor_dist;

    //Abrir o arquivo
    FILE *in=fopen(argv[1],"r");

    /* PARA DEBUG */
    //FILE *in=fopen("input.txt","r");

    int num_pontos = 0;

    fscanf(in, "%d", &num_pontos);;
    Ponto* pontos_ord_x = (Ponto*)malloc((num_pontos)*sizeof(Ponto));
    Ponto* pontos_ord_y = (Ponto*)malloc((num_pontos)*sizeof(Ponto));

    for (int i = 0; i < num_pontos; i++)
    {
        double x, y;
        fscanf(in, "%lf %lf", &x, &y);
        pontos_ord_x[i].x = x;
        pontos_ord_x[i].y = y;
        pontos_ord_y[i].x = x;
        pontos_ord_y[i].y = y;
    }

    //Fechar o arquivo
    fclose(in);

    //Ordena os pontos pela coordenada x e salva em um vetor X
    qsort(pontos_ord_x, num_pontos, sizeof(Ponto), compararX);

    //Ordena os pontos pela coordenada y e salva em um vetor Y
    qsort(pontos_ord_y, num_pontos, sizeof(Ponto), compararY);

    //Chama a fun��o que pega a menor dist�ncia
    menor_dist = closest(pontos_ord_x, pontos_ord_y, num_pontos);

    //Parar tempo de execu��o
    end_closest = clock();
    cpu_time_clo = ((float) (end_closest - start_closest)) / CLOCKS_PER_SEC;

    //Printa sa�da (tempo dist�ncia x1 y1 x2 y2)
    printf("%f %lf %lf %lf %lf %lf\n", cpu_time_clo, menor_dist.d, menor_dist.p1.x, menor_dist.p1.y, menor_dist.p2.x, menor_dist.p2.y);

    free(pontos_ord_x);
    free(pontos_ord_y);

    return 0;
}
