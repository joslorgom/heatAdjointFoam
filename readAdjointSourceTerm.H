	volScalarField g
	(
	    IOobject
	    (
	    	"g",
	    	runTime.timeName(),
	    	mesh,
	    	IOobject::MUST_READ,
	    	IOobject::NO_WRITE
	    ),
	    mesh
	);
