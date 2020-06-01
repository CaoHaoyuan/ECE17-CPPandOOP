#Statistical analysis
**calWordUsage**: Word are read from the stringstream one by one. A map called helperDict are constructed and updated to count the number of times each word is incurred. After reading all the words, the helperDict is used to create the word_col and count_col;
**calWordPairs**: Word pairs are read from the stringstream using Cur and Next. A map called helperDict are constructed and updated to count the number of times each word pair is incurred. After reading all the words, the helperDict is used to create the word_pairs_col and count_col.

#Compression and Decompression
Running dictionary algorithm is implemented here. Do note that the "shared information" between the method compress and decompress are strictly limited to the encoded string. Class member encodeDict is only used in compression and decodeDict is only used in decompression.

**compress**: Word are read one by one. If the word has not appeared before, encodeDict will add this word as key and an integer named count as value. Count increment by one after this operation. If the word is already in encodeWord(has appeared before), encodeWord[word], which is an integer, will be further converted to string and send to the output stream. 
This conversion is done in such way: 
50789 -> 5|07|89 -> 128+5|128+7|128+89 -> 133|135|217 -> (char)133+(char)135+(char)217

**decompres**: Word are read one by one. If the word is within the first 128 character in ASCII table, it is regarded as uncoded word. This word is added to the decodeDict. Otherwise, it is regarded as coded word. Coded string is passed to the function str2int to get the original index. Then the corresponding string in the decodeDict is extracted as reconstructed word. 
For example: 
(char)133+(char)135+(char)217 -> (133-128)\*10000+(135-128)\*100+(217-128) -> 50789

#Evaluation
Since no matter how long the word is, it can always be converted to 1/2/3 bytes if there are less than 10^6 different words in the text. Compression rate is expected to be high for texts with long words. Besides, since words who occured the first time won't be encoded, long texts with more repeated words is expected to have higher compression rate. 
For the story.txt, compression rate is: 28.18%
For the story1.txt, compression rate is: 21.57%
For the story2.txt, compression rate is: 21.65%
If we put story, story1 and story2 together, compression rate is 32.75%