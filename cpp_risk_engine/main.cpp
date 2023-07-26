#include <iostream>
#include <map>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>

int main () {
    using namespace boost::numeric::ublas;
    matrix<double> m (3, 3);

    // Assign values to the matrix
    int value = 1;
    for (unsigned i = 0; i < m.size1 (); ++ i)
        for (unsigned j = 0; j < m.size2 (); ++ j)
            m (i, j) = value++;

    std::cout << m << std::endl;

    return 0;
}


//using std::string, std::map, std::vector;
//#include <fmt/format.h>
//#include <fmt/core.h>
//
//
//
//class Constituent{
//private:
//    map <string, vector<double>> priceSeries;
//public:
//    Constituent(string ticker, vector<double> series ){
//        this -> priceSeries[ticker] = series;
//    }
//    map <string, vector<double>> getPriceSeries(){
//        return this -> priceSeries;
//    };
//
//};
//
//int main() {
//    vector<double> aaplSeries = {0.1,0.2,.3};
//    Constituent aapl("AAPL", (aaplSeries));
//    auto myPriceSeries = aapl.getPriceSeries();
//    for (const auto& price: myPriceSeries){
//        for (const auto& item: price.second){
//            std::cout << "Price: " << item << "\n";
//        }
//    }
//    fmt::print("The answer is {}.", 42);
//    std::cout << "Hello, World!" << std::endl;
//    return 0;
//}
