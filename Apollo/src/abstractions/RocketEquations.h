#pragma once

#include <iostream>
#include <vector>
#include <cmath>
#include <utility>

class RocketEquations
{
private:
    
    const double tolerance = 1e-3; // Tolerância para o método de Newton-Raphson
    const int maxIterations = 6000; // Número máximo de iterações para o método de Newton-Raphson
    const double waterDensity = 0.997;
    const double gravity = 980.0;
    const double pi = 3.14159265359;
    double RocketLiquidVolume_Derivative(double height)
    {
        return 924.572 -
            3156.35 * 0.5 * (1 / sqrt(0.3944 + 0.0339556 * height)) * 0.0339556 -
            1795.88 * 4.5 * pow((0.3944 + 0.0339556 * height), 3.5) * 0.0339556 +
            91.4699 +
            4 * 0.00139864 * pow((11.6152 + height), 3) +
            8 * 5.28948e-10 * pow((11.6152 + height), 7);
    }
public:
    int erros = 0;
	double RocketLiquidVolume_PerHeight(double height)
	{
		return 9177.3580153 + 17629.7 * (
			(-3.5151 * pow((0.3944 + 0.0339556 * height), 1.5)) -
			(0.545455 * pow((0.3944 + 0.0339556 * height), 5.5)) +
			(0.052444 * height) +
			(0.0025942 * pow((11.6152 + height), 2)) +
			(1.58669e-8 * pow((11.6152 + height), 5)) +
			(3.33369e-15 * pow((11.6152 + height), 9)));		
	}
    double RocketLiquidHeight_PerVolume(double volume)
    {
        double height = pow((volume + 1.4) / 0.4, 1.0 / 3.0) - 1.4; // Chute inicial
        int iterations = 0;

        while (iterations < maxIterations)
        {
            double f_value = RocketLiquidVolume_PerHeight(height) - volume;
            double f_derivative = RocketLiquidVolume_Derivative(height);

            // Checa se a derivada é muito pequena para evitar divisão por zero
            if (fabs(f_derivative) < tolerance)
            {
                std::cerr << "Derivada muito pequena, não é possível continuar o método de Newton-Raphson." << std::endl;
                break;
            }

            double new_height = height - f_value / f_derivative;

            // Verifica se a diferença é menor que a tolerância
            if (fabs(new_height - height) < tolerance)
                return new_height;

            height = new_height;
            iterations++;
        }

        std::cerr << "O método de Newton-Raphson não convergiu após " << maxIterations << " iterações." << std::endl;
        erros++;
        return height;
    }

    std::tuple<double, double, double> RocketVelocityEquation(double Pressure, double volume, double rocketMass, double time, double nRT)
    {
        double waterVelocity = sqrt((1250 * ((Pressure - 1013250.0) + (waterDensity * gravity * RocketLiquidHeight_PerVolume(volume)))) / (624 * waterDensity));
        double waterForce = waterVelocity * (waterVelocity * pi * waterDensity);
        double rocketMassWithFuel = volume * waterDensity + rocketMass;
        double rocketForce = waterForce;
        double rocketAcelleration = rocketForce / rocketMassWithFuel;
        double rocketDeltaV = rocketAcelleration * time;
        double newVolume = volume - (waterVelocity * time) * pi;
        double newPressure = nRT / (2000.0 - newVolume);
        return std::tuple<double, double, double>(rocketDeltaV, newVolume, newPressure);
    }

};