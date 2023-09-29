// Project UID db1f506d06d84ab787baf250c265e24e
#include <iostream>
#include <fstream>
#include <set>
#include <map>
#include "csvstream.h"
#include <cmath>
using namespace std;

class Classifier{
  public:
  Classifier(string TrainFilename, bool debug){
    csvstream trainIn(TrainFilename); 
    if(debug)
      cout<<"training data:"<<endl;
    while(trainIn>>row_value){
      string tag = row_value["tag"];
      tags.insert(tag);
      labels[tag]++;
      istringstream content(row_value["content"]);
      string word;
      set<string> unique_words;
      if(debug){
        cout<<"  label = "<<tag<<", content = "<<content.str()<<endl;
      }
      while(content>>word){
        unique_words.insert(word);
        word ="";
      }
      for(auto it = unique_words.begin();it!=unique_words.end();it++){
        NumPostswWord[*it]++;
        NumPostswLabelwWord[tag][*it]++;
      }
      //first add all words to set to get unique words
      // for every word in each post split them and add to a map of #training 
      //posts w wordwords[row_value["content"]]++;
      // for every word in post also add to thing given its label
      numTrainPosts++;
    }
    cout<<"trained on "<<numTrainPosts<<" examples"<<endl;
    if(debug)
      cout<<"vocabulary size = "<<NumPostswWord.size()<<endl;

    //predicting want to auto for loop through all the labels seeing if 
    //each has lower log score and then returns which one has smallest
    //
    cout<<endl;
    if(debug)
      debugTrainOutput();
  }
  
  void debugTrainOutput(){
    cout<<"classes:"<<endl;
    for(auto it = labels.begin(); it!=labels.end();it++){
      cout<<"  "<<it->first<<", "<<it->second<<" examples, log-prior = "
      <<ln(it->second,numTrainPosts)<<endl;
    }
    cout<<"classifier parameters:"<<endl;
    for(auto it = NumPostswLabelwWord.begin(); it!=NumPostswLabelwWord.end();it++)
    {
      for(auto itWord = it->second.begin();itWord!=it->second.end();itWord++)
      cout<<"  "<<it->first<<":"<<itWord->first<<", count = "<<itWord->second
      <<", log-likelihood = "<<ln(itWord->second,labels[it->first])<<endl;
    }
    cout<<endl;
  }

  void predict(string testFilein) {
    csvstream testIn(testFilein); 
    cout<<"test data:"<<endl;
    int testPosts =0, correct =0;
    while(testIn>>row_value){
      testPosts++;
      pair<string, double> curGuess;
      curGuess.first = "";
      for(auto it = tags.begin();it!=tags.end();it++){
        double logScore =0;
        string tag = *it;
        logScore += ln(labels[tag],numTrainPosts);
        istringstream content(row_value["content"]);
        set<string> unique_words;
        string word;
        while(content>>word){
          unique_words.insert(word);
          word ="";
        }
        for(auto it = unique_words.begin();it!=unique_words.end();it++){
          int postsLabelWord =NumPostswLabelwWord[tag][*it];
          if(postsLabelWord!=0)
            logScore += ln(postsLabelWord,labels[tag]);
          else if(NumPostswWord[*it]!=0)
            logScore += ln(NumPostswWord[*it],numTrainPosts);
          else
            logScore += ln(1,numTrainPosts);
        }
        if(curGuess.first == ""||logScore>curGuess.second){
          curGuess.first = tag;
          curGuess.second = logScore;
        }
      }
      if(curGuess.first == row_value["tag"])
        correct++;
      cout<<"  correct = "<<row_value["tag"]<<", predicted = "<<curGuess.first
      <<", log-probability score = "<<curGuess.second<<endl;
      cout<<"  content = "<<row_value["content"]<<endl;
      cout<<endl;
    }
    cout<<"performance: "<<correct<<" / "<<testPosts
    <<" posts predicted correctly"<<endl;

  }

  private:
    map<string, map<string,int>> NumPostswLabelwWord; 
    map<string, string> row_value;
    map<string, int> labels;
    map<string, int>NumPostswWord;
    set<string> tags;
    int numTrainPosts =0;

    double ln(int a, int b){
      return log(static_cast<double>(a)/static_cast<double>(b));
    }
};


int main(int argc, char **argv) {
  bool debug=false;

  cout.precision(3);
  char debugstr[] = "debug";
  if((argc!=3&&argc!=4)||(argc==4&&strcmp(argv[3],debugstr)==0)){
        cout << "Usage: main.exe TRAIN_FILE TEST_FILE [--debug]" << endl;
        return -1;
  }
  if(argc==4)
    debug=true;
  string filenameTrain = argv[1];
  string filenameTest = argv[2];
  try{
    csvstream finTrain(filenameTrain); 
    
  }
  catch(const csvstream_exception &e){
    cout<<"Error opening file: " << filenameTrain << endl;
    return -1;
  }
  try{
    csvstream finTest(filenameTest);
  }
  catch(const csvstream_exception &e){
    cout<<"Error opening file: " << filenameTest << endl;
    return -1;
  }

  Classifier postClassifier(filenameTrain,debug);
  postClassifier.predict(filenameTest);

  
  
}