# Spring 2020 -- Assignment 6
## Let's build a data compressor, and get practice with templates and the STL
### Due Wednesday, May 27, 2020 at 11:00pm

For this assignment, we're going to use all the skills we're developing for Object-oriented design, templates, iterators, STL and algorithms. 

## Introduction

In this assignment, you will read, analyze, compress, and decompress a text file. You may work alone, or you may work with a partner.

There are three parts to the project, briefly:

1. Doing some basic statistical analysis on the words in the given text file (measured by accuracy, we've implemented this for you)
2. Using your creative ideas to compress the text in the given file (measured by compression ratio % )
3. Decompressing the compressed version, to restore the file to its original form (measured by accuracy against original)

### Part 1 -- Reading the given text file

If you examine the project files above, you'll notice a file called `ProcessController`. This class in this file will coordinate the process your code goes through to complete the tasks in this assignment.  The `main.cpp` will measure the time your code takes to complete the assigned tasks, and measure your overall results.   You will implement various methods in this class, and you may add your own member data and methods as needed. You may NOT remove any methods from the class, or change the basic way it works (so you don't break auto-testing). 

The `main.cpp` file automatically reads your text file into a string. The methods for each task (statistical, compression, decompression) accept a `std::string` that contains the file contents. So yay -- that part is given. :)


### Part 2 -- Statistical Analysis 

For this part of the assignment, you will measure various aspects of the words you've read in the given file. This step begins with a call to `ProcessController.runStatistics(std::string &anInput, std::ostream &anOutput)`.  This method accepts a string (containing your file contents) and an output buffer where you should write your results. This method automatically calls your two statistical functions that print tables based upon two function you will implement:

```
    void calcWordUsage(std::list<std::string> &word_col,
                       std::list<int> &count_col,
                       std::string &anInput);

    void calcWordPairs(std::list<std::string> &word_pairs_col,
                       std::list<int> &count_col,
                       std::string &anInput);
```


#### Step 2A -- Counting word usage

In this step, you will count how many times each word was found in the given input file. Word comparisons should be performed in a CASE SENSITIVE manner and all punctuation should be ignored. You will implement this logic in the `ProcessController.calcWordUsage()` method. You can write any code you like, but we suggest using STL containers to store any information you need. Try to use `iterator` objects from the STL collections as much as you can (within reason) to simplify your solution. 

When this method is complete, it will populate two lists of equal length, one with a list of words and the second list with a list of word counts. The list of counts should correspond to the list of words (i.e. first count corresponds to first word, second count corresponds to the second word). The words on the list should not have any punctuation or extra spaces. `The best food for you to eat, is not always the best food for you.` you should count:
```
| Word                          | Count      |
----------------------------------------------
| The                           | 1          |
| always                        | 1          |
| best                          | 2          |
| eat                           | 1          |
| food                          | 2          |
| for                           | 2          |
| is                            | 1          |
| not                           | 1          |
| the                           | 1          |
| to                            | 1          |
| you                           | 2          |
```

#### Step 2B -- Counting word pairs

In this step, you will implement the method, `ProcessController.calcWordPairs()`. This method will scan all the word-pairs in the document, and will count repeated pairs and place them on `word_pairs_col` with corresponding counts in `count_col`. ONLY count pairs that are repeated more than once. Do NOT remove punctuation and make this analysis CASE SENSITIVE. Given the text, `The best food for you to eat, is not always the best food for you.` you should count:
```
| Word Pairs                    | Count      |
----------------------------------------------
| best food                     | 2          |
| food for                      | 2          |
````

### Part 3 -- Compressing the input file

In this step, you will implmenent the method, `ProcessController.compress()`. You will implement your own, creative approach to compressing the underying data. Options include (but aren't limited to) -- a substitution cypher, dictionary compression (recommended), statistical compression (like LZW or Huffman encoding), and so on. If you use a published method, you must implement your own version from scratch-- and be prepared to prove you understand it. You are free to use any solution provided by the STL too (if one exists).

The key to success in this step is to use creativity and research to find various techniques to compress your text. We really want to see you try multiple ideas (and combinations). 

Standard compression methods can reduce text files by 75% or more. You should aim to find a way to compress the filesize by 25% or more. In a simple test case, Vlad found a "resonably simple" way to reduce the file by 30% using dictionary compression (see more below). Give this task some thought. Do some research on the google machine if necessary. Just do your own work, and try to keep is as simple as possible. Again, you may be asked to explain your solution.

#### -- A note on dictionary compression

This read.me file has more than 1000 words. But it has only about 400 unique words, which means that many words are used repeatedly. That observation enables dictionary compression.  Imagine that every unique word in this document is in a 400-word dictionary, and that each word has a unique integer-id (1..n). A _very simple_ dictionary compressor looks at each word in the document (in order):

1. If we haven't seen the word yet, add it to the dictionary, and then write the word to the output stream
2. If we have see the word before, write the word-id (from dictionary) to the output stream

The resulting document will therefore be a combination of words, interspersed with word-ids.  Since the word-id is small (2 bytes), it occupies much less space that the actual word (5-6 characters long, on average).  As an example, given this input file:

```The best food is the best food for your body```  

Using a trivial dictionary compressor, the rewritten output looks like this (nearly an 20% reduction):

```the best food is 1 2 3 for your body```  

> The embedded numbers are word id's (1=the, 2=best, 3=food). 

The STL collections and algorithms can make a task like this much easier, so explore them before you write your code.

#### -- Additional ideas to further reduce content size

A very powerful technique for further compression is to reduce compound words to roots and suffixes. For example, your actual input file is a short document about "skateboarding" (root=skateboard). Rather than storing the words: `skateboard`, `skateboarding`, `stakeboarder` (etc) in the dictionary, you can store the root (skateboard) in the dictionary. Then as you see words like `skateboarding` you output the root word-id (for `skateboard`) and the suffix "ing" -- which may also be a word id. If your input file says:

```
skateboarding requires the stakeboarder to balance on her skateboard
```

...and your compressed output (33% compression) looks like this:

```
stakeboard 1-ing requires the 1-er to balance on her 1
```

A another excellent technique worth considering, is to reduce repeating word-pairs to dictionary entries. For larger documents, this technique can pay a considerable benefit.  


### Part 4 -- Decompressing your compressed version

In this step, you will implement the method, `ProcessController.decompress(istream &anInputBuffer)`. This method will accept a a string  that contains the compressed output you generated earlier.  You task here is to restore the compressed version back to it's original format, that is equivalent to the original version of the file. 

### Part 5 -- Document your compression / decompression method!

In addition to documenting your code. Write a file `algorithm.md` that describes your strategy for compression and decompression.  We'll run another code review and this file will help those reviewing your code to better understand your approach.  You can write this file using "markdown" (the scripting language used to write this `README.md` file!).  For more on markdown, checkout: https://guides.github.com/features/mastering-markdown/

Such documents are common when solving bigger problems with software!


## Testing

It's up to you to write a validation functions for each part of this assignment. For example, when you find word counts, you should call your own validaton function that confirms that your word counts are correct.
