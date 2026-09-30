#pragma once

double setDegreesToRadians(double theta);

std::vector<double> solveQuadratic(double a, double b, double c);

double getTimeOfFlight(double verticalComponent, double yInitial, double yFinal);

double resolveComponents(double magnitude, double theta);

double getHorizontalDiaplacement(double horizontalComponent, double timeOfFlight);

double getLandingVelocity(double initialVx, double initialVy, double timeOfFlight);

double getSpeed(double vX, double vY);

double getMaximumHeight(double initialVy, double launchGeight);


