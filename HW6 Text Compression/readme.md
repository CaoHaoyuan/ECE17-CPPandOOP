# Overview
The class **Process Controller** is designed to compress and decompress the text file. Simple statistical analysis is also provided. 

# Data analysis
Two functions **calWordUsage** and **calWordPairs** are provided. Fist function computes the number of occurance of each word in the text file, while the second function computes the number of occurance of each word pair(two adjacent words) in the file. 

**calWordUsage**: Words are read sequentially. A map named *wordMap* is utilized to count how many times certain word is occured. After constructing the *wordMap*, its <key,value> pairs are extracted and used to construct the *word_col* and *count_col*. Besides, two protected members *encodeDict* and *decodeDict* is constructed using *wordMap*, such that *encodeDict*[an original word] = a integer, and *decodeDict*[a integer] = the original word. 

**calWordPairs**: Words are read sequentially. *Cur* and *Next* are current word and the next word read from the stream. A map named *wordPairMap* is utilized to count how many times certain word pair(*Cur*+*Next*) is occured. After constructing the *wordPairMap*, its<key,value> are extracted and used to construct the *word_pairs_col* and *count_col*. 

# Compression and Decompression
String is compressed using **compress** function. Coded string is passed to the **decompress** function and write to the output stream. Do not that the *encodeDict* and *decodeDict* are passed implicitly since they are accesseable by both functions. 

**compress**: A word(string, length\*1 bytes) is mapped to a number code(unsigned short, two bytes) using *encodeDict*. the number code, is further transformed to another string called coded string. To avoid collision with non-printable character(the first 32 characters in ASCII table) and punctuations which are transfered without compression, only the characters with ASCII number 128 to 227 are used.   
For example: school(original word) -> 4501(number code) -> (45) | (01) -> (45+128) | (01+128) -> (173) | (129) -> (char)173 | (char)129   
or: base(original word) -> 51(number code) -> 51+128 -> 179 -> (char)179   
This coded string is then passed to the *anOutput*;

**decompress**: A coded string is parsed to mutiple chars. Then these chars are used to reconstruct the number code. Number code is mapped to the original word using *decodeDict*.   
For example: ‰Š(‰ and Š has ASCII code 137 and 138 individually) -> (137) | (138) -> (137-128) | (138-128) -> 910(number code) -> textbook(original word)   
or: Ÿ(ASCII code 159) -> 159 -> 31(number code) -> abandon(original word)

# Result
Since any word can be represented by 1/2/3 characters with 1/2/3 bytes. The compression ratio, theoritically, is supposed to be very high.    
For story.txt, compression ratio is 48.02%   
For story1.txt, compression ratio is 55.62%   
For stroy2.txt,compression ratio is 55.52%   
