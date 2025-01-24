#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>

struct Node {
    Node* direita;
    Node* abaixo;
    int linha;
    int coluna;
    double valor;
};

class SparseMatrix {
private:
    int linhas, colunas;
    Node** headLinhas;
    Node** headColunas;

    void desalocar() {
        for (int i = 0; i < linhas; ++i) {
            Node* atual = headLinhas[i];
            while (atual) {
                Node* temp = atual;
                atual = atual->direita;
                delete temp;
            }
        }
        delete[] headLinhas;
        delete[] headColunas;
    }

public:
    SparseMatrix(int m, int n) {
        if (m <= 0 || n <= 0)
            throw std::invalid_argument("Dimensões devem ser positivas.");

        linhas = m;
        colunas = n;
        headLinhas = new Node*[linhas]();
        headColunas = new Node*[colunas]();
    }

    ~SparseMatrix() {
        desalocar();
    }

    void insert(int i, int j, double value) {
        if (i < 1 || i > linhas || j < 1 || j > colunas)
            throw std::out_of_range("Índice fora dos limites.");

        if (value == 0) return;

        Node* novo = new Node{nullptr, nullptr, i, j, value};

        // Inserção na linha
        if (!headLinhas[i - 1]) {
            headLinhas[i - 1] = novo;
        } else {
            Node* atual = headLinhas[i - 1];
            while (atual->direita && atual->coluna < j)
                atual = atual->direita;

            if (atual->coluna == j) {
                atual->valor = value;
                delete novo;
                return;
            } else {
                novo->direita = atual->direita;
                atual->direita = novo;
            }
        }

        // Inserção na coluna
        if (!headColunas[j - 1]) {
            headColunas[j - 1] = novo;
        } else {
            Node* atual = headColunas[j - 1];
            while (atual->abaixo && atual->linha < i)
                atual = atual->abaixo;

            if (atual->linha == i) {
                atual->valor = value;
                delete novo;
                return;
            } else {
                novo->abaixo = atual->abaixo;
                atual->abaixo = novo;
            }
        }
    }

    double get(int i, int j) {
        if (i < 1 || i > linhas || j < 1 || j > colunas)
            throw std::out_of_range("Índice fora dos limites.");

        Node* atual = headLinhas[i - 1];
        while (atual && atual->coluna != j)
            atual = atual->direita;

        return atual ? atual->valor : 0.0;
    }

    void print() {
        for (int i = 1; i <= linhas; ++i) {
            for (int j = 1; j <= colunas; ++j) {
                std::cout << get(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }
};

// Funções externas
void readSparseMatrix(SparseMatrix& m, const std::string& nomeArquivo) {
    std::ifstream arquivo(nomeArquivo);
    if (!arquivo.is_open())
        throw std::runtime_error("Erro ao abrir arquivo.");

    int mLinhas, mColunas;
    arquivo >> mLinhas >> mColunas;

    SparseMatrix temp(mLinhas, mColunas);
    int i, j;
    double valor;

    while (arquivo >> i >> j >> valor) {
        temp.insert(i, j, valor);
    }

    m = std::move(temp);
}

int main() {
    try {
        int linhas, colunas;
        std::cout << "Digite o número de linhas e colunas: ";
        std::cin >> linhas >> colunas;

        SparseMatrix mat(linhas, colunas);

        std::cout << "Digite os valores da matriz (linha por linha):\n";
        for (int i = 1; i <= linhas; ++i) {
            for (int j = 1; j <= colunas; ++j) {
                double valor;
                std::cin >> valor;
                mat.insert(i, j, valor);
            }
        }

        std::cout << "Matriz inserida:\n";
        mat.print();
    } catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }

    return 0;
}
