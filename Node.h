#ifndef Node_H
#define Node_H
#include <iostream>

struct Node {
    Node * direita ; // Nó para o proximo elemento da linha
    Node * abaixo ; // Nó para o proximo elemento da coluna
    int linha ; 
    int coluna ;
    double valor; 

    // construtor do node
    Node( double valor, Node* direta = nullptr, Node* abaixo = nullptr, int linha = 0, int coluna = 0) {
        double valor;
        this->direita = direita;
        this-> abaixo = abaixo;

    }
};

#endif