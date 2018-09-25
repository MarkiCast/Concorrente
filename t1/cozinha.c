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
      for(int i = 0; i< n_pedidos; i++) {
        sem_wait(&coziheirosLivres);
        sem_post(&cozinheirosOcupados);
        //locka a thread para pegar oq vai fazer e incrementa pro pŕoximo pedido
        char* nomePrato = pedido_prato_to_name(pedidosBuffer[posicao].pedido_prato_t);
        posicao++;
        //unlocka
      // depois a gente faz as paradas do semaforo to fazendo o esqueleto pra te ruma nocao
        if (strcmp(nomePrato, "SPAGHETTI") == 0))
            fazer_spaghetti(pedido);
        else if (strcmp(nomePrato, "SOPA") == 0))
            fazer_sopa(pedido);
        else if (strcmp(nomePrato, "CARNE") == 0))
            fazer_carne(pedido);
     }
}

void fazerSpaghetti(pedido_t pedido) {
  molho_t molho = create_molho();
  agua_t agua = create_agua();
  bacon_t bacon = create_bacon();
  spaghetti_t spaghetti = create_spaghetti();
  prato_t prato = create_prato(pedido);

  sem_wait(&bocasLivres);
  sem_post(&bocasOcupadas);
  ferver_agua(agua);
  sem_post(&bocasLivres);
  sem_wait(&bocasOcupadas);

  sem_wait(&frigideirasLivres);
  sem_post(&frigideirasOcupadas);
  sem_wait(&bocasLivres);
  sem_post(&bocasOcupadas);
  dourar_bacon(bacon);
  sem_post(&bocasLivres);
  sem_wait(&bocasOcupadas);
  sem_post(&frigideirasLivres);
  sem_wait(&frigideirasOcupadas);

  sem_wait(&bocasLivres);
  sem_post(&bocasOcupadas);
  esquentar_molho(molho);
  sem_post(&bocasLivres);
  sem_wait(&bocasOcupadas);

  cozinhar_spaghetti(spaghetti, agua);
  empratar_spaghetti(spaghetti,molho,bacon,prato);
  destroy_agua(agua);
  destroy_bacon(bacon);
  destroy_molho(molho);
  destroy_spaghetti(spaghetti);
  sem_wait(&balcaoLivre);
  sem_post(&balcaoOcupado);
  notificar_prato_no_balcao(prato);

  sem_post(&cozinheirosLivres);
  sem_wait(&cozinheirosOcupados);

}

void fazerSopa(pedido_t pedido) {
  agua_t agua = create_agua();
  legumes_t legumes = create_legumes();
  prato_t prato = create_prato(pedido);

  sem_wait(&bocasLivres);
  sem_post(&bocasOcupadas);
  ferver_agua(agua);
  caldo_t caldo = preparar_caldo(agua);
  cortar_legumes(legumes);
  cozinhar_legumes(legumes,caldo);
  sem_post(&bocasLivres);
  sem_wait(&bocasOcupadas);

  empratar_sopa(legumes,caldo,prato);
  destroy_agua(agua);
  destroy_caldo(caldo);
  destroy_legumes(legumes);
  sem_wait(&balcaoLivre);
  sem_post(&balcaoOcupado);
  notificar_prato_no_balcao(prato);

  sem_post(&cozinheirosLivres);
  sem_wait(&cozinheirosOcupados);

}

void fazerCarne(pedido_t pedido){
  carne_t carne = create_carne();
  prato_t prato = create_prato(pedido);

  cortar_carne(carne);
  temperar_carne(carne);
  sem_wait(&frigideirasLivres);
  sem_post(&frigideirasOcupadas);
  sem_wait(&bocasLivres);
  sem_post(&bocasOcupadas);
  grelhar_carne(carne);
  sem_post(&bocasLivres);
  sem_wait(&bocasOcupadas);
  sem_post(&frigideirasLivres);
  sem_wait(&frigideirasOcupadas);

  empratar_carne(carne, prato);
  destroy_carne(carne);
  sem_wait(&balcaoLivre);
  sem_post(&balcaoOcupado);
  notificar_prato_no_balcao(prato);

  sem_post(&cozinheirosLivres);
  sem_wait(&cozinheirosOcupados);

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
