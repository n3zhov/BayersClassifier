//
// Created by nezhov on 27.04.22.
//

#include "BayersClassifier.h"

void strTolower(string& arg){
    transform(cbegin(arg), cend(arg), begin(arg),
                   [](unsigned char c)-> unsigned char{ return tolower(c); });
    arg.erase(remove_if(arg.begin(),
                        arg.end(),
                        [](unsigned char c){return !('a' <= c && c <= 'z');}),
              arg.end());
}

vector<string> SplitIntoWords(const string& line) {
    istringstream words_input(line);
    vector<string> res {istream_iterator<string>(words_input), istream_iterator<string>()};
    for(string& str: res){
        strTolower(str);
    }
    return res;
}


void BayersClassifier::learnClassifier(const vector<CategoryItem> &trainingSample) {
    fClassWords.clear();
    sClassWords.clear();

    fClassCount.clear();
    sClassCount.clear();



    double subSum1 = 0.; //считаем кол-во документов 0-го класса
    double subSum2 = 0.; //и 1-го

    for(const auto & [category, line]: trainingSample){
        if(category){
            CountWords(line, fClassWords, fClassCount);
            ++subSum2;
        }
        else{
            CountWords(line, sClassWords, sClassCount);
            ++subSum1;
        }
    }

    N1 = accumulate(fClassCount.begin(), fClassCount.end(), 0.,
                    [](double value, const pair<string, double> &valArg){
        return value + valArg.second;
    });
    N2 = accumulate(sClassCount.begin(), sClassCount.end(), 0.,
                    [](double value, const pair<string, double> &valArg){
                        return value + valArg.second;
                    });;

    P1 = log(subSum1/(subSum1+subSum2));
    P2 = log(subSum2/(subSum1+subSum2));

    M = static_cast<double>(fClassWords.size()) + static_cast<double>(sClassWords.size());
}

bool BayersClassifier::classify(const string &str) {
    return calculateP(str, N1, fClassCount, P1) > calculateP(str, N2, sClassCount, P2);
}

double BayersClassifier::calculateP(const string &arg, double Nk, const map<string, double> &ClassCount, double Pq) {
    double subM = M + Nk;
    double alpha = 1.;

    double subRes = 0.;
    for(const string& word: SplitIntoWords(arg)){
        try{
            double Nik = 0.;
            Nik = ClassCount.at(word);
            subRes += log((alpha + Nik) / subM);
        }catch(out_of_range &e) {
            subRes += log((alpha) / subM);
        }
    }
    return Pq + subRes;
}

void BayersClassifier::CountWords(const string &line, set<string> &curUnique, map<string, double> &curCount) {
    for(const string& word: SplitIntoWords(line)){
        curUnique.insert(word);
        ++curCount[word];
    }
}
