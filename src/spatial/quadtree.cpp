//
// Created by ace on 2026-02-02.
//
#include <iostream>
#include <boost/heap/priority_queue.hpp>
#include <spatial/quadtree.h>
#include <spatial/spatial_coordinates.h>
#include <spatial/bounding_box.h>

#include <utility/not_implemented_error.h>


bool Quadtree::Proximity::operator<(const Proximity& rhs) const
{
    return proximity > rhs.proximity;
}

bool Quadtree::Proximity::operator>(const Proximity& rhs) const
{
    return proximity < rhs.proximity;
}

bool Quadtree::Distance::operator<(const Distance& rhs) const
{
    return distance < rhs.distance;
}

bool Quadtree::Distance::operator>(const Distance& rhs) const
{
    return distance > rhs.distance;
}


Quadtree::Quadtree(std::vector<Spatial::Spatial2D> const& data, BoundingBox2D const& bounding_box) :  node_count_(1), item_count_(data.size()), data_(data), index_tree_(kMax_tree_size,kDefault_value)
{
    data_.push_back(bounding_box.min());
    data_.push_back(bounding_box.max());

    Load(data,bounding_box);
}

Quadtree::Quadtree(const Quadtree& other) = default;

Quadtree& Quadtree::operator=(const Quadtree& other)
{
    item_count_= other.item_count();
    node_count_ = other.node_count();
    data_ = other.data_;
    index_tree_ = other.index_tree_;
    return *this;
}

int Quadtree::root_idx() const
{
    return item_count();
}

int Quadtree::FindNearestNeighbour(const Spatial::Spatial2D& query_point) const
{
    int closest_neighbour_idx = root_idx();

    int node_idx = root_idx();

    auto bbox = BoundingBox2D(data_[node_idx],data_[node_idx + 1]);

    bool first_best_found = false;
    float distance_to_closest_neighbour = bbox.GetRadiusOfSmallestBoundingCircle() * 2;
    boost::heap::priority_queue<Proximity> search_heap;

    auto of_root = Proximity{0,0};

    // heap.push(of_root);

    for(int idx = 0; idx < std::pow(2,bbox.dim()); ++idx)
    {
        auto sub_bbox = bbox.GetSubspaceByIndex(idx);
        auto distance = sub_bbox.DistanceTo(query_point);

        if (distance < distance_to_closest_neighbour)
        {
            auto d = Proximity{distance, idx};
            search_heap.push(d);
        }
    }


    while (!search_heap.empty())
    {
        Proximity of_node = search_heap.top();
        search_heap.pop();
        node_idx = index_tree_[of_node.idx];

        if (node_idx == kDefault_value)
        {

        }
        else if (node_idx < item_count()) // Item found, check distance and update best if necessary
        {

            if (!first_best_found)
            {
                first_best_found = true;
                if (data_[node_idx] != query_point)
                {
                    std::cout << data_[node_idx] << " and " << query_point << " : " << node_idx << " : " << of_node.proximity  << std::endl;

                    for (auto i : search_heap)
                    {
                        std::cout << "Index: " <<  index_tree_[i.idx] << "\tPoint: "<< data_[index_tree_[i.idx]] << "\tProximity: " << i.proximity << std::endl;
                    }
                    throw std::logic_error("This should be impossible.");
                }
                continue;
            }
            auto distance_to_node = query_point.DistanceTo(data_[node_idx]);

            if (distance_to_node < distance_to_closest_neighbour)
            {
                distance_to_closest_neighbour = distance_to_node;
                closest_neighbour_idx = node_idx;
            }
        }
        else if (node_idx >= item_count()) // Node found, add children to heap if they can contain points closer compared to current best
        {
            bbox = BoundingBox2D(data_[node_idx],data_[node_idx + 1]);
            for(int idx = 0; idx < 1 << bbox.dim(); ++idx)
            {
                auto sub_bbox = bbox.GetSubspaceByIndex(idx);
                auto distance = sub_bbox.DistanceTo(query_point);


                if (distance < distance_to_closest_neighbour)
                {
                    int child_idx = (of_node.idx + 1) * kNumber_of_cells + idx;
                    Proximity d = Proximity{distance,child_idx};

                    std::cout << "Point: " << index_tree_[child_idx] << "\tProximity: " << distance << "\tBounding Box" << bbox << "\tSub-Bounding Box: " << sub_bbox << std::endl;
                    std::cout <<  idx << std::endl;

                    if (index_tree_[child_idx] != kDefault_value)
                        search_heap.push(d);
                }
            }
        }
    }

    return closest_neighbour_idx;
}

int Quadtree::size() const
{
    return item_count_ + node_count_;
}

int Quadtree::node_count() const
{
    return node_count_;
}

int Quadtree::item_count() const
{
    return item_count_;
}

bool Quadtree::operator==(const Quadtree& rhs) const
{
    auto equal = item_count() == rhs.item_count() && node_count() == rhs.node_count();

    int matching_counter = 0;
    int matching_empty_c = 0;

    BoundingBox2D  lhs_node = {data_[item_count()],data_[item_count()+1]};
    BoundingBox2D  rhs_node = {rhs.data_[item_count()],rhs.data_[item_count()+1]};
    Spatial::Spatial2D const* lhs_item;
    Spatial::Spatial2D const* rhs_item;


    matching_counter += lhs_node == rhs_node ? 1 : 0;

    for (int i = 0; i < kMax_tree_size; ++i)
    {

        int lhs_idx = index_tree_[i];
        int rhs_idx = rhs.index_tree_[i];

        if (lhs_idx == kDefault_value || rhs_idx == kDefault_value)
        {

            matching_empty_c += lhs_idx == rhs_idx ? 1 : 0;
        }
        else if (lhs_idx < item_count_ && lhs_idx >= 0)
        {
            lhs_item = &(data_[lhs_idx]);
            rhs_item = &(rhs.data_[rhs_idx]);

            matching_counter += (*lhs_item == *rhs_item) ? 1 : 0;
        }
        else
        {
            lhs_node = {data_[item_count()],data_[item_count()+1]};
            rhs_node = {rhs.data_[item_count()],rhs.data_[item_count()+1]};

            matching_counter += lhs_node == rhs_node ? 1 : 0;
        }
    }


    equal = equal && matching_counter == size() && matching_empty_c == kMax_tree_size - (node_count() - 1) - item_count();
    printf("%d : %d\n",matching_counter, size());
    printf("%d : %lu\n",matching_empty_c, kMax_tree_size - (node_count() - 1) - item_count());
    return equal;
}

void Quadtree::BulkLoad(const std::vector<Spatial::Spatial2D>& data)
{
    auto bounding_box = BoundingBox2D({data_[item_count()],data_[item_count() + 1]});
    BulkLoad(data,bounding_box);
}

void Quadtree::BulkLoad(const std::vector<Spatial::Spatial2D>& data, const BoundingBox2D& bounding_box)
{
    Clean();
    data_.push_back(bounding_box.min());
    data_.push_back(bounding_box.max());
    Load(data,bounding_box);
}

void Quadtree::Clean()
{
    data_.clear();
    index_tree_.clear();

    node_count_ = 0;
    item_count_ = 0;
}

int Quadtree::depth()
{

    auto idx = std::ranges::find_if_not(index_tree_.crbegin(), index_tree_.crend(),[](int i) {return i == -1;});


    if (idx == index_tree_.crend()) // No index found
        return -1;

    auto d = std::ranges::distance(index_tree_.crend(), idx) * (-1) - 1;
    printf("idx : %d\t d : %d\n",*idx,d);
    return std::ceil((d >> 2) / 4.0f) + 1;
}

std::vector<int> Quadtree::FindNearestNeighbours(const Spatial::Spatial2D& point, const unsigned int amount) const
{

    int node_idx = root_idx();

    auto bbox = BoundingBox2D(data_[node_idx],data_[node_idx + 1]);

    bool first_best_found = false;
    float distance_to_nth_closest_neighbour = bbox.GetRadiusOfSmallestBoundingCircle() * 2;
    boost::heap::priority_queue<Proximity> search_heap;
    search_heap.reserve(log2(item_count() + node_count()));


    boost::heap::priority_queue<Distance> k_nodes;
    k_nodes.reserve(amount);


    auto of_root = Proximity{0,0};


    for(int idx = 0; idx < std::pow(2,bbox.dim()); ++idx)
    {
        auto sub_bbox = bbox.GetSubspaceByIndex(idx);
        auto distance = sub_bbox.DistanceTo(point);

        if (distance < distance_to_nth_closest_neighbour)
        {
            auto d = Proximity{distance, idx};
            search_heap.push(d);
        }
    }


    while (!search_heap.empty())
    {
        Proximity of_node = search_heap.top();
        search_heap.pop();
        node_idx = index_tree_[of_node.idx];

        if (node_idx == kDefault_value)
        {

        }
        else if (node_idx < item_count()) // Item found, check distance and update best if necessary
        {

            if (!first_best_found)
            {
                first_best_found = true;
                continue;
            }
            auto distance_to_node = point.DistanceTo(data_[node_idx]);

            auto d = Distance{of_node.proximity,node_idx};

            if (k_nodes.size() < amount)
            {
                k_nodes.push(d);
                if (k_nodes.size() == amount)
                    distance_to_nth_closest_neighbour = k_nodes.top().distance;
            }
            else
            {
                if (distance_to_node < distance_to_nth_closest_neighbour)
                {
                    k_nodes.pop();
                    k_nodes.push(d);
                }
                distance_to_nth_closest_neighbour = k_nodes.top().distance;
            }
        }
        else if (node_idx >= item_count()) // Node found, add children to heap if they can contain points closer compared to current best
        {
            bbox = BoundingBox2D(data_[node_idx],data_[node_idx + 1]);
            for(int idx = 0; idx < std::pow(2,bbox.dim()); ++idx)
            {
                auto sub_bbox = bbox.GetSubspaceByIndex(idx);
                auto distance = sub_bbox.DistanceTo(point);

                if (distance < distance_to_nth_closest_neighbour)
                {
                    auto p = Proximity{distance,(of_node.idx + 1) * kNumber_of_cells + idx};
                    search_heap.push(p);
                }
            }
        }
    }

    std::vector<int> k_nodes_indices;
    while (!k_nodes.empty())
    {
        auto nodes = k_nodes.top();
        k_nodes.pop();
        k_nodes_indices.push_back(nodes.idx);
    }

    return k_nodes_indices;
}

Spatial::Spatial2D Quadtree::operator[](int idx) const
{
    if (idx >= item_count_)
    {
        throw std::out_of_range("Index out of range");
    }
    return data_[idx];
}



void Quadtree::AllocateItemIndex(const int index_to_allocate, const BoundingBox2D& bounding_box)
{

    auto bbox = bounding_box;

    Spatial::Spatial2D centroid = bbox.GetCentroid();


    int parent_idx = kRoot;



    auto point_to_allocate = data_[index_to_allocate];
    int child_path = centroid.PositionOther(point_to_allocate);

    int child_idx = child_path + parent_idx * kNumber_of_cells;



    while (index_tree_[child_idx] > item_count_)
    {
        bbox = bbox.GetSubspaceOf(point_to_allocate);
        centroid = bbox.GetCentroid();

        parent_idx = child_idx;
        child_path = centroid.PositionOther(point_to_allocate);
        child_idx = child_path + (1+parent_idx) * kNumber_of_cells;
    }
    if (index_tree_[child_idx] != kDefault_value) /// Index already occupied by an item
    {


        auto prev_bbox = bbox;
        auto prev_idx = index_tree_[child_idx];

        do
        {
            parent_idx = child_idx;
            int cell = parent_idx;


            prev_bbox = prev_bbox.GetSubspaceOf(data_[prev_idx]);
            AllocateNode(cell,prev_bbox); // Allocate parent of both previous and current idx

            centroid = prev_bbox.GetCentroid();
            child_path = centroid.PositionOther(data_[prev_idx]);
            child_idx = child_path + (1+cell) * kNumber_of_cells;
            index_tree_[child_idx] = prev_idx;

            child_path = centroid.PositionOther(data_[index_to_allocate]);
            child_idx = child_path + (1+cell) * kNumber_of_cells;

        } while (index_tree_[child_idx] != kDefault_value); // While no default value is found
    }
    index_tree_[child_idx] = index_to_allocate;
}

void Quadtree::AllocateNode(int tree_idx, BoundingBox2D& bbox)
{
    index_tree_[tree_idx] = item_count_ + node_count_ * 2; /// Equal to current data_.size()
    node_count_++;
    data_.push_back(bbox.min());
    data_.push_back(bbox.max());
}

void Quadtree::Load(const std::vector<Spatial::Spatial2D> data,const BoundingBox2D& bounding_box)
{
    for (size_t i = 0; i < data.size(); i++)
    {
        AllocateItemIndex(i,bounding_box);
    }
}


