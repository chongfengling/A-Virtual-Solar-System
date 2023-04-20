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
    std::vector<std::shared_ptr<Particle>> SS_initial;
    // add the Sun
    std::shared_ptr<Particle> sun = std::make_shared<Particle>(1, Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    SS_initial.push_back(sun);
    // add the Earth
    double r = 1;
    double theta = 2023;
    std::shared_ptr<Particle> earth = std::make_shared<Particle>(
        1. / 332946.038,
        Eigen::Vector3d(r * sin(theta), r * cos(theta), 0),
        Eigen::Vector3d(-cos(theta) / sqrt(r), sin(theta) / sqrt(r), 0), Eigen::Vector3d(0, 0, 0));
    SS_initial.push_back(earth);
    // one year after
    double dt(0.0001);
    double total_time(2 * M_PI);
    int n_steps(total_time / dt);
    // update the solar system
    std::vector<std::shared_ptr<Particle>> SS_updated = update_Solar_System(SS_initial, dt, total_time, n_steps);
    // After a time of 2Pi, the Earth should be back to its initial position
    Eigen::Vector3d expected_position_final{r * sin(theta), r * cos(theta), 0};
    bool earth_back_to_initial_pos = SS_updated[1]->getPosition().isApprox(expected_position_final, 0.01);
    //
    std::cout << "Earth position sun: " << SS_updated[0]->getPosition() << std::endl;
    std::cout << "Earth position earth: " << SS_updated[1]->getPosition() << std::endl;
    std::cout << "Expected position: " << expected_position_final << std::endl;
    REQUIRE(earth_back_to_initial_pos);
}

TEST_CASE("Unit Test: calKineticEnergy", "[calKineticEnergy]")
{
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>(2, Eigen::Vector3d(1, 2, 3), Eigen::Vector3d(4, 5, 6), Eigen::Vector3d(7, 8, 9));
    double expected_kinetic_energy = 0.5 * 2 * (4 * 4 + 5 * 5 + 6 * 6);

    REQUIRE_THAT(p1->calKineticEnergy(), WithinRel(expected_kinetic_energy, 1e-6));
}

TEST_CASE("Unit Test: calPotentialEnergy", "[calPotentialEnergy]")
{
    // origin particle
    std::shared_ptr<Particle> p0 = std::make_shared<Particle>(3, Eigen::Vector3d(1, 2, 3), Eigen::Vector3d(4, 5, 6), Eigen::Vector3d(7, 8, 9));
    // two added particles
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>(4, Eigen::Vector3d(11, 12, 13), Eigen::Vector3d(14, 15, 16), Eigen::Vector3d(17, 18, 19));
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>(5, Eigen::Vector3d(23, 22, 21), Eigen::Vector3d(24, 25, 26), Eigen::Vector3d(27, 28, 29));
    // list of particles
    std::vector<std::shared_ptr<Particle>> p_list_with_p0{p0, p1, p2};
    std::vector<std::shared_ptr<Particle>> p_list_without_p0{p1, p2};
    // Define the expected value for the potential energy
    double d1 = sqrt((11 - 1) * (11 - 1) + (12 - 2) * (12 - 2) + (13 - 3) * (13 - 3));
    double d2 = sqrt((23 - 1) * (23 - 1) + (22 - 2) * (22 - 2) + (21 - 3) * (21 - 3));
    double expected_potential_energy = (-0.5 * 3 * 4 / d1) + (-0.5 * 3 * 5 / d2);
    // Check if the potential energy is correct
    REQUIRE_THAT(p0->calPotentialEnergy(p_list_with_p0), WithinRel(expected_potential_energy, 1e-6));
    REQUIRE_THAT(p0->calPotentialEnergy(p_list_without_p0), WithinRel(expected_potential_energy, 1e-6));
}

TEST_CASE("Unit Test: calTotalEnergy", "[calTotalEnergy]")
{
    // three particles
    std::shared_ptr<Particle> p0 = std::make_shared<Particle>(10, Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0), Eigen::Vector3d(0, 0, 0));
    std::shared_ptr<Particle> p1 = std::make_shared<Particle>(1, Eigen::Vector3d(1, 2, 3), Eigen::Vector3d(4, 5, 6), Eigen::Vector3d(7, 8, 9));
    std::shared_ptr<Particle> p2 = std::make_shared<Particle>(2, Eigen::Vector3d(13, 12, 11), Eigen::Vector3d(14, 15, 16), Eigen::Vector3d(17, 18, 19));

    // Expected value for the kinetic energy
    double ke_0 = 0.5 * 10 * (0 * 0 + 0 * 0 + 0 * 0);
    double ke_1 = 0.5 * 1 * (4 * 4 + 5 * 5 + 6 * 6);
    double ke_2 = 0.5 * 2 * (14 * 14 + 15 * 15 + 16 * 16);
    // Expected value for the potential energy
    double d_01 = sqrt((1 - 0) * (1 - 0) + (2 - 0) * (2 - 0) + (3 - 0) * (3 - 0));
    double d_02 = sqrt((11 - 0) * (11 - 0) + (12 - 0) * (12 - 0) + (13 - 0) * (13 - 0));
    double d_12 = sqrt((13 - 1) * (13 - 1) + (12 - 2) * (12 - 2) + (11 - 3) * (11 - 3));
    double pe_0 = (-0.5 * 10 * 1 / d_01) + (-0.5 * 10 * 2 / d_02);
    double pe_1 = (-0.5 * 1 * 10 / d_01) + (-0.5 * 1 * 2 / d_12);
    double pe_2 = (-0.5 * 2 * 10 / d_02) + (-0.5 * 2 * 1 / d_12);
    // Expected value for the total energy
    double expected_total_energy = ke_0 + ke_1 + ke_2 + pe_0 + pe_1 + pe_2;
    // Check if the total energy is correct
    std::vector<std::shared_ptr<Particle>> p_list{p0, p1, p2};
    REQUIRE_THAT(calTotalEnergy(p_list), WithinRel(expected_total_energy, 1e-6));
}