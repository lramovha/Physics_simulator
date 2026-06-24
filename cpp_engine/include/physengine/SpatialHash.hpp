#ifndef SPATIAL_HASH_HPP
#define SPATIAL_HASH_HPP

#include "RigidBody.hpp"
#include <unordered_map>
#include <vector>
#include <cstdint>

class SpatialHash {
private:
    std::unordered_map<uint64_t, std::vector<RigidBody*>> grid;
    double cellSize = 2.0;  // Should be >= 2 * max radius

    uint64_t hash(int x, int y, int z) const {
        return ((uint64_t)x * 73856093ULL) ^
               ((uint64_t)y * 19349663ULL) ^
               ((uint64_t)z * 83492791ULL);
    }

public:
    void clear() { grid.clear(); }

    void insert(RigidBody* body) {
        if (!body) return;
        double r = body->radius;
        double minX = body->position.x() - r;
        double minY = body->position.y() - r;
        double minZ = body->position.z() - r;
        double maxX = body->position.x() + r;
        double maxY = body->position.y() + r;
        double maxZ = body->position.z() + r;

        int x0 = (int)floor(minX / cellSize);
        int y0 = (int)floor(minY / cellSize);
        int z0 = (int)floor(minZ / cellSize);
        int x1 = (int)floor(maxX / cellSize);
        int y1 = (int)floor(maxY / cellSize);
        int z1 = (int)floor(maxZ / cellSize);

        for (int x = x0; x <= x1; ++x) {
            for (int y = y0; y <= y1; ++y) {
                for (int z = z0; z <= z1; ++z) {
                    uint64_t key = hash(x, y, z);
                    grid[key].push_back(body);
                }
            }
        }
    }

    std::vector<std::pair<RigidBody*, RigidBody*>> getPotentialPairs() {
        std::vector<std::pair<RigidBody*, RigidBody*>> pairs;
        for (const auto& [key, cell] : grid) {
            for (size_t i = 0; i < cell.size(); ++i) {
                for (size_t j = i + 1; j < cell.size(); ++j) {
                    pairs.emplace_back(cell[i], cell[j]);
                }
            }
        }
        return pairs;
    }
};

#endif