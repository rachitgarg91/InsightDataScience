#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <map>
#include <cstdlib>
#include <list>
#include <cstdio>

//#define DEBUG 1

using namespace std;

map <string,string> nextmonth;
map <string,int> daysinmonth;

void push_to_graph(map<string,map<string,int> >  *graph, vector<string>  new_tags, int * total_vertices, int * total_edges)
{
	#ifdef DEBUG
		printf("nts = %d   ",new_tags.size());
	#endif
	for (int i=0; i<new_tags.size(); i++)                               // Go through all the tags in the tweet
	{
		if ((*graph)[new_tags[i]].size()==0)                        // If this tag was not there in the graph
			(*total_vertices)++;                                // Increment total vertices
		for (int j=0; j<new_tags.size(); j++)                       //for each jth tag in the tweet
		{
			if (i!=j)                                           // which is not same as the ith tag
			{
				if ((*graph)[new_tags[i]][new_tags[j]]==0)  // if the jth tag was not in the ith tag's list
					(*total_edges)++;                   // increment total_edges
				(*graph)[new_tags[i]][new_tags[j]]++;       // Increment the weight of this edge
			}
		}
	}
}
void pop_from_graph(map<string,map<string,int> > * graph, vector<string>  old_tags, int * total_vertices, int * total_edges)
{
	#ifdef DEBUG
		printf("ots = %d   ",old_tags.size());
	#endif
	for(int i=0; i<old_tags.size(); i++)                                      // for all ith tags in the tweet
	{
		for (int j=0; j<old_tags.size(); j++)                             // go through all the jth tags in the tweet 
		{
			if (i!=j)                                                 // we do not want a self edge
			{
				if ((*graph)[old_tags[i]][old_tags[j]] == 1)      // if the weight of this edge was just 1
				{
					(*graph)[old_tags[i]].erase(old_tags[j]);  // erase this jth tag from adjacency list of ith tag
					(*total_edges)--;                          // dectement total_edges
				}
				else
					(*graph)[old_tags[i]][old_tags[j]]--;   // otherwise, just reduce the weight of this edge
 			}                                                       // this means, there is a tweet withing 60 sec window which has caused 
									        // this edge to remain
		}
		if ((*graph)[old_tags[i]].size() == 0)                          // if adjacency list of ith list is empty
		{								// erase it, decrement total_vertices
			(*graph).erase(old_tags[i]);
			(*total_vertices)--;
		}
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ts_within60: This function takes in the time stamps of two tweets and tells whether they are 60 seconds apart.
//It takes care of all the boundary conditions like, hour transition, day transition, month transition, year transition, leap year.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ts_within60(string ts_old, string ts_new)
{
	int time_old=0,time_new = 0;
	ts_old.erase(16,1);
	ts_old.erase(13,1);
	ts_new.erase(16,1);
	ts_new.erase(13,1);
	time_old = atoi((ts_old.substr(11,6)).c_str());
	time_new = atoi((ts_new.substr(11,6)).c_str());
	if (ts_old.substr(0,10) == ts_new.substr(0,10) && ts_old.substr(24,4) == ts_new.substr(24,4))  // if date and year is same, means within a day
	{
		#ifdef DEBUG
			cout<<"same date, month and year\n";
		#endif
		if (time_old+100>=time_new) return 1;
		else if ((time_old/100)%100 == 59)    // Transition of hour
		{
			if (time_old+4100>=time_new) return 1;
		}
		return 0;
	}
	else if (ts_old.substr(24,4) == ts_new.substr(24,4) && ts_old.substr(4,3) == ts_new.substr(4,3) &&\
		 atoi((ts_new.substr(8,2)).c_str())-1 == atoi((ts_old.substr(8,2)).c_str()))  
	{	// if year is same and month is same and older date is 1 more than the older
	
		#ifdef DEBUG
			cout<<"The date is different\n"<<(time_old/100)%100;
		#endif
		if ((time_old/100)%100 == 59 && (time_old/10000)==23)  // If it is of the form 23:59:XX
		{ 
			if (time_old-235900 >=time_new) return 1;  // Change of day within a month
		}
		return 0;
	}
	else if (ts_old.substr(24,4) == ts_new.substr(24,4) && nextmonth[ts_old.substr(4,3)] == ts_new.substr(4,3)) 
	{	 // If year is same and later ts is from one later month
	
		#ifdef DEBUG	
			cout<<"month is different\n";
		#endif
		if ((time_old/100)%100 == 59 && (time_old/10000)==23 && ts_new.substr(8,2) == "01") 
		{
			if (ts_old.substr(4,3) != "Feb" && daysinmonth[ts_old.substr(4,3).c_str()]==atoi((ts_old.substr(8,2)).c_str()))
			{
				if (time_old-235900 >= time_new) return 1;
			}
			else if(ts_old.substr(4,3) == "Feb" && time_old-235900 >= time_new) 
			{
				if (atoi(ts_old.substr(24,4).c_str())%4 == 0 && ts_old.substr(8,2) == "29")  // Leap Year
					return 1;
				if (atoi(ts_old.substr(24,4).c_str())%4 != 0 && ts_old.substr(8,2) == "28") 
					return 1;	
			}
			return 0;
		}
	}
	else if (atoi(ts_old.substr(24,4).c_str())+1 == atoi(ts_new.substr(24,4).c_str()) && ts_new.substr(4,3)=="Jan" && ts_old.substr(4,3)=="Dec" )
	{	 // different year
		if (ts_old.substr(8,2)=="31" && ts_new.substr(8,2)=="01")
		{
			//cout<<"year is different\n";
			if ((time_old/100)%100 == 59 && (time_old/10000)==23)  // If it is of the form 23:59:XX
			{  
				if (time_old-235900 >=time_new) return 1;  // Change of day within a month
			}
		}
		return 0;
	}
	else
		return 0;
}
void init()
{
	nextmonth["Jan"] = "Feb";
	nextmonth["Feb"] = "Mar";
	nextmonth["Mar"] = "Apr";
	nextmonth["Apr"] = "May";
	nextmonth["May"] = "Jun";
	nextmonth["Jun"] = "Jul";
	nextmonth["Jul"] = "Aug";
	nextmonth["Aug"] = "Sep";
	nextmonth["Sep"] = "Oct";
	nextmonth["Oct"] = "Nov";
	nextmonth["Nov"] = "Dec";
	daysinmonth["Jan"] = 31;
	daysinmonth["Feb"] = 28;
	daysinmonth["Mar"] = 31;
	daysinmonth["Apr"] = 30;
	daysinmonth["May"] = 31;
	daysinmonth["Jun"] = 30;
	daysinmonth["Jul"] = 31;
	daysinmonth["Aug"] = 31;
	daysinmonth["Sep"] = 30;
	daysinmonth["Oct"] = 31;
	daysinmonth["Nov"] = 30;
}

int main()
{	
	init();
	string tweet_in;
	int tag_s;int tag_e; int ts_start;
	ifstream tweets_input;
	FILE * ft2;
	ft2 = fopen("../tweet_output/ft2.txt","w+");
	tweets_input.open("../tweet_input/tweets.txt");  // open the input file
	map <string, map<string,int> >  graph; // data structure for graph, a map of map, makes searching logn.
	list<vector<string> > tags;            // data structure for all the tags in 1 minute window. List of vectors. Makes popping/pushing O(1).
	list<string> timestamps;
	vector<string> latest_tags;
	int total_vertices=0; int total_edges=0; int tweet_end; int pos; int text_end;
	string tweet_raw;
	
	while(getline(tweets_input,tweet_raw))
	{
		text_end = tweet_raw.find("\"",109);  // length of text is variable, determine the ending of text by searching for "
		if (text_end>0)                      // Process the tweet iff the text has ending.
		{
			tweet_in = tweet_raw.substr(109,text_end-109);
			pos = tweet_in.find("\\");
			while (pos>=0)
			{
				if ((tweet_in[pos+1]=='u'))
					tweet_in.erase(pos,6);    // Erase the unicodes from text
				else
					tweet_in.erase(pos,1);    // Erase other escape characters
				pos = tweet_in.find("\\");       
			}
			#ifdef DEBUG
				cout<<"Read one line from the file"<<endl;
			#endif
			latest_tags.clear();
			timestamps.push_back(tweet_raw.substr(15,30));
		
			#ifdef DEBUG
				cout<<"timestamps compared are "<<timestamps.front()<<" "<<timestamps.back()<<endl;
			#endif
				
			while (!ts_within60(timestamps.front(),timestamps.back())) //evict from graph all the tweets which are older than 60 seconds
			{
				#ifdef DEBUG
					cout<<"timestamp is not within 60 "<<timestamps.front()<<" "<<timestamps.back()<<endl;
				#endif

				if ((tags.front()).size()>1)
					pop_from_graph(&graph, tags.front(), &total_vertices, &total_edges);  //update graph

				#ifdef DEBUG
					cout<<"Popped: total_edges = "<<total_edges<<" total_vertices = "<<total_vertices<<endl;
				#endif
	
				tags.pop_front();           // pop the oldest tag
				timestamps.pop_front();     // pop corresponding TS
			}
			tag_s = tweet_in.find("#")+1;  // Search for hashtag in the tweet text
			tag_e = tag_s;
			while(tag_e>0)
    			{
				int valid_hashtag=0;
        			while((tweet_in[tag_e]!=' ') && tweet_in[tag_e])
				{
					tweet_in[tag_e] = tolower(tweet_in[tag_e]);  //convert all the characters in hashtag to lowercase
					if (ispunct(tweet_in[tag_e])){               //if its a puctuation, remove it.
						 tweet_in.erase(tag_e,1);
					}
					else
					{
						if (tweet_in[tag_e] != ' ') valid_hashtag=1;  // if its an empty tag, ignore
            						tag_e++;
					}	 
				}
				if (valid_hashtag)
        			latest_tags.push_back(tweet_in.substr(tag_s, tag_e-tag_s));
				
				#ifdef DEBUG
					cout<<"found: "<<(tweet_in.substr(tag_s, tag_e-tag_s))<<"   ";
				#endif
				tag_s = tweet_in.find("#",tag_e)+1;   // find the next hash tag 
				tag_e=tag_s;
			}
			#ifdef DEBUG
				cout<<endl;
			#endif
		
			if (ts_within60(timestamps.front(),timestamps.back()))  // If the timestamp of this tweet is within 60, Add to the graph
			{
				tags.push_back(latest_tags);     //push tags to the list 
				if (latest_tags.size()>1)        //if the number of tags in the tweet more than or equal 2
				{
					#ifdef DEBUG
						cout<<"\n\n\n";
						cout<<"More than 2 hashtags"<<endl;
						cout<<"timestamp is within 60 "<<timestamps.front()<<" "<<timestamps.back()<<endl;
					#endif
					push_to_graph(&graph, latest_tags, &total_vertices, &total_edges);  // update the graph
					#ifdef DEBUG
						cout<<"Pushed: total_edges = "<<total_edges<<" total_vertices = "<<total_vertices<<endl;
					#endif
				}
			}
			if (total_edges==0)
				fprintf(ft2,"0.00\n");   // If total edges is 0, print 0 (0/0)
			else 
				fprintf(ft2,"%.2f\n",(double)((float)total_edges/(float)total_vertices)); // output the rolling average to the file
		}
	}
	fclose(ft2);
	tweets_input.close();
}	
