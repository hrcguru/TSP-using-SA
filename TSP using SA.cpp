#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <limits>
#include <utility>  // for std::pair
#include <random>   // for better randomness using std::mt19937

using namespace std;

// Function to calculate Euclidean distance between two points
double distance(const pair<int, int>& city1, const pair<int, int>& city2) {
    return sqrt(pow(city1.first - city2.first, 2) + pow(city1.second - city2.second, 2));
}

// Function to calculate the total cost of a TSP tour
double totalCost(const vector<int>& path, const vector<pair<int, int>>& cities) {
    double cost = 0.0;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        cost += distance(cities[path[i]], cities[path[i + 1]]);
    }
    // Add the return trip to the starting point
    cost += distance(cities[path.back()], cities[path[0]]);
    return cost;
}

// Simulated Annealing algorithm for TSP
vector<int> simulatedAnnealing(const vector<pair<int, int>>& cities, int maxIterations) {
    int n = cities.size();
    vector<int> currentSolution(n);  // Initial solution (0, 1, 2, ..., n-1)
    iota(currentSolution.begin(), currentSolution.end(), 0);  // Fill with 0, 1, ..., n-1

    double currentCost = totalCost(currentSolution, cities);
    vector<int> bestSolution = currentSolution;
    double bestCost = currentCost;

    double temperature = 10000.0;  // Initial temperature
    double coolingRate = 0.995;    // Cooling rate

    random_device rd;  // Seed for random number generator
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, n - 1);  // Distribution for city index
    uniform_real_distribution<> probDist(0.0, 1.0);  // Distribution for probabilities

    for (int iteration = 0; iteration < maxIterations; ++iteration) {
        // Generate a neighbor by swapping two random cities
        vector<int> newSolution = currentSolution;
        int city1 = dist(gen);
        int city2 = dist(gen);
        swap(newSolution[city1], newSolution[city2]);

        double newCost = totalCost(newSolution, cities);

        // Calculate acceptance probability
        double acceptanceProbability = exp((currentCost - newCost) / temperature);

        // Accept the new solution if it has lower cost or by probability
        if (newCost < currentCost || probDist(gen) < acceptanceProbability) {
            currentSolution = newSolution;
            currentCost = newCost;

            // Update the best solution found so far
            if (currentCost < bestCost) {
                bestSolution = currentSolution;
                bestCost = currentCost;
            }
        }

        // Cool down the temperature
        temperature *= coolingRate;
    }

    return bestSolution;
}

int main() {
    int numCities;
    cout << "Enter the number of cities: ";
    cin >> numCities;

    vector<pair<int, int>> cities(numCities);
    cout << "Enter the coordinates (x y) of each city:\n";
    for (int i = 0; i < numCities; ++i) {
        cout << "City " << i + 1 << ": ";
        cin >> cities[i].first >> cities[i].second;
    }

    int maxIterations = 10000;  // Number of iterations

    // Run simulated annealing to solve TSP
    vector<int> bestPath = simulatedAnnealing(cities, maxIterations);

    // Output the final achieved cost and solution
    cout << "Optimal TSP path: ";
    for (int city : bestPath) {
        cout << city << " ";
    }
    cout << endl;

    double bestCost = totalCost(bestPath, cities);
    cout << "Total cost: " << bestCost << endl;

    return 0;
}
    