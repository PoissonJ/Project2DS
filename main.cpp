#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <map>
#include <stack>
#include <vector>
#include <utility>

using namespace std;

struct Realm {
  string charm;
  vector<unsigned int> magi;
  vector<Realm*> neighbors;
  int operator() (const pair<Realm*, unsigned int> &a,
                  const pair<Realm*, unsigned int> &b) {
    return a.second > b.second;
  }
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
static vector<unsigned int> maxMagi(vector<unsigned int> v) {
  vector<unsigned int> ret;

  // todo

  return ret;
}

// prints minimum number of incantations needed to go from the Realm with charm
// `source` to the Realm with charm `target` given that all Realms of the world
// are presented in `graph` and also prints the number of gems required to pay
// the magi for the incantations
static void dijkstra(vector<Realm*> world, Realm* source, Realm* destination) {
  map<string, unsigned int> dist;
  map<string, Realm*> prev;

  priority_queue<pair<Realm*, unsigned int>,
                      vector<pair<Realm*, unsigned int> >, Realm > Q;

  for (unsigned int i = 0; i < world.size(); i++) {
    Realm* v = world[i];
    if (v != source) {
      dist[v->charm] = numeric_limits<unsigned int>::max();
      prev[v->charm] = NULL;
    }
  }

  dist[source->charm] = 0;

  Q.push(make_pair(source, dist[source->charm]));

  while (!Q.empty()) {
    Realm* u = Q.top().first;
    Q.pop();

    // exit search early since we've just found the shortest path
    if (u == destination) {
      break;
    }

    for (unsigned int i = 0; i < u->neighbors.size(); i++) {
      Realm* v = u->neighbors[i];
      unsigned int alt = dist[u->charm] + leven(u->charm, v->charm);
      if (alt < dist[v->charm]) {
        dist[v->charm] = alt;
        prev[v->charm] = u;
        Q.push(make_pair(v, alt));
      }
    }
  }

  stack<Realm*> S;
  Realm* u = destination;
  while (prev[destination->charm] != NULL) {
    S.push(u);
    u = prev[u->charm];
  }
  S.push(u);

  // only the source Realm is in the path, therefore no path from the source to
  // the destination could be found
  if (S.size() == 1) {
    cout << "IMPOSSIBLE" << endl;
  }

  unsigned int gems = 0;
  unsigned int incantations = 0;
  while (!S.empty()) {
    Realm* r = S.top();
    S.pop();

    cout << r->charm << endl;
  }
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

  Realm* sourceRealm = NULL;
  Realm* destinationRealm = NULL;

  for (unsigned int i = 0; i < world.size(); i++) {
    if (world[i]->charm == source) {
      sourceRealm = world[i];
    }
    else if (world[i]->charm == destination) {
      destinationRealm = world[i];
    }
  }

  dijkstra(world, sourceRealm, destinationRealm);
  dijkstra(world, destinationRealm, sourceRealm);

  return 0;
}
