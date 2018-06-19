Info << "Reading physicalProperties\n" << endl;
IOdictionary physicalProperties
(
    IOobject
    (
	"physicalProperties",
	runTime.constant(),
	mesh,
	IOobject::MUST_READ,
	IOobject::NO_WRITE
    )
);

dimensionedScalar k(physicalProperties.lookup("k"));
dimensionedScalar c(physicalProperties.lookup("c"));
dimensionedScalar eps(physicalProperties.lookup("eps"));

dimensionedScalar beta1(physicalProperties.lookup("beta1"));
dimensionedScalar beta2(physicalProperties.lookup("beta2"));
dimensionedScalar beta3(physicalProperties.lookup("beta3"));

/***************************************************************************/

Info << "Reading numericalParameters\n" << endl;
IOdictionary numericalParameters
(
    IOobject
    (
	"numericalParameters",
	runTime.constant(),
	mesh,
	IOobject::MUST_READ,
	IOobject::NO_WRITE
    )
);

scalar maxIter(numericalParameters.lookupOrDefault<scalar>("maxIter", 100));
scalar tol(numericalParameters.lookupOrDefault<scalar>("tol", 1e-5));

/****************************************************************************/

Info << "Reading target state\n" << endl;
volScalarField yd
(
    IOobject
    (
	"yd",
	runTime.constant(),
	mesh,
	IOobject::MUST_READ,
	IOobject::NO_WRITE
    ),
    mesh
);

Info << "Reading target state at time T\n" << endl;
volScalarField Yd
(
    IOobject
    (
	"Yd",
	runTime.constant(),
	mesh,
	IOobject::MUST_READ,
	IOobject::NO_WRITE
    ),
    mesh
);

Info << "Reading indicator function\n" << endl;
volScalarField omega
(
    IOobject
    (
	"omega",
	runTime.constant(),
	mesh,
	IOobject::MUST_READ,
	IOobject::NO_WRITE
    ),
    mesh
);

/****************************************************************************/

Info << "Reading state variable y\n" << endl;
volScalarField y
(
    IOobject
    (
	"y",
	runTime.timeName(),
	mesh,
	IOobject::MUST_READ,
	IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField y0 = y;

Info << "Reading control variable u\n" << endl;
volScalarField u
(
    IOobject
    (
	"u",
	runTime.timeName(),
	mesh,
	IOobject::MUST_READ,
	IOobject::NO_WRITE
    ),
    mesh
);

volScalarField u0 = u;

Info << "Reading adjoint field\n" << endl;
volScalarField lambda
(
    IOobject
    (
	"lambda",
	runTime.timeName(),
	mesh,
	IOobject::MUST_READ,
	IOobject::AUTO_WRITE
    ),
    mesh
);

volScalarField lambda0 = lambda;

volScalarField p
(
    IOobject
    (
	"p",
	runTime.timeName(),
	mesh,
	IOobject::NO_READ,
	IOobject::NO_WRITE
    ),
    lambda + beta1*u0
);

scalarField volField = mesh.V();

