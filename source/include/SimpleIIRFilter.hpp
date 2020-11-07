/*
 * This file implements an average filter with a configurable depth
 *
 * y[n] = y[n-1] + ( (x[n] - y[n-1]) /  COFF )
 * = y[n-1] + x[n]/COEF - y[n-1]/COEF
 * = y[n-1] + 1/COEF*x[n] - 1/COEF*y[n-1]
 * = 1/COEF*x[n] + (1 - 1/COEF)y[n-1]
 * = 1/COEF*x[n] - (1/COEF - 1)y[n-1]
 * = 1/COEF( x[n] - (1 - COEF)y[n-1] )
 *
 * a0 = COEF
 * b0 = 1
 * a1 = 1 - COEF
 */
#define FIXPOINT_DOT 13

// every 16ms
#define SAMPLE_FREQUENCY 62
#define CUT_OFF_FREQUENCY 1

#define COEFFICIENT_CALC (SAMPLE_FREQUENCY / (2 * M_PI * CUT_OFF_FREQUENCY))
#define COEFFICIENT 50

template <class T, class I>
class SimpleIIRFilter {
private:
	I lastResultFixPoint;

public:
	SimpleIIRFilter() : lastResultFixPoint(0) {}

	T get() {
		const T result = lastResultFixPoint; //>> FIXPOINT_DOT;
		return result;
	}

	void update(T input) {
		const I valueFixPoint = (I)(input); //<< FIXPOINT_DOT;

//		const I diffFixPoint =
//			(valueFixPoint - lastResultFixPoint) / COEFFICIENT;
//		const I resultFixPoint = lastResultFixPoint + diffFixPoint;
		lastResultFixPoint = valueFixPoint; //resultFixPoint;
	}
};

