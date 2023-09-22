#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <numeric>
#include <stdexcept>
#include <string>
#include <vector>

struct StatResult
{
    std::string description;
    double value;

    StatResult(const std::string& desc, double val)
        : description(desc)
        , value(val)
    {
    }
};

using Data = std::vector<double>;
using Results = std::vector<StatResult>;


class Analyze
{
public:
    virtual ~Analyze() = default;
    virtual void count(const Data& data, Results& results_) = 0;
};

class AnalyzeMinMax : public Analyze
{
public:
    virtual void count(const Data& data, Results& results_)
    {
        double min = *(std::min_element(data.begin(), data.end()));
        double max = *(std::max_element(data.begin(), data.end()));

        results_.push_back(StatResult("Min", min));
        results_.push_back(StatResult("Max", max));
    }
};

class AnalyzeAvg : public Analyze
{
public:
    virtual void count(const Data& data, Results& results_)
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);
        double avg = sum / data.size();

        StatResult result("Avg", avg);
        results_.push_back(result);
    }
};

class AnalyzeSum : public Analyze
{
public:
    void count(const Data& data, Results& results_) override
    {
        double sum = std::accumulate(data.begin(), data.end(), 0.0);

        results_.push_back(StatResult("Sum", sum));
    }
};

class StatGroup : public Analyze
{
    std::vector<std::shared_ptr<Analyze>> stats_;
public:
    void count(const Data& data, Results& results_) override
    {
        for(const auto& stat : stats_)
        {
            stat->count(data, results_);
        }
    }

    void add(std::shared_ptr<Analyze> stat)
    {
        stats_.push_back(stat);
    }
};

class DataAnalyzer
{
    std::shared_ptr<Analyze> mAnalyze;
    Data data_;
    Results results_;

public:
    DataAnalyzer(std::shared_ptr<Analyze> analyze) :
        mAnalyze(analyze)
    {

    }

    void load_data(const std::string& file_name)
    {
        data_.clear();
        results_.clear();

        std::ifstream fin(file_name.c_str());
        if (!fin)
            throw std::runtime_error("File not opened");

        double d;
        while (fin >> d)
        {
            data_.push_back(d);
        }

        std::cout << "File " << file_name << " has been loaded...\n";
    }

    void set_statistics(std::shared_ptr<Analyze> analyze)
    {
        mAnalyze = analyze;
    }

    void calculate()
    {
        mAnalyze->count(data_, results_);
    }

    const Results& results() const
    {
        return results_;
    }
};

void show_results(const Results& results)
{
    for (const auto& rslt : results)
        std::cout << rslt.description << " = " << rslt.value << std::endl;
}

int main()
{
    auto avg = std::make_shared<AnalyzeAvg>();
    auto min_max = std::make_shared<AnalyzeMinMax>();
    auto sum = std::make_shared<AnalyzeSum>();

    auto std_group = std::make_shared<StatGroup>();
    std_group->add(avg);
    std_group->add(min_max);
    std_group->add(sum);

    DataAnalyzer da{ std_group };
    da.load_data("stats_data.dat");
    da.calculate();    

    show_results(da.results());

    std::cout << "\n\n";

    da.load_data("new_stats_data.dat");
    da.calculate();

    show_results(da.results());
}