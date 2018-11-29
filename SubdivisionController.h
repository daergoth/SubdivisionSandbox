#ifndef SUBDIVISIONCONTROLLER_H
#define SUBDIVISIONCONTROLLER_H

#include <iostream>
#include <map>
#include <vector>

#include "SubdivisionStrategy.h"
#include "LoopSubdivisionStrategy.h"
#include "ButterflySubdivisionStrategy.h"
#include "CatmullClarkSubdivisionStrategy.h"
#include "KobbeltSubdivisionStrategy.h"
#include "CustomSchemeSubdivisionStrategy.h"

enum SubdivisionScheme {Loop, Butterfly, CatmullClark, Kobbelt, Custom};

class SubdivisionController
{
public:
    ~SubdivisionController();

    static SubdivisionController& getInstance() {
        static SubdivisionController instance;
        return instance;
    }

    SubdivisionController(SubdivisionController const&) = delete;
    void operator=(SubdivisionController const&) = delete;

    void switchTo(SubdivisionScheme scheme);

    void doSubdivision();
    void doBackwardStep();

    Mesh getCurrentMesh();
    void setBaseMesh(Mesh baseMesh);

private:
    SubdivisionController();

    SubdivisionStrategy* currentStrategy = nullptr;
    std::map<SubdivisionScheme, SubdivisionStrategy*> strategyMap;

    int currentMeshIndex = 0;
    std::vector<Mesh> meshHistory;
};

#endif // SUBDIVISIONCONTROLLER_H
