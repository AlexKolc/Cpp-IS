#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <libxml++/libxml++.h>

using namespace std;

enum direction {
	NONE,
	FROM,
	TO
};

class ValuteTransfer {
private:
	string fileName;
	string from; //from valute
	string to; //to valute
	int fromCount;
	int toCount;
	double fromCost;
	double toCost;

	xmlpp::DomParser domParser;

	xmlpp::Node* root;

	void valuteSearch() {
		xmlpp::Node::NodeList allValutes = root->get_children();

		for (auto valute: allValutes) {
			auto fields = valute->get_children();

			direction flag = NONE;
			for (auto field: fields) {
				if (field->get_name() == "CharCode") {
					//node to elementNode
					auto element = dynamic_cast<const xmlpp::Element*>(field);
					//check is instance of Element
					if (!element) 
					    continue;
					         
					string charCode = element->get_child_text()->get_content();
					if (charCode == from)
						flag = FROM;
					if (charCode == to)
						flag = TO;
				} 

				if (field->get_name() == "Value") {
					//node to elementNode
					auto element = dynamic_cast<const xmlpp::Element*>(field);
					//check is instance of Element
					if (!element) 
					    continue;

					string value = element->get_child_text()->get_content();
					int pos = value.find(",");
					if (pos != string::npos) {
						value.erase(pos, 1);
						value.insert(pos, ".");
					}
					if (flag == FROM) 
						fromCost = stod(value.c_str());
					
					if (flag == TO) 
						toCost = stod(value.c_str());
				}

				if (field->get_name() == "Nominal") {
					//node to elementNode
					auto element = dynamic_cast<const xmlpp::Element*>(field);
					//check is instance of Element
					if (!element) 
					    continue;

					string value = element->get_child_text()->get_content();
					if (flag == FROM) 
						fromCount = atoi(value.c_str());
					
					if (flag == TO) 
						toCount = atoi(value.c_str());
				}
			}
		}
			
	}

public:
	ValuteTransfer(const string &fileName, const string &from, const string &to) {
		this->fileName = fileName;
		this->from = from;
		this->to = to;
		fromCount = -1;
		toCount = -1;
		fromCost = -1;
		toCost = -1;

		try {
		    domParser.set_validate(false);
		    domParser.set_substitute_entities(); //We just want the text to be resolved/unescaped automatically.
		    domParser.parse_file(fileName);
		    if (!domParser) 
		    	throw runtime_error("File broken");
		} catch(const exception& ex) {
		    cout << "Exception caught: " << ex.what() << endl;
		}

		root = domParser.get_document()->get_root_node();
	}

	~ValuteTransfer() = default;

	double rate() {
		valuteSearch();

		if (from == "RUB") {
			fromCost = 1.0;
			fromCount = 1;
		}
		if (to == "RUB") {
			toCost = 1.0;
			toCount = 1;
		}

		if (fromCost == -1) {
			if (from != to) {
				cout << "Not found valute: " << from << "\n";
				exit(EXIT_FAILURE);
			}
			fromCost = toCost;
			fromCount = toCount;
		}
		if (toCost == -1) {
			if (from != to) {
				cout << "Not found valute: " << to << "\n";
				exit(EXIT_FAILURE);
			}
			toCost = fromCost;
			toCount = fromCount;
		}

		fromCost /= fromCount;
		toCost /= toCount;

		return fromCost / toCost;
	}

};

int main(int argc, char* argv[]) {

	if (argc != 4) {
		cout << "Use form: " << argv[0] << " --data=daily.xml --from=USD --to=EUR\n";
		return 1;
	}

	string fileName;
	string from, to;

	for (int i = 1; i < argc; i++) {
		string curArg = argv[i];

		int pos = curArg.find("=");
		if (pos == string::npos) {
			cout << "Unknown comand: " << curArg << "\n";
			cout << "Use form: " << argv[0] << " --data=daily.xml --from=USD --to=EUR\n";
			return 1;
		}

		string command = curArg.substr(0, pos + 1);

		if (command == "--data=") {
			fileName = curArg.substr(pos + 1, curArg.size() - 1);
		} else if (command == "--from=") {
			from = curArg.substr(pos + 1, curArg.size() - 1);
		} else if (command == "--to=") {
			to = curArg.substr(pos + 1, curArg.size() - 1);
		} else {
			cout << "Unknown command: " << curArg << "\n";
			cout << "Use form: " << argv[0] << " --data=daily.xml --from=USD --to=EUR\n";
			return 1;
		}
	}

	ifstream fin(fileName);
	if (fin.fail()) {
		cout << fileName << " didn't find\n";
		return 1;
	}
	fin.close();

	ValuteTransfer valuteTransfer(fileName, from, to);
	
	cout << "1 " << from << " = ";
	cout << fixed << setprecision(4) << valuteTransfer.rate() << " " << to << endl; 	
	
	return 0;
}
