//
// Created by nezhov on 27.04.22.
//

#pragma once
#include <map>
#include <vector>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cctype>
#include <set>
#include <iterator>
#include <cmath>
#include <string>
using namespace std;

void strTolower(string& arg);

vector<string> SplitIntoWords(const string& line);

struct CategoryItem{
    bool category;
    string text;
};

class BayersClassifier {
public:
    void learnClassifier(const vector<CategoryItem>& trainingSample);
    bool classify(const string& str);
private:
    double M = 0.;

    double P1 = 0.;
    double P2 = 0.;

    double N1 = 0.;
    double N2 = 0.;

    set<string> fClassWords; // множество уникальных слов для 0 класса
    set<string> sClassWords; // множество уникальных слов для 1 класса

    map<string, double> fClassCount; // кол-во вхождений слова в 0 класс
    map<string, double> sClassCount; // кол-во вхождений слова в 1 класс

    void CountWords(const string& line, set <string>& curUnique, map<string, double>& curCount);

    double calculateP(const string &arg, double Nk, const map<string, double> &ClassCount, double Pq);
};



