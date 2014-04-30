#include <iostream>

#include "Avahi.hpp"
#include "Browser.hpp"

using namespace std;

int main() {
	Avahi::Browser browser("_hotdec._tcp");
	sleep(2);
	while (!browser.ready()) sleep(1);
	auto result = browser.lookup_by_name("ruby-vision");
	cout << result.size() << endl;
	result = browser.lookup_by_name("vision");
	cout << result.size() << endl;
	result = browser.lookup_by_name("ruby-vision", [](Avahi::Service const &x){ (void)x; return true; });
	cout << result.size() << endl;
	return 0;
}
