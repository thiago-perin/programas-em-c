#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// faz o movimento da torre, que vai  se mover uma a quantidade de 'casas'' na 'direcao'' definida
void torre(int casas, char direcao[10])  //funcao torre
   {           if (casas > 0) // vai definir quantas vezes moverá a torre
                   {
                               printf("Movendo a torre para a %s %d \n", direcao, casas); // imprime o movimento pedido
                               torre(casas - 1, direcao); // repete o uso da funcao torre
                   }
   }
  
// faz o movimento do bispo
void bispo(int casas, char direcao1[10], char direcao2[10])
   {           if (casas > 0)
                   {
                               printf("Movendo o Bispo para a %s e para %s: %d \n", direcao1, direcao2, casas);
                               bispo(casas - 1, direcao1, direcao2);
                   }
   }


// faz o movimento da rainha
void rainha(int casas, char direcao1[10], char direcao2[10])
   {           if (casas > 0)
                   {
                               printf("Movendo a Rainha para a %s e para %s: %d \n", direcao1, direcao2, casas);
                               rainha(casas - 1, direcao1, direcao2);
                   }
   }


// faz o movimento do cavalo
void cavalo(int casas, char direcao[10])
   {           if (casas > 0)
                   {
                               printf("Movendo o Cavalo para a %s : %d\n", direcao, casas);
                               cavalo(casas - 1, direcao);
                   }
   }


// faz o movimento da peão
void peao(int casas)
   {           if (casas > 0)
                   {
                               printf("Movendo o peão para a frente %d \n", casas);
                               peao(casas - 1);
                   }
   }


// faz o movimento do rei
void rei(int casas, char direcao1[10], char direcao2[10])
   {           if (casas > 0)
                   {
                               printf("Movendo o rei para %s e para %s.\n", direcao1, direcao2, casas);
                               rei(casas - 1, direcao1, direcao2);
                   }
   }
          
int main() {


       int opcao, peca;
       while(opcao != 3) // enquanto a opcao digitada for diferente de 3,o programa nao sera finalizado
       {


       // entrando no menu principal e escolhendo uma opção
       printf("### Jogo de xadrez ###\n"); // nome do app
       printf("1. Escolher uma peça para mover \n"); // entra no menu para mover uma peca
       printf("2. Ver regras de movimento de peças\n"); //entende o movimento das pecas
       printf("3. Sair do jogo \n"); //finaliza o programa


       printf("Digite um número  de 1 a  3 \n"); //informa ao usuario que deve escolher uma opcao


       // o usuário fará a sua escolha
       scanf("%d", &opcao);
      
       switch (opcao)
       {          case 1: // o usuario escolheu mover uma peca
                               printf("Escolha uma peça para mover: \n"); // informa ao usuario que ele deve mover uma peca
                               printf("1. Torre. \n"); // escolher mover com a torre
                               printf("2. Bispo. \n"); // escolher mover com o bispo
                               printf("3. Rainha. \n"); // escolher mover com a rainha
                               printf("4. Cavalo. \n"); // escolher mover com o cavalo
                               printf("5. Peão. \n"); // escolher mover com o peao
                               printf("6. Rei. \n"); // escolher mover com o rei
                               printf("Digite um numero de 1 a 6: \n"); //pergunta ao usuario qual peca ele deseja mover
                               scanf("%d", &peca);
                              
                               switch (peca)
                               {          case 1:
                                                       printf("Jogador escolheu Torre. \n");
                                                       int casas_torre;
                                                       char* direcao_torre[10];
                                                       printf("Quantas casas deseja mover? \n");
                                                       scanf("%d", &casas_torre);
                                                       printf("Qual direção deseja mover? \n");
                                                       scanf("%s", &direcao_torre);
                                                       printf("Mova %d casas para a %s com a torre. \n", casas_torre, direcao_torre);
                                                       torre(casas_torre, direcao_torre);
                                                       break;
                                           case 2:
                                                       printf("Jogador escolheu Bispo. \n");
                                                       int casas_bispo;
                                                       char direcao_bispo_h[10];
                                                       char direcao_bispo_v[10];
                                                       printf("Quantas casas deseja mover? \n");
                                                       scanf("%d", &casas_bispo);
                                                       printf("Qual direção horizontal deseja mover? Esquerda ou direita \n");
                                                       scanf("%s", &direcao_bispo_h);
                                                       printf("Qual direção vertical deseja mover? Cima ou baixo \n");
                                                       scanf("%s", &direcao_bispo_v);
                                                       printf("Mova %d casas para a %s e para %s com o bispo. \n", casas_bispo, direcao_bispo_h, direcao_bispo_v);
                                                       bispo(casas_bispo, direcao_bispo_h, direcao_bispo_v);
                                                       break;
                                           case 3:
                                                       printf("Jogador escolheu Rainha. \n");
                                                       int casas_rainha;
                                                       char direcao_rainha_h[10];
                                                       char direcao_rainha_v[10];
                                                       printf("Quantas casas deseja mover? \n");
                                                       scanf("%d", &casas_rainha);
                                                       printf("Qual direção horizontal deseja mover? Esquerda ou direita ou nenhum\n");
                                                       scanf("%s", &direcao_rainha_h);
                                                       printf("Qual direção vertical deseja mover? Cima ou baixo ou nenhum \n");
                                                       scanf("%s", &direcao_rainha_v);
                                                       printf("Mova %d casas para a %s e para %s com a rainha. \n", casas_rainha, direcao_rainha_h, direcao_rainha_v);
                                                       rainha(casas_rainha, direcao_rainha_h, direcao_rainha_v);
                                                       break;
                                           case 4:
                                                       printf("Jogador escolheu Cavalo. \n");
                                                       int casas_cavalo_h, casas_cavalo_v;
                                                       char direcao_cavalo_h[10];
                                                       char direcao_cavalo_v[10];
                                                       printf("Qual direção horizontal deseja mover? Esquerda ou direita\n");
                                                       scanf("%s", &direcao_cavalo_h);
                                                       printf("Quantas casas deseja mover para a %s ? 1 ou 2\n", direcao_cavalo_h);
                                                       scanf("%d", &casas_cavalo_h);
                                                       printf("Qual direção vertical deseja mover? Cima ou baixo \n");
                                                       scanf("%s", &direcao_cavalo_v);
                                                       printf("Quantas casas deseja mover para a %s ? 1 ou 2\n", direcao_cavalo_v);
                                                       scanf("%d", &casas_cavalo_v);
                                                       printf("Mova %d casas para a %s e %d para %s com o cavalo. \n", casas_cavalo_h, direcao_cavalo_h, casas_cavalo_v, direcao_cavalo_v);
                                                       cavalo(casas_cavalo_h, direcao_cavalo_h);
                                                       cavalo(casas_cavalo_v, direcao_cavalo_v);
                                                       break;
                                            case 5:
                                                       printf("Jogador escolheu Peão. \n");
                                                       int casas_peao;
                                                       printf("Quantas casas deseja mover? 1 ou 2\n");
                                                       scanf("%d", &casas_peao);
                                                       printf("Mova %d casas para a frente com o peao. \n", casas_peao);
                                                       peao(casas_peao);
                                                       break;
                                               case 6:
                                                       printf("Jogador escolheu Rei. \n");
                                                       int casas_rei;
                                                       char direcao_rei_h[10];
                                                       char direcao_rei_v[10];
                                                       casas_rei = 1;
                                                       printf("Qual direção horizontal deseja mover? Esquerda ou direita \n");
                                                       scanf("%s", &direcao_rei_h);
                                                       printf("Qual direção vertical deseja mover? Cima ou baixo \n");
                                                       scanf("%s", &direcao_rei_v);
                                                       printf("Mova %d casa para a %s e %d casas para %s com o rei. \n", casas_rei, direcao_rei_h, casas_rei, direcao_rei_v);
                                                       rei(casas_rei, direcao_rei_h, direcao_rei_v);
                                                       break;
                                           default:
                                                       printf("Opção inválida. \n"); break;
                                    } break;
                   case 2:         printf("Regras do jogo:\n");
                                           printf("Regra 1: Torre se move na horizontal ou vertical. \n");
                                           printf("Regra 2: Bispo se move na diagonal. \n");
                                           printf("Regra 3: Rainha pode se mover na horizontal, vertical ou diagonal. \n");
                                           printf("Regra 4: Cavalo se move na horizontal e na vertical, sendo 1 casa para uma direção e 2 casas para outra. \n");
                                           printf("Regra 5: Peão se move uma casa para frente ou 2 caso esteja na sua casa inicial. \n");
                                           printf("Regra 3: Rei pode se mover na horizontal, vertical ou diagonal somente 1 casa. \n"); break;
                   case 3:         printf("Saindo ...\n"); break;
                   default:          printf("Opção inválida. Tente novamente \n");
       }
       }
       return 0;
}

