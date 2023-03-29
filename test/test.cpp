#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "particle.hpp"
#include "nbody.hpp"
#include <iostream>

using Catch::Matchers::WithinRel;

TEST_CASE("Particle sets mass correctly", "[particle]")
{
    std::shared_ptr<Particle> p = std::make_shared<Particle>(3.14);
    REQUIRE_THAT(p->getMass(), WithinRel(3.14, 0.01));
}

TEST_CASE("No acceleration", "[particle]")
{
    std::shared_ptr<Particle> p = std::make_shared<Particle>(
        1,
        Eigen::Vector3d{1, 2, 3},
        Eigen::Vector3d{1, 1, 1},
        Eigen::Vector3d{0, 0, 0});
    p->update(1.0);
    Eigen::Vector3d expected_position{2, 3, 4};
    Eigen::Vector3d expected_velocity{1, 1, 1};
    Eigen::Vector3d expected_error{0.01, 0.01, 0.01};

    bool position_correctness = p->getPosition().isApprox(expected_position, 0.01);
    bool velocity_correctness = p->getVelocity().isApprox(expected_velocity, 0.01);
    REQUIRE(position_correctness);
    REQUIRE(velocity_correctness);
}

TEST_CASE("Constant acceleration", "[particle]")
{
    std::shared_ptr<Particle> p = std::make_shared<Particle>(
        1,
        Eigen::Vector3d{1, 2, 3},
        Eigen::Vector3d{1, 1, 1},
        Eigen::Vector3d{1, 1, 1});
    p->update(1.0);
    Eigen::Vector3d expected_position{2, 3, 4};
    Eigen::Vector3d expected_velocity{2, 2, 2};
    Eigen::Vector3d expected_error{0.01, 0.01, 0.01};

    bool position_correctness = p->getPosition().isApprox(expected_position, 0.01);
    bool velocity_correctness = p->getVelocity().isApprox(expected_velocity, 0.01);
    REQUIRE(position_correctness);
    REQUIRE(velocity_correctness);
}

TEST_CASE("Artificial acceleration where vec_acceleration = - vec_position.", "[particle]")
{
    std::shared_ptr<Particle> p = std::make_shared<Particle>(
        1,
        Eigen::Vector3d{1, 0, 0},
        Eigen::Vector3d{0, 1, 0},
        Eigen::Vector3d{-1, 0, 0});

    double dt = 0.001;
    double total_time = 2 * M_PI;
    double time = 0;

    while (time < total_time)
    {
        p->update(dt);
        p->setAcceleration(-1 * p->getPosition());
        time += dt;
    }

    Eigen::Vector3d expected_position_final{1, 0, 0};
    Eigen::Vector3d expected_velocity_final{0, 1, 0};

    bool position_correctness = p->getPosition().isApprox(expected_position_final, 0.01);
    bool velocity_correctness = p->getVelocity().isApprox(expected_velocity_final, 0.01);
    REQUIRE(position_correctness);
    REQUIRE(velocity_correctness);
}

TEST_CASE("Gravitational force between two particles", "[Gravity]")
{
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>(1, Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>(5, Eigen::Vector3d(1, 1, 1), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));

    Eigen::Vector3d expected_acceleration{0.9622, 0.9622, 0.9622};

    bool acceleration_correctness = calcAcceleration(*p1, *p2, 0.0).isApprox(expected_acceleration, 0.01);
    REQUIRE(acceleration_correctness);
}

TEST_CASE("Gravitational force due to a list of particles including itself)", "[Gravity]")
{
    std::shared_ptr<Particle> p0 = std::make_shared<Particle>(10, Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>(1, Eigen::Vector3d(1, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>(1, Eigen::Vector3d(0, 2, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    std::shared_ptr<Particle> p3 = std::make_shared<Particle>(1, Eigen::Vector3d(0, 0, 3), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));

    std::vector<std::shared_ptr<Particle>> p_list_included{p0, p1, p2, p3};
    std::vector<std::shared_ptr<Particle>> p_list_excluded{p1, p2, p3};

    p0->updateAcceleration(p_list_included, 0.0);
    Eigen::Vector3d expected_acceleration_included = p0->getAcceleration();

    p0->setAcceleration({0, 0, 0});
    p0->updateAcceleration(p_list_excluded, 0.0);
    Eigen::Vector3d expected_acceleration_excluded = p0->getAcceleration();

    bool acceleration_correctness = expected_acceleration_included.isApprox(expected_acceleration_excluded, 1e-9);
    REQUIRE(acceleration_correctness);
}

TEST_CASE("Gravitational force due to two opposite particles", "[Gravity]")
{
    // origin particle
    std::shared_ptr<Particle> p0 = std::make_shared<Particle>(10, Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    // two opposite particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>(1, Eigen::Vector3d(1, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>(1, Eigen::Vector3d(-1, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    // list of particles
    std::vector<std::shared_ptr<Particle>> p_list{p0, p1, p2};
    // update acceleration of p0
    p0->updateAcceleration(p_list, 0.0);
    // check if acceleration is zero
    Eigen::Vector3d expected_acceleration{0, 0, 0};
    bool acceleration_correctness = p0->getAcceleration().isApprox(expected_acceleration, 0.01);
    REQUIRE(acceleration_correctness);
}

TEST_CASE("a simple Solar System (the Sun and the Earth only)", "[the Solar System]")
{
    std::cout << "a simple Solar System" << std::endl;
    std::vector<std::shared_ptr<Particle>> initial_solar_system;
    // add the Sun
    std::shared_ptr<Particle> sun = std::make_shared<Particle>(1, Eigen::Vector3d(1, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    initial_solar_system.push_back(sun);
    // add the Earth
    double r = 1;
    double theta = 0;
    std::shared_ptr<Particle> earth = std::make_shared<Particle>(
        1. / 332946.038,
        Eigen::Vector3d(r * sin(theta), r * cos(theta), 0),
        Eigen::Vector3d(-cos(theta) / sqrt(r), sin(theta) / sqrt(r), 0), Eigen::Vector3d(0, 0, 0));
    initial_solar_system.push_back(earth);
    // one year after
    double dt(0.0001);
    double total_time(2 * M_PI);
    int n_steps(total_time / dt);
    // update the solar system
    std::vector<std::shared_ptr<Particle>> updated_solar_system = update_Solar_System(initial_solar_system, dt, total_time, n_steps);
    // After a time of 2Pi, the Earth should be back to its initial position
    Eigen::Vector3d expected_position_final{r * sin(theta), r * cos(theta), 0};
    bool earth_back_to_initial_pos = updated_solar_system[1]->getPosition().isApprox(expected_position_final, 0.01);
    //
    std::cout << "Earth position: " << updated_solar_system[1]->getPosition() << std::endl;
    std::cout << "Expected position: " << expected_position_final << std::endl;
    REQUIRE(earth_back_to_initial_pos);
}
