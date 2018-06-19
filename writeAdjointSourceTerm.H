// Reverse source term in the adjoint equation
instantList Times = runTime.times();
scalar nTimes = Times.size() - 1;

for (label i = 1; i <= nTimes; i++)
{
	// Read state variable backwards in time from nTimes to 1

	runTime.setTime(Times[nTimes - i + 1], nTimes - i + 1);

	volScalarField yr
	(
	    IOobject
	    (
		"y",
		runTime.timeName(),
		mesh,
		IOobject::MUST_READ,
		IOobject::NO_WRITE
	    ),
	    mesh
	);

	// Write source term for the adjoint equation

	runTime.setTime(Times[i], i);

	volScalarField gw
	(
	    IOobject
	    (
		"g",
		runTime.timeName(),
		mesh,
		IOobject::NO_READ,
		IOobject::NO_WRITE
	    ),
	    beta2*(yr - yd)
	);

	gw.write();
}
