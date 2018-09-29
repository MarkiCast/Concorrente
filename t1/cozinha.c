#include "cozinha.h"
#include "tarefas.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t cozinheirosLivres, pedidos, balcaoLivre, balcaoOcupado, garconsSem,
frigideiras, bocas;

pthread_mutex_t cozinheirosMutex;
pthread_mutex_t balcaoMutex;
pthread_mutex_t garconMutex;
int indiceCozinheiro = 0;
int indiceGarcom = 0;
int nPedidos = 0;
prato_t** balcao;
pedido_t* pedidosBuffer;
int i = 0;
int posicao = 0;

int numcozinheiros;
int numgarcons;
int tambalcao;
pthread_t* cozinheirosThreads;
pthread_t* garconsThreads;

void processar_pedido(pedido_t p) {
      pedidosBuffer[i++] = p;
      nPedidos++;
      pedidosBuffer = realloc(pedidosBuffer, sizeof(pedido_t)*nPedidos*2);
      sem_post(&pedidos);
}

void fazerSpaghetti();
void fazerSopa();
void fazerCarne();

void* funcCozinheiros(void *arg) {
  pedido_prato_t prato_;
  pedido_t pedido_;
      while(1) {
        // avisa se chegou novo pedido
        sem_wait(&pedidos);
        // verifica se há cozinheiros livres
        sem_wait(&cozinheirosLivres);
        // pega a posição e incrementa;
        pthread_mutex_lock(&cozinheirosMutex);
        prato_ = pedidosBuffer[posicao].prato;
        printf("## prato=%d\n", prato_);
        pedido_ = pedidosBuffer[posicao];
        posicao++;
        pthread_mutex_unlock(&cozinheirosMutex);

        if (prato_ == PEDIDO_SPAGHETTI) {
            fazerSpaghetti(pedido_);
        } else if (prato_ == PEDIDO_SOPA) {
            fazerSopa(pedido_);
        } else if (prato_ == PEDIDO_CARNE) {
            fazerCarne(pedido_);
        } else {
          pthread_exit(NULL);
        }
      }
}

void* funcGarcons(void *arg) {
  while(1) {
    sem_wait(&garconsSem);
    sem_wait(&balcaoOcupado);
    if(balcao[indiceGarcom] != NULL){
      pthread_mutex_lock(&garconMutex);
      prato_t* prato = balcao[indiceGarcom];
      indiceGarcom++;
      pthread_mutex_unlock(&garconMutex);
      entregar_pedido(prato);
      sem_post(&balcaoLivre);
      sem_post(&garconsSem);
    } else {
      pthread_exit(NULL);
    }
  }
}

void* func_agua(void * arg) {
  agua_t* agua = (agua_t*)arg;
  sem_wait(&bocas); // usou uma boca
  ferver_agua(agua);
  sem_post(&bocas); // liberou uma boca
  return NULL;
}

void *func_molho(void * arg) {
  molho_t* molho = (molho_t*)arg;
  sem_wait(&bocas); // usou uma boca
  esquentar_molho(molho);
  sem_post(&bocas); // liberou uma boca
  return NULL;
}

void *func_bacon(void * arg) {
  bacon_t* bacon = (bacon_t*)arg;
  sem_wait(&frigideiras); // usei uma frigideira
  sem_wait(&bocas); // usou uma boca
  dourar_bacon(bacon);
  sem_post(&frigideiras); //liberei uma frigideira
  sem_post(&bocas); // liberou uma boca
  return NULL;
}

void fazerSpaghetti(pedido_t pedido) {
  bacon_t* bacon = create_bacon();
  agua_t* agua = create_agua();
  molho_t* molho = create_molho();
  spaghetti_t* spaghetti = create_spaghetti();
  prato_t* pratoSpaghetti = create_prato(pedido);

  pthread_t threadAgua;
  pthread_t threadMolho;
  pthread_t threadBacon;

  pthread_create(&threadAgua, NULL, func_agua, (void*)agua);
  pthread_create(&threadMolho, NULL, func_molho,(void*)molho);
  pthread_create(&threadBacon, NULL, func_bacon, (void*)bacon);

  pthread_join(threadAgua, NULL);
  sem_wait(&bocas);
  cozinhar_spaghetti(spaghetti,agua);
  sem_post(&bocas);
  destroy_agua(agua);

  pthread_join(threadMolho, NULL);
  pthread_join(threadBacon, NULL);

  empratar_spaghetti(spaghetti, molho, bacon, pratoSpaghetti);
  sem_wait(&balcaoLivre);
  pthread_mutex_lock(&balcaoMutex);
  balcao[indiceCozinheiro] = pratoSpaghetti;
  indiceCozinheiro++;
  pthread_mutex_unlock(&balcaoMutex);
  sem_post(&balcaoOcupado);
  notificar_prato_no_balcao(pratoSpaghetti);

  sem_post(&cozinheirosLivres);
}

void fazerSopa(pedido_t pedido) {
  agua_t* aguaSopa = create_agua();
  legumes_t* legumes = create_legumes();
  prato_t* pratoSopa = create_prato(pedido);

  pthread_t threadAgua;
  pthread_create(&threadAgua, NULL, func_agua, (void*)aguaSopa);

  cortar_legumes(legumes);

  pthread_join(threadAgua, NULL);

  sem_wait(&bocas);
  caldo_t* caldo = preparar_caldo(aguaSopa);
  sem_post(&bocas);

  sem_wait(&bocas);
  cozinhar_legumes(legumes, caldo);
  sem_post(&bocas);

  empratar_sopa(legumes, caldo, pratoSopa);

  sem_wait(&balcaoLivre);
  pthread_mutex_lock(&balcaoMutex);
  balcao[indiceCozinheiro] = pratoSopa;
  indiceCozinheiro++;
  pthread_mutex_unlock(&balcaoMutex);
  sem_post(&balcaoOcupado);
  notificar_prato_no_balcao(pratoSopa);

  sem_post(&cozinheirosLivres);
}

void fazerCarne(pedido_t pedido){
  carne_t* carne = create_carne();
  prato_t* pratoCarne = create_prato(pedido);

  cortar_carne(carne);
  temperar_carne(carne);

  sem_wait(&frigideiras);
  sem_wait(&bocas);
  grelhar_carne(carne);
  sem_post(&frigideiras);
  sem_post(&bocas);

  empratar_carne(carne, pratoCarne);

  sem_wait(&balcaoLivre);
  pthread_mutex_lock(&balcaoMutex);
  balcao[indiceCozinheiro] = pratoCarne;
  indiceCozinheiro++;
  pthread_mutex_unlock(&balcaoMutex);
  sem_post(&balcaoOcupado);
  notificar_prato_no_balcao(pratoCarne);

  sem_post(&cozinheirosLivres);
}

void cozinha_init(int cozinheiros, int bocasParam, int frigideirasParam, int garconsParam, int tam_balcao) {

    balcao = malloc(sizeof(prato_t*)*tam_balcao);
    pedidosBuffer = malloc(sizeof(pedido_t)*10);

    cozinheirosThreads = malloc(sizeof(pthread_t)*cozinheiros);
    garconsThreads = malloc(sizeof(pthread_t)*garconsParam);
    numcozinheiros = cozinheiros;
    numgarcons = garconsParam;
    tambalcao = tam_balcao;

    pthread_mutex_init(&cozinheirosMutex, NULL);
    pthread_mutex_init(&balcaoMutex, NULL);
    pthread_mutex_init(&garconMutex, NULL);

    sem_init(&cozinheirosLivres, 0 , cozinheiros);
    sem_init(&pedidos, 0, 0);
    sem_init(&balcaoLivre, 0 , tam_balcao);
    sem_init(&balcaoOcupado, 0, 0);
    sem_init(&garconsSem, 0, garconsParam);
    sem_init(&frigideiras, 0 , frigideirasParam);
    sem_init(&bocas, 0 , bocasParam);

    for (int i =0; i< cozinheiros; i++){
      pthread_create(&cozinheirosThreads[i], NULL, funcCozinheiros, NULL);
    }

    for (int i = 0; i < garconsParam; i++) {
      pthread_create(&garconsThreads[i], NULL, funcGarcons, NULL);
    }
}

void cozinha_destroy() {

      pedido_t p ={0, PEDIDO_NULL };

      for(int i = 0; i < numcozinheiros; i++)
        processar_pedido(p);

      for(int i =0; i< numcozinheiros; i++)
        pthread_join(cozinheirosThreads[i], NULL);

      for(int i = 0; i< tambalcao; i++){
        sem_wait(&balcaoLivre);
        balcao[indiceGarcom] = NULL;
        sem_post(&balcaoOcupado);
      }

      for(int i=0; i < numgarcons; i++) {
        pthread_join(garconsThreads[i], NULL);
      }

      sem_destroy(&cozinheirosLivres);
      sem_destroy(&pedidos);
      sem_destroy(&balcaoLivre);
      sem_destroy(&garconsSem);
      sem_destroy(&frigideiras);
      sem_destroy(&bocas);
      sem_destroy(&balcaoOcupado);

      pthread_mutex_destroy(&cozinheirosMutex);
      pthread_mutex_destroy(&balcaoMutex);
      pthread_mutex_destroy(&garconMutex);
      free(balcao);
      free(pedidosBuffer);
      free(garconsThreads);
      free(cozinheirosThreads);
}
