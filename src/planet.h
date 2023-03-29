/* date = March 28th 2023 9:52 pm */

#ifndef PLANET_H
#define PLANET_H

#include "types.h"

struct Orbit {
    f64 semi_major_axis;
    f64 eccentricity;
    f64 inclination;
    f64 mean_longitude;
    f64 perihelion;
    f64 ascending_node;
};

struct Planet {
    const char *name;
    Orbit state;
    Orbit rate;
    f64 epoch;
};

enum PlanetName {
    PLANET_MERCURY = 0,
    PLANET_VENUS,
    PLANET_MARS,
    PLANET_JUPITER,
    PLANET_SATURN,
    PLANET_URANUS,
    PLANET_NEPTUNE,
    PLANET_COUNT
};

Planet *planets();
Planet *planet_by_name(PlanetName name);

#endif //PLANET_H
