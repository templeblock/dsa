/*
 * MathVector.h
 *
 *  Created on: 05.04.2012
 *      Author: jung
 */

#ifndef MATHVECTOR_H_
#define MATHVECTOR_H_

#include <cmath>
#include <fstream>
#include <vector>

namespace dsa {

template <class T>
class MathVector : public std::vector<T> {

	/* Bestimme den Betrag (bzw. nur die Quadratsumme).
	 * Aktuell benötigen wir das nur für den Vergleich der Länge zweier Vektoren.
	 * Da die Wurzelfunktion streng monoton steigend ist, spielt sie für den Vergleich
	 * keine Rolle.
	*/
	T cmpAbsolute(const MathVector &mathVector) const {
		assert(mathVector.size() > 0);
		T squareSum = mathVector[0]*mathVector[0];
		for(size_t i = 1; i < mathVector.size(); i++)
			squareSum += mathVector[i]*mathVector[i];
		return(squareSum);
	}

public:
	MathVector(const size_t elements=3) : std::vector<T>(elements) {
	}

	MathVector(std::vector<T> &elementVector) : std::vector<T>(elementVector) {
	}

	// copy constructor
	MathVector(const MathVector& rhs) : std::vector<T>(rhs) {
	}

	~MathVector() {
	}

	// Zuweisung
	MathVector operator=(MathVector const &rhs) const {
		MathVector tmp(rhs); // Kopie des rechten Operanden
		return(tmp);
	}

	// Zuweisung anhand von Vector
	MathVector operator=(std::vector<T> const &elementVector) const {
		MathVector tmp(elementVector);
		return(tmp);
	}

	// Prüfung auf (Un)Gleichheit
	bool operator==(MathVector const &rhs) const {
		if(this->size() == rhs.size()) {
			for(size_t i = 0; i < this->size(); i++)
				if((*this)[i] != rhs[i])
					return(false);	// one element not equal
			return(true);	// none unequal
		} else return(false);
	}
	bool operator!=(MathVector const &rhs) const {
		return(!(*this == rhs));
	}

	// Arithmetische Negation
	MathVector operator-() const {
		MathVector tmp(*this);
		for(size_t i = 0; i < tmp.size(); i++)
			tmp[i] = -(tmp[i]);
		return(tmp);
	}
	// Addition
	MathVector operator+(MathVector const &rhs) const {
		assert(this->size() == rhs.size());
		MathVector tmp(*this);
		for(size_t i = 0; i < tmp.size(); i++)
			tmp[i] = tmp[i] + rhs[i];
		return(tmp);
	}
	// Subtraktion
	MathVector operator-(MathVector const &rhs) const {
		assert(this->size() == rhs.size());
		MathVector tmp(*this);
		for(size_t i = 0; i < tmp.size(); i++)
			tmp[i] = tmp[i] - rhs[i];
		return(tmp);
	}

	// Vektormuliplikation (Kreuzprodukt im R_3)
	MathVector operator*(MathVector const &rhs) const {
		assert(this->size() == 3 && rhs.size() == 3);	// sonst ist das Ergebnis kein Vector!
		MathVector tmp(3);
		tmp[0] = (*this)[1]*rhs[2] - (*this)[2]*rhs[1];
		tmp[1] = (*this)[2]*rhs[0] - (*this)[0]*rhs[2];
		tmp[2] = (*this)[0]*rhs[1] - (*this)[1]*rhs[0];
		return(tmp);
	}

	// Skalare Multiplikation
	MathVector operator*(int lhs) const {
		MathVector tmp(*this);
		for(size_t i = 0; i < tmp.size(); i++)
			tmp[i] = lhs * tmp[i];
		return(tmp);
	}
	MathVector operator*(float lhs) const {
		MathVector tmp(*this);
		for(size_t i = 0; i < tmp.size(); i++)
			tmp[i] = lhs * tmp[i];
		return(tmp);
	}
	MathVector operator*(double lhs) const {
		MathVector tmp(*this);
		for(size_t i = 0; i < tmp.size(); i++)
			tmp[i] = lhs * tmp[i];
		return(tmp);
	}

	// Vergleich der Vektorbeträge
	bool operator<(MathVector const &rhs) const {
		return(cmpAbsolute(*this) < cmpAbsolute(rhs));
	}
	// Vergleich der Vektorbeträge
	bool operator<=(MathVector const &rhs) const {
		return(cmpAbsolute(*this) <= cmpAbsolute(rhs));
	}
	// Vergleich der Vektorbeträge
	bool operator>(MathVector const &rhs) const {
		return(cmpAbsolute(*this) > cmpAbsolute(rhs));
	}
	// Vergleich der Vektorbeträge
	bool operator>=(MathVector const &rhs) const {
		return(cmpAbsolute(*this) >= cmpAbsolute(rhs));
	}

    friend std::ostream& operator<<(std::ostream &lhs, const MathVector &rhs) {
    	lhs << "(";
    	for(size_t i = 0; i < rhs.size()-1; i++) {
    		lhs << rhs[i] << ",";
    	}
    	lhs << rhs[rhs.size()-1] << ")";
    	return(lhs);
    }
};

}
#endif /* MATHVECTOR_H_ */
