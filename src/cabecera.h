#include <iostream>
#include <vector>
#include <cmath>
#include <limits>

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QWidget>

#include <QtGui/QVector3D>
#include <QtGui/QScreen>
#include <QtGui/QColor>
#include <QtGui/QGuiApplication>
#include <QtGui/QPainter>

#include <QtWidgets/QScrollArea>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QFileDialog>

#include <Qt3DCore/QTransform>
#include <Qt3DCore/QEntity>

#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DRender/QMaterial>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QMesh>
#include <Qt3DRender/QTextureImage>
#include <Qt3DRender/QTexture>
#include <Qt3DRender/QPointLight>

#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QCuboidGeometry>
#include <Qt3DExtras/Qt3DWindow>
#include <Qt3DExtras/QPhongAlphaMaterial>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QDiffuseMapMaterial>


extern int global_depth;

struct AABB {
    QVector3D min;
    QVector3D max;
};

struct Face {
    QVector3D v1;
    QVector3D v2;
    QVector3D v3;
};

class KDNode {
    public:
        AABB aabb;
        std::vector<Face> faces;
        KDNode* left;
        KDNode* right;
        KDNode(const AABB& aabb, KDNode* left, KDNode* right);
        KDNode(const AABB& aabb, const std::vector<Face>& faces);
};




class ArbolWidget : public QWidget {
    KDNode* raiz;
    public:
        ArbolWidget(KDNode* raiz, QWidget* parent = nullptr);
    protected:
        void paintEvent(QPaintEvent* event) override;
    private:
        void dibujarNodo(QPainter* painter, KDNode* nodo, int w, int h, int x, int y, int gap);
};

void printKDTree(KDNode* root, int level, int depth);
void drawAABB(Qt3DCore::QEntity* parent, KDNode* node, bool isRoot, AABB& parentAABB, bool
isLeftChild, int depth);




std::pair<std::vector<Face>, std::vector<Face>> splitFaces(const std::vector<Face>& faces, const
AABB& aabbLeft, const AABB& aabbRight, int axis) ;
float calculateSurfaceArea(const AABB& aabb);

std::pair<float, std::pair<AABB, AABB>> calculateSAHCost(const AABB& aabb, const std::vector<Face>& faces, 
float t_traversal, float t_intersect, int axis) ;

KDNode* build_KD_tree(const AABB& aabb, const std::vector<Face>& faces, int depth, int
maxDepth, int minFaces, int axis);
AABB calculateAABB(const QVector<QVector3D>& vertices);
bool isPointInAABB(const QVector3D& point, const AABB& aabb);