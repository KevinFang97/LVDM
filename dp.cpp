#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <cctype>
#include <functional>
#include <map>

using namespace std;

/*
process a file where:
  1.every line is a sentence
  2.no punctuation
*/

/*
in order to:
  1.attach every high frequency word with a unique int (1~wordsize)
  2.attach low frequency word to 0(UNK)
  4.padding with <EOS> (-1) so that every sentence has a fixed size
  5.give a file "Processed_Data", first line is the size of sentences, else storing the setence consist of int
  6.give a file "Dict", each high frequency word occupies a line, the number of the line is the attached number of the word
*/


string source_file_name = "source.txt";
string dict_file_name = "dict.txt";
string processed_file_name = "processed.txt"; 
int max_words = 0;
const int freq_boundary = 10; //words with frequency no less than freq_boundary are called high frequency words

/*SUPPORT FUNC*/
unsigned CountWords(const string& s ) {
  string x = s;
  replace_if(x.begin(), x.end(), std::ptr_fun<int, int>(isspace), ' ');
  x.erase(0, x.find_first_not_of(" "));
  if (x.empty()) return 0;
  return count(x.begin(), unique(x.begin(), x.end()), ' ') + !isspace(*s.rbegin());
}
/*SUPPORT FUNC*/

int main () {
  ifstream source_file;
  
  //read max words of a sentence
  source_file.open(source_file_name);
  string line;
  while(getline(source_file,line)) {
    unsigned words = CountWords(line);
    if(words > max_words) max_words = words;
  }
  source_file.close();
  cout << "Max_words is: " << max_words << endl; //for test
  
  
  //build a frequency mapping
  map<string,int> word_freq;
  map<string,int>::iterator freq_iter;
  source_file.open(source_file_name);
  string line2;
  //for every line in the file
  while(getline(source_file, line2)) {
    istringstream iss(line2);
    string word;
    //for every word in the line
    while(iss >> word) {
      freq_iter = word_freq.find(word);
      if(freq_iter == word_freq.end()) 
        word_freq.insert(pair<string, int>(word, 1));
      else word_freq[word] += 1; 
    }
  }
  source_file.close();
  
  //build an dictionary (word to int)
  map<string,int> word_dict;
  int index = 1;
  for(freq_iter = word_freq.begin(); freq_iter != word_freq.end(); freq_iter++) {
    if(freq_iter->second < freq_boundary)
      word_dict.insert(pair<string,int>(freq_iter->first,0)); //0(unk) if low freq
    else 
      word_dict.insert(pair<string,int>(freq_iter->first,index++))£»
  }
  
  //output the dictionary
  int dict_size = word_dict.size();
  map<string,int>::iterator dick_iter; //iterator of word dictionary
  {
    vector<string> dict_vec(dict_size,"");
    dict_vec[0] = "<UNK>";
    for(dick_iter = word_dict.begin(); dick_iter != word_dict.end(); dick_iter++) 
      if(dick_iter->second > 0) 
        dict_vec[dick_iter->second - 1] = dick_iter->first;      
    
    ofstream dict_file;
    dict_file.open(dict_file_name);
    dict_file << "<UNK>" << endl;
    for(int i = 0; i != dict_size; i++)
      dict_file << dict_vec[i] << endl;
    dict_file.close();
  } // use this block to clear memory after used
  
  //interpret(modify) the data into our format(int+padding)
  ofstream processed_file;
  processed_file.open(processed_file_name);
  source_file.open(source_file_name);
  //if both file opened, starting interpreting
  string source_line;
  int source_line_size;
  //for every line in the file
  while(getline(source_file,source_line)) {
    source_line_size = CountWords(source_line);
    istringstream iss(source_line);
    string word;
    //for every word in the line, interpret
    bool start_of_line = true;
    while(iss >> word) {
      dick_iter = word_dict.find(word);
      int word_index = 0;
      if(dick_iter != word_dict.end()) word_index = dick_iter->second;
      if(!start_of_line) processed_file << " ";
      processed_file << dick_iter->second;
      start_of_line = false;
    }
    //padding with -1(EOS)
    for(int i = 0; i <= max_words - source_line_size; i++)
      processed_file << " -1";
    processed_file << endl;
  } 
  source_file.close();
  processed_file.close();
  
}