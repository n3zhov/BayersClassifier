#include <iostream>
#include "BayersClassifier.h"
using namespace std;

int main() {
    int training, test;
    cin >> training >> test;

    vector<CategoryItem> items;
    items.reserve(training);

    bool category;
    string text;

    for(int i = 0; i < training; ++i){
        cin >> category;
        cin.get();
        getline(cin, text);
        items.push_back({category, text});
    }

    BayersClassifier predictor;
    predictor.learnClassifier(items);

    for(int i = 0; i < test; ++i){
        getline(cin, text);
        if(predictor.classify(text)){
            cout << "1\n";
        }
        else{
            cout << "0\n";
        }
    }
    cout << endl;
    return 0;
}
