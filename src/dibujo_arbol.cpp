#include "cabecera.h"


void printKDTree(KDNode* node, int depth = 0, int nodeNumber = 1) 
{
    if (node == nullptr)
        return;

    if(depth > global_depth)
        global_depth = depth;

    // Recorrer el subárbol izquierdo
    printKDTree(node->left, depth + 1, nodeNumber * 2);
    // Recorrer el subárbol derecho
    printKDTree(node->right, depth + 1, nodeNumber * 2 + 1);

}



ArbolWidget::ArbolWidget(KDNode* raiz, QWidget* parent) : QWidget(parent), raiz(raiz) {
   setMinimumSize(7680, 4320);
}



void ArbolWidget::paintEvent(QPaintEvent* event) 
{
    QPainter painter(this);

    int w = 40; // ancho del nodo
    int h = 40; // altura del nodo
    int vGap = 60; // espacio vertical entre nodos
    int maxDepth = global_depth; // profundidad máxima del árbol
    int initialHGap = 11;
    for(int i=1; i <= maxDepth; i++)
       initialHGap *=2;

    //int initialHGap = 2400; //width() / (1 << maxDepth); // Espacio horizontal inicial entre nodos
    dibujarNodo(&painter, raiz, w, h, width() / 2, vGap, initialHGap);
    QWidget::paintEvent(event);
}




void ArbolWidget::dibujarNodo(QPainter* painter, KDNode* nodo, int w, int h, int x, int y, int gap) 
{
    if (nodo == nullptr)
        return;

    painter->drawRect(x - w / 2, y, w, h);
    if (nodo->left != nullptr) {
        painter->drawLine(x, y + h, x - gap, y + h + h);
        dibujarNodo(painter, nodo->left, w, h, x - gap, y + h + h, gap / 2);
    }

    if (nodo->right != nullptr) {
        painter->drawLine(x, y + h, x + gap, y + h + h);
        dibujarNodo(painter, nodo->right, w, h, x + gap, y + h + h, gap / 2);
    }
}