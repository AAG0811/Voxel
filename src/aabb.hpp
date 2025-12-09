#include <cmath>
#include <glm/glm.hpp>

enum class CollisionDirection {
    NONE,
    X_POSITIVE,   // Collision from the right (positive X axis)
    X_NEGATIVE,   // Collision from the left (negative X axis)
    Y_POSITIVE,   // Collision from above (positive Y axis)
    Y_NEGATIVE,   // Collision from below (negative Y axis)
    Z_POSITIVE,   // Collision from the front (positive Z axis)
    Z_NEGATIVE    // Collision from the back (negative Z axis)
};
inline glm::vec3 aabbIntersect(glm::vec3 player, glm::vec3 block) {
    glm::vec3 pmin = player - glm::vec3(1., 0.0, 1.);
    glm::vec3 pmax = player + glm::vec3(1., 3.6, 1.);

    glm::vec3 bmin = block - glm::vec3(1);
    glm::vec3 bmax = block + glm::vec3(1);

    glm::vec3 delta(0.0);
    if (pmax.x > bmin.x && pmin.x < bmax.x &&
        pmax.y > bmin.y && pmin.y < bmax.y &&
        pmax.z > bmin.z && pmin.z < bmax.z) {

        // Compute overlaps
        float overlapX1 = bmax.x - pmin.x; // block right - player left
        float overlapX2 = pmax.x - bmin.x; // player right - block left
        float overlapX = (overlapX1 < overlapX2) ? overlapX1 : -overlapX2;

        float overlapY1 = bmax.y - pmin.y;
        float overlapY2 = pmax.y - bmin.y;
        float overlapY = (overlapY1 < overlapY2) ? overlapY1 : -overlapY2;

        float overlapZ1 = bmax.z - pmin.z;
        float overlapZ2 = pmax.z - bmin.z;
        float overlapZ = (overlapZ1 < overlapZ2) ? overlapZ1 : -overlapZ2;

        // Resolve along smallest axis
        using namespace std; 
        if (fabs(overlapX) <= fabs(overlapY) && fabs(overlapX) <= fabs(overlapZ)) delta.x = overlapX;
        else if (fabs(overlapY) <= fabs(overlapX) && fabs(overlapY) <= fabs(overlapZ)) delta.y = overlapY;
        else delta.z = overlapZ;


        player += delta; // push player out
    }
    return player;
}
