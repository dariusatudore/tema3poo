#include "expo.h"

int main() {
	ifstream in("carsIn.txt");
	ofstream out("carsOut.txt");
	expo<supersport> Super;
	expo<coupe> Coupe;
	expo<cabrio> decap;
	expo<hothatch> Hothatch;
	int n;
	in >> n;
	for (int i = 0; i < n; i++) {
		string s;
		in >> s;
		if (s == "Coupe") {
			coupe c;
			in >> c;
			Coupe += c;
		}
		else if (s == "Cabrio") {
			cabrio c;
			in >> c;
			decap += c;
		}
		else if (s == "Hot-Hatch") {
			hothatch h;
			in >> h;
			Hothatch += h;
		}
		else {
			supersport sp;
			in >> sp;
			Super += sp;
		}
	}
	int v;
	in >> v;
	while (v != 0) {
		supersport sp;
		in >> sp;
		try {
			Super -= sp;
		}
		catch (wrongPrice&) {}
		catch (notInStock&) {
			sp.print(cout);
			cout << "\n";
		};
		v--;
	}
	Coupe.print(out);
	Super.print(out);
	decap.print(out);
	Hothatch.print(out);
	return 0;
}