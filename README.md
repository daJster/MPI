---
marp: true
theme: uncover
class: invert
math: mathjax
---

# <!--fit--> Multiplications matricielles en MPI :rocket:
<p style="font-size: 43px">
    <span style="color: grey;">Par:</span><br>
    Jad Elkarchi<br>
    Mouhcine Ahbaiz<br>
    Anas Naami
</p>

---
## Note : 
* Cette présentation a pour but de clarifier notre code et la façon dont nous avons résolu chacune des questions posées dans le TP.
---
### Dépendance entre les valeurs $A_{ij}$ et son processeur correspendant $P$
```C
    void mat_init_alea(matrix_t *mat, int width, int height)
    {
        ... Code ...

        /* --- Initialisation  ---*/
        for(i=0 ; i<height*width ; i++)
            mat->data[i] = i + my_id; // correler le rang du processeur avec les valeurs de la matrice
    }

```
---
###### Algorithme qui vérifie si le nombre de processeurs $nb\_proc$ est un carré parfait.
*  Cela nous a permis de répartir équitablement les processeurs sur la matrice.
_Dans un premier temps on a géré le où $nb\_proc$ est un carré parfait_

```C
    void init_communicateurs(int nb_procs)
    {
        ... Code ...
        int p = (int) sqrt(nb_procs); // racine carré dunombre de procs
        if  ( p*p == nb_procs){
            nb_col=p; nb_row=p;
        } else {
            exit(-1); // à implementer le cas où nb_proc n'est pas un carré parfait
        }
        ... Reste du code ...
    }
```
---
* Après avoir calculé l'entier $p = \sqrt{nb\_ proc}$, nous sommes prêts à subdiviser notre communicateur MPI_WORLD en communicateurs horizontaux et verticaux. Chaque proc a accès à seulement **deux** Communicateurs dont il appartient.<br> -Un vertical et un horizontal-

---
### Implémentation
```C
    void init_communicateurs(int nb_procs)
    {
        ... Code ...
        /* Partitionnez MPI_COMM_WORLD en communicateurs pour les lignes 
        et en communicateurs pour les colonnes */

        // Vertical Split
        MPI_Comm_split(MPI_COMM_WORLD, my_id%p, (int) my_id/p, &MPI_VERTICAL);

        // Horizontal Split
        MPI_Comm_split(MPI_COMM_WORLD, (int) my_id/p, my_id%p, &MPI_HORIZONTAL);

        // colour et key correspond au proc my_id est unique à lui même

        /* Vérifiez que votre rang dans le nouveau communicateur 
        correspond bien à celui que vous attendez. */
        // if (my_id==15){ // Test pour nb_proc = 16
        //     MPI_Comm_rank(MPI_HORIZONTAL,&rank1);
        //     MPI_Comm_rank(MPI_VERTICAL,&rank2);
        //     printf("rank1 : %d,rank2 : %d", rank1, rank2);
        // }
    }
```
---
### Etat d'une matrice $3$x$3$ sur $9$ procs
![](./assets/etatMatrice.png)

---
### Calcul parallèle du produit matricielle
* nous sommes entrain de réaliser un produit matricielle outer product, et alors la boucle en $k$ de $1$ à $N$ emboîte les autres boucles.
* À chaque itération le proc en cours _crée_ et _initialize_ deux vecteurs ($bufferA, bufferB$) où l'on va stocker les valeurs d'une colonne $colOffset = k \%p$ de $A$ (Resp. les valeurs ligne de $B$).
---
* On determine les deux rangs des procs qui doivent effectuer un broadcast ($rankSourceA, rankSourceB$) dans leurs commutateurs, en utilisant un 
$procOffset = \lfloor \frac{k}{p} \rfloor \% p$. 
---
```C
void parallel_mat_mult(matrix_t A, matrix_t B, matrix_t C)
{
    int k, i, j, rankH, rankV;  
    for(k=0; k<(nb_col*C.col) ; k++) {
      
      matrix_t bufferA;
      matrix_t bufferB;
      mat_init_empty(&bufferA, 1, A.row);
      mat_init_empty(&bufferB, B.col, 1);


      /*Rang du proc en cours dans son comm horizontal*/
      MPI_Comm_rank(MPI_HORIZONTAL, &rankH);
      /*Rang du proc en cours dans son comm vertical*/
      MPI_Comm_rank(MPI_VERTICAL, &rankV);

      int colOffset = k%nb_col;
      int procOffset = ((int) k/nb_col)%nb_col;

      int rankSourceA = rankV*nb_col + procOffset;
      int rankSourceB = rankH + procOffset*nb_col;

      ... Reste du code ...
      }
}
```
---
* Ce qu'il nous reste à faire maintenant est de remplir correctement les buffers avec les valeurs requises, et de les broadcast dans les communicateurs vertical (Resp. horizontal) correspondant.
* Ensuite on effectue un produit vectoriel de $bufferA$ et $bufferB$ et stocker le resultat en $C$.
---

```C
void parallel_mat_mult(matrix_t A, matrix_t B, matrix_t C)
{
    ... Code ...
    for(k=0; k<(nb_col*C.col) ; k++) {
        ... Suite du code ...

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

      // Broadcast dands les comm horizontal/vertical correspondant
      MPI_Bcast(bufferA.data, bufferA.row, MPI_INT, procOffset, MPI_HORIZONTAL);
      MPI_Bcast(bufferB.data, bufferB.col, MPI_INT, procOffset, MPI_VERTICAL);
      
      // Réalisation de la multiplication dans le procs en cours
      mat_mult(bufferA, bufferB, C);

      free(bufferA.data);
      free(bufferB.data);
    }
}
```
---
* Il est important de remarquer que nous réutilisons et réinitialisons les vecteurs buffers à chaque fois que nous bouclons, cela signifie que nous gaspillons du bon matériel qui pourrait nous aider à les réutiliser dans les prochains calculs. Surtout si le prochain bloc à calculer et dans le même comm vertical/horizontal
---
* Toutefois, il convient de noter que le produit externe peut être mis en œuvre de manière parallèle à l'aide de techniques telles que le $loop$  $tiling$. Cette approches peut contribuer à réduire le nombre de calculs redondants et à améliorer les performances du produit externe dans un cadre parallèle.
---
# Utilisation du makefile
* Le makefile fourni avec le code source sert à compiler, exécuter et tester notre code source. Afin de le lancer correctement il suffit de :
```bash
desktop ~> make # compile et exécute le programme avec SIZE = 9 par defaut
desktop ~> make PROCS=<nombre_de_processeurs> SIZE=<taille_des_matrices> 
# le nb_proc doit nécessairement être un carré parfait (4, 9, 16, ...)
```
<span style="margin: 50px;">Merci de votre attention</span>