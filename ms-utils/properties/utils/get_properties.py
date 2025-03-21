#!/usr/bin/env python

"""
Purpose: Gets fluid property tables that can be loaded into FluidProperties.cpp
Author: Sam Evani - Axiom Space - 2024
Copyright 2025 United States Government as represented by the Administrator of the National Aeronautics and Space Administration.  All Rights Reserved.
"""

# Standard imports
#-

# Additional imports
import CoolProp.CoolProp as CP
import numpy as np

# Local imports
#-

def get_density_table(fluid, temperature_list, pressure_list):
    """
    Generates and prints a density table for a given fluid based on lists of temperatures and pressures.
    This function calculates the density of the specified fluid at various temperatures and pressures
    using the CoolProp library. The results are printed in a format suitable for GUNNS FluidProperties.cpp.
    Args:
        fluid (str): The name of the fluid for which to calculate densities.
        temperature_list (list of float): A list of temperatures (in Kelvin) at which to calculate densities.
        pressure_list (list of float): A list of pressures (in kPa) at which to calculate densities.
    Returns:
        None
    """

    print(f"Density Table for {fluid}")

    # Get density
    density_table = []
    for temp in temperature_list:
        row = [0.0]  # Start each row with 0.0
        for pressure in pressure_list:
            density = CP.PropsSI('D', 'T', temp, 'P', pressure * 1e3, fluid)
            row.append(density)
        density_table.append(row)

    # Print properties in format for GUNNS FluidProperties.cpp
    print("            {", end="")
    for i, row in enumerate(density_table):
        formatted_row = ", ".join([f"{value:8.3f}" for value in row])
        if i == 0:
            print(f"{formatted_row},  // T = {temperature_list[i]:.3f} K")
        else:
            print(f"             {formatted_row},  // T = {temperature_list[i]:.3f} K")
    print("            };")
    print()

# Main
if __name__ =="__main__":

    # Nitrogen
    nitrogen_temp_list =  [160.0, 213.636, 267.273, 320.909, 374.545, 428.182, 481.818, 535.455, 589.091, 642.727, 696.364, 750.0]
    nitrogen_press_list = [4545.45, 9090.9,  13636.4, 18181.8, 22727.3, 27272.7, 31818.2, 36363.6, 40909.1, 45454.5, 50000.0, 54545.5, 59090.9]
    get_density_table('N2', nitrogen_temp_list, nitrogen_press_list)

    # Oxygen
    oxygen_temp_list = [160.0, 213.636, 267.273, 320.909, 374.545, 428.182, 481.818, 535.455, 589.091, 642.727, 696.364, 750.0]
    oxygen_press_list = [4545.45, 9090.9,  13636.4, 18181.8, 22727.3, 27272.7, 31818.2, 36363.6, 40909.1, 45454.5, 50000.0, 54545.5, 59090.9]
    get_density_table('O2', oxygen_temp_list, oxygen_press_list)

    # Methane
    methane_temp_list = np.linspace(91.8532, 625.0, 12).tolist()
    print("CH4 Temperature List", [round(temp, 3) for temp in methane_temp_list])
    methane_press_list = np.linspace(0, 59090.9, 14).tolist()
    methane_press_list = methane_press_list[1:]
    print("CH4 Pressure List", [round(press, 3) for press in methane_press_list])
    get_density_table('CH4', methane_temp_list, methane_press_list)

    # Hydrogen
    hydrogen_temp_list = [64.0, 142.0, 220.0, 298.0, 376.0, 454.0, 532.0, 610.0, 688.0, 766.0, 844.0,  922.0, 1000.0]
    hydrogen_press_list = [6666.67, 13333.3, 20000.0, 26666.7, 33333.3, 40000.0, 46666.7, 53333.3, 60000.0, 66666.7, 73333.3, 80000.0]
    get_density_table('H2', hydrogen_temp_list, hydrogen_press_list)





