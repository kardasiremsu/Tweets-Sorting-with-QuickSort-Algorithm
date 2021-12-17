/*
Author: İremsu Kardaş
Student Number: 150180081
Date: 24.11.2021
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;


//Tweet data type for using in vector
class Tweet{
    public:
        string tweet_id;
        string tweet_datetime;
        string tweet_unixtime;
        string artist_name;
        string track_title;
        string country_name;
        //Constructor
        Tweet(string tweet_id = "", string tweet_datetime = "", string tweet_unixtime = "", string artist_name = "", string track_title = "", string country_name = ""){
            this->tweet_id = tweet_id;
            this->tweet_datetime = tweet_datetime;
            this->tweet_unixtime = tweet_unixtime;
            this->artist_name = artist_name;
            this->track_title = track_title;
            this->country_name = country_name;
        };
};

void swap(Tweet *a, Tweet *b){
    Tweet temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
string tolower(string str) // to make case intensitive comparing, convert all string to lowercase
{
    locale c;
    for (string::size_type i=0; i<str.length(); ++i)
        str[i] = std::tolower(str[i],c);
    
    return str;
}

int Partition(vector<Tweet> &A, int p, int r){ //partition part sorts the array firstly looking country then artist then unixtime
    Tweet pivot = A.at(r); 
    int i = p-1;
    for(int j=p; j<= r-1; j++){
        if(tolower(A.at(j).country_name) < tolower(pivot.country_name)){
            i++;
            swap(&A.at(i), &A.at(j));
        }
        else if(A.at(j).country_name == pivot.country_name && tolower(A.at(j).artist_name) < tolower(pivot.artist_name)){
            //if country name same sort in case artist name
            i++;
            swap(&A.at(i), &A.at(j));
        }
        else if(A.at(j).country_name == pivot.country_name && tolower(A.at(j).artist_name) == tolower(pivot.artist_name) && A.at(j).tweet_unixtime < pivot.tweet_unixtime){
            // artist name and country name same, sort for unix time
            i++;
            swap(&A.at(i), &A.at(j));  
            }
        }
    swap(&A.at(i+1), &A.at(r));
    return (i+1);
}

void Deterministic_Quicksort(vector<Tweet> &A, int p, int r){ 
    if(p < r){
        int q = Partition(A,p,r); //pivot
        Deterministic_Quicksort(A,p,q-1); //before pivot
        Deterministic_Quicksort(A,q+1,r); //after pivot
    }
}

int RandomizedPartition(vector<Tweet> &A, int p, int r){
    int i= rand()%(r-p+1)+p; //random number generator based on between p and r
    swap(&A.at(i), &A.at(r));//Swapping 
    return Partition(A,p,r);
}

void RandomizedQuicksort(vector<Tweet> &A, int p, int r){
    if (p < r){
        int q = RandomizedPartition(A,p,r);
        RandomizedQuicksort(A,p,q-1); //before pivot
        RandomizedQuicksort(A,q+1, r); //after pivot
    }
}


int main(int argc, char **argv){
    clock_t time;
	int no_tweets = atoi(argv[1]); // number of integers to read
	string algorithm = argv[2];
	string file_name = argv[3];
	string output_file_name = argv[4];
    ifstream input_file(file_name);
	ofstream output_file;
    vector<Tweet> tweets;
    string line,header;

    //Read from input csv file 
    getline(input_file, header);
    for(int i=1; i<=no_tweets ; i++){
        getline(input_file,line);
        string item;
        stringstream string_stream(line);
        Tweet t;
        //parsing by tab
        getline(string_stream,item,'\t');
        t.tweet_id = item;
        getline(string_stream,item,'\t');
        t.tweet_datetime = item;
        getline(string_stream,item,'\t');
        t.tweet_unixtime = item;
        getline(string_stream,item,'\t');
        t.artist_name = item;
        getline(string_stream,item,'\t');
        t.track_title = item;
        getline(string_stream,item,'\t');
        t.country_name = item;
        //tweet pushing to tweets vector which all tweets are stored here
        tweets.push_back(t);
    }
    input_file.close(); //all tweets added to tweets vector

    output_file.open(output_file_name);
    output_file << header << endl; 

    time = clock(); 
    //Decide which algorithm works 
    if(algorithm == "deterministic"){
        Deterministic_Quicksort(tweets, 0, (tweets.size())-1);
    }
    if(algorithm == "randomized"){
        RandomizedQuicksort(tweets, 0, (tweets.size())-1);
    }
  
    // opening file to write
	if (output_file.fail()) {
		cerr << "Error output File" << endl;
		exit(EXIT_FAILURE);
	}
    //write to output file sorted array
    for(int i=0; i<no_tweets; i++){
       output_file << tweets.at(i).tweet_id << '\t' << tweets.at(i).tweet_datetime  << '\t' << tweets.at(i).tweet_unixtime  << '\t' 
        << tweets.at(i).artist_name  << '\t' << tweets.at(i).track_title << '\t' << tweets.at(i).country_name <<  endl;
   }
    output_file.close();
 
    time = clock() - time;
    cout << "Sorted in "  << (float)time/CLOCKS_PER_SEC * 1000 <<  " milliseconds by using " << algorithm << " pivot selection." << endl;
    return EXIT_SUCCESS;
}
