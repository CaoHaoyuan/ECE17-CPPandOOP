//
//  STLWordProcessor.hpp
//  assignment6
//
//  Created by rick gessner on 11/12/19.
//  Copyright © 2019 rick gessner. All rights reserved.
//

#ifndef ProcessController_hpp
#define ProcessController_hpp

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <list>

namespace ECE17 {
  
  class ProcessController {
  public:
    ProcessController();
    ProcessController(const ProcessController &aCopy);
    ~ProcessController();
    ProcessController& operator=(const ProcessController &aCopy);
    
    void showStatistics(std::string &anInput, std::ostream &anOutput);
    void calcWordUsage(std::list<std::string> &word_col,
                       std::list<int> &count_col,
                       std::string &anInput);

    void calcWordPairs(std::list<std::string> &word_pairs_col,
                       std::list<int> &count_col,
                       std::string &anInput);


    bool compress(std::string &anInput, std::ostream &anOutput);
    bool decompress(std::string &anInput, std::ostream &anOutput);
    
  protected:
    void cleanWord(std::string &aWord);
    void encodeWord(std::string aWord,std::ostream &anOutput);
    void decodeWord(std::string aWord,std::ostream &anOutput);
    void showWordUsage(std::string &anInput, std::ostream &anOutput);
    void showWordPairs(std::string &anInput, std::ostream &anOutput);

    void printTable(std::string col1_label, std::string col2_label,
                    std::list<std::string> &col1, std::list<int> &col2,
                    std::ostream &anOutput);


    //what data structures make sense to you? Add all you want! :)
    std::map<std::string,unsigned short> encodeDict;
    std::vector<std::string> decodeDict;
    
  };

}


#endif /* ProcessController */
