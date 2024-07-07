# class-BigDecimal

In this repository, the BigDecimal class is implemented using C++.

In addition, I have performed 10000 random digit quadrature calculations.

As a background, the calculation results may be rounded in single-precision and double-precision calculations.

BigDecimal keeps more digits of real numbers internally and realizes error-free computation in four arithmetic operations.

Specifically, the implementation is divided into a sign part, an exponent part, and a mantissa part, which are retained inside the computer.

IEEE notation is probably the most well-known method of maintaining these parts.

One of the factors that cause the accuracy to be terminated is the problem of not being able to retain the mantissa.

In contrast, BigDecimal keeps mantissa in "vector" form with integer elements, and performs four arithmetic operations by taking into account sign and exponent.

In this case, operator overloading is implemented for each operator, which enables computation between BigDicimals.

Note that these operators focus only on the accuracy of the calculation results and do not make the best use of the computer architecture, since they are held in integer type internally.
