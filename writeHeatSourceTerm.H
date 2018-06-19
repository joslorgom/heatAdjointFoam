pL2 = 0;
ta = 0;

for (label i = 1; i <= nTimes; i++)
{
	runTime.setTime(Times[nTimes - i + 1], nTimes - i + 1);

	volScalarField lambda
	(
	    IOobject
	    (
		"lambda",
		runTime.timeName(),
		mesh,
		IOobject::MUST_READ,
		IOobject::NO_WRITE
	    ),
	    mesh
	);

	runTime.setTime(Times[i], i);

	volScalarField u
	(
	    IOobject
	    (
		"u",
		runTime.timeName(),
		mesh,
		IOobject::READ_IF_PRESENT,
		IOobject::NO_WRITE
	    ),
	    u0
	);

	// Compute direction
	p = (lambda + beta1*u)*omega;
	// Update control
	u = u - eps*p;

	// Compute L2 norm of control update
	pL2t = gSum(volField*p.internalField()*p.internalField());

	t = runTime.value();
	pL2 = pL2 + 0.5*(pL2t + pL2ta)*(t - ta);

        pL2ta = pL2t;
        ta = t;
	/******************************************************************/

	u.write();
}

pL2 = ::sqrt(pL2);
