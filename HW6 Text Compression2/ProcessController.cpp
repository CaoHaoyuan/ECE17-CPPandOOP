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

    void ProcessController::calcWordUsage(std::list<std::string> &word_col, std::list<int> &count_col,
                                          std::string &anInput) {
      word_col.clear();
      count_col.clear();
      std::map<std::string,int> helperDict;
      std::istringstream InputStream(anInput);

      //Parse the input
      string Word;
      while(InputStream>>Word){
        removePunct(Word);
        helperDict[Word]+=1;
      }

      // Construct the word_col and count_col
      for(auto i = helperDict.begin();i!=helperDict.end();i++){
        word_col.push_back(i->first);
        count_col.push_back(i->second);
      }

    }

    void ProcessController::calcWordPairs(std::list<std::string> &word_pairs_col, std::list<int> &count_col,
                                          std::string &anInput) {
      word_pairs_col.clear();
      count_col.clear();
      std::map<std::string,int> helperDict;
      std::istringstream InputStream(anInput);

      //Parse the input
      string Cur,Next,Word;
      if(anInput.empty()) return;
      InputStream>>Cur;

      while(InputStream>>Next){
        Word = Cur + " " + Next;
        helperDict[Word]+=1;
        Cur = Next;
      }

      // Construct the word_pair_col and count_col
      for(auto i = helperDict.begin();i!=helperDict.end();i++) {
        if (i->second > 1) {
          word_pairs_col.push_back(i->first);
          count_col.push_back(i->second);
        }
      }
    }
    string Int2Str(int input){
      // convert the integer to string
      auto i = input;
      string ret = "";
      int intCode;
      if(i == 0){
        return ret + (char)128;
      }
      while(i>0){
        intCode = i - 100*(int)(i/100);
        ret = (char)(intCode+128)+ret;
        i = i/100;
      }
      return ret;
    }

    int Str2Int(string input){
      // convert the string to integer
      int ret = 0;
      for(size_t i = 0;i<input.length();i++){
        ret = 100*ret;
        ret += (int)input[i] + 128;
      }
      return ret;
    }

    string ProcessController::encodeWord(string anInput,int &counter){
      size_t i,j;
      for(i=0;i<anInput.length();i++){
        if(isalpha(anInput[i])) break;
      }
      for(j=anInput.length()-1;j>=0;j--){
        if(isalpha(anInput[j])) break;
      }
      string cleanedWord = anInput.substr(i,j-i+1);
      if(encodeDict.count(cleanedWord)){
       // std::cout << cleanedWord << " Intcode is" << encodeDict[cleanedWord];
        cleanedWord = Int2Str(encodeDict[cleanedWord]);
       // std::cout << " " << (int) cleanedWord[0] << std::endl;
      }
      else{
       // std::cout << "encodeDict insert "<< cleanedWord << " " << counter << std::endl;
        encodeDict.insert({cleanedWord,counter});
        counter += 1;
      }
      return anInput.substr(0,i)+cleanedWord+anInput.substr(j+1,anInput.length()-j);
    }

    string ProcessController::decodeWord(string anInput){
      size_t i,j;
      for(i=0;i<anInput.length();i++){
        if((int)anInput[i]<0 or isalpha(anInput[i])) break;
      }
      for(j=anInput.length()-1;j>=0;j--){
        if((int)anInput[j]<0 or isalpha(anInput[j])) break;
      }
      string cleanedWord = anInput.substr(i,j-i+1);
      if(cleanedWord.empty()) {}
      else if(isalpha(cleanedWord[0])){
        // this is uncoded word
       // std::cout << "decodeDict push back " << cleanedWord << std::endl;
        decodeDict.push_back(cleanedWord);
      }
      else{
        // this is coded word
       // std::cout << cleanedWord << " Str2Int(cleanedWord) is " << Str2Int(cleanedWord) << std::endl;
        cleanedWord = decodeDict[Str2Int(cleanedWord)];
      }
      return anInput.substr(0,i)+cleanedWord+anInput.substr(j+1,anInput.length()-j);
    }

    //output your compressed version input the anOutput stream...
    bool ProcessController::compress(std::string &anInput, std::ostream &anOutput) {
      //STUDENT: implement this method...
      std::istringstream InputStream(anInput);
      string Line,Word;
      int counter = 0;   //update the index of the current word

      bool newLine = false;
      while(std::getline(InputStream,Line)){
        if(newLine) anOutput << '\n';
        newLine = true;
        std::stringstream LineStream(Line);
        if(Line.empty()){ continue;}
        bool space = false;
        while(LineStream>>Word){
          if(space) anOutput << " ";
          space = true;
          anOutput << encodeWord(Word,counter);
        }
      }
      if(anInput.back()=='\n'){
        anOutput << "\n";
      }
      std::cout << "counter value is " << counter << std::endl;

      return true;
    }

    //de-compress given string back to original form, and write it out to anOutput...
    bool ProcessController::decompress(std::string &aBuffer, std::ostream &anOutput) {
      //STUDENT: implement this method...
      std::istringstream InputStream(aBuffer);
      string Line,Word;
      bool newLine = false;
      while(std::getline(InputStream,Line)){
        if(newLine) anOutput<<'\n';
        newLine = true;
        std::stringstream LineStream(Line);
        if(Line.empty()){continue;}
        bool space = false;
        while(LineStream>>Word){
          if(space) anOutput << " ";
          space = true;
          anOutput << decodeWord(Word);
        }
      }
      if(aBuffer.back()=='\n'){
        anOutput << "\n";
      }
      return true;
    }

}
