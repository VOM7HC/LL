#ifndef _BABL_POLYNOMIAL_HPP
#define _BABL_POLYNOMIAL_HPP

#define BABL_POLYNOMIAL_MIN_DEGREE 0
#define BABL_POLYNOMIAL_MAX_DEGREE 10

#define BABL_POLYNOMIAL_MIN_SCALE 1
#define BABL_POLYNOMIAL_MAX_SCALE 2

typedef struct BablPolynomial BablPolynomial;

typedef double (*BablPolynomialEvalFunc)(const BablPolynomial* poly, double x);

struct BablPolynomial {
    BablPolynomialEvalFunc eval;
    int degree;
    int scale;
    double coeff[BABL_POLYNOMIAL_MAX_DEGREE + 1];
};

static inline double babl_polynomial_eval(const BablPolynomial* poly, double x)
{
    return poly->eval(poly, x);
}

void babl_polynomial_approximate_gamma(BablPolynomial* poly, double gamma, double x0, double x1, int degree, int scale);

#endif
