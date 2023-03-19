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
}
