#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include <random>
#include <algorithm>
//#include <boost/math/distributions/students_t.hpp>
//#include <boost/math/distributions/normal.hpp>
using std::string, std::vector, std::cout;

enum Distribution {Normal, T};

enum ReturnType {
    PriceReturn,
    LogReturn
};
class Constituent;
class Strategy;


class Constituent {
private:
    string ticker;
    std::vector<double> priceSeries;

    int obtainValueAtRiskIndex(double alpha) {
        std::sort(priceSeries.begin(), priceSeries.end());
        int index = static_cast<int> (alpha * priceSeries.size() / 100);
    }

    Distribution seriesDistribution;

public:
    Constituent(std::string ticker, std::vector<double> series) {
        this->ticker = ticker;
        this->priceSeries = series;
    }

    std::vector<double> getPriceSeries() {
        return this->priceSeries;
    }

    void setDistribution(Distribution seriesDistribution) {
        this->seriesDistribution = seriesDistribution;
    }
    bool operator==(const Constituent& other) const {
        // Here we compare the tickers of the two Constituents.
        // You might want to compare other data members as well,
        // depending on what it means for two Constituents to be equal.
        return ticker == other.ticker;
    }
    const string& getTicker() const { return ticker; }


    vector<double> calculateReturn(ReturnType returnType) {
        auto prices = this->priceSeries;
        if (prices.size() < 2) {
            std::cerr << "Error: The vector must have at least two elements to calculate the returns.\n";
            return {};
        }

        vector<double> returns;
        returns.reserve(prices.size() - 1); // reserves the memory for the return vector

        for (size_t i = 1; i < prices.size(); ++i) {
            double returnVal = 0.0;

            if (returnType == ReturnType::PriceReturn) {
                double initialPrice = prices[i - 1];
                double finalPrice = prices[i];
                returnVal = (finalPrice - initialPrice) / initialPrice;
            } else if (returnType == ReturnType::LogReturn) {
                double initialPrice = prices[i - 1];
                double finalPrice = prices[i];
                returnVal = std::log(finalPrice / initialPrice);
            } else {
                std::cerr << "Error: Invalid return type specified.\n";
                return {};
            }
            returns.push_back(returnVal);
        }
        return returns;
    }

    double calculateMonteCarloVar(int num_simulations = 10000, double confidence_level = 0.95) {
        vector<double> log_returns;
        auto prices = this->priceSeries;
        for (size_t i = 1; i < prices.size(); ++i) {
            log_returns.push_back(std::log(prices[i] / prices[i - 1]));
        }

        double mean_return = std::accumulate(log_returns.begin(), log_returns.end(), 0.0) / log_returns.size();
        double std_dev = 0.0;
        for (double log_return: log_returns) {
            std_dev += (log_return - mean_return) * (log_return - mean_return);
        }
        std_dev = std::sqrt(std_dev / (log_returns.size() - 1));

        std::default_random_engine generator;
        std::normal_distribution<double> distribution(mean_return, std_dev);

        std::vector<std::vector<double>> sim_returns(num_simulations, std::vector<double>(log_returns.size()));
        for (int i = 0; i < num_simulations; ++i) {
            for (size_t j = 0; j < log_returns.size(); ++j) {
                sim_returns[i][j] = distribution(generator);
            }
        }

        std::vector<std::vector<double>> sim_prices(num_simulations, std::vector<double>(prices.size()));
        for (int i = 0; i < num_simulations; ++i) {
            sim_prices[i][0] = prices.back();
            for (size_t j = 1; j < prices.size(); ++j) {
                sim_prices[i][j] = sim_prices[i][j - 1] * std::exp(sim_returns[i][j - 1]);
            }
        }

        std::vector<double> final_returns;
        for (const auto &sim_price: sim_prices) {
            final_returns.push_back(std::log(sim_price.back() / prices.back()));
        }

        std::sort(final_returns.begin(), final_returns.end());

        double var = final_returns[static_cast<int>((1 - confidence_level) * num_simulations)];
        return var;
    }

    // TODO: Implement other interpolation methodologies
    double calculateHistoricalValueAtRisk(double alpha) {
        // Get the vector associated with the specified ticker
        // Sort the price series in ascending order
        vector<double> sortedPrices = priceSeries;
        std::sort(sortedPrices.begin(), sortedPrices.end());

        // Calculate the index corresponding to the alpha level
        int index = std::round<int>(alpha * sortedPrices.size() / 100);

        // Return the Value at Risk (VaR) at the specified alpha level
        return sortedPrices[index];
    }
//
//    double var_parametric(std::vector<double> portfolioReturns, double portfolioStd,
//                          Distribution distribution = Distribution::Normal, double alpha = 5, int dof = 6) {
//        double VaR = 0.0;
//        double portfolioMean =
//                std::accumulate(portfolioReturns.begin(), portfolioReturns.end(), 0.0) / portfolioReturns.size();
//
//        if (distribution == Distribution::Normal) {
//            boost::math::normal norm;
//            VaR = boost::math::quantile(norm, 1 - alpha / 100) * portfolioStd - portfolioMean;
//        } else if (distribution == Distribution::T) {
//            if (dof <= 2) {
//                throw std::invalid_argument("Degrees of freedom must be greater than 2 for t-distribution.");
//            }
//            boost::math::students_t dist(dof);
//            VaR = std::sqrt((dof - 2) / static_cast<double>(dof)) * boost::math::quantile(dist, 1 - alpha / 100) *
//                  portfolioStd - portfolioMean;
//        } else {
//            throw std::invalid_argument("Expected distribution type 'Normal'/'T'");
//        }
//        return VaR;
//    }
};

namespace std {
    template<>
    struct hash<Constituent> {
        size_t operator()(const Constituent& c) const {
            // Implement your hashing function here.
            // As an example, you might use the hash of the `ticker` string:
            return std::hash<std::string>()(c.getTicker());
        }
    };
}


class Strategy {
private:
    string ticker;
    std::unordered_map<Constituent, double> holdings; // stores the {ticker:weight} mapping
public:
    Strategy(string ticker, std::unordered_map<Constituent, double> strategyHolding) {
        this-> ticker =  ticker;
        this->holdings = strategyHolding;
    }
    void updateWeight(){

    }
    void addConstituent(Constituent constituent, double weight){
        this->holdings.insert({constituent, weight});
    }

    vector<double> generateStrategyLevel(const std::vector<std::vector<double>>& priceSeries,
                                              const std::vector<double>& weights) {
        // Determine the number of time points from the first price series
        int numTimePoints = priceSeries[0].size();

        // Initialize the strategy level series with 0s
        std::vector<double> strategyLevel(numTimePoints, 0.0);

        // Check if the sizes of priceSeries and weights match
        if(priceSeries.size() != weights.size()){
            throw std::invalid_argument("Mismatch between number of price series and weights.");
        }

        // For each price series...
        for (int i = 0; i < priceSeries.size(); ++i) {
            // For each time point...
            for (int t = 0; t < numTimePoints; ++t) {
                // Add the weighted price to the strategy level series
                strategyLevel[t] += priceSeries[i][t] * weights[i];
            }
        }

        return strategyLevel;
    }

};

//Write a method in C++ to update an unordered map (holdings) inside the class strategy. holding contains the ticker weight mapping {"AAPL":0,5, "BBB": 0.5}. The update function should take a timc

int main() {
    std::vector<double> aaplSeries = {152.04212951660156, 150.70016479492188, 155.85931396484375, 156.41600036621094, 161.54537963867188, 160.5513153076172, 159.06019592285156, 165.14389038085938, 164.82579040527344, 164.5968475341797, 164.1190185546875, 164.16879272460938, 168.4691162109375, 167.7225341796875, 171.3160858154297, 172.401123046875, 172.2418670654297, 173.7549285888672, 173.3567352294922, 170.7387237548828, 166.80673217773438, 166.46826171875, 166.76690673828125, 169.25550842285156, 162.8747100830078, 160.64491271972656, 158.18617248535156, 156.50387573242188, 157.24050903320312, 155.10028076171875, 153.8261260986328, 155.24960327148438, 153.75643920898438, 156.65316772460938, 162.68557739257812, 153.13925170898438, 154.60255432128906, 151.67596435546875, 150.01356506347656, 153.77633666992188, 156.18531799316406, 153.01980590820312, 152.04428100585938, 149.74478149414062, 150.08326721191406, 151.06874084472656, 149.157470703125, 141.8310089111328, 137.57049560546875, 141.8011474609375, 145.4345245361328, 145.733154296875, 144.76756286621094, 139.45188903808594, 139.7803955078125, 138.34693908691406, 137.70985412597656, 142.3386993408203, 137.7496795654297, 141.76132202148438, 143.09521484375, 143.2047119140625, 142.7368621826172, 146.59918212890625, 148.76925659179688, 151.64610290527344, 148.66970825195312, 144.14044189453125, 155.03060913085938, 152.64154052734375, 149.9637908935547, 144.369384765625, 138.2473907470703, 137.97817993164062, 138.51661682128906, 139.09494018554688, 134.47837829589844, 146.4435272216797, 149.2653045654297, 147.8494415283203, 149.6043243408203, 148.3579559326172, 150.28236389160156, 150.8507080078125, 147.58021545410156, 149.74391174316406, 150.63134765625, 147.679931640625, 143.80123901367188, 140.7600860595703, 147.60015869140625, 147.8793487548828, 147.38079833984375, 146.20423889160156, 142.4950408935547, 140.53076171875, 142.2357940673828, 141.74722290039062, 144.07044982910156, 145.047607421875, 142.79417419433594, 136.1036376953125, 134.11941528320312, 131.9856414794922, 131.9158477783203, 135.0566864013672, 131.84603881835938, 131.4771270751953, 129.65243530273438, 125.67402648925781, 129.23365783691406, 129.55271911621094, 124.70683288574219, 125.99309539794922, 124.65697479248047, 129.24362182617188, 129.77207946777344, 130.35040283203125, 133.10238647460938, 133.0226287841797, 134.3686981201172, 135.54527282714844, 134.81739807128906, 134.87722778320312, 137.46966552734375, 140.7002716064453, 142.1161346435547, 141.44808959960938, 143.5419921875, 145.50625610351562, 142.58477783203125, 143.8710174560547, 145.00770568847656, 150.382080078125, 154.05137634277344, 151.2894287109375, 154.20094299316406, 151.4788818359375, 150.4319305419922, 150.80140686035156, 153.6374969482422, 152.98838806152344, 155.11544799804688, 153.49769592285156, 152.33929443359375, 148.27490234375, 148.70431518554688, 149.19363403320312, 146.50735473632812, 147.71568298339844, 147.20639038085938, 145.10928344726562, 145.70846557617188, 150.82138061523438, 153.61752319335938, 151.39060974121094, 152.65884399414062, 150.38198852539062, 148.2948760986328, 150.2621612548828, 152.3792266845703, 152.7786865234375, 155.63473510742188, 154.78590393066406, 157.18258666992188, 159.0599822998047, 157.61199951171875, 158.71046447753906, 160.02865600585938, 158.06137084960938, 157.43223571777344, 160.54794311523438, 162.1357421875, 164.67222595214844, 165.9404754638672, 165.40122985839844, 163.53379821777344, 164.43255615234375, 161.80618286132812, 160.57789611816406, 159.8788604736328, 165.33131408691406, 164.9818115234375, 165.00177001953125, 166.2400665283203, 167.39846801757812, 166.4197998046875, 164.7920684814453, 165.1016387939453, 163.54379272460938, 163.53379821777344, 168.17738342285156, 169.44561767578125, 169.35574340820312, 168.30718994140625, 167.2187042236328, 165.5609893798828, 173.33026123046875, 173.26034545898438, 171.53274536132812, 173.3202667236328, 173.510009765625, 172.57000732421875, 172.07000732421875, 172.07000732421875, 172.69000244140625, 175.0500030517578, 175.16000366210938, 174.1999969482422, 171.55999755859375, 171.83999633789062, 172.99000549316406, 175.42999267578125, 177.3000030517578, 177.25, 180.08999633789062, 180.9499969482422, 179.5800018310547, 179.2100067138672, 177.82000732421875, 180.57000732421875, 180.9600067138672, 183.7899932861328, 183.30999755859375, 183.9499969482422, 186.00999450683594, 184.9199981689453, 185.00999450683594, 183.9600067138672, 187.0, 186.67999267578125, 185.27000427246094, 188.05999755859375, 189.25, 189.58999633789062, 193.97000122070312, 192.4600067138672, 191.3300018310547, 191.80999755859375, 190.67999267578125, 188.61000061035156, 188.0800018310547, 189.77000427246094, 190.5399932861328, 190.69000244140625, 193.99000549316406, 193.72999572753906, 195.10000610351562, 193.1300048828125, 191.94000244140625, 192.75, 193.6199951171875};
    Constituent aapl("AAPL", aaplSeries);
    auto myPriceSeries = aapl.getPriceSeries();
    double alpha = 0.05; // 5% alpha level
    double var = aapl.calculateHistoricalValueAtRisk(alpha);
    std::cout << "Value at Risk at " << alpha * 100 << "% alpha level: " << var << std::endl;

    auto priceReturn = aapl.calculateReturn(ReturnType::PriceReturn);
    for (const auto& r : priceReturn){
        cout << "daily return " << r << std::endl;
    }

    return 0;
}
