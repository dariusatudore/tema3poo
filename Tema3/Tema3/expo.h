#include <iostream>
#include <vector>
#include <string>
#include <exception>
#include <fstream>
using namespace std;

class notInStock : public exception {
public:
	notInStock() {
		cout << "We don't have that car in stock!\n";
	}
};

class wrongPrice : public exception {
public:
	wrongPrice(string s) {
		cout << "You didn't pay the correct price for: " << s << "\n";
	}
};

class car {
protected:
	string brand, model;
public:
	car() {};
	car(string b, string m) : brand(b), model(m) {}
	car(const car& M) = default;
	virtual ~car() = default;
	car& operator= (const car&);
	friend istream& operator>> (istream&, car&);
	virtual void print(ostream&);
	string getBrand() const {
		return brand;
	}
	string getModel() const {
		return model;
	}
};

car& car::operator= (const car& c) {
	brand = c.brand;
	model = c.model;
	return *this;
}

istream& operator>> (istream& in, car& c) {
	in >> c.brand >> c.model;
	return in;
}

void car::print(ostream& out) {
	string n = typeid(*this).name();
	while (n[0] >= '0' && n[0] <= '9') {
		n.erase(n.begin());
	}
	out << n << ": " << brand << " " << model;
}



class coupe : public car {
public:
	coupe() :car() {}
	coupe(string b, string m) : car(b, m) {}
	coupe(const coupe& C) : car(C.brand, C.model) {}
	~coupe() override = default;
	coupe& operator= (const coupe&);
	friend istream& operator>> (istream&, coupe&);
	void print(ostream&) override;
};

coupe& coupe::operator= (const coupe& C) {
	brand = C.brand;
	model = C.model;
	return *this;
}

istream& operator>> (istream& in, coupe& C) {
	car& M = C;
	return in >> M;
}

void coupe::print(ostream& out) {
	car::print(out);
}




class supersport : public coupe {
	int pret;
public:
	supersport() :coupe(), pret(0) {}
	supersport(string b, string m, int p) : coupe(b, m), pret(p) {}
	supersport(const supersport& S) : coupe(S.brand, S.model), pret(S.pret) {}
	~supersport() override = default;
	supersport& operator= (const supersport&);
	friend istream& operator>> (istream&, supersport&);
	void print(ostream&) override;
	int getPret() {
		return pret;
	}
	void setPret(int p) {
		pret = p;
	}
};


supersport& supersport::operator= (const supersport& S) {
	brand = S.brand;
	model = S.model;
	pret = S.pret;
	return *this;
}

istream& operator>> (istream& in, supersport& S) {
	coupe& C = S;
	in >> C;
	int p;
	in >> p;
	S.setPret(p);
	return in;
}

void supersport::print(ostream& out) {
	coupe::print(out);
}


class cabrio : public coupe {
public:
	cabrio() :coupe() {}
	cabrio(string b, string m) : coupe(b, m) {}
	cabrio(const cabrio& C) : coupe(C.brand, C.model) {}
	~cabrio() override = default;
	cabrio& operator= (const cabrio&);
	friend istream& operator>> (istream&, cabrio&);
	//friend ostream& operator<< ( ostream&, const Cabrio&);
	void print(ostream&) override;
};

cabrio& cabrio::operator= (const cabrio& C) {
	brand = C.brand;
	model = C.model;
	return *this;
}
istream& operator>> (istream& in, cabrio& Ca) {
	coupe& Co = Ca;
	return in >> Co;
}
void cabrio::print(ostream& out) {
	coupe::print(out);
}



class hothatch : public car {
public:
	hothatch() :car() {}
	hothatch(string b, string m) : car(b, m) {}
	hothatch(const hothatch& C) : car(C.brand, C.model) {}
	~hothatch() override = default;
	hothatch& operator= (const hothatch&);
	friend istream& operator>> (istream&, hothatch&);
	void print(ostream&) override;
};

hothatch& hothatch::operator= (const hothatch& H) {
	brand = H.brand;
	model = H.model;
	return *this;
}
istream& operator>> (istream& in, hothatch& H) {
	car& M = H;
	return in >> M;
}
void hothatch::print(ostream& out) {
	car::print(out);
}


template < class T>
class expo {
	unsigned int nr;
	vector <pair<T*, int>> collection;
public:
	expo() :nr(0) {}
	explicit expo(vector<T> v) :nr(v.size()) {
		for (auto m : v)
			collection.push_back(new T(m));
	}
	~expo() {
		for (auto m : collection)
			delete(m.first);
		collection.clear();
	}
	void operator+= (T& m) {
		collection.emplace_back(new T(m), -1);
		nr++;
	}
	vector <pair<T*, int>> getColecttion() {
		return collection;
	}
	unsigned int getNr() {
		return nr;
	}
	void print(ostream& out);
};

template <class T>
void expo<T>::print(ostream& out) {
	for (auto x : collection) {
		x.first->print(out);
		out << " " << x.second << "\n";
	}
}


template <>
class expo <supersport> {
	unsigned int inStock, nrSold;
	vector <pair <supersport*, int> > sold, exposed;
public:
	expo() : inStock(0), nrSold(0) {}
	~expo() {
		for (auto m : exposed) {
			delete(m.first);
		}
		exposed.clear();
		for (auto m : sold) {
			delete(m.first);
		}
		sold.clear();
	}
	void operator += (supersport& s);
	void operator -= (supersport& s);
	vector <pair<supersport*, int>> getExpuse() {
		return exposed;
	}
	vector <pair <supersport*, int>> getVandute() {
		return sold;
	}
	unsigned int getNr_expuse() {
		return inStock;
	}
	unsigned int getNr_vandute() {
		return nrSold;
	}
	void print(ostream& out);
};

void expo<supersport>::operator += (supersport& s) {
	exposed.emplace_back(new supersport(s), -1);
	inStock++;
}
void expo<supersport>::operator -= (supersport& s) {
	if (inStock == 0)
		throw notInStock();
	for (int i = 0; i < inStock; i++) {
		if (exposed[i].first->getBrand() == s.getBrand() && exposed[i].first->getModel() == s.getModel()) {
			if (s.getPret() != exposed[i].first->getPret()) {
				throw wrongPrice(s.getBrand() + " " + s.getModel());
			}
			sold.emplace_back(new supersport(s), s.getPret());
			nrSold++;
			delete(exposed[i].first);
			if (i != inStock - 1) {
				exposed[i].first = exposed[inStock - 1].first;
				exposed[inStock - 1].first = nullptr;
			}
			exposed.pop_back();
			break;
		}
		if (i == inStock - 1) {
			throw notInStock();
		}
	}
	inStock--;
}

void expo<supersport>::print(ostream& out) {
	for (auto x : exposed) {
		x.first->print(out);
		out << " " << x.second << "\n";
	}
	for (auto x : sold) {
		x.first->print(out);
		out << " " << x.second << "\n";
	}
}