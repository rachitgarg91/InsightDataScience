#include <fstream>
#include <string>

using namespace std;

int main()
{
	string tweet_in;
	int text_end,ts_end;
	string tweet_text;
	ifstream tweets_input;
	ofstream cleaned_tweets;
	cleaned_tweets.open("../tweet_output/ft1.txt"); // open the output file	
	tweets_input.open("../tweet_input/tweets.txt");  // open the input file
	int count = 0,pos,ucode=0;
	while(getline(tweets_input,tweet_in))
	{
		count=0;
		text_end = tweet_in.find("\"",109);  // length of text is variable, determine the ending of text by searching for "
		if (text_end>0)                      // Process the tweet iff the text has ending.
		{
			tweet_text = tweet_in.substr(109,text_end-109);
			pos = tweet_text.find("\\");
			while (pos>=0)
			{
				if ((tweet_text[pos+1]=='u'))
					tweet_text.erase(pos,6);    // Erase the unicodes from text
				else
					tweet_text.erase(pos,1);    // Erase other escape characters
					count++;
				pos = tweet_text.find("\\");       
			}
			if (count) ucode++;                         // Update number of tweets with unicode
			cleaned_tweets<<tweet_text + " (timestamp: "+tweet_in.substr(15,30)+")"<<endl;  // Write to the output file
		}
	}
	cleaned_tweets<<endl<<ucode<<" tweets contained unicode."<<endl;  // print number of weets with unicode.
	cleaned_tweets.close();
	tweets_input.close();
}
	
