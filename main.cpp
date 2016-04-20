#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Realm {
  string charm;
  vector<unsigned int> magi;
};

// returns the Levenshtein distance between two strings (a.k.a. how many
// incantations/magi are necessary to go from one realm to another)
static unsigned int leven(string a, string b) {

  /* Time complexity O(aLength x bLength) */

  const unsigned int aLength = a.length();
  const unsigned int bLength = b.length();

  // Create a table to store subproblems
  unsigned int subTable[aLength+1][bLength+1];

  // Fill table bottom up
  for (unsigned int i = 0; i <= aLength; i++) {
      for (unsigned int j = 0; j <= bLength; j++) {

          // First string empty, insert all of second string
          if (i == 0)
              subTable[i][j] = j;  // Minimum operations = j

          // Second string empty, insert all of first string
          else if (j == 0)
              subTable[i][j] = i;  // Minimum operations = i

          // If last characters are the same, ignore last character and
          // continue for remaining string
          else if (a[i-1] == b[j-1])
              subTable[i][j] = subTable[i-1][j-1];

          // If last characters are not the same, find minimum from all
          // possibilities
          else
              subTable[i][j] = 1 + min(min(
                                            subTable[i-1][j-1], // Replace
                                            subTable[i][j-1]),  // Insert
                                            subTable[i-1][j]    // Remove
                                           );
      }
  }

  return subTable[aLength][bLength];
}

// returns the longest increasing subsequence (a.k.a. the most magi we can use
// in a particular realm)
static vector<unsigned int> maxMagi( vector<unsigned int> a){

  unsigned int n = a.size();

  vector<unsigned int> returnVector;
  // create array for dynamic method
  unsigned int *lis = new unsigned int[n];
  unsigned int j,k,maxLIS = 0;

  // initialize array with 1s
  for(unsigned int i=0; i<n; i++)
    lis[i] = 1;

  // calculate LIS values in dynamic method
  for(unsigned int i=1; i<n; i++){
    for(unsigned int j=0; j<i; j++){
      if(a[i] > a[j] && lis[i] < lis[j]+1)
        lis[i] = lis[j]+1;
    }
  }

  // find maxLIS
  for(unsigned int i=0; i<n; i++){
    if(maxLIS < lis[i]){
      maxLIS = lis[i];
      returnVector.push_back(a[i]);
    }
  }

  return returnVector;
}

// prints minimum number of incantations needed to go from the Realm with charm
// `source` to the Realm with charm `target` given that all Realms of the world
// are presented in `graph` and also prints the number of gems required to pay
// the magi for the incantations
static void dijkstra(vector<Realm*> graph, string source,
                string target) {

  // todo

}

int main() {

  // number of realms
  unsigned int n;
  cin >> n;

  vector<Realm*> world;

  while (n--) {
    string charm;
    getline(cin, charm);

    unsigned int magiCount;
    cin >> magiCount;

    vector<unsigned int> magi;
    while (magiCount--) {
      unsigned int cost;
      cin >> cost;
      magi.push_back(cost);
    }

    Realm* realm = new Realm();
    realm->charm = charm;
    realm->magi = maxMagi(magi);
    world.push_back(realm);
  }

  string source;
  getline(cin, source);

  string destination;
  getline(cin, destination);

  dijkstra(world, source, destination);

  return 0;
}
