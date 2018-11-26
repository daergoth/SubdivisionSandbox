#ifndef CUSTOMSCHEME_H
#define CUSTOMSCHEME_H

#include <vector>

class CustomScheme
{
public:
    enum MeshType {Triangular, Quadrilateral};
    enum RefinementType {Approximating, Interpolating};

    class OddWeight {
    public:
        OddWeight(int level, double value): level(level), value(value){}
        int level;
        double value;
    };

    class EvenWeights {
    public:
        EvenWeights(double beta, double gamma): beta(beta), gamma(gamma){}
        double beta;
        double gamma;
    };

    class Weights {
    public:
        Weights(std::vector<OddWeight> odd, EvenWeights even): odd(odd), even(even){}
        std::vector<OddWeight> odd;
        EvenWeights even;
    };

    CustomScheme(MeshType mesh_type, RefinementType refinement_type, int neighbour_level, Weights weights):
        mesh_type(mesh_type),
        refinement_type(refinement_type),
        neighbour_level(neighbour_level),
        weights(weights){}

    MeshType getMeshType();
    void setMeshType(MeshType mesh_type);

    RefinementType getRefinementType();
    void getMeshType(RefinementType refinement_type);

    int getNeighbourLevel();
    void setNeighbourLevel(int neighbour_level);

    Weights getWeights();
    void setWeights(Weights weights);

private:
    MeshType mesh_type;
    RefinementType refinement_type;
    int neighbour_level;
    Weights weights;
};


#endif // CUSTOMSCHEME_H
