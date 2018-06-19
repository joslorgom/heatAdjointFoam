// Read control if file u is present
// Otherwise (first iteration) u = u0

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
