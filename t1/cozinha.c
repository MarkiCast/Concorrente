#include "cozinha.h"
#include "tarefas.c"
#include <pthread.h>


sem_t cozinheirosLivres, cozinheirosOcupados, cozinheirosEsperando,
      garconsOcupados, garconsLivres,
      balcaoOcupado, balcaoLivre,
      frigideirasOcupadas, frigideirasLivres,
      bocasLivres, bocasOcupadas,
      pedidosLivre, pedidosCheio;

int* frigideirasBuffer;
int* bocasBuffer;
int* tam_balcaoBuffer;
pedido_t* listaPedidos
pthread_t* cozinheiros;
pthread_t* garcons;

void processar_pedido(pedido_t p) {


}

*void funcCozinheiros(void *arg) {

}

void fazerSopa() {
  agua_t agua = create_agua();
  legumes_t legumes = create_legumes();


}


void cozinha_init( int cozinheiros, int bocas, int frigideiras, int garcons, int tam_balcao) {

    frigideirasBuffer = malloc(sizeof(int)*frigideiras);
    bocasBuffer = malloc(sizeof(int)*bocas_total);
    tam_balcaoBuffer = malloc(sizeof(int)*tam_balcao);

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
