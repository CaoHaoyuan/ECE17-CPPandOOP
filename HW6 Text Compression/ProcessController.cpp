//
//  STLWordProcessor.cpp
//  assignment6
//
//  Created by rick gessner on 11/12/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#include "ProcessController.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <list>
#include <map>
#include <ctype.h>
using std::cout;
using std::endl;
using std::string;

namespace ECE17 {

    std::string columnPad(std::string str, int column_width) {
      if(str.length() < column_width) str = str + std::string(column_width - str.length(), ' ');
      return str;
    }

    bool isPunct(char aChar) {
      static std::string punct(".;?!:,'-()");
      return std::string::npos != punct.find(aChar);
    }

    void removePunct(std::string &str) {
      std::string str_orig = str;
      str.clear();

      for(char c: str_orig) {
        if(!isPunct(c)) str.push_back(c);
      }
    }

    ProcessController::ProcessController() {
      //don't forget to initialze your data members...
    }

    ProcessController::ProcessController(const ProcessController &aCopy) {
      //ocf requirement
    }

    ProcessController::~ProcessController() {
      //dont forget me!
    }

    ProcessController &ProcessController::operator=(const ProcessController &aCopy) {
      return *this;
    }

    //This drives your statistical functions...
    void ProcessController::showStatistics(std::string &anInput, std::ostream &anOutput) {

      anOutput << "\nWord Counts: \n";
      showWordUsage(anInput, anOutput);
      anOutput << "\nWord Pairs: \n";
      showWordPairs(anInput, anOutput);
    }

    void ProcessController::showWordUsage(std::string &anInput, std::ostream &anOutput) {
      std::list<std::string> word_col;
      std::list<int> count_col;

      calcWordUsage(word_col, count_col, anInput);
      printTable("Word", "Count", word_col, count_col, anOutput);
    }

    void ProcessController::showWordPairs(std::string &anInput, std::ostream &anOutput) {
      std::list<std::string> word_pairs_col;
      std::list<int> count_col;

      calcWordPairs(word_pairs_col, count_col, anInput);
      printTable("Word Pairs", "Count", word_pairs_col, count_col, anOutput);
    }

    void ProcessController::printTable(std::string col1_label, std::string col2_label,
                                       std::list<std::string> &col1, std::list<int> &col2,
                                       std::ostream &anOutput) {
      std::cout << "| " << columnPad(col1_label, 30);
      std::cout << "| " << columnPad(col2_label, 10) << " |" << std::endl;
      std::cout << std::string(46, '-') << std::endl;

      std::list<std::string>::iterator col1_it = col1.begin();
      std::list<int>::iterator col2_it = col2.begin();

      while(col1_it != col1.end() && col2_it != col2.end()) {
        std::cout << "| "  << columnPad(*col1_it++, 30);
        std::cout << "| " << columnPad(std::to_string(*col2_it++), 10) << " |" << std::endl;
      }
    }

    void ProcessController::cleanWord(string &aWord){
      // Remove the punctuation on two sides of the word
      while(aWord.size()>0){
        if(isalpha(aWord.front())) break;
        else aWord.erase(0,1);
      }
      while(aWord.size()>0){
        if(isalpha((aWord.back()))) break;
        else aWord.pop_back();
      }
    }


    void ProcessController::calcWordUsage(std::list<std::string> &word_col, std::list<int> &count_col,
                                          std::string &anInput) {
      word_col.clear();
      count_col.clear();

      size_t l = anInput.size();
     //cout << "Input string length is: " << l << endl;
      std::istringstream InputStringStream(anInput);
      string theWord;
      std::map<std::string,int> wordCount;  //wordCount[a word] = the word's count
      string theLine;

      //read word from anInput
      while(std::getline(InputStringStream,theLine)){
        std::istringstream LineStringStream(theLine);
        while(!LineStringStream.eof() and theLine!=""){
          LineStringStream >> theWord;
          cleanWord(theWord);
          auto ret = wordCount.insert({theWord,1});
          if(ret.second == false){
            // pair already exist
            wordCount[theWord]++;
          }
        }
      }

      // move word count from wordMap to word_col and count_col
      for(auto i = wordCount.begin();i!=wordCount.end();i++){
        word_col.push_back(i->first);
        count_col.push_back(i->second);
      }

      // construct the encodeDict and decodeDict.
      // encodeDict[a word] = the words' code
      // decodeDict[a code] = the word
      unsigned short code = 0;
      for(auto i = wordCount.begin();i!=wordCount.end();i++){
        encodeDict.insert({i->first,code});
        decodeDict.push_back(i->first);
        code ++;
      }
    }

    void ProcessController::calcWordPairs(std::list<std::string> &word_pairs_col, std::list<int> &count_col,
                                          std::string &anInput) {
      word_pairs_col.clear();
      count_col.clear();
      std::istringstream InputStringStream(anInput);
      string cur,next,theWord;
      if(anInput.size()==0) return; //cur cannot be assigned
      InputStringStream >> cur;
      std::map<string,int> wordPairCount; //wordPair[a Pair] = the pair's count
      while(!InputStringStream.eof()){
         InputStringStream >> next;
         theWord = cur+" "+next;
         auto ret = wordPairCount.insert({theWord,1});
         if(ret.second == false){
           // pair already exist
           wordPairCount[theWord]++;
         }
         cur = next;
      }
      // move word pair count from wordPairCount to word_pairs_col and count_col
      for(auto i = wordPairCount.begin();i!=wordPairCount.end();i++){
        if(i->second>1){
          word_pairs_col.push_back(i->first);
          count_col.push_back(i->second);
        }
      }
    }

    void ProcessController::encodeWord(string aWord,std::ostream &anOutput){
      auto copy = aWord;
      size_t i,j;
      for(i=0;i<aWord.length();i++){
        if(isalpha(aWord[i])) break;
      }
      anOutput << aWord.substr(0,i);
      cleanWord(copy);
      try {
        int Index = encodeDict.at(copy);
        if(Index<100){
          // need one char to encode
          anOutput << (char) (Index+128);
        }
        else if(Index<10000){
          // need two chars to encode
          int firsthalf = Index/100+128;
          int secondhalf = Index-100*firsthalf+128;
          anOutput << (char) firsthalf << (char) secondhalf;
        }
        else{
          // need three chars to encode
          int firsthalf = Index/10000+128;
          Index -= 10000*firsthalf;
          int secondhalf = Index/100+128;
          Index -= 100*secondhalf;
          int thirdhalf = Index+128;
          anOutput << (char)firsthalf << (char)secondhalf <<(char)thirdhalf;
        }

      } catch (const std::out_of_range &oor) {
        std::cerr << "Input stream contains words not in the list."
                  << oor.what() << endl;
      }
      for(j=aWord.length()-1;j>=0;j--){
        if(isalpha(aWord[j])) break;
      }
      anOutput<< aWord.substr(j+1,aWord.length()-j);
    }

    //output your compressed version input the anOutput stream...
    bool ProcessController::compress(std::string &anInput, std::ostream &anOutput) {
      //STUDENT: implement this method...
      //original string -> unsigned short int -> encoded string
      std::istringstream InputStringStream(anInput);
      string theLine,theWord;

      //read word from anInput
      bool addLb = (anInput.back()=='\n');

      bool flag_lb = false;
      while(std::getline(InputStringStream,theLine)){
        if(flag_lb) anOutput << "\n";
        flag_lb = true;
        std::istringstream LineStringStream(theLine);
        if(theLine.empty()){
        }
        else {
          bool flag_ws = false;
          while (!LineStringStream.eof()) {
            if(flag_ws) anOutput << " ";
            flag_ws = true;
            LineStringStream >> theWord;
            encodeWord(theWord,anOutput);
          }
        }
      }
      if(addLb) anOutput << "\n";
      return true;
    }

    void ProcessController::decodeWord(string aString, std::ostream &anOutput){
      size_t i,j;
      for(i=0;i<aString.length();i++){
        if((int)aString[i]<0) break;
      }
      for(j=aString.length()-1;j>=0;j--){
        if((int)aString[j]<0) break;
      }
      auto copy = aString.substr(i,j-i+1);
      auto l = copy.length();
      int Index;
      if(l>3){
        cout<< "Decode error.Coded word should not have more than 3 chars." << endl;
      }
      if(l==1){
        // index 0-99
//        cout << i << " " << j << endl;
//        cout << copy[0] << (int)copy[0] << endl;
        Index = (int)copy[0]+128;
      } else if(l==2){
        Index = 100*(int)(copy[0]+128)+(int)copy[1]+128;
      } else{
        Index = 10000*((int)copy[0]+128)+100*((int)copy[1]+128)+(int)copy[2]+128;
      }
//      cout << "Index is " << Index << endl;
      string aWord = decodeDict[Index];

      anOutput << aString.substr(0,i) << aWord << aString.substr(j+1,aString.length()-j);
    }

    //de-compress given string back to original form, and write it out to anOutput...
    bool ProcessController::decompress(std::string &aBuffer, std::ostream &anOutput) {
      //STUDENT: implement this method...
      //encoded string -> unsigned short -> original string
      std::istringstream InputStringStream(aBuffer);
      string theLine,theString;

      //read word from a buffer
      bool addLb = aBuffer.back()=='\n';

      bool flag_lb = false;
      while(std::getline(InputStringStream,theLine)){
        if(flag_lb) anOutput << "\n";
        flag_lb = true;
        std::istringstream LineStringStream(theLine);
        if(theLine==""){
        }
        else {
          bool flag_ws = false;
          while (!LineStringStream.eof()) {
            if(flag_ws) anOutput << " ";
            flag_ws = true;
            LineStringStream >> theString;
            decodeWord(theString,anOutput);
          }
        }
      }
      if(addLb) anOutput << '\n';
      return true;
    }

}
