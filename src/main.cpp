#include "cabecera.h"

int global_depth = 0;

KDNode::KDNode(const AABB& aabb, KDNode* left, KDNode* right)
: aabb(aabb), left(left), right(right) {}

// Constructor para nodos hoja
KDNode::KDNode(const AABB& aabb, const std::vector<Face>& faces)
: aabb(aabb), faces(faces), left(nullptr), right(nullptr) {}


void drawAABB(Qt3DCore::QEntity* parent, KDNode* node, int depth) 
{
    if (!node)
        return;

    Qt3DCore::QEntity* rootEntity = new Qt3DCore::QEntity(parent);
    QColor edgeColor;

    QVector<QVector3D> vertices (8);
    vertices[0] = node->aabb.min;
    vertices[1] = {node->aabb.max.x(), node->aabb.min.y(), node->aabb.min.z()};
    vertices[2] = {node->aabb.max.x(), node->aabb.max.y(), node->aabb.min.z()};
    vertices[3] = {node->aabb.min.x(), node->aabb.max.y(), node->aabb.min.z()};
    vertices[4] = {node->aabb.min.x(), node->aabb.min.y(), node->aabb.max.z()};
    vertices[5] = {node->aabb.max.x(), node->aabb.min.y(), node->aabb.max.z()};
    vertices[6] = {node->aabb.min.x(), node->aabb.max.y(), node->aabb.max.z()};
    vertices[7] = node->aabb.max;

    QVector<quint32> indices = {
    0, 1, 1, 2, 2, 3, 3, 0,  // Cara trasera
    4, 5, 5, 7, 7, 6, 6, 4,  // Cara frontal
    0, 4, 1, 5, 2, 7, 3, 6   // Conectores entre caras
    };



    Qt3DCore::QGeometry* geometry = new Qt3DCore::QGeometry(rootEntity);

    // Crear el buffer de vértices
    Qt3DCore::QBuffer* vertexBuffer = new Qt3DCore::QBuffer();
    vertexBuffer->setUsage(Qt3DCore::QBuffer::StaticDraw);
    QByteArray vertexData;
    vertexData.resize(vertices.size() * 3 * sizeof(float));
    float* rawVertexData = reinterpret_cast<float*>(vertexData.data());
    for (const QVector3D& vertex : vertices) {
        *rawVertexData++ = vertex.x();
        *rawVertexData++ = vertex.y();
        *rawVertexData++ = vertex.z();
    }
    vertexBuffer->setData(vertexData);

    Qt3DCore::QAttribute* positionAttribute = new Qt3DCore::QAttribute();
    positionAttribute->setName(Qt3DCore::QAttribute::defaultPositionAttributeName());
    positionAttribute->setVertexBaseType(Qt3DCore::QAttribute::Float);
    positionAttribute->setVertexSize(3);
    positionAttribute->setAttributeType(Qt3DCore::QAttribute::VertexAttribute);
    positionAttribute->setBuffer(vertexBuffer);
    positionAttribute->setCount(vertices.size());

    // Buffer de índices
    Qt3DCore::QBuffer* indexBuffer = new Qt3DCore::QBuffer();
    indexBuffer->setUsage(Qt3DCore::QBuffer::StaticDraw);
    QByteArray indexData;
    indexData.resize(indices.size() * sizeof(quint32));
    memcpy(indexData.data(), indices.constData(), indexData.size());
    indexBuffer->setData(indexData);
    
    Qt3DCore::QAttribute* indexAttribute = new Qt3DCore::QAttribute();
    indexAttribute->setAttributeType(Qt3DCore::QAttribute::IndexAttribute);
    indexAttribute->setVertexBaseType(Qt3DCore::QAttribute::UnsignedInt);
    indexAttribute->setBuffer(indexBuffer);
    indexAttribute->setCount(indices.size());
    geometry->addAttribute(indexAttribute);
    geometry->addAttribute(positionAttribute);

    // Crear el objeto renderizable
    Qt3DRender::QGeometryRenderer* cubeRenderer = new Qt3DRender::QGeometryRenderer();
    cubeRenderer->setGeometry(geometry);
    cubeRenderer->setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);


    Qt3DCore::QEntity* cubeEntity = new Qt3DCore::QEntity(rootEntity);
    cubeEntity->addComponent(cubeRenderer);
    cubeEntity->addComponent(new Qt3DExtras::QPhongMaterial(rootEntity));

    switch (depth) {
        case 0:
            edgeColor = Qt::red; // Convierte el argumento en color
            break;
        case 1:
            edgeColor = Qt::blue; // Convierte el argumento en color     
            break;
        case 2:  
            edgeColor = Qt::green; // Convierte el argumento en color
            break;
    }


    Qt3DExtras::QPhongMaterial* material = new Qt3DExtras::QPhongMaterial();
    material->setDiffuse(edgeColor);
    material->setAmbient(edgeColor);
    cubeEntity->addComponent(material);

    // Recorrer los nodos hijos
    drawAABB(rootEntity, node->left,  (depth + 1)%3);
    drawAABB(rootEntity, node->right, (depth + 1)%3);
}




int main(int argc, char *argv[]) 
{
    QApplication app(argc, argv);
    QString fileName = QFileDialog::getOpenFileName(nullptr, "Load model", "~", "Object files (*.obj)");
    QVector<QVector3D> vertices;
    std::vector<Face> faces;

    // constantes sah
    int maxDepth = 100;
    int minFaces = 1;

    if (fileName.isEmpty()) {
        qDebug() << "No se seleccionó ningún archivo";
        return -1;
    }

    QFile file(fileName);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Error al abrir el archivo OBJ";
        return -1;
    }

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (line.startsWith("v ")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() == 4) {
                float x = parts[1].toFloat();
                float y = parts[2].toFloat();
                float z = parts[3].toFloat();
                vertices.append(QVector3D(x, y, z));
            }
       }
    }

    file.seek(0);

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        if (line.startsWith("f ")) {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);
            if (parts.size() >= 4) {
                QStringList v1 = parts[1].split('/');
                QStringList v2 = parts[2].split('/');
                QStringList v3 = parts[3].split('/');
                if (v1.size() >= 1 && v2.size() >= 1 && v3.size() >= 1) {
                    int index1 = v1[0].toInt() - 1;
                    int index2 = v2[0].toInt() - 1;
                    int index3 = v3[0].toInt() - 1;
                    if (index1 >= 0 && index1 < vertices.size() && index2 >= 0 && index2 < vertices.size() &&
                    index3 >= 0 && index3 < vertices.size()) {
                        Face face;
                        face.v1 = vertices[index1];
                        face.v2 = vertices[index2];
                        face.v3 = vertices[index3];
                        faces.push_back(face);
                    }
                }
            }
        }
    }

    file.close();

    // Calcular la AABB del modelo
    AABB aabb = calculateAABB(vertices);

    // Crear una ventana de Qt 3D
    Qt3DExtras::Qt3DWindow view;
    view.defaultFrameGraph()->setClearColor(QColor(QRgb(0x4d4d4f)));

    // Crear una entidad raíz
    Qt3DCore::QEntity* rootEntity = new Qt3DCore::QEntity();

    // Crear entidad de maya
    Qt3DRender::QMesh* mesh = new Qt3DRender::QMesh();
    mesh->setSource(QUrl::fromLocalFile(fileName));

    // Crear material y prepearar textura
    Qt3DExtras::QDiffuseMapMaterial* material_model = new Qt3DExtras::QDiffuseMapMaterial();
    Qt3DRender::QTextureImage* textureImageComponent = new Qt3DRender::QTextureImage();
    material_model->diffuse()->addTextureImage(textureImageComponent);

    // Aplica malla y material
    Qt3DCore::QEntity* modelEntity = new Qt3DCore::QEntity(rootEntity);
    modelEntity->addComponent(mesh);
    modelEntity->addComponent(material_model);



    // Inicializar el nodo raíz
    KDNode* root = build_KD_tree(aabb, faces, 0, maxDepth, minFaces, 0);

    // Imprimir el árbol KD
    printKDTree(root, 0, 1);

    // Dibujar los cubos de alambre para todos los nodos del árbol KD
     drawAABB(rootEntity, root, 0);




    Qt3DRender::QCamera* cameraEntity = view.camera();
    cameraEntity->lens()->setPerspectiveProjection(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
    cameraEntity->setPosition(QVector3D(0, 0, 5.0f));
    cameraEntity->setViewCenter(QVector3D(0, 0, 0));

    // Controlador de cámara para la interacción del mouse
    Qt3DExtras::QOrbitCameraController* cameraController = new Qt3DExtras::QOrbitCameraController(rootEntity);
    cameraController->setCamera(cameraEntity);
    cameraController->setLinearSpeed(50.0f); // Configura la velocidad de movimiento
    cameraController->setLookSpeed(180.0f); // Configura la velocidad de rotación



    ArbolWidget* widget = new ArbolWidget(root);
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidget(widget);
    scrollArea->show();
    scrollArea->resize( 7680,4320);




    // Establecer la entidad raíz de la escena
    view.setRootEntity(rootEntity);

    // Mostrar la ventana de Qt 3D
    view.show();

    return app.exec();
}
