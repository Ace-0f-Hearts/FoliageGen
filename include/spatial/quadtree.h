//
// Created by ace on 2026-02-02.

#ifndef GENERATOR_QUADTREE_H
#define GENERATOR_QUADTREE_H
#include <vector>
#include <spatial/spatial_coordinates.h>
#include <spatial/bounding_box.h>
#include <spatial/quadtree.h>

/**
 * Data structure intended to be used for holding points and efficiently searching for nearest neighbor queries.
 */
class Quadtree
{
protected:
    /**
     * Helper struct for comparing the proximity between two points and processing first those that are closer to each other.
     */
    struct Proximity
    {
        float proximity;
        int idx;


        /**
         * Proximity is smaller if distance is measured to be greater
         * @param rhs const Proximity&
         * @return true if \code proximity \endcode is higher than \code rhs.promity \endcode, false otherwise
         */
        bool operator<(const Proximity& rhs) const;


        /**
         * Proximity is greater if distance is measured to be smaller
         * @param rhs const Proximity&
         * @return true if \code proximity \endcode is smaller than \code rhs.proximity \endcode, false otherwise
         */
        bool operator>(const Proximity& rhs) const;
    };

    /**
     * Helper struct for comparing the distance between points, and processing and replacing farther away points.
     */
    struct Distance
    {
        float distance;
        int idx;

        /**
         *
         * @param rhs const Distance&
         * @return true if \code distance \endcode is smaller than \code rhs.distance \endcode, false otherwise
         */
        bool operator<(const Distance& rhs) const;
        /**
         *
         * @param rhs const Distance&
         * @return true if \code distance \endcode is greater than \code rhs.distance \endcode, false otherwise
         */
        bool operator>(const Distance& rhs) const;
    };

public:
    Quadtree() = delete;
    Quadtree(const std::vector<Spatial::Spatial2D>& data, const BoundingBox2D& bounding_box);
    Quadtree(const Quadtree& other);
    Quadtree& operator=(const Quadtree& other);

    void Clean();

    void BulkLoad(const std::vector<Spatial::Spatial2D>& data);
    void BulkLoad(const std::vector<Spatial::Spatial2D>& data, const BoundingBox2D& bounding_box);
    [[nodiscard]] int FindNearestNeighbour(const Spatial::Spatial2D& query_point) const;
    [[nodiscard]] std::vector<int> FindNearestNeighbours(const Spatial::Spatial2D& point, const unsigned int amount) const;

    bool operator==(const Quadtree&) const;
    Spatial::Spatial2D operator[](int idx) const;

    [[nodiscard]] int depth();
    /**
     *
     * @return Returns the full size of the quadtree, including both the number of items and number of nodes.
     */
    [[nodiscard]] int size() const;
    [[nodiscard]] int node_count() const;
    [[nodiscard]] int item_count() const;
    [[nodiscard]] int root_idx() const;

private:
    constexpr static int kDefault_value = -1;
    constexpr static int kNumber_of_cells = 4;
    constexpr static size_t kRoot = 0;
    constexpr static size_t kMax_tree_size = 4096; //TODO: Recalc


    void Load(std::vector<Spatial::Spatial2D> data, const BoundingBox2D& bounding_box);
    void AllocateItemIndex(int index_to_allocate, const BoundingBox2D& bbox);

    void AllocateNode(int tree_idx, BoundingBox2D& bbox);

    int node_count_ = 0;
    int item_count_ = 0;
    std::vector<Spatial::Spatial2D> data_;
    std::vector<int> index_tree_;
};

inline std::ostream& operator<<(std::ostream& os, const Quadtree& rhs)
{
    os << rhs.item_count();
    return os;
}

#endif //GENERATOR_QUADTREE_H
