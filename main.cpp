#include <iostream>
#include <vector>

struct Realm {
  std::string charm;
  std::vector<unsigned int> magi;
};

// returns the Levenshtein distance between two strings (a.k.a. how many
// incantations/magi are necessary to go from one realm to another)
static unsigned int leven(std::string a, std::string b) {
  unsigned int distance = 0;

  // todo

  return distance;
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
