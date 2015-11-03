#InsightDataScience

## Run.sh:
run.sh compiles the two C++ programs (average_degree.cpp and tweets_cleaned.cpp) and then executes them back to back.
The compilation is done with compiler optimization "-O3". So the run time of the features cannot be called as the runtime of run.sh as it includes both the compilation and execution.

## Dependencies:
The code is written in basic C/C++ with almost no prebuilt libraries, so there are no known dependencies.

## Feature 1: tweets_cleaned.cpp

###Input: The program reads the file "tweets.txt" located at a relative location of "../tweet_input/tweets.txt".
The program expects a new tweet on new line in the JSON format, in which length of text is variable.

###Functionality: 
The program reads a tweet and extracts text and timestamp from the it. 
1. It removes all the unicodes from the text.
2. It also removes all the escape characters, if it is not a unicode to make tweet readable.
3. It counts number of tweets which had unicodes in them. 

###Output: 
The program writes to a file "ft1.txt" located at "../tweet_output/". The format of output is:
text (timestamp: xxx)
At the end of the file, it prints out the number of tweets which required unicode cleaning.

## Feature 2: average_degree.cpp:

###Input: 
The program reads the file "tweets.txt" located at a relative location of "../tweet_input/tweets.txt".
The program expects a new tweet on new line in the JSON format, in which length of text is variable. 

###Functionality:
 This program extracts time stamp and cleans text from the tweet just like the feature 1 again. Then it calculates the rolling average for 60 seconds of history.
1. It builds a graph out of 60 seconds worth of tweets such that there is an edge between A and B iff there was a tweet in which A and B tagged simulataneously.   
2. It prints out the average degree of vertex, i.e. sum of degrees of all the vertices/number of vertices in graph.

###Output: 
The program writes to a file "ft2.txt" located at "../tweet_output/". The program writes the average value after reading every tweet.
