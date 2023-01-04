/*****************************************************************/
/*                                                               */
/* Produit de matrice par double diffusion                       */
/*                                                               */
/*****************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <mpi/mpi.h>
#include <math.h>

/******* Fonctions d'affichage ***********/
#define VOIRD(expr) do {printf("P%d (%d,%d) <%.3d> : \t{ " #expr " = %d }\n", \
                               my_id,i_row,i_col,__LINE__,expr);} while(0)
#define PRINT_MESSAGE(expr) do {printf("P%d (%d,%d) <%.3d> : \t" #expr "\n", \
                                      my_id,i_row,i_col,__LINE__);} while(0)

typedef struct { 
  int row;   /* le nombre de lignes   */
  int col;   /* le nombre de colonnes */
  int* data; /* les valeurs           */
} matrix_t;



int my_id, nb_proc;
int nb_col,nb_row;       /* Largeur et hauteur de la grille */
MPI_Comm MPI_HORIZONTAL; /* Communicateur pour diffusions horizontales */
MPI_Comm MPI_VERTICAL;   /* Communicateur pour diffusions verticales */
int i_col,i_row;         /* Position dans la grille */



/*******************************************/
/* initialisation al�atoire de la matrice  */
/*******************************************/
void mat_init_alea(matrix_t *mat, int width, int height)
{
  int i;

  mat->row = height;
  mat->col = width;

  mat->data=(int*)malloc(sizeof(int)*height*width);
  for(i=0 ; i<height*width ; i++)
    mat->data[i] = i + my_id; // correler le rang du processeur avec les valeurs de la matrice
}

/*******************************************/
/* initialisation � 0 de la matrice        */
/*******************************************/
void mat_init_empty(matrix_t *mat,int width, int height)
{
  mat->row = height;
  mat->col = width;
  mat->data=(int*)calloc((mat->row)*(mat->col),sizeof(int));
}

/*******************************************/
/* affichage de la matrice                 */
/*******************************************/
void mat_display(matrix_t A)
{
  int i,j,t=0;
  printf("      ");
  for(j=0;j<A.col;j++)
    printf("%.3d ",j);
  printf("\n");
  printf("    __");
  for(j=0;j<A.col;j++)
    printf("____");
  printf("_\n");

  for(i=0;i<A.row;i++)
    {
      printf("%.3d | ",i);
      for(j=0;j<A.col;j++)
	/*printf("%d ",A.data[i*A.row+j]);*/
	printf("%.3d ",A.data[t++]);
      printf("|\n");
    }
  printf("    --");
  for(j=0;j<A.col;j++)
    printf("----");
  printf("-\n");
}

/*******************************************/
/* C+=A*B                                  */
/*******************************************/
void mat_mult(matrix_t A, matrix_t B, matrix_t C)
{
  int i,j,k,M,N,K,*_A,*_B,*_C;
 
  _A=A.data;
  _B=B.data;
  _C=C.data;

  M = C.row;
  N = C.col;
  K = A.col;

  if((M!=A.row) || (N!=B.col) || (K!=B.row)) {
    PRINT_MESSAGE("Attention, tailles incompatibles");
    VOIRD(A.row);VOIRD(A.col);VOIRD(B.row);
    VOIRD(C.col);VOIRD(C.row);VOIRD(C.col);
    exit(1);
  }

  for(i=0 ; i<M ; i++) {
    for(j=0 ; j<N ; j++) {
      for(k=0 ; k<K ; k++) {
	    _C[i*N+j]+=_A[i*K+k]*_B[k*N+j];
      }
    }
  }


}

/*******************************************/
/* Initialisations la grille               */
/*******************************************/
void init_communicateurs(int nb_procs)
{
    int acc, rank1, rank2;

    int p = (int) sqrt(nb_procs); // racine carré du nombre de procs
    if  ( p*p == nb_procs){
        nb_col=p; nb_row=p;
    } else {
        exit(-1); // à implementer le cas où nb_proc n'est pas un carré parfait
    }
    /* Am�liorez la disposition */

    /* Partitionnez MPI_COMM_WORLD en communicateurs pour les lignes 
    et en communicateurs pour les colonnes */
    MPI_Comm_split(MPI_COMM_WORLD, my_id%p, (int) my_id/p, &MPI_VERTICAL);
    MPI_Comm_split(MPI_COMM_WORLD, (int) my_id/p, my_id%p, &MPI_HORIZONTAL);


    /* V�rifiez que votre rang dans le nouveau communicateur 
    correspond bien � celui que vous attendez. */
    // if (my_id==15){ // TEST
    //     MPI_Comm_rank(MPI_HORIZONTAL,&rank1);
    //     MPI_Comm_rank(MPI_VERTICAL,&rank2);
    //     printf("rank1 : %d,rank2 : %d", rank1, rank2);
    // }
}

/*******************************************/
/* Produit de matrice par double diffusion */
/*******************************************/
void parallel_mat_mult(matrix_t A, matrix_t B, matrix_t C) // parallel
{
    int k, i, j, rankH, rankV;  

    for(k=0; k<(nb_col*C.col) ; k++) {
      
      matrix_t bufferA;
      matrix_t bufferB;
      mat_init_empty(&bufferA, 1, A.row);
      mat_init_empty(&bufferB, B.col, 1);


      /*diffusion horizontale */
      MPI_Comm_rank(MPI_HORIZONTAL, &rankH);
      /* diffusion verticale */
      MPI_Comm_rank(MPI_VERTICAL, &rankV);

      int colOffset = k%nb_col;
      int procOffset = ((int) k/nb_col)%nb_col;

      int rankSourceA = rankV*nb_col + procOffset;
      int rankSourceB = rankH + procOffset*nb_col;

      if (my_id == rankSourceA) { // remplir le buffer A
          for(i=0; i<A.row; i++){
            bufferA.data[i] = A.data[colOffset + i*A.col];
          }
      }

      if (my_id == rankSourceB){ // remplir le buffer B
          for(i=0; i<B.col; i++){
            bufferB.data[i] = B.data[colOffset*B.col + i];
          }
      }

      MPI_Bcast(bufferA.data, bufferA.row, MPI_INT, procOffset, MPI_HORIZONTAL);
      MPI_Bcast(bufferB.data, bufferB.col, MPI_INT, procOffset, MPI_VERTICAL);
      
      mat_mult(bufferA, bufferB, C);

      free(bufferA.data);
      free(bufferB.data);
    }
}


int main(int argc, char **argv) 
{
  int taille;

  if(argc == 0){
    fprintf(stderr,"Usage : %s <taille de matrice>",argv[0]);
  }  else {
    taille = atoi(argv[1]); // taille de la matrice
  }

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD,&nb_proc);
  MPI_Comm_rank(MPI_COMM_WORLD,&my_id);
  
  
    init_communicateurs(nb_proc);
  
    matrix_t A,B,C;
    
    mat_init_alea(&A,taille/nb_col,taille/nb_row);
    mat_init_alea(&B,taille/nb_col,taille/nb_row);
    mat_init_empty(&C,taille/nb_col,taille/nb_row);

    parallel_mat_mult(A,B,C);
  
    if (my_id == 0){
      mat_display(A);
      mat_display(B);
      mat_display(C);
    }

    // freeing allocated memory
    free(A.data);
    free(B.data);
    free(C.data);
    MPI_Finalize();
    return (0);
}
