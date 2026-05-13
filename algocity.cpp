/*
============================================================
         ALGOCITY - Smart City Simulation System
         A Comprehensive DSA Project in C++
============================================================
  Modules:
    1. City Data Manager         (STL)
    2. Population & Traffic      (Array & Binary Search)
    3. Math & Encryption Engine  (Number Theory & Bit Manip)
    4. Resource War Simulator    (Game Theory)
    5. Road Network Analyzer     (Graph & DSU)
    6. Power Grid Tree System    (Advanced Tree Algorithms)
============================================================
*/

#include <bits/stdc++.h>
using namespace std;

// ============================================================
//  UTILITIES / DISPLAY HELPERS
// ============================================================
void printLine(char c = '=', int n = 60) {
    for (int i = 0; i < n; i++) cout << c;
    cout << "\n";
}

void printHeader(const string& title) {
    cout << "\n";
    printLine('=');
    cout << "  " << title << "\n";
    printLine('=');
}

void printSubHeader(const string& title) {
    cout << "\n";
    printLine('-');
    cout << "  >> " << title << "\n";
    printLine('-');
}

void pauseScreen() {
    cout << "\n[Press ENTER to continue...]";
    cin.ignore();
    cin.get();
}

// ============================================================
//  MODULE 1: CITY DATA MANAGER (STL)
// ============================================================

struct City {
    string name;
    long long population;
    int resources;      // 0-100
    double aqi;         // Air Quality Index
    int crimeIndex;
    string zone;        // "A", "B", "C"

    bool operator<(const City& o) const {
        return population < o.population;
    }
};

struct CmpByResource {
    bool operator()(const City& a, const City& b) const {
        return a.resources > b.resources; // max first
    }
};

class CityDataManager {
private:
    vector<City> cities;
    map<string, City> cityMap;               // name -> City
    unordered_map<string, int> resourceMap;   // name -> resource index
    set<string> registeredZones;
    multiset<long long> populationSet;
    priority_queue<City, vector<City>, CmpByResource> resourcePQ;
    stack<string> recentActions;
    queue<string> maintenanceQueue;
    deque<pair<string, long long>> recentPopUpdates;

    void rebuildPQ() {
        while (!resourcePQ.empty()) resourcePQ.pop();
        for (auto& c : cities) resourcePQ.push(c);
    }

public:
    void addCity(const string& name, long long pop, int res, double aqi, int crime, const string& zone) {
        City c = {name, pop, res, aqi, crime, zone};
        cities.push_back(c);
        cityMap[name] = c;
        resourceMap[name] = res;
        registeredZones.insert(zone);
        populationSet.insert(pop);
        recentActions.push("Added: " + name);
        recentPopUpdates.push_back({name, pop});
        if (recentPopUpdates.size() > 5) recentPopUpdates.pop_front();
        rebuildPQ();
        cout << "  [+] City '" << name << "' added successfully.\n";
    }

    void updatePopulation(const string& name, long long newPop) {
        if (cityMap.find(name) == cityMap.end()) {
            cout << "  [!] City not found.\n"; return;
        }
        populationSet.erase(populationSet.find(cityMap[name].population));
        cityMap[name].population = newPop;
        for (auto& c : cities) if (c.name == name) c.population = newPop;
        populationSet.insert(newPop);
        recentActions.push("Updated Pop: " + name);
        recentPopUpdates.push_back({name, newPop});
        if (recentPopUpdates.size() > 5) recentPopUpdates.pop_front();
        rebuildPQ();
        cout << "  [+] Population updated.\n";
    }

    void addMaintenance(const string& task) {
        maintenanceQueue.push(task);
        cout << "  [+] Maintenance task enqueued: " << task << "\n";
    }

    void processMaintenance() {
        if (maintenanceQueue.empty()) { cout << "  [!] No tasks.\n"; return; }
        cout << "  [*] Processing: " << maintenanceQueue.front() << "\n";
        maintenanceQueue.pop();
    }

    void displayAllCities() {
        if (cities.empty()) { cout << "  [!] No cities registered.\n"; return; }
        printSubHeader("All Registered Cities");
        cout << left << setw(18) << "Name"
             << setw(14) << "Population"
             << setw(10) << "Resources"
             << setw(8)  << "AQI"
             << setw(12) << "Crime Idx"
             << setw(6)  << "Zone" << "\n";
        printLine('-');
        for (auto& c : cities) {
            cout << left << setw(18) << c.name
                 << setw(14) << c.population
                 << setw(10) << c.resources
                 << setw(8)  << fixed << setprecision(1) << c.aqi
                 << setw(12) << c.crimeIndex
                 << setw(6)  << c.zone << "\n";
        }
    }

    void sortAndDisplay(const string& by) {
        vector<City> temp = cities;
        if (by == "population")
            sort(temp.begin(), temp.end(), [](const City& a, const City& b){ return a.population > b.population; });
        else if (by == "resources")
            sort(temp.begin(), temp.end(), [](const City& a, const City& b){ return a.resources > b.resources; });
        else if (by == "aqi")
            sort(temp.begin(), temp.end(), [](const City& a, const City& b){ return a.aqi < b.aqi; });
        else if (by == "crime")
            sort(temp.begin(), temp.end(), [](const City& a, const City& b){ return a.crimeIndex < b.crimeIndex; });

        printSubHeader("Sorted by: " + by);
        cout << left << setw(18) << "Name"
             << setw(14) << "Population"
             << setw(10) << "Resources"
             << setw(8)  << "AQI"
             << setw(12) << "Crime Idx"
             << setw(6)  << "Zone" << "\n";
        printLine('-');
        for (auto& c : temp) {
            cout << left << setw(18) << c.name
                 << setw(14) << c.population
                 << setw(10) << c.resources
                 << setw(8)  << fixed << setprecision(1) << c.aqi
                 << setw(12) << c.crimeIndex
                 << setw(6)  << c.zone << "\n";
        }
    }

    void rangePopulationQuery(long long lo, long long hi) {
        // lower_bound / upper_bound on multiset
        auto it1 = populationSet.lower_bound(lo);
        auto it2 = populationSet.upper_bound(hi);
        cout << "\n  Cities with population in [" << lo << ", " << hi << "]:\n";
        int cnt = 0;
        for (auto& c : cities) {
            if (c.population >= lo && c.population <= hi) {
                cout << "    -> " << c.name << " : " << c.population << "\n";
                cnt++;
            }
        }
        cout << "  Count in multiset range: " << distance(it1, it2) << "\n";
        if (!cnt) cout << "  (none)\n";
    }

    void topResourceCities(int k) {
        printSubHeader("Top " + to_string(k) + " Cities by Resource");
        priority_queue<City, vector<City>, CmpByResource> pq = resourcePQ;
        int i = 0;
        while (!pq.empty() && i < k) {
            auto c = pq.top(); pq.pop();
            cout << "  " << (i+1) << ". " << c.name << " -> Resources: " << c.resources << "\n";
            i++;
        }
    }

    void showRecentActions(int k = 5) {
        printSubHeader("Recent Actions (stack - LIFO)");
        stack<string> temp = recentActions;
        int i = 0;
        while (!temp.empty() && i < k) {
            cout << "  " << (i+1) << ". " << temp.top() << "\n";
            temp.pop(); i++;
        }
    }

    void showRecentPopUpdates() {
        printSubHeader("Recent Population Updates (deque)");
        for (auto& p : recentPopUpdates)
            cout << "  " << p.first << " -> " << p.second << "\n";
    }

    void showZones() {
        printSubHeader("Registered Zones (set)");
        for (auto& z : registeredZones)
            cout << "  Zone: " << z << "\n";
    }

    void showMinMaxPopulation() {
        if (populationSet.empty()) { cout << "  [!] No data.\n"; return; }
        cout << "\n  Min Population: " << *populationSet.begin() << "\n";
        cout << "  Max Population: " << *populationSet.rbegin() << "\n";
    }

    bool hasCity(const string& name) { return cityMap.count(name); }
    vector<City>& getCities() { return cities; }
    int size() { return cities.size(); }

    void runMenu() {
        int ch;
        do {
            printHeader("MODULE 1: City Data Manager (STL)");
            cout << "  1. Add City\n";
            cout << "  2. Display All Cities\n";
            cout << "  3. Sort Cities\n";
            cout << "  4. Range Population Query\n";
            cout << "  5. Top-K Resource Cities\n";
            cout << "  6. Update Population\n";
            cout << "  7. Add Maintenance Task\n";
            cout << "  8. Process Maintenance Task\n";
            cout << "  9. Show Recent Actions\n";
            cout << "  10. Show Recent Pop Updates (deque)\n";
            cout << "  11. Show Zones (set)\n";
            cout << "  12. Min/Max Population\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> ch;
            if (ch == 1) {
                string name, zone; long long pop; int res, crime; double aqi;
                cout << "  Name: "; cin >> name;
                cout << "  Population: "; cin >> pop;
                cout << "  Resources(0-100): "; cin >> res;
                cout << "  AQI: "; cin >> aqi;
                cout << "  Crime Index: "; cin >> crime;
                cout << "  Zone(A/B/C): "; cin >> zone;
                addCity(name, pop, res, aqi, crime, zone);
            } else if (ch == 2) displayAllCities();
            else if (ch == 3) {
                cout << "  Sort by (population/resources/aqi/crime): ";
                string by; cin >> by;
                sortAndDisplay(by);
            } else if (ch == 4) {
                long long lo, hi;
                cout << "  Low: "; cin >> lo;
                cout << "  High: "; cin >> hi;
                rangePopulationQuery(lo, hi);
            } else if (ch == 5) {
                int k; cout << "  K: "; cin >> k;
                topResourceCities(k);
            } else if (ch == 6) {
                string name; long long pop;
                cout << "  City name: "; cin >> name;
                cout << "  New population: "; cin >> pop;
                updatePopulation(name, pop);
            } else if (ch == 7) {
                string task; cout << "  Task: "; cin.ignore(); getline(cin, task);
                addMaintenance(task);
            } else if (ch == 8) processMaintenance();
            else if (ch == 9) showRecentActions();
            else if (ch == 10) showRecentPopUpdates();
            else if (ch == 11) showZones();
            else if (ch == 12) showMinMaxPopulation();
            if (ch != 0) pauseScreen();
        } while (ch != 0);
    }
};

// ============================================================
//  MODULE 2: POPULATION & TRAFFIC ANALYSIS
// ============================================================

class PopulationTrafficAnalysis {
private:
    vector<long long> traffic;   // hourly traffic
    vector<long long> prefix;
    vector<long long> suffix;

    void buildPrefix() {
        int n = traffic.size();
        prefix.assign(n + 1, 0);
        for (int i = 0; i < n; i++) prefix[i+1] = prefix[i] + traffic[i];
    }

    void buildSuffix() {
        int n = traffic.size();
        suffix.assign(n + 1, 0);
        for (int i = n-1; i >= 0; i--) suffix[i] = suffix[i+1] + traffic[i];
    }

public:
    void loadTraffic(const vector<long long>& data) {
        traffic = data;
        buildPrefix();
        buildSuffix();
        cout << "  [+] Traffic data loaded (" << data.size() << " entries).\n";
    }

    // Prefix sum range query
    long long rangeSum(int l, int r) {
        if (l < 0 || r >= (int)traffic.size() || l > r) { cout << "  [!] Invalid range.\n"; return -1; }
        return prefix[r+1] - prefix[l];
    }

    // Difference array: add val to [l,r]
    void differenceArrayUpdate(vector<long long>& arr, int l, int r, long long val) {
        int n = arr.size();
        vector<long long> diff(n + 1, 0);
        diff[l] += val;
        if (r + 1 < n) diff[r+1] -= val;
        for (int i = 1; i < n; i++) diff[i] += diff[i-1];
        for (int i = 0; i < n; i++) arr[i] += diff[i];
    }

    // Sliding window max (fixed size k)
    vector<long long> slidingWindowMax(int k) {
        int n = traffic.size();
        vector<long long> result;
        if (k > n || k <= 0) return result;
        deque<int> dq;
        for (int i = 0; i < n; i++) {
            while (!dq.empty() && dq.front() <= i - k) dq.pop_front();
            while (!dq.empty() && traffic[dq.back()] <= traffic[i]) dq.pop_back();
            dq.push_back(i);
            if (i >= k - 1) result.push_back(traffic[dq.front()]);
        }
        return result;
    }

    // Sliding window min sum (variable size: sum >= target)
    int minWindowSize(long long target) {
        int n = traffic.size(), l = 0, ans = INT_MAX;
        long long sum = 0;
        for (int r = 0; r < n; r++) {
            sum += traffic[r];
            while (sum >= target) {
                ans = min(ans, r - l + 1);
                sum -= traffic[l++];
            }
        }
        return (ans == INT_MAX) ? -1 : ans;
    }

    // Two pointer: pairs with sum == target
    vector<pair<long long,long long>> twoPairSum(long long target) {
        vector<long long> sorted = traffic;
        sort(sorted.begin(), sorted.end());
        int l = 0, r = sorted.size() - 1;
        vector<pair<long long,long long>> res;
        while (l < r) {
            long long s = sorted[l] + sorted[r];
            if (s == target) { res.push_back({sorted[l], sorted[r]}); l++; r--; }
            else if (s < target) l++;
            else r--;
        }
        return res;
    }

    // Kadane's algorithm: max subarray sum
    pair<long long, pair<int,int>> kadane() {
        int n = traffic.size();
        long long maxSum = LLONG_MIN, curSum = 0;
        int start = 0, end = 0, tempStart = 0;
        for (int i = 0; i < n; i++) {
            curSum += traffic[i];
            if (curSum > maxSum) {
                maxSum = curSum;
                start = tempStart;
                end = i;
            }
            if (curSum < 0) { curSum = 0; tempStart = i + 1; }
        }
        return {maxSum, {start, end}};
    }

    // Binary search on sorted traffic: find target
    int binarySearch(long long target) {
        vector<long long> sorted = traffic;
        sort(sorted.begin(), sorted.end());
        auto it = lower_bound(sorted.begin(), sorted.end(), target);
        if (it != sorted.end() && *it == target)
            return distance(sorted.begin(), it);
        return -1;
    }

    // Binary search on answer: smallest window with avg >= target
    int smallestWindowAvg(double target) {
        int n = traffic.size();
        int lo = 1, hi = n, ans = -1;
        auto check = [&](int k) {
            for (int i = 0; i + k <= n; i++) {
                double avg = (double)(prefix[i+k] - prefix[i]) / k;
                if (avg >= target) return true;
            }
            return false;
        };
        while (lo <= hi) {
            int mid = (lo + hi) / 2;
            if (check(mid)) { ans = mid; hi = mid - 1; }
            else lo = mid + 1;
        }
        return ans;
    }

    void displayTraffic() {
        printSubHeader("Traffic Data");
        int n = traffic.size();
        cout << "  Hours: " << n << "\n";
        cout << "  Data: ";
        for (int i = 0; i < min(n, 20); i++) cout << traffic[i] << " ";
        if (n > 20) cout << "...";
        cout << "\n";
    }

    void runMenu() {
        int ch;
        do {
            printHeader("MODULE 2: Population & Traffic Analysis");
            cout << "  1. Load Sample Traffic Data\n";
            cout << "  2. Display Traffic Data\n";
            cout << "  3. Range Sum Query (Prefix Sum)\n";
            cout << "  4. Sliding Window Max (fixed k)\n";
            cout << "  5. Min Window Size (sum >= target)\n";
            cout << "  6. Two-Pointer Pair Sum\n";
            cout << "  7. Kadane's Algorithm (Max Subarray)\n";
            cout << "  8. Binary Search in Traffic\n";
            cout << "  9. Smallest Window with Avg >= Target\n";
            cout << "  10. Difference Array Bulk Update\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> ch;
            if (ch == 1) {
                // Load 24-hour traffic
                vector<long long> sample = {
                    120, 340, 560, 890, 1200, 1500, 2100, 3200,
                    4500, 5600, 5200, 4800, 4600, 4700, 5100, 5500,
                    6200, 7000, 6500, 5000, 3800, 2400, 1200, 600
                };
                loadTraffic(sample);
            } else if (ch == 2) displayTraffic();
            else if (ch == 3) {
                int l, r;
                cout << "  L R (0-indexed): "; cin >> l >> r;
                long long s = rangeSum(l, r);
                if (s != -1) cout << "  Sum[" << l << ".." << r << "] = " << s << "\n";
            } else if (ch == 4) {
                int k; cout << "  Window size k: "; cin >> k;
                auto res = slidingWindowMax(k);
                cout << "  Max in each window of size " << k << ":\n  ";
                for (auto v : res) cout << v << " ";
                cout << "\n";
            } else if (ch == 5) {
                long long t; cout << "  Target sum: "; cin >> t;
                int ans = minWindowSize(t);
                if (ans == -1) cout << "  No valid window found.\n";
                else cout << "  Minimum window size with sum >= " << t << ": " << ans << "\n";
            } else if (ch == 6) {
                long long t; cout << "  Target sum: "; cin >> t;
                auto res = twoPairSum(t);
                if (res.empty()) cout << "  No pairs found.\n";
                else for (auto& p : res) cout << "  (" << p.first << ", " << p.second << ")\n";
            } else if (ch == 7) {
                auto [maxSum, range] = kadane();
                cout << "  Max Subarray Sum: " << maxSum << "\n";
                cout << "  From index " << range.first << " to " << range.second << "\n";
            } else if (ch == 8) {
                long long t; cout << "  Search value: "; cin >> t;
                int idx = binarySearch(t);
                if (idx == -1) cout << "  Not found.\n";
                else cout << "  Found at sorted index: " << idx << "\n";
            } else if (ch == 9) {
                double t; cout << "  Target average: "; cin >> t;
                int ans = smallestWindowAvg(t);
                if (ans == -1) cout << "  No window found.\n";
                else cout << "  Smallest window with avg >= " << t << ": " << ans << "\n";
            } else if (ch == 10) {
                vector<long long> arr = traffic;
                int l, r; long long val;
                cout << "  L R val: "; cin >> l >> r >> val;
                differenceArrayUpdate(arr, l, r, val);
                cout << "  Updated array (first 20): ";
                for (int i = 0; i < min((int)arr.size(), 20); i++) cout << arr[i] << " ";
                cout << "\n";
            }
            if (ch != 0) pauseScreen();
        } while (ch != 0);
    }
};

// ============================================================
//  MODULE 3: MATH & ENCRYPTION ENGINE
// ============================================================

class MathEncryptionEngine {
private:
    vector<bool> sieve;
    int sieveLimit;

    void buildSieve(int n) {
        sieveLimit = n;
        sieve.assign(n + 1, true);
        sieve[0] = sieve[1] = false;
        for (int i = 2; (long long)i*i <= n; i++)
            if (sieve[i])
                for (int j = i*i; j <= n; j += i)
                    sieve[j] = false;
    }

public:
    MathEncryptionEngine() : sieveLimit(0) { buildSieve(100000); }

    long long gcd(long long a, long long b) { return b ? gcd(b, a%b) : a; }
    long long lcm(long long a, long long b) { return a / gcd(a,b) * b; }

    // Binary Exponentiation
    long long binpow(long long base, long long exp, long long mod) {
        long long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = result * base % mod;
            base = base * base % mod;
            exp >>= 1;
        }
        return result;
    }

    // Modular Inverse (Fermat's Little Theorem, mod must be prime)
    long long modInverse(long long a, long long mod) {
        return binpow(a, mod - 2, mod);
    }

    // Euler Totient
    long long eulerTotient(long long n) {
        long long result = n;
        for (long long i = 2; i * i <= n; i++) {
            if (n % i == 0) {
                while (n % i == 0) n /= i;
                result -= result / i;
            }
        }
        if (n > 1) result -= result / n;
        return result;
    }

    // Prime Factorization
    map<long long, int> primeFactors(long long n) {
        map<long long, int> factors;
        for (long long i = 2; i * i <= n; i++) {
            while (n % i == 0) { factors[i]++; n /= i; }
        }
        if (n > 1) factors[n]++;
        return factors;
    }

    // Count primes in range [l, r]
    int countPrimesInRange(int l, int r) {
        int cnt = 0;
        for (int i = max(l, 2); i <= min(r, sieveLimit); i++)
            if (sieve[i]) cnt++;
        return cnt;
    }

    vector<int> getPrimesInRange(int l, int r) {
        vector<int> result;
        for (int i = max(l, 2); i <= min(r, sieveLimit); i++)
            if (sieve[i]) result.push_back(i);
        return result;
    }

    // Prefix XOR for range queries
    vector<int> buildPrefixXOR(const vector<int>& arr) {
        int n = arr.size();
        vector<int> px(n + 1, 0);
        for (int i = 0; i < n; i++) px[i+1] = px[i] ^ arr[i];
        return px;
    }

    int rangeXOR(const vector<int>& px, int l, int r) {
        return px[r+1] ^ px[l];
    }

    // Simple XOR cipher
    string xorEncrypt(const string& msg, int key) {
        string result = msg;
        for (char& c : result) c ^= key;
        return result;
    }

    // Bit manipulation utilities
    int countSetBits(long long n) { return __builtin_popcountll(n); }
    bool isPowerOf2(long long n) { return n > 0 && !(n & (n-1)); }
    long long clearBit(long long n, int bit) { return n & ~(1LL << bit); }
    long long setBit(long long n, int bit) { return n | (1LL << bit); }
    long long toggleBit(long long n, int bit) { return n ^ (1LL << bit); }
    bool checkBit(long long n, int bit) { return (n >> bit) & 1; }

    // Display binary representation
    string toBinary(long long n, int bits = 16) {
        string result = "";
        for (int i = bits-1; i >= 0; i--) result += ((n >> i) & 1) ? "1" : "0";
        return result;
    }

    // RSA-like mini demo (educational)
    void rsaDemo() {
        // Use small primes for demo
        long long p = 61, q = 53;
        long long n_mod = p * q;               // 3233
        long long phi = (p-1) * (q-1);          // 3120
        long long e = 17;                        // public exponent
        // d = modInverse(e, phi)
        long long d = modInverse(e, phi);        // private exponent

        printSubHeader("RSA Mini Demo (Educational)");
        cout << "  Primes p=" << p << ", q=" << q << "\n";
        cout << "  n = p*q = " << n_mod << "\n";
        cout << "  phi(n) = " << phi << "\n";
        cout << "  Public key e = " << e << "\n";
        cout << "  Private key d = " << d << "\n";

        long long msg = 65; // ASCII 'A'
        long long cipher = binpow(msg, e, n_mod);
        long long decrypted = binpow(cipher, d, n_mod);
        cout << "  Message M = " << msg << "\n";
        cout << "  Ciphertext C = M^e mod n = " << cipher << "\n";
        cout << "  Decrypted = C^d mod n = " << decrypted << "\n";
    }

    void runMenu() {
        int ch;
        do {
            printHeader("MODULE 3: Math & Encryption Engine");
            cout << "  1. GCD & LCM\n";
            cout << "  2. Binary Exponentiation (a^b mod m)\n";
            cout << "  3. Modular Inverse\n";
            cout << "  4. Euler Totient Function\n";
            cout << "  5. Prime Factorization\n";
            cout << "  6. Sieve - Primes in Range\n";
            cout << "  7. Prefix XOR Range Query\n";
            cout << "  8. XOR Cipher (Encrypt/Decrypt)\n";
            cout << "  9. Bit Manipulation Operations\n";
            cout << "  10. RSA Mini Demo\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> ch;
            if (ch == 1) {
                long long a, b; cout << "  a b: "; cin >> a >> b;
                cout << "  GCD(" << a << "," << b << ") = " << gcd(a,b) << "\n";
                cout << "  LCM(" << a << "," << b << ") = " << lcm(a,b) << "\n";
            } else if (ch == 2) {
                long long base, exp, mod;
                cout << "  base exp mod: "; cin >> base >> exp >> mod;
                cout << "  " << base << "^" << exp << " mod " << mod << " = " << binpow(base, exp, mod) << "\n";
            } else if (ch == 3) {
                long long a, mod; cout << "  a mod: "; cin >> a >> mod;
                cout << "  Inverse of " << a << " mod " << mod << " = " << modInverse(a, mod) << "\n";
            } else if (ch == 4) {
                long long n; cout << "  n: "; cin >> n;
                cout << "  phi(" << n << ") = " << eulerTotient(n) << "\n";
            } else if (ch == 5) {
                long long n; cout << "  n: "; cin >> n;
                auto factors = primeFactors(n);
                cout << "  Factors of " << n << ":\n";
                for (auto& [p, e] : factors) cout << "    " << p << "^" << e << "\n";
            } else if (ch == 6) {
                int l, r; cout << "  L R: "; cin >> l >> r;
                auto primes = getPrimesInRange(l, r);
                cout << "  Primes in [" << l << "," << r << "]: ";
                for (int p : primes) cout << p << " ";
                cout << "\n  Count: " << primes.size() << "\n";
            } else if (ch == 7) {
                int n; cout << "  Array size: "; cin >> n;
                vector<int> arr(n);
                cout << "  Elements: ";
                for (int& x : arr) cin >> x;
                auto px = buildPrefixXOR(arr);
                int l, r; cout << "  Range L R: "; cin >> l >> r;
                cout << "  XOR[" << l << ".." << r << "] = " << rangeXOR(px, l, r) << "\n";
            } else if (ch == 8) {
                string msg; int key;
                cout << "  Message: "; cin.ignore(); getline(cin, msg);
                cout << "  Key (0-255): "; cin >> key;
                string enc = xorEncrypt(msg, key);
                string dec = xorEncrypt(enc, key);
                cout << "  Encrypted: "; for (char c : enc) cout << (int)(unsigned char)c << " "; cout << "\n";
                cout << "  Decrypted: " << dec << "\n";
            } else if (ch == 9) {
                long long n; cout << "  Number: "; cin >> n;
                cout << "  Binary (16-bit): " << toBinary(n) << "\n";
                cout << "  Set bits: " << countSetBits(n) << "\n";
                cout << "  Is power of 2: " << (isPowerOf2(n) ? "Yes" : "No") << "\n";
                int bit; cout << "  Check/Toggle bit: "; cin >> bit;
                cout << "  Bit " << bit << " is: " << (checkBit(n, bit) ? "1" : "0") << "\n";
                cout << "  After toggle: " << toggleBit(n, bit) << "\n";
            } else if (ch == 10) rsaDemo();
            if (ch != 0) pauseScreen();
        } while (ch != 0);
    }
};

// ============================================================
//  MODULE 4: RESOURCE WAR SIMULATOR (GAME THEORY)
// ============================================================

class ResourceWarSimulator {
private:
    // Nim Game
    bool nimWinner(const vector<int>& piles) {
        int xorSum = 0;
        for (int p : piles) xorSum ^= p;
        return xorSum != 0; // true = first player wins
    }

    // Grundy number (memoized)
    unordered_map<int, int> memo;
    int grundy(int n) {
        if (n == 0) return 0;
        if (memo.count(n)) return memo[n];
        set<int> reachable;
        // Simple game: can remove 1, 2, or 3 stones
        for (int take = 1; take <= min(n, 3); take++)
            reachable.insert(grundy(n - take));
        // Mex
        int mex = 0;
        while (reachable.count(mex)) mex++;
        return memo[n] = mex;
    }

    // Mex of a set
    int mex(vector<int> S) {
        set<int> s(S.begin(), S.end());
        int m = 0;
        while (s.count(m)) m++;
        return m;
    }

    // Sprague-Grundy for a compound game (multiple piles, custom moves)
    int grundyCustom(int n, const vector<int>& moves) {
        if (memo.count(n)) return memo[n];
        set<int> reachable;
        for (int m : moves)
            if (n - m >= 0)
                reachable.insert(grundyCustom(n - m, moves));
        int mex_val = 0;
        while (reachable.count(mex_val)) mex_val++;
        return memo[n] = mex_val;
    }

    // Resource allocation scenario: two cities compete for resources
    void resourceAllocationGame(int totalResources, int maxTake) {
        printSubHeader("Resource Allocation Game (Nim Variant)");
        cout << "  Total resources: " << totalResources << ", Max take per turn: " << maxTake << "\n";
        cout << "  Two cities (A=first, B=second) take turns removing resources.\n";
        cout << "  City that takes the LAST resource WINS.\n\n";

        // Precompute winning/losing states
        vector<string> state(totalResources + 1);
        state[0] = "LOSE"; // can't move
        for (int i = 1; i <= totalResources; i++) {
            state[i] = "LOSE";
            for (int take = 1; take <= min(i, maxTake); take++) {
                if (state[i - take] == "LOSE") { state[i] = "WIN"; break; }
            }
        }

        cout << "  State table (first 20 positions):\n  ";
        for (int i = 0; i <= min(totalResources, 20); i++)
            cout << i << ":" << state[i] << "  ";
        cout << "\n\n";
        cout << "  With " << totalResources << " resources: City A " << state[totalResources] << "S!\n";
    }

public:
    void runMenu() {
        int ch;
        do {
            printHeader("MODULE 4: Resource War Simulator (Game Theory)");
            cout << "  1. Nim Game (XOR Strategy)\n";
            cout << "  2. Grundy Number Calculator\n";
            cout << "  3. Sprague-Grundy for Custom Moves\n";
            cout << "  4. MEX Function Demo\n";
            cout << "  5. Resource Allocation Game Simulation\n";
            cout << "  6. Multi-Pile Grundy (Compound Game)\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> ch;
            if (ch == 1) {
                int n; cout << "  Number of piles: "; cin >> n;
                vector<int> piles(n);
                cout << "  Pile sizes: ";
                for (int& p : piles) cin >> p;
                int xorSum = 0;
                for (int p : piles) xorSum ^= p;
                cout << "\n  XOR of all piles: " << xorSum << "\n";
                if (nimWinner(piles))
                    cout << "  First player (City A) WINS!\n";
                else
                    cout << "  Second player (City B) WINS!\n";
                cout << "  Strategy: ";
                if (xorSum != 0) {
                    cout << "Make XOR = 0 after your move.\n";
                    for (int i = 0; i < n; i++) {
                        int target = piles[i] ^ xorSum;
                        if (target < piles[i]) {
                            cout << "  Remove " << piles[i] - target << " from pile " << (i+1) << " (pile becomes " << target << ")\n";
                            break;
                        }
                    }
                } else cout << "Opponent wins with optimal play.\n";
            } else if (ch == 2) {
                memo.clear();
                int n; cout << "  Stones (remove 1/2/3): "; cin >> n;
                for (int i = 0; i <= n; i++)
                    cout << "  G(" << i << ") = " << grundy(i) << "\n";
            } else if (ch == 3) {
                memo.clear();
                int n; cout << "  Stones: "; cin >> n;
                int m; cout << "  Allowed moves count: "; cin >> m;
                vector<int> moves(m);
                cout << "  Move sizes: ";
                for (int& mv : moves) cin >> mv;
                for (int i = 0; i <= n; i++)
                    cout << "  G(" << i << ") = " << grundyCustom(i, moves) << "\n";
            } else if (ch == 4) {
                int m; cout << "  Set size: "; cin >> m;
                vector<int> S(m);
                cout << "  Elements: "; for (int& x : S) cin >> x;
                cout << "  MEX = " << mex(S) << "\n";
            } else if (ch == 5) {
                int total, maxT;
                cout << "  Total resources: "; cin >> total;
                cout << "  Max take per turn: "; cin >> maxT;
                resourceAllocationGame(total, maxT);
            } else if (ch == 6) {
                memo.clear();
                int n; cout << "  Number of piles: "; cin >> n;
                int m; cout << "  Allowed moves count: "; cin >> m;
                vector<int> moves(m);
                cout << "  Move sizes: "; for (int& mv : moves) cin >> mv;
                vector<int> piles(n);
                cout << "  Pile sizes: "; for (int& p : piles) cin >> p;
                int xorG = 0;
                for (int p : piles) xorG ^= grundyCustom(p, moves);
                cout << "\n  XOR of Grundy values: " << xorG << "\n";
                cout << "  " << (xorG != 0 ? "First" : "Second") << " player wins!\n";
            }
            if (ch != 0) pauseScreen();
        } while (ch != 0);
    }
};

// ============================================================
//  MODULE 5: ROAD NETWORK ANALYZER (GRAPH & DSU)
// ============================================================

class DSU {
public:
    vector<int> parent, rank_, size_;
    int components;

    DSU(int n) : parent(n), rank_(n, 0), size_(n, 1), components(n) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]); // path compression
        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) swap(a, b); // union by rank
        parent[b] = a;
        size_[a] += size_[b];
        if (rank_[a] == rank_[b]) rank_[a]++;
        components--;
        return true;
    }

    bool connected(int a, int b) { return find(a) == find(b); }
    int getSize(int x) { return size_[find(x)]; }
};

class RoadNetworkAnalyzer {
private:
    int N; // nodes (city sectors)
    vector<vector<pair<int,int>>> adj; // adjacency list (node, weight)
    vector<vector<int>> adjUnweighted;
    vector<tuple<int,int,int>> edges; // (u, v, w)
    vector<string> nodeNames;

    // Tarjan's SCC
    vector<int> disc, low, sccId;
    vector<bool> onStack;
    stack<int> tarjanStack;
    int timer_t, sccCount;

    void tarjanDFS(int u) {
        disc[u] = low[u] = timer_t++;
        tarjanStack.push(u);
        onStack[u] = true;
        for (int v : adjUnweighted[u]) {
            if (disc[v] == -1) {
                tarjanDFS(v);
                low[u] = min(low[u], low[v]);
            } else if (onStack[v]) {
                low[u] = min(low[u], disc[v]);
            }
        }
        if (low[u] == disc[u]) {
            while (true) {
                int v = tarjanStack.top(); tarjanStack.pop();
                onStack[v] = false;
                sccId[v] = sccCount;
                if (v == u) break;
            }
            sccCount++;
        }
    }

    // Bridge detection
    vector<int> bridgeDisc, bridgeLow, parent_bridge;
    vector<bool> visited_bridge;
    vector<pair<int,int>> bridges;
    int bridge_timer;

    void bridgeDFS(int u, int parent) {
        visited_bridge[u] = true;
        bridgeDisc[u] = bridgeLow[u] = bridge_timer++;
        for (auto [v, w] : adj[u]) {
            if (!visited_bridge[v]) {
                bridgeDFS(v, u);
                bridgeLow[u] = min(bridgeLow[u], bridgeLow[v]);
                if (bridgeLow[v] > bridgeDisc[u])
                    bridges.push_back({u, v});
            } else if (v != parent)
                bridgeLow[u] = min(bridgeLow[u], bridgeDisc[v]);
        }
    }

    // Articulation Points
    vector<bool> isAP;
    void apDFS(int u, int parent, vector<int>& disc2, vector<int>& low2, int& timer2) {
        disc2[u] = low2[u] = timer2++;
        int children = 0;
        for (auto [v, w] : adj[u]) {
            if (disc2[v] == -1) {
                children++;
                apDFS(v, u, disc2, low2, timer2);
                low2[u] = min(low2[u], low2[v]);
                if (parent == -1 && children > 1) isAP[u] = true;
                if (parent != -1 && low2[v] >= disc2[u]) isAP[u] = true;
            } else if (v != parent)
                low2[u] = min(low2[u], disc2[v]);
        }
    }

    // Topological Sort (Kahn's BFS)
    vector<int> topoSort() {
        vector<int> indegree(N, 0);
        for (int u = 0; u < N; u++)
            for (int v : adjUnweighted[u])
                indegree[v]++;
        queue<int> q;
        for (int i = 0; i < N; i++) if (indegree[i] == 0) q.push(i);
        vector<int> order;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            for (int v : adjUnweighted[u])
                if (--indegree[v] == 0) q.push(v);
        }
        return order;
    }

    // Dijkstra
    vector<long long> dijkstra(int src) {
        vector<long long> dist(N, LLONG_MAX);
        priority_queue<pair<long long,int>, vector<pair<long long,int>>, greater<>> pq;
        dist[src] = 0; pq.push({0, src});
        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            for (auto [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    pq.push({dist[v], v});
                }
            }
        }
        return dist;
    }

    // MST - Kruskal
    long long kruskal() {
        DSU dsu(N);
        vector<tuple<int,int,int>> sortedEdges = edges;
        sort(sortedEdges.begin(), sortedEdges.end());
        long long total = 0;
        int cnt = 0;
        cout << "  MST Edges:\n";
        for (auto [w, u, v] : sortedEdges) {
            if (dsu.unite(u, v)) {
                cout << "  " << nodeNames[u] << " -- " << nodeNames[v] << " (weight: " << w << ")\n";
                total += w;
                cnt++;
            }
        }
        cout << "  Total MST Weight: " << total << " | Edges: " << cnt << "\n";
        return total;
    }

public:
    void initialize(int n, const vector<string>& names) {
        N = n;
        nodeNames = names;
        adj.assign(n, {});
        adjUnweighted.assign(n, {});
        edges.clear();
    }

    void addEdge(int u, int v, int w, bool directed = false) {
        adj[u].push_back({v, w});
        adjUnweighted[u].push_back(v);
        edges.push_back({w, u, v});
        if (!directed) {
            adj[v].push_back({u, w});
            adjUnweighted[v].push_back(u);
            edges.push_back({w, v, u});
        }
    }

    void showGraph() {
        printSubHeader("Road Network (Adjacency List)");
        for (int u = 0; u < N; u++) {
            cout << "  " << nodeNames[u] << " -> ";
            for (auto [v, w] : adj[u])
                cout << nodeNames[v] << "(w=" << w << ") ";
            cout << "\n";
        }
    }

    void runDijkstra(int src) {
        auto dist = dijkstra(src);
        printSubHeader("Shortest Paths from " + nodeNames[src]);
        for (int i = 0; i < N; i++) {
            cout << "  To " << nodeNames[i] << ": ";
            if (dist[i] == LLONG_MAX) cout << "INF\n";
            else cout << dist[i] << "\n";
        }
    }

    void runKruskal() {
        printSubHeader("Minimum Spanning Tree (Kruskal)");
        kruskal();
    }

    void runTarjan() {
        disc.assign(N, -1); low.assign(N, -1);
        sccId.assign(N, -1); onStack.assign(N, false);
        while (!tarjanStack.empty()) tarjanStack.pop();
        timer_t = sccCount = 0;
        for (int i = 0; i < N; i++)
            if (disc[i] == -1) tarjanDFS(i);
        printSubHeader("Strongly Connected Components (Tarjan)");
        cout << "  Total SCCs: " << sccCount << "\n";
        vector<vector<string>> sccs(sccCount);
        for (int i = 0; i < N; i++) sccs[sccId[i]].push_back(nodeNames[i]);
        for (int i = 0; i < sccCount; i++) {
            cout << "  SCC " << (i+1) << ": ";
            for (auto& s : sccs[i]) cout << s << " ";
            cout << "\n";
        }
    }

    void runBridges() {
        bridges.clear();
        visited_bridge.assign(N, false);
        bridgeDisc.assign(N, -1);
        bridgeLow.assign(N, -1);
        bridge_timer = 0;
        for (int i = 0; i < N; i++)
            if (!visited_bridge[i]) bridgeDFS(i, -1);
        printSubHeader("Bridges (Critical Roads)");
        if (bridges.empty()) cout << "  No bridges found.\n";
        for (auto [u, v] : bridges)
            cout << "  BRIDGE: " << nodeNames[u] << " -- " << nodeNames[v] << "\n";
    }

    void runAP() {
        isAP.assign(N, false);
        vector<int> disc2(N, -1), low2(N, -1);
        int timer2 = 0;
        for (int i = 0; i < N; i++)
            if (disc2[i] == -1) apDFS(i, -1, disc2, low2, timer2);
        printSubHeader("Articulation Points (Critical Sectors)");
        bool found = false;
        for (int i = 0; i < N; i++)
            if (isAP[i]) { cout << "  AP: " << nodeNames[i] << "\n"; found = true; }
        if (!found) cout << "  No articulation points.\n";
    }

    void runTopoSort() {
        auto order = topoSort();
        printSubHeader("Topological Order");
        if ((int)order.size() != N) { cout << "  Graph has a cycle! Topological sort impossible.\n"; return; }
        for (int u : order) cout << "  " << nodeNames[u] << "\n";
    }

    void runDSUDemo() {
        DSU dsu(N);
        printSubHeader("DSU - Connected Components");
        cout << "  Building connections...\n";
        for (auto [w, u, v] : edges) {
            dsu.unite(u, v);
        }
        cout << "  Connected Components: " << dsu.components << "\n";
        for (int i = 0; i < N; i++)
            cout << "  " << nodeNames[i] << " -> Component root: " << nodeNames[dsu.find(i)] << " (size: " << dsu.getSize(i) << ")\n";
    }

    void loadSampleCity() {
        int n = 7;
        vector<string> names = {"Downtown", "NorthEnd", "SouthEnd", "EastSide", "WestSide", "Airport", "Harbor"};
        initialize(n, names);
        addEdge(0, 1, 4);  addEdge(0, 2, 3);  addEdge(1, 3, 5);
        addEdge(1, 4, 2);  addEdge(2, 4, 6);  addEdge(3, 5, 7);
        addEdge(4, 5, 1);  addEdge(5, 6, 3);  addEdge(3, 6, 8);
        cout << "  [+] Sample city network loaded (7 sectors).\n";
    }

    void runMenu() {
        int ch;
        do {
            printHeader("MODULE 5: Road Network Analyzer (Graph & DSU)");
            cout << "  1. Load Sample City Network\n";
            cout << "  2. Display Graph\n";
            cout << "  3. Dijkstra Shortest Paths\n";
            cout << "  4. Kruskal MST\n";
            cout << "  5. Tarjan SCC\n";
            cout << "  6. Bridges Detection\n";
            cout << "  7. Articulation Points\n";
            cout << "  8. Topological Sort\n";
            cout << "  9. DSU Connected Components\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> ch;
            if (ch == 1) loadSampleCity();
            else if (ch == 2) showGraph();
            else if (ch == 3) {
                cout << "  Source node (0-" << N-1 << "): "; int s; cin >> s;
                runDijkstra(s);
            }
            else if (ch == 4) runKruskal();
            else if (ch == 5) runTarjan();
            else if (ch == 6) runBridges();
            else if (ch == 7) runAP();
            else if (ch == 8) runTopoSort();
            else if (ch == 9) runDSUDemo();
            if (ch != 0) pauseScreen();
        } while (ch != 0);
    }
};

// ============================================================
//  MODULE 6: POWER GRID TREE SYSTEM (ADVANCED TREE ALGORITHMS)
// ============================================================

class PowerGridTreeSystem {
private:
    int N;
    vector<vector<int>> children;
    vector<int> parent_, depth_, subtreeSize, euler, first, last;
    vector<vector<int>> up; // binary lifting table
    int LOG;
    int eulerTime;
    vector<string> stationNames;
    vector<int> power; // power at each station
    vector<int> prefixPower; // for subtree queries
    bool built = false;

    void dfs(int u, int p, int d) {
        parent_[u] = p;
        depth_[u] = d;
        subtreeSize[u] = 1;
        first[u] = eulerTime;
        euler[eulerTime++] = u;
        up[u][0] = (p == -1) ? u : p;
        for (int j = 1; j < LOG; j++)
            up[u][j] = up[up[u][j-1]][j-1];
        for (int v : children[u]) {
            dfs(v, u, d+1);
            subtreeSize[u] += subtreeSize[v];
        }
        last[u] = eulerTime - 1;
    }

    void buildPrefixPower() {
        prefixPower.resize(2*N+1, 0);
        for (int i = 0; i < eulerTime; i++)
            prefixPower[i+1] = prefixPower[i] + power[euler[i]];
    }

public:
    void initialize(int n, const vector<string>& names, const vector<int>& pwr) {
        N = n;
        stationNames = names;
        power = pwr;
        LOG = 1;
        while ((1 << LOG) < n) LOG++;
        LOG++;
        children.assign(n, {});
        parent_.assign(n, -1);
        depth_.assign(n, 0);
        subtreeSize.assign(n, 0);
        euler.assign(2*n, 0);
        first.assign(n, 0);
        last.assign(n, 0);
        up.assign(n, vector<int>(LOG, 0));
        eulerTime = 0;
        built = false;
    }

    void addEdge(int parent, int child) {
        children[parent].push_back(child);
    }

    void build(int root = 0) {
        dfs(root, -1, 0);
        buildPrefixPower();
        built = true;
        cout << "  [+] Power grid tree built from root " << stationNames[root] << ".\n";
    }

    // Kth Ancestor using Binary Lifting
    int kthAncestor(int node, int k) {
        if (k > depth_[node]) return -1;
        for (int j = 0; j < LOG; j++)
            if ((k >> j) & 1)
                node = up[node][j];
        return node;
    }

    // LCA using Binary Lifting
    int lca(int u, int v) {
        if (depth_[u] < depth_[v]) swap(u, v);
        int diff = depth_[u] - depth_[v];
        for (int j = 0; j < LOG; j++)
            if ((diff >> j) & 1) u = up[u][j];
        if (u == v) return u;
        for (int j = LOG-1; j >= 0; j--)
            if (up[u][j] != up[v][j]) { u = up[u][j]; v = up[v][j]; }
        return up[u][0];
    }

    // Distance between two nodes
    int distance(int u, int v) {
        int l = lca(u, v);
        return depth_[u] + depth_[v] - 2 * depth_[l];
    }

    // Subtree power sum using Euler tour
    long long subtreePowerSum(int u) {
        // Euler tour: first[u]..last[u] covers subtree of u
        return prefixPower[last[u]+1] - prefixPower[first[u]];
    }

    void displayTree(int u = 0, int indent = 0) {
        cout << string(indent * 3, ' ') << "+-- " << stationNames[u]
             << " [Power: " << power[u] << ", Depth: " << depth_[u]
             << ", SubtreeSize: " << subtreeSize[u] << "]\n";
        for (int v : children[u]) displayTree(v, indent + 1);
    }

    void displayEulerTour() {
        printSubHeader("Euler Tour");
        cout << "  Order: ";
        for (int i = 0; i < eulerTime; i++) cout << stationNames[euler[i]] << " ";
        cout << "\n";
        cout << "  First/Last indices:\n";
        for (int i = 0; i < N; i++)
            cout << "  " << stationNames[i] << ": first=" << first[i] << ", last=" << last[i] << "\n";
    }

    void displayBinaryLiftingTable() {
        printSubHeader("Binary Lifting Table (up[node][j])");
        cout << left << setw(14) << "Node";
        for (int j = 0; j < min(LOG, 5); j++) cout << setw(14) << ("2^" + to_string(j) + " anc.");
        cout << "\n"; printLine('-');
        for (int i = 0; i < N; i++) {
            cout << left << setw(14) << stationNames[i];
            for (int j = 0; j < min(LOG, 5); j++) cout << setw(14) << stationNames[up[i][j]];
            cout << "\n";
        }
    }

    // Re-rooting technique: sum of depths if we re-root at each node
    void reRootingDemo() {
        printSubHeader("Re-rooting Demo: Sum of Depths from Each Node");
        // First compute sum of depths from root 0
        long long rootSum = 0;
        for (int i = 0; i < N; i++) rootSum += depth_[i];
        cout << "  Sum of depths from " << stationNames[0] << " = " << rootSum << "\n";
        // Re-root: moving root from u to child v:
        // newSum = oldSum - subtreeSize[v] + (N - subtreeSize[v])
        vector<long long> ans(N);
        ans[0] = rootSum;
        // BFS/DFS to propagate
        queue<int> q; q.push(0);
        vector<bool> vis(N, false); vis[0] = true;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : children[u]) {
                ans[v] = ans[u] - subtreeSize[v] + (N - subtreeSize[v]);
                vis[v] = true;
                q.push(v);
            }
        }
        for (int i = 0; i < N; i++)
            cout << "  Root at " << stationNames[i] << ": sum of depths = " << ans[i] << "\n";
    }

    void loadSampleGrid() {
        int n = 9;
        vector<string> names = {"Central", "North", "South", "East", "West", "NE", "NW", "SE", "SW"};
        vector<int> pwr =      {100,        80,      70,      60,     75,     40,  35,  50,  45};
        initialize(n, names, pwr);
        // Build tree: 0 is root
        addEdge(0, 1); addEdge(0, 2); addEdge(0, 3); addEdge(0, 4);
        addEdge(1, 5); addEdge(1, 6);
        addEdge(2, 7); addEdge(2, 8);
        build(0);
    }

    void runMenu() {
        int ch;
        do {
            printHeader("MODULE 6: Power Grid Tree System (Advanced Trees)");
            cout << "  1. Load Sample Power Grid\n";
            cout << "  2. Display Tree Structure\n";
            cout << "  3. LCA (Lowest Common Ancestor)\n";
            cout << "  4. Kth Ancestor Query\n";
            cout << "  5. Distance Between Two Stations\n";
            cout << "  6. Subtree Power Sum\n";
            cout << "  7. Display Euler Tour\n";
            cout << "  8. Binary Lifting Table\n";
            cout << "  9. Re-Rooting Demo\n";
            cout << "  0. Back\n";
            cout << "\n  Choice: ";
            cin >> ch;
            if (ch == 1) loadSampleGrid();
            else if (!built && ch != 0) { cout << "  [!] Load sample grid first.\n"; }
            else if (ch == 2) { printSubHeader("Tree Structure"); displayTree(); }
            else if (ch == 3) {
                cout << "  Node u (0-" << N-1 << "): "; int u; cin >> u;
                cout << "  Node v (0-" << N-1 << "): "; int v; cin >> v;
                int l = lca(u, v);
                cout << "  LCA(" << stationNames[u] << ", " << stationNames[v] << ") = " << stationNames[l] << "\n";
            } else if (ch == 4) {
                cout << "  Node (0-" << N-1 << "): "; int u; cin >> u;
                cout << "  K: "; int k; cin >> k;
                int anc = kthAncestor(u, k);
                if (anc == -1) cout << "  Does not exist (k > depth).\n";
                else cout << "  " << k << "th ancestor of " << stationNames[u] << " = " << stationNames[anc] << "\n";
            } else if (ch == 5) {
                cout << "  u v: "; int u, v; cin >> u >> v;
                cout << "  Distance(" << stationNames[u] << ", " << stationNames[v] << ") = " << distance(u, v) << "\n";
            } else if (ch == 6) {
                cout << "  Node (0-" << N-1 << "): "; int u; cin >> u;
                cout << "  Subtree power sum at " << stationNames[u] << " = " << subtreePowerSum(u) << "\n";
            } else if (ch == 7) displayEulerTour();
            else if (ch == 8) displayBinaryLiftingTable();
            else if (ch == 9) reRootingDemo();
            if (ch != 0) pauseScreen();
        } while (ch != 0);
    }
};

// ============================================================
//  MAIN MENU
// ============================================================

void displayMainBanner() {
    cout << "\n";
    printLine('*');
    cout << "  *                                                        *\n";
    cout << "  *          A L G O C I T Y  v1.0                        *\n";
    cout << "  *      Smart City Simulation System (C++ DSA)           *\n";
    cout << "  *                                                        *\n";
    printLine('*');
    cout << "\n";
    cout << "  [1] City Data Manager         (STL)\n";
    cout << "  [2] Population & Traffic      (Array & Binary Search)\n";
    cout << "  [3] Math & Encryption Engine  (Number Theory & Bit Manip)\n";
    cout << "  [4] Resource War Simulator    (Game Theory)\n";
    cout << "  [5] Road Network Analyzer     (Graph & DSU)\n";
    cout << "  [6] Power Grid Tree System    (Advanced Tree Algorithms)\n";
    cout << "  [0] Exit\n";
    cout << "\n  Enter your choice: ";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    CityDataManager       cdm;
    PopulationTrafficAnalysis pta;
    MathEncryptionEngine  mee;
    ResourceWarSimulator  rws;
    RoadNetworkAnalyzer   rna;
    PowerGridTreeSystem   pgts;

    // Pre-load some sample cities for demonstration
    cdm.addCity("AlphaVille", 1500000, 85, 42.5, 32, "A");
    cdm.addCity("BetaBurg",   980000,  72, 55.1, 45, "B");
    cdm.addCity("GammaTown",  2300000, 91, 38.2, 28, "A");
    cdm.addCity("DeltaPort",  760000,  60, 70.3, 58, "C");
    cdm.addCity("EpsilonCity",1100000, 78, 48.7, 39, "B");

    int choice;
    do {
        displayMainBanner();
        cin >> choice;
        switch (choice) {
            case 1: cdm.runMenu(); break;
            case 2: pta.runMenu(); break;
            case 3: mee.runMenu(); break;
            case 4: rws.runMenu(); break;
            case 5: rna.runMenu(); break;
            case 6: pgts.runMenu(); break;
            case 0:
                printLine('=');
                cout << "  Thank you for using AlgoCity! Goodbye.\n";
                printLine('=');
                break;
            default:
                cout << "  [!] Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
