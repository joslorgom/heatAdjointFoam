//Jt = gSum(volField*( alpha * Foam::pow(y.internalField() - yd.internalField(), 2) + beta * omega.internalField() * Foam::pow(u.internalField(), 2) ) )/2;

Jyt = gSum(volField*( beta2 * Foam::pow(y.internalField() - yd.internalField(), 2) ) )/2;
Jut = gSum(volField*( beta1 * omega.internalField() * Foam::pow(u.internalField(), 2) ) )/2;
