/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Application
    heatAdjointFoam

Description

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    #include "createMesh.H"
    #include "createFields.H"
    #include "createScalars.H"

    // Disable solver output
    lduMatrix::debug = 0;
    solverPerformance::debug = 0;

    std::ofstream file("results.csv");
    file << 0 << "," << J << "," << Ju << "," << Jy << "," << JY << "," << 0 << nl;

    // Initialize iteration counter
    label iter = 1;

    while (iter <= maxIter && pL2 >= tol && J <= Ja)
    {
	// Initialize cost function
	Ja = J;
        J = 0;
	Ju = 0;
	Jy = 0;
	JY = 0;
        #include "costFunction.H"

	// Store cost at previous time step
	Jyta = Jyt;
	Juta = Jut;

	// Start time
	t0 = runTime.value();
	// Previous time value
	ta = t0;

	#include "readHeatSourceTerm.H"
	volScalarField ua = u;
	volScalarField un = u;

	// Set initial condition
	y = y0;
	y.write();

	// Solve heat equation forward in time
	while (runTime.loop())
	{
	    #include "readHeatSourceTerm.H"
	    un = u;
	    u = 0.5*(ua + un);
	    ua = un;

	    // Solve state equation
	    solve(c*(fvm::ddt(y)) - fvm::laplacian(k, y) - u*omega);

	    // Integrate cost from time ta to time t
	    t = runTime.value();
	    #include "costFunction.H"
	    Jy = Jy + 0.5*(Jyt + Jyta)*(t - ta);
	    Ju = Ju + 0.5*(Jut + Juta)*(t - ta);

	    Jyta = Jyt;
	    Juta = Jut;
	    ta = t;

	    // Write state at current time
	    y.write();
        }

	// Cost functional
	JY = 0.5 * gSum(volField * (beta3 * Foam::pow(y.internalField() - Yd.internalField(), 2) ) );
	J = Jy + Ju + JY;

	// Write source term for the adjoint equation
        #include "writeAdjointSourceTerm.H"

	// Restart time
        runTime.setTime(Times[1], 1);

	#include "readAdjointSourceTerm.H"
	volScalarField ga = g;
	volScalarField gn = g;

	// Set initial condition for the adjoint problem
	lambda = beta3*(y - Yd)/c;
	//Info << max(Yd).value() << " - " << min(Yd).value() << " - " << max(y).value() << " - " << max(gamma*(y - Yd)/c).value() << " - " << min(gamma*(y - Yd)/c).value() << endl;
	lambda.write();

        // Solve adjoint problem forward in time
        while (runTime.loop())
        {
	    // Read source term for the adjoint equation
	    #include "readAdjointSourceTerm.H"
	    gn = g;
	    g = 0.5*(ga + gn);
	    ga = gn;

            //Info << "Time = " << runTime.timeName() << endl;
            solve(c*(fvm::ddt(lambda)) - fvm::laplacian(k, lambda) - g);

	    // Write adjoint variable at current time
	    lambda.write();
        }

	// Update source term for the heat equation (control variable)
        #include "writeHeatSourceTerm.H"

        // Restart time
        runTime.setTime(Times[1], 1);

	file << iter << "," << J << "," << Ju << "," << Jy << "," << JY << "," << pL2 << nl;

	Info << "Iteration " << iter << " - J " << J << " - Jy " << Jy << "  - Ju " << Ju << " - JY " << JY << " - du_L2 " << pL2 << endl;

        iter++;
    }
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

    Info << nl << endl;
    Info << "ExecutionTime = " << runTime.elapsedCpuTime() << " s"
	<< "  ClockTime = " << runTime.elapsedClockTime() << " s"
        << nl << endl;

    Info<< "\nEnd\n" << endl;
    return 0;
}


// ************************************************************************* //
