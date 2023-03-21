#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "particle.hpp"
#include <iostream>

using Catch::Matchers::WithinRel;

TEST_CASE("Particle sets mass correctly", "[particle]")
{
    Particle p{3.14};
    REQUIRE_THAT(p.getMass(), WithinRel(3.14, 0.01));
}

TEST_CASE("No acceleration", "[particle]")
{
    Particle p{1, {1, 2, 3}, {1, 1, 1}, {0, 0, 0}};
    p.update(1.0);
    Eigen::Vector3d expected_position{2, 3, 4};
    Eigen::Vector3d expected_velocity{1, 1, 1};
    Eigen::Vector3d expected_error{0.01, 0.01, 0.01};

    bool position_correctness = p.getPosition().isApprox(expected_position, 0.01);
    bool velocity_correctness = p.getVelocity().isApprox(expected_velocity, 0.01);
    REQUIRE(position_correctness);
    REQUIRE(velocity_correctness);
}

TEST_CASE("Constant acceleration", "[particle]")
{
    Particle p{1, {1, 2, 3}, {1, 1, 1}, {1, 1, 1}};
    p.update(1.0);
    Eigen::Vector3d expected_position{2, 3, 4};
    Eigen::Vector3d expected_velocity{2, 2, 2};
    Eigen::Vector3d expected_error{0.01, 0.01, 0.01};

    bool position_correctness = p.getPosition().isApprox(expected_position, 0.01);
    bool velocity_correctness = p.getVelocity().isApprox(expected_velocity, 0.01);
    REQUIRE(position_correctness);
    REQUIRE(velocity_correctness);
}

TEST_CASE("Artificial acceleration where vec_acceleration = - vec_position.", "[particle]")
{
    Particle p{1, {1, 0, 0}, {0, 1, 0}, {-1, 0, 0}};

    double dt = 0.001;
    double total_time = 2 * M_PI;
    double time = 0;

    while (time < total_time)
    {
        p.update(dt);
        p.setAcceleration(-1 * p.getPosition());
        time += dt;
    }

    Eigen::Vector3d expected_position_final{1, 0, 0};
    Eigen::Vector3d expected_velocity_final{0, 1, 0};

    bool position_correctness = p.getPosition().isApprox(expected_position_final, 0.01);
    bool velocity_correctness = p.getVelocity().isApprox(expected_velocity_final, 0.01);
    REQUIRE(position_correctness);
    REQUIRE(velocity_correctness);
}

TEST_CASE("Gravitational force between two particles", "[Gravity]")
{
    Particle p1{1, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Particle p2{5, {1, 1, 1}, {0, 0, 0}, {0, 0, 0}};

    Eigen::Vector3d expected_acceleration{0.625, 0.625, 0.625};

    bool acceleration_correctness = calcAcceleration(p1, p2, 1.0).isApprox(expected_acceleration, 0.01);
    REQUIRE(acceleration_correctness);
}

TEST_CASE("Gravitational force due to a list of particles including itself)", "[Gravity]")
{
    Particle p0{10, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Particle p1{1, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Particle p2{1, {0, 1, 0}, {0, 0, 0}, {0, 0, 0}};
    Particle p3{1, {0, 0, 1}, {0, 0, 0}, {0, 0, 0}};
    std::vector<Particle> p_list_included{p0, p1, p2, p3};
    std::vector<Particle> p_list_excluded{p1, p2, p3};

    p0.updateAcceleration(p_list_included);
    Eigen::Vector3d expected_acceleration_included = p0.getAcceleration();

    p0.setAcceleration({0, 0, 0});
    p0.updateAcceleration(p_list_excluded);
    Eigen::Vector3d expected_acceleration_excluded = p0.getAcceleration();

    bool acceleration_correctness = expected_acceleration_included.isApprox(expected_acceleration_excluded, 1e-9);
    REQUIRE(acceleration_correctness);
}

TEST_CASE("Gravitational force due to two opposite particles", "[Gravity]")
{
    // origin particle
    Particle p0(10, {0, 0, 0}, {0, 0, 0}, {0, 0, 0});
    // two opposite particles
    Particle p1{1, {1, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    Particle p2{1, {-1, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    // list of particles
    std::vector<Particle> p_list{p0, p1, p2};
    // update acceleration of p0
    p0.updateAcceleration(p_list);
    // check if acceleration is zero
    Eigen::Vector3d expected_acceleration{0, 0, 0};
    bool acceleration_correctness = p0.getAcceleration().isApprox(expected_acceleration, 0.01);
    REQUIRE(acceleration_correctness);
}
