#include <iostream>
#include <iomanip>
#include <string>
#include<vector>
#include<algorithm>
using namespace std;

// Structure to store user information
struct User {
    int age;
    char gender;          // M or F
    float height;         // in meters
    float weight;         // in kg
    int sleepHours;
    int stepsPerDay;
    string allergy;
    string goal;          // lose / gain / maintain
};

// Function to take user input
void inputUserData(User &u) {
    cout << "\n--- Enter User Health Details ---\n";

    do {
        cout << "Age: ";
        cin >> u.age;
        if (u.age <= 0 || u.age > 120)
            cout << "Invalid age! Please enter a value between 1 and 120.\n";
    } while (u.age <= 0 || u.age > 120);

    do {
        cout << "Gender (M/F): ";
        cin >> u.gender;
        if (u.gender != 'M' && u.gender != 'F' &&
            u.gender != 'm' && u.gender != 'f')
            cout << "Invalid gender! Enter M or F.\n";
    } while (u.gender != 'M' && u.gender != 'F' &&
             u.gender != 'm' && u.gender != 'f');

    do {
        cout << "Height (in meters): ";
        cin >> u.height;
        if (u.height <= 0 || u.height > 2.5)
            cout << "Invalid height! Enter a value between 0.5m and 2.5m.\n";
    } while (u.height <= 0 || u.height > 2.5);

    do {
        cout << "Weight (in kg): ";
        cin >> u.weight;
        if (u.weight <= 0 || u.weight > 500)
            cout << "Invalid weight! Enter a realistic value.\n";
    } while (u.weight <= 0 || u.weight > 500);

    do {
        cout << "Sleep hours per day: ";
        cin >> u.sleepHours;
        if (u.sleepHours <= 0 || u.sleepHours > 24)
            cout << "Invalid sleep hours! Enter a value between 1 and 24.\n";
    } while (u.sleepHours <= 0 || u.sleepHours > 24);

     do {
        cout << "Steps per day: ";
        cin >> u.stepsPerDay;
        if (u.stepsPerDay < 0)
            cout << "Invalid step count! Cannot be negative.\n";
    } while (u.stepsPerDay < 0);

    cin.ignore(); // clears newline

    cout << "Any food allergy (type 'none' if no): ";
    getline(cin, u.allergy);

    do {
        cout << "Goal (lose weight/gain weight/maintain): ";
        getline(cin, u.goal);
        if (u.goal != "lose weight" && u.goal != "gain weight" &&
            u.goal != "maintain") {
            cout << "Invalid goal! Enter 'lose weight', 'gain weight', or 'maintain'.\n";
        }
    } while (u.goal != "lose weight" && u.goal != "gain weight" &&
             u.goal != "maintain");
}

// Function to display user data
void displayUserData(const User &u) {
    cout << "\n--- User Profile Summary ---\n";
    cout << "Age: " << u.age << endl;
    cout << "Gender: " << u.gender << endl;
    cout << "Height: " << u.height << " m\n";
    cout << "Weight: " << u.weight << " kg\n";
    cout << "Sleep Hours: " << u.sleepHours << endl;
    cout << "Steps per Day: " << u.stepsPerDay << endl;
    cout << "Allergy: " << u.allergy << endl;
    cout << "Goal: " << u.goal << endl;
}

// Calculate BMI
float calculateBMI(float weight, float height) {
    return weight / (height * height);
}

// Get BMI Category
string getBMICategory(float bmi) {
    if (bmi < 18.5)
        return "Underweight";
    else if (bmi < 25)
        return "Normal";
    else if (bmi < 30)
        return "Overweight";
    else
        return "Obese";
}

// Calculate BMR (Harris-Benedict)
float calculateBMR(const User &u) {
    if (u.gender == 'M' || u.gender == 'm')
        return 88.36 + (13.4 * u.weight) + (4.8 * u.height * 100) - (5.7 * u.age);
    else
        return 447.6 + (9.2 * u.weight) + (3.1 * u.height * 100) - (4.3 * u.age);
}

// Calculate daily calorie requirement
float calculateDailyCalories(const User &u) {
    float bmr = calculateBMR(u);
    float activityFactor = 1.2; // default sedentary

    if (u.stepsPerDay >= 12000)
        activityFactor = 1.75;  // Very Active
    else if (u.stepsPerDay >= 8000)
        activityFactor = 1.55;  // Active
    else if (u.stepsPerDay >= 5000)
        activityFactor = 1.375; // Moderately Active
    else
        activityFactor = 1.2;

    float calories = bmr * activityFactor;

    if (u.goal == "lose")
        calories -= 500;
    else if (u.goal == "gain")
        calories += 500;

    return calories;
}

string getActivityLevel(int steps) {
    if (steps >= 12000) return "Very Active";
    if (steps >= 8000) return "Active";
    if (steps >= 5000) return "Moderately Active";
    return "Sedentary";
}

//BMI Score
float bmiScore(float bmi) {
    if (bmi >= 18.5 && bmi <= 24.9)
        return 100;
    else if (bmi < 18.5)
        return 60;
    else if (bmi <= 29.9)
        return 50;
    else
        return 30;
}

//Sleep Score
float sleepScore(int sleepHours) {
    if (sleepHours >= 7 && sleepHours <= 9)
        return 100;
    else if (sleepHours >= 6)
        return 70;
    else
        return 40;
}

//Steps Score
float stepsScore(int steps) {
    if (steps >= 8000)
        return 100;
    else if (steps >= 5000)
        return 70;
    else
        return 40;
}

//Age Score
float ageScore(int age) {
    if (age <= 30)
        return 100;
    else if (age <= 45)
        return 80;
    else if (age <= 60)
        return 60;
    else
        return 40;
}

// Divide & Conquer to combine scores
float combineScores(vector<float> &scores, int left, int right) {
    // Base case: only one score
    if (left == right)
        return scores[left];

    int mid = (left + right) / 2;

    // Divide
    float leftScore = combineScores(scores, left, mid);
    float rightScore = combineScores(scores, mid + 1, right);

    // Conquer (merge)
    return (leftScore + rightScore) / 2;
}

float calculateHealthScore(const User &u) {
    vector<float> scores;

    float bmi = calculateBMI(u.weight, u.height);

    scores.push_back(bmiScore(bmi));
    scores.push_back(sleepScore(u.sleepHours));
    scores.push_back(stepsScore(u.stepsPerDay));
    scores.push_back(ageScore(u.age));

    return combineScores(scores, 0, scores.size() - 1);
}

struct Food {
    string name;
    int calories;
    int protein;      // grams
    int carbs;        // grams
    int fat;          // grams
};

vector<Food> foodList = {

    //Eggs & Dairy
    {"Boiled Egg", 78, 6, 1, 5},
    {"Milk (1 glass)", 150, 8, 12, 8},
    {"Curd (1 cup)", 100, 6, 8, 4},
    {"Cheese (1 slice)", 113, 7, 1, 9},

    //Meat & Fish
    {"Grilled Chicken Breast", 165, 31, 0, 4},
    {"Chicken Curry", 220, 27, 6, 10},
    {"Beef Steak", 250, 26, 0, 15},
    {"Fish Curry", 180, 22, 5, 7},
    {"Grilled Fish", 140, 24, 0, 3},

    //Beans & Legumes
    {"Lentils (Dal, 1 cup)", 230, 18, 40, 1},
    {"Chickpeas (1 cup)", 269, 15, 45, 4},
    {"Kidney Beans", 225, 15, 40, 1},
    {"Soybeans", 298, 29, 30, 15},

    //Carbohydrates
    {"Rice (1 cup)", 200, 4, 45, 1},
    {"Brown Rice (1 cup)", 215, 5, 44, 2},
    {"Whole Wheat Roti", 120, 4, 20, 2},
    {"Oats (1 bowl)", 150, 5, 27, 3},

    //Fruits
    {"Apple", 52, 0, 14, 0},
    {"Banana", 89, 1, 23, 0},
    {"Orange", 47, 1, 12, 0},
    {"Mango", 60, 1, 15, 0},
    {"Papaya", 43, 1, 11, 0},

    //Vegetables
    {"Vegetable Salad", 120, 3, 10, 5},
    {"Boiled Spinach", 23, 3, 4, 0},
    {"Carrot", 41, 1, 10, 0},
    {"Broccoli", 55, 4, 11, 1}
};

bool compareByProteinEfficiency(const Food &a, const Food &b) {
    float ratioA = (float)a.protein / a.calories;
    float ratioB = (float)b.protein / b.calories;
    return ratioA > ratioB;
}

struct DietResult {
    vector<Food> foods;
    int totalCalories;
    int totalProtein;
};

//Greedy Diet
DietResult recommendDiet(float dailyCalories) {
    sort(foodList.begin(), foodList.end(), compareByProteinEfficiency);

    float remainingCalories = dailyCalories;
    DietResult result{};

    for (const auto& food : foodList) {
        if (food.calories <= remainingCalories) {
            result.foods.push_back(food);
            remainingCalories -= food.calories;
            result.totalCalories += food.calories;
            result.totalProtein += food.protein;
        }
    }
    return result;
}

//Dp Diet
DietResult recommendDietDP(float dailyCalories) {
    int calorieLimit = (int)dailyCalories;
    int n = foodList.size();
    vector<vector<int>> dp(n + 1, vector<int>(calorieLimit + 1, 0));

    for (int i = 1; i <= n; i++) {
        for (int c = 0; c <= calorieLimit; c++) {
            if (foodList[i - 1].calories <= c)
                dp[i][c] = max(dp[i - 1][c],
                               dp[i - 1][c - foodList[i - 1].calories] +
                               foodList[i - 1].protein);
            else
                dp[i][c] = dp[i - 1][c];
        }
    }

    DietResult result{};
    int c = calorieLimit;

    for (int i = n; i >= 1; i--) {
        if (dp[i][c] != dp[i - 1][c]) {
            result.foods.push_back(foodList[i - 1]);
            result.totalCalories += foodList[i - 1].calories;
            result.totalProtein += foodList[i - 1].protein;
            c -= foodList[i - 1].calories;
        }
    }
    return result;
}

void printDietTableHeader() {
    cout << "--------------------------------------------------\n";
    cout << left << setw(25) << "Food Item"
         << right << setw(12) << "Calories"
         << setw(12) << "Protein\n";
    cout << "--------------------------------------------------\n";
}

void displayQuickDiet(const vector<Food>& selectedFoods,
                      float totalCalories,
                      float totalProtein,
                      float calorieLimit) {

    cout << "\n\n--- Quick & Simple Diet Plan ---\n";
    cout << "(Easy to follow, balanced selection)\n\n";

    printDietTableHeader();

    int shown = 0;
    for (const auto& food : selectedFoods) {
        if (shown == 6) break;   // limit items

        cout << left << setw(25) << food.name
             << right << setw(8) << food.calories << " kcal"
             << setw(8) << food.protein << " g\n";

        shown++;
    }

    cout << "--------------------------------------------------\n";
    cout << "Calories Used    : " << totalCalories << " kcal\n";
    cout << "Remaining Calories: " << calorieLimit - totalCalories << " kcal\n";
    cout << "Protein Intake   : " << totalProtein << " g\n";
}

void displayOptimizedDiet(const vector<Food>& selectedFoods,
                          float totalCalories,
                          float totalProtein,
                          float calorieLimit) {

    cout << "\n\n--- Recommended Daily Diet Plan ---\n";
    cout << "(Best choice for maximum protein within calorie limit)\n\n";

    printDietTableHeader();

    int shown = 0;
    for (const auto& food : selectedFoods) {
        if (shown == 6) break;

        cout << left << setw(25) << food.name
             << right << setw(8) << food.calories << " kcal"
             << setw(8) << food.protein << " g\n";

        shown++;
    }

    cout << "==================================================\n";
    cout << "Total Calories Used : " << totalCalories << " kcal\n";
    cout << "Remaining Calories  : " << calorieLimit - totalCalories << " kcal\n";
    cout << "Total Protein Intake: " << totalProtein << " g\n";
    cout << "==================================================\n";
}

// Display user data + health metrics
void displayCompleteHealthReport(const User &u) {
    float bmi = calculateBMI(u.weight, u.height);
    float dailyCalories = calculateDailyCalories(u);
    float healthScore = calculateHealthScore(u);

    cout << "\n--- Complete Health Analysis Report ---\n";

    // User health metrics
    cout << "BMI: " << bmi << endl;
    cout << "BMI Category: " << getBMICategory(bmi) << endl;
    cout << "Activity Level: " << getActivityLevel(u.stepsPerDay) << endl;
    cout << "Estimated Daily Calories Needed: " << dailyCalories << " kcal\n";

    // Health Score + Status (Divide & Conquer)
    cout << "Health Score: " << healthScore << " / 100\n";
    if (healthScore >= 80)
        cout << "Health Status: Excellent\n";
    else if (healthScore >= 60)
        cout << "Health Status: Good\n";
    else
        cout << "Health Status: Needs Improvement\n";

    //Greedy approach
    DietResult quick = recommendDiet(dailyCalories);
    displayQuickDiet(
        quick.foods,
        quick.totalCalories,
        quick.totalProtein,
        dailyCalories
    );

    //DP approach
    DietResult optimal = recommendDietDP(dailyCalories);
    displayOptimizedDiet(
        optimal.foods,
        optimal.totalCalories,
        optimal.totalProtein,
        dailyCalories
    );
}

int main() {
    User user;

    inputUserData(user);
    displayUserData(user);
    displayCompleteHealthReport(user);

    cout << "\nBasic health analysis completed.\n";
    return 0;
}

