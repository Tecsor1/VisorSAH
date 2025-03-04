#include "cabecera.h"


AABB calculateAABB(const QVector<QVector3D>& vertices) 
{
    AABB aabb;
    aabb.min = aabb.max = vertices[0];
    for (const QVector3D& vertex : vertices) {
        if (vertex.x() < aabb.min.x()) aabb.min.setX(vertex.x());
        if (vertex.y() < aabb.min.y()) aabb.min.setY(vertex.y());
        if (vertex.z() < aabb.min.z()) aabb.min.setZ(vertex.z());
        if (vertex.x() > aabb.max.x()) aabb.max.setX(vertex.x());
        if (vertex.y() > aabb.max.y()) aabb.max.setY(vertex.y());
        if (vertex.z() > aabb.max.z()) aabb.max.setZ(vertex.z());
    }

    return aabb;
}


bool isPointInAABB(const QVector3D& point, const AABB& aabb) 
{
    return (point.x() >= aabb.min.x() && point.x() <= aabb.max.x() &&
            point.y() >= aabb.min.y() && point.y() <= aabb.max.y() &&
            point.z() >= aabb.min.z() && point.z() <= aabb.max.z());
}


std::pair<std::vector<Face>, std::vector<Face>> splitFaces(const std::vector<Face>& faces, const
AABB& aabbLeft, const AABB& aabbRight, int axis) 
{

    std::vector<Face> facesLeft;
    std::vector<Face> facesRight;

    for (const Face& face : faces) {
        if (face.v1[axis] <= aabbLeft.max[axis] && face.v2[axis] <= aabbLeft.max[axis] && face.v3[axis] <= aabbLeft.max[axis])
            facesLeft.push_back(face);
        else 
            if (face.v1[axis] >= aabbRight.min[axis] && face.v2[axis] >= aabbRight.min[axis] && face.v3[axis] >= aabbRight.min[axis])
                facesRight.push_back(face);
    }

    return std::make_pair(facesLeft, facesRight);
}



float calculateSurfaceArea(const AABB& aabb) 
{
    QVector3D extents = aabb.max - aabb.min;
    return 2.0f * (extents.x() * extents.y() + extents.x() * extents.z() + extents.y() * extents.z());
}



std::pair<float, std::pair<AABB, AABB>> calculateSAHCost(const AABB& aabb, const std::vector<Face>& faces, float t_traversal, 
float t_intersect, int axis) 
{

    float totalSurfaceArea = calculateSurfaceArea(aabb);
    float sahCost = std::numeric_limits<float>::infinity();
    int numFaces = faces.size();
    std::pair<AABB, AABB> bestAABBs;

    float splitPosition1 = aabb.min[axis] + 0.5f * (aabb.max[axis] - aabb.min[axis]);

    AABB aabbA1, aabbB1;

    aabbA1.min = aabb.min;
    aabbA1.max = QVector3D(aabb.max.x(), aabb.max.y(), aabb.max.z());
    aabbA1.max[axis] = splitPosition1;

    aabbB1.min = aabb.min;
    aabbB1.max = QVector3D(aabb.max.x(), aabb.max.y(), aabb.max.z());
    aabbB1.min[axis] = splitPosition1;

    float areaA1 = calculateSurfaceArea(aabbA1);
    float areaB1 = calculateSurfaceArea(aabbB1);
    int numFacesA1 = 0;
    int numFacesB1 = 0;


    for (int i = 0; i < numFaces; i++) {
        const Face& face = faces[i];
        if (face.v1[axis] <= splitPosition1 && face.v2[axis] <= splitPosition1 && face.v3[axis] <= splitPosition1)
            numFacesA1++; 
        else 
            if (face.v1[axis] >= splitPosition1 && face.v2[axis] >= splitPosition1 && face.v3[axis] >= splitPosition1)
                numFacesB1++;
    }

    float probabilityA1 = areaA1 / totalSurfaceArea;
    float probabilityB1 = areaB1 / totalSurfaceArea;

    float sahCost1 = t_traversal + probabilityA1 * numFacesA1 * t_intersect + probabilityB1 *
    numFacesB1 * t_intersect;

    if (sahCost1 < sahCost) {
        sahCost = sahCost1;
        bestAABBs.first = aabbA1;
        bestAABBs.second = aabbB1;
    }


    float splitPosition2 = aabb.min[axis] + 0.2f * (aabb.max[axis] - aabb.min[axis]);
    AABB aabbA2, aabbB2;

    aabbA2.min = aabb.min;
    aabbA2.max = QVector3D(aabb.max.x(), aabb.max.y(), aabb.max.z());
    aabbA2.max[axis] = splitPosition2;

    aabbB2.min = aabb.min;
    aabbB2.max = QVector3D(aabb.max.x(), aabb.max.y(), aabb.max.z());
    aabbB2.min[axis] = splitPosition2;

    float areaA2 = calculateSurfaceArea(aabbA2);
    float areaB2 = calculateSurfaceArea(aabbB2);
    int numFacesA2 = 0;
    int numFacesB2 = 0;

    for (int i = 0; i < numFaces; i++) {
        const Face& face = faces[i];
        if (face.v1[axis] <= splitPosition2 && face.v2[axis] <= splitPosition2 && face.v3[axis] <= splitPosition2)
            numFacesA2++; 
        else 
            if (face.v1[axis] >= splitPosition2 && face.v2[axis] >= splitPosition2 && face.v3[axis] >= splitPosition2)
                numFacesB2++;
    }


    float probabilityA2 = areaA2 / totalSurfaceArea;
    float probabilityB2 = areaB2 / totalSurfaceArea;

    float sahCost2 = t_traversal + probabilityA2 * numFacesA2 * t_intersect + probabilityB2 * numFacesB2 * t_intersect;

    if (sahCost2 < sahCost) {
        sahCost = sahCost2;
        bestAABBs.first = aabbA2;
        bestAABBs.second = aabbB2;
    }

    float splitPosition3 = aabb.min[axis] + 0.8f * (aabb.max[axis] - aabb.min[axis]);
    AABB aabbA3, aabbB3;


    aabbA3.min = aabb.min;
    aabbA3.max = QVector3D(aabb.max.x(), aabb.max.y(), aabb.max.z());
    aabbA3.max[axis] = splitPosition3;


    aabbB3.min = aabb.min;
    aabbB3.max = QVector3D(aabb.max.x(), aabb.max.y(), aabb.max.z());
    aabbB3.min[axis] = splitPosition3;

    float areaA3 = calculateSurfaceArea(aabbA3);
    float areaB3 = calculateSurfaceArea(aabbB3);
    int numFacesA3 = 0;
    int numFacesB3 = 0;

    for (int i = 0; i < numFaces; i++) {
        const Face& face = faces[i];
        if (face.v1[axis] <= splitPosition3 && face.v2[axis] <= splitPosition3 && face.v3[axis] <= splitPosition3)
            numFacesA3++; 
        else 
            if (face.v1[axis] >= splitPosition3 && face.v2[axis] >= splitPosition3 && face.v3[axis] >= splitPosition3)
                numFacesB3++;
    }

    float probabilityA3 = areaA3 / totalSurfaceArea;
    float probabilityB3 = areaB3 / totalSurfaceArea;
    float sahCost3 = t_traversal + probabilityA3 * numFacesA3 * t_intersect + probabilityB3 *
    numFacesB3 * t_intersect;

    if (sahCost3 < sahCost) {
        sahCost = sahCost3;
        bestAABBs.first = aabbA3;
        bestAABBs.second = aabbB3;
    }

    return std::make_pair(sahCost, bestAABBs);
}




KDNode* build_KD_tree(const AABB& aabb, const std::vector<Face>& faces, int depth, int
maxDepth, int minFaces, int axis) 
{

    // Verificar condición de parada
    if (faces.size() <= minFaces )
        return new KDNode(aabb, faces);

    // Calcular puntos de división y evaluar costos SAH
    float t_traversal = 1.0f;
    float t_intersect = 0.5f;
    auto sahOutput = calculateSAHCost(aabb, faces, t_traversal, t_intersect, axis);
    float sahCost = sahOutput.first;
    AABB aabbLeft = sahOutput.second.first;
    AABB aabbRight = sahOutput.second.second;

    // Dividir las caras basándose en el punto de división óptimo
    std::vector<Face> facesLeft, facesRight;
    std::tie(facesLeft, facesRight) = splitFaces(faces, aabbLeft, aabbRight,axis);
    axis = axis + 1;
    axis = axis % 3;

    // Llamada recursiva para construir los subárboles
    KDNode* leftChild = build_KD_tree(aabbLeft, facesLeft, depth + 1, maxDepth, minFaces, axis );
    KDNode* rightChild = build_KD_tree(aabbRight, facesRight, depth + 1, maxDepth, minFaces, axis);

    // Crear y retornar el nodo actual
    return new KDNode(aabb, leftChild, rightChild);
}
