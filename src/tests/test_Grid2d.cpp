#include "catch.hpp"

#include <Eigen/Core>

#include <Grid2d.hpp>

TEST_CASE("Test Grid functionment", "[Grid2d]")
{
    Grid2d grid(2, 2);
    REQUIRE(grid.position(0, 0) == Eigen::Vector2d(0, 0));
    REQUIRE(grid.position(1, 1) == Eigen::Vector2d(1, 1));
    Grid2d grid2(3, 3);
    REQUIRE(grid2.position(1, 1) == Eigen::Vector2d(0.5, 0.5));
    REQUIRE(grid2.position(2, 2) == Eigen::Vector2d(1, 1));
}

TEST_CASE("Test grid get grid position", "[Grid2d]")
{
    Grid2d grid(3, 3);

    REQUIRE(grid.grid_position(0.5, 0.5) == Eigen::Vector2i(1, 1));
    REQUIRE(grid.grid_position(0.25, 0.25) == Eigen::Vector2i(0, 0));
    REQUIRE(grid.grid_position(0.75, 0.75) == Eigen::Vector2i(1, 1));
}

TEST_CASE("Test grid construction", "[Grid2d]")
{
    Grid2d grid(3, 3);

    SECTION("Copy constructor is working")
    {
        Grid2d grid2(grid);

        REQUIRE(grid2.cell_number() == grid.cell_number());
        REQUIRE(grid2.width() == grid.width());
    }
    SECTION("Move constructor is working")
    {
        Grid2d grid2(std::move(grid));

        REQUIRE(grid2.cell_number() == grid.cell_number());
        REQUIRE(grid2.width() == grid.width());
    }
    SECTION("Affectation is working")
    {
        Grid2d grid2(8, 8, {0, 0}, {1.5, 1.5});
        grid2 = grid;
        REQUIRE(grid2.cell_number() == grid.cell_number());
        REQUIRE(grid2.width() == grid.width());
    }
    SECTION("Move is working")
    {
        Grid2d grid2(8, 8, {0, 0}, {1.5, 1.5});
        grid2 = std::move(grid);
        REQUIRE(grid2.cell_number() == grid.cell_number());
        REQUIRE(grid2.width() == grid.width());
    }
}