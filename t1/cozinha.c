#include "cozinha.h"
#include "tarefas.c"
#include <pthread.h>


sem_t cozinheirosLivres, cozinheirosOcupados, cozinheirosEsperando,
      garconsOcupados, garconsLivres,
      balcaoOcupado, balcaoLivre,
      frigideirasOcupadas, frigideirasLivres,
      bocasLivres, bocasOcupadas;

int* frigideirasBuffer;
int* bocasBuffer;
int* tam_balcaoBuffer;
pedido_t* listaPedidos
pthread_t* cozinheiros;
pthread_t* garcons;
int n_pedidos;
pedido_t pedidosBuffer;
int i = 0;
int posicao = 0;

void processar_pedido(pedido_t p) {
      n_pedidos++;
      pedidosBuffer[++i] = p;

}

*void funcCozinheiros(void *arg) {
      for(int i = 0; i< n_pedidos; i++)
        sem_wait(&coziheirosLivres)
        //locka a thread para pegar oq vai fazer e incrementa pro pŕoximo pedido
        char* nomePrato = pedido_prato_to_name(pedidosBuffer[posicao].pedido_prato_t);
        posicao++;
        //unlocka
      // depois a gente faz as paradas do semaforo to fazendo o esqueleto pra te ruma nocao
        if (strcmp(nomePrato, "SPAGHETTI") == 0))
            fazer_spaghetti();
        else if (strcmp(nomePrato, "SOPA") == 0))
            fazer_sopa();
        else if (strcmp(nomePrato, "CARNE") == 0))
            fazer_carne();
}

void fazerSopa() {
  agua_t agua = create_agua();
  legumes_t legumes = create_legumes();


}


void cozinha_init( int cozinheiros, int bocas, int frigideiras, int garcons, int tam_balcao) {

    frigideirasBuffer = malloc(sizeof(int)*frigideiras);
    bocasBuffer = malloc(sizeof(int)*bocas_total);
    tam_balcaoBuffer = malloc(sizeof(int)*tam_balcao);
    pedidosBuffer = malloc(sizeof(pedido_t)*20);

    for (int i =0; i< cozinheiros; i++){
      pthread_create(&cozinheiros[i], NULL, funcCozinheiros, );
    }

    for (int i = 0; i < garcons; i++) {
      pthread_create(&garcons[i], NULL, funcGarcons, NULL);
    }

    sem_init(&cozinheirosLivres, 0 , cozinheiros);
    sem_init(&cozinheirosOcupados, 0 , 0);
    sem_init(&cozinheirosEsperando, 0 , 0);

    sem_init(&garconsLivres, 0 ,garcons);
    sem_init(&garconsOcupados, 0 , 0);

    sem_init(&balcaoLivre, 0 , tam_balcao);
    sem_init(&balcaoOcupado, 0 , 0);

    sem_init(&frigideirasLivres, 0 , frigideiras);
    sem_init(&frigideirasOcupadas, 0 , 0);

    sem_init(&bocasLivres, 0 , bocas);
    sem_init(&bocasOcupadas, 0 , 0);
}
