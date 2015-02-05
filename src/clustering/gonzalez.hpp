#include <ifgt/clustering.hpp>


namespace ifgt
{


class Gonzalez : public Clustering
{
public:

    Gonzalez(const arma::mat& source, int K, double bandwidth, double epsilon,
            bool use_starting_idx, arma::uword starting_idx);

private:

    virtual void cluster();

    bool m_use_starting_idx;
    arma::uword m_starting_idx;

};


}