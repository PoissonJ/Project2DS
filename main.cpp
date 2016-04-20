#include <algorithm>
#include <assert.h>
#include <iostream>
#include <vector>

struct Realm {
  std::string charm;
  std::vector<unsigned int> magi;
};

// returns the Levenshtein distance between two strings (a.k.a. how many
// incantations/magi are necessary to go from one realm to another)
static unsigned int leven(std::string a, std::string b) {

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
              subTable[i][j] = i;  // Minimum operations = j

          // If last characters are the same, ignore last character and
          // continue for remaining string
          else if (a[i-1] == b[j-1])
              subTable[i][j] = subTable[i-1][j-1];

          // If last characters are not the same, find minimum from all
          // possibilities
          else
              subTable[i][j] = 1 + std::min(std::min(
                                            subTable[i-j][j-1], // Replace
                                            subTable[i][j-1]),  // Insert
                                            subTable[i-1][j]    // Remove
                                           );
      }
  }

  return subTable[aLength][bLength];
}

// returns the longest increasing subsequence (a.k.a. the most magi we can use
// in a particular realm)
static std::vector<unsigned int> maxMagi(std::vector<unsigned int> v) {
  std::vector<unsigned int> ret;

  // todo

  return ret;
}

// prints minimum number of incantations needed to go from the Realm with charm
// `source` to the Realm with charm `target` given that all Realms of the world
// are presented in `graph` and also prints the number of gems required to pay
// the magi for the incantations
static void dijkstra(std::vector<Realm*> graph, std::string source,
                std::string target) {

  // todo

}

int main() {

  // number of realms
  unsigned int n;
  std::cin >> n;

  std::vector<Realm*> world;

  while (n--) {
    std::string charm;
    std::getline(std::cin, charm);

    unsigned int magiCount;
    std::cin >> magiCount;

    std::vector<unsigned int> magi;
    while (magiCount--) {
      unsigned int cost;
      std::cin >> cost;
      magi.push_back(cost);
    }

    Realm* realm = new Realm();
    realm->charm = charm;
    realm->magi = maxMagi(magi);
    world.push_back(realm);
  }

  std::string source;
  std::getline(std::cin, source);

  std::string destination;
  std::getline(std::cin, destination);

  dijkstra(world, source, destination);

  return 0;
}
