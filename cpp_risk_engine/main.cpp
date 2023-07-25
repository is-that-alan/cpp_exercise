#include <iostream>
#include <map>
using std::string, std::map, std::vector;

class Constituent{
private:
    map <string, vector<double>> priceSeries;
public:
    Constituent(string ticker, vector<double> series ){
        this -> priceSeries[ticker] = series;
    }
    map <string, vector<double>> getPriceSeries(){
        return this -> priceSeries;
    };

};

int main() {
    vector<double> aaplSeries = {0.1,0.2,.3};
    Constituent aapl("AAPL", (aaplSeries));
    auto myPriceSeries = aapl.getPriceSeries();
    for (const auto& price: myPriceSeries){
        for (const auto& item: price.second){
            std::cout << "Price: " << item << "\n";
        }
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
