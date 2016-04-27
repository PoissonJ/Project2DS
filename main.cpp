#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <map>
#include <stack>
#include <string>
#include <vector>
#include <utility>

using namespace std;

struct Realm {
  string charm;
  vector<unsigned int> magi;
  vector<Realm*> neighbors;

  // This is used by the min heap to find the closest Realm
  int operator() (const pair<Realm*, unsigned int> &a,
                  const pair<Realm*, unsigned int> &b) {
    return a.second > b.second;
  }
};

// Returns the Levenshtein distance between two strings (a.k.a. how many
// incantations/magi are necessary to go from one realm to another)
static unsigned int leven(string a, string b) {
  const unsigned int aLength = a.length();
  const unsigned int bLength = b.length();

  unsigned int subTable[aLength+1][bLength+1];

  // Fill table bottom up
  for (unsigned int i = 0; i <= aLength; i++) {
    for (unsigned int j = 0; j <= bLength; j++) {

      // First string empty, so we must insert all of second string. Minimum
      // number of operations is therefore j.
      if (i == 0) {
        subTable[i][j] = j;
      }

      // Second string empty, so we must insert all of first string. Minimum
      // number of operations is therefore i.
      else if (j == 0) {
        subTable[i][j] = i;
      }

      // If the last character of each string are the same, ignore that last
      // character and continue for remaining string
      else if (a[i-1] == b[j-1]) {
        subTable[i][j] = subTable[i-1][j-1];
      }

      // If last characters are not the same, find minimum from all
      // possibilities
      else {
        subTable[i][j] = 1 + min(min(
          subTable[i-1][j-1], // Replace
          subTable[i][j-1]),  // Insert
          subTable[i-1][j]    // Remove
         );
      }
    }
  }

  return subTable[aLength][bLength];
}

// Returns the longest increasing subsequence (a.k.a. the most magi we can use
// in a particular realm)
static vector<unsigned int> maxMagi(vector<unsigned int> a) {

  // Each row in this matrix will be an increasing subsequence of a
  vector<vector<unsigned int> > L(a.size());

  L[0].push_back(a[0]);

  for (unsigned int i = 1; i < a.size(); i++) {
    for (unsigned int j = 0; j < i; j++) {
      if (a[j] < a[i] && L[i].size() < L[j].size() + 1) {
        L[i] = L[j];
      }
    }

    L[i].push_back(a[i]);
  }

  // Find the longest row (= longest increasing subsequence)
  unsigned int maxSeq = 0;
  unsigned int maxI = 0;
  for (unsigned int i = 1; i < L.size(); i++) {
    if (L[i].size() > maxSeq) {
      maxSeq = L[i].size();
      maxI = i;
    }
  }

  return L[maxI];
}

// Prints minimum number of incantations needed to go from the Realm with charm
// `source` to the Realm with charm `target` given that all Realms of the world
// are presented in `world` and also prints the number of gems required to pay
// the magi for the incantations
static void dijkstra(vector<Realm*> world, Realm* source, Realm* destination) {
  map<string, unsigned int> dist;
  map<string, Realm*> prev;

  // Min heap of (Realm, dist) pairs
  priority_queue<pair<Realm*, unsigned int>,
                      vector<pair<Realm*, unsigned int> >, Realm > Q;

  for (unsigned int i = 0; i < world.size(); i++) {
    Realm* v = world[i];
    dist[v->charm] = numeric_limits<unsigned int>::max();
    prev[v->charm] = NULL;
  }

  dist[source->charm] = 0;

  Q.push(make_pair(source, dist[source->charm]));

  while (!Q.empty()) {
    Realm* u = Q.top().first;
    Q.pop();

    // Exit search early since we've just found the shortest path
    if (u == destination) {
      break;
    }

    for (unsigned int i = 0; i < u->neighbors.size(); i++) {
      Realm* v = u->neighbors[i];

      unsigned int alt;
      if (dist[u->charm] == numeric_limits<unsigned int>::max()) {
        alt = leven(u->charm, v->charm);
      }
      else {
        alt = dist[u->charm] + leven(u->charm, v->charm);
      }

      if (alt < dist[v->charm]) {
        dist[v->charm] = alt;
        prev[v->charm] = u;
        Q.push(make_pair(v, alt));
      }
    }
  }

  // Traceback through `prev` from the destination to the source
  stack<Realm*> S;
  Realm* u = destination;
  while (prev[u->charm] != NULL) {
    S.push(u);
    u = prev[u->charm];
  }
  S.push(u);

  // Only the destination Realm is in the path, therefore no path from the
  // source to the destination could be found
  if (S.size() == 1) {
    cout << "IMPOSSIBLE" << endl;
    return;
  }

  // Go through the stack, which is essentially going from source to destination
  unsigned int gems = 0;
  unsigned int incantations = 0;
  while (!S.empty()) {
    Realm* r = S.top();
    S.pop();

    // If the stack is now empty, then we are at the destination realm and there
    // is no need to add any more incantations or gems
    if (!S.empty()) {
      unsigned int incants = leven(r->charm, S.top()->charm);
      incantations += incants;
      for (unsigned int i = 0; i < incants; i++) {
        gems += r->magi[i];
      }
    }
  }

  cout << incantations << ' ' << gems << endl;
}

int main() {

  // number of realms
  unsigned int n;
  cin >> n;

  vector<Realm*> world;

  while (n--) {
    string charm;
    while (charm.empty()) {
      getline(cin, charm);
    }

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
  while (source.empty()) {
    getline(cin, source);
  }

  string destination;
  while (destination.empty()) {
    getline(cin, destination);
  }

  Realm* sourceRealm = NULL;
  Realm* destinationRealm = NULL;

  for (unsigned int i = 0; i < world.size(); i++) {
    // Get pointers to the source and destination realms (for passing to the
    // search later)
    if (world[i]->charm == source) {
      sourceRealm = world[i];
    }
    else if (world[i]->charm == destination) {
      destinationRealm = world[i];
    }

    // Get the neighbors of this ith realm by going through every other realm
    // and comparing the edit distance to the number of available magi. If the
    // edit distance is less than or equal to the number of magi, then we can
    // say that the jth realm is a neighbor of the ith (one-way).
    for (unsigned int j = 0; j < world.size(); j++) {
      // Realms are not neighbors of themselves
      if (i == j) {
        continue;
      }

      unsigned cost = leven(world[i]->charm, world[j]->charm);
      if (cost <= world[i]->magi.size()) {
        world[i]->neighbors.push_back(world[j]);
      }
    }
  }

  dijkstra(world, sourceRealm, destinationRealm);
  dijkstra(world, destinationRealm, sourceRealm);

  return 0;
}
